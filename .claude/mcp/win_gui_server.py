# win-gui MCP server
# Launch/inspect/interact with Windows GUI apps (OpenGL, freeglut, SFML, ...).
# Tools: list_windows, launch_app, screenshot_window, click, click_sequence,
#        drag, key, activate_window, close_window
# Screenshots use PrintWindow(PW_RENDERFULLCONTENT) -> works for background
# windows; falls back to client-area BitBlt if the capture comes back black.
# Input uses SendInput (needs foreground) or PostMessage (no focus steal).
import ctypes
import os
import subprocess
import tempfile
import time
from ctypes import wintypes

import win32api
import win32con
import win32gui
import win32ui
from PIL import Image
from mcp.server.mcpserver import MCPServer

try:  # real pixel coordinates regardless of display scaling
    ctypes.windll.shcore.SetProcessDpiAwareness(2)
except Exception:
    pass

server = MCPServer(
    "win-gui",
    instructions=(
        "Drive Windows GUI applications: launch an exe, screenshot its window, "
        "click/drag/type at coordinates. Coordinates for click/drag default to "
        "space='window', i.e. the same pixel space as screenshot_window output. "
        "View returned PNG paths with the Read tool."
    ),
)

LAUNCHED = {}  # pid -> {"proc": Popen, "exe": str, "title_hint": str}

SM_XVIRTUALSCREEN, SM_YVIRTUALSCREEN = 76, 77
SM_CXVIRTUALSCREEN, SM_CYVIRTUALSCREEN = 78, 79
PW_RENDERFULLCONTENT = 0x00000002
INFINITE_TM = 1000


# ---------------------------------------------------------------- windows ---
def _enum_windows():
    found = []

    def cb(hwnd, _):
        if win32gui.IsWindowVisible(hwnd) and win32gui.GetWindowText(hwnd):
            l, t, r, b = win32gui.GetWindowRect(hwnd)
            found.append({
                "hwnd": hwnd,
                "title": win32gui.GetWindowText(hwnd),
                "rect": [l, t, r, b],
            })

    win32gui.EnumWindows(cb, None)
    return found


def _window_pid(hwnd):
    pid = wintypes.DWORD()
    ctypes.windll.user32.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
    return pid.value


def _find_by_title(title):
    """Visible windows whose title contains `title` (case-insensitive)."""
    hits = [w for w in _enum_windows() if title.lower() in w["title"].lower()]
    return hits


def _resolve_hwnd(window: str):
    """window: '' (single launched app), numeric hwnd, or title substring."""
    if window:
        if window.isdigit():
            hwnd = int(window)
            if win32gui.IsWindow(hwnd):
                return hwnd, None
            raise ValueError(f"hwnd {hwnd} is not a valid window")
        hits = _find_by_title(window)
        if not hits:
            raise ValueError(f"no visible window matching {window!r}; "
                             f"open: {[w['title'] for w in _enum_windows()][:15]}")
        if len(hits) > 1:
            raise ValueError(f"ambiguous title {window!r}, matches: "
                             f"{[(h['hwnd'], h['title']) for h in hits]}")
        return hits[0]["hwnd"], hits[0]
    # no name given: use the single window of processes we launched
    cands = []
    for pid, info in LAUNCHED.items():
        cands += [w for w in _enum_windows()
                  if _window_pid(w["hwnd"]) == pid and w["title"]]
    if not cands:
        raise ValueError("no window from a launched app; pass window=<title or hwnd>")
    if len(cands) > 1:
        raise ValueError(f"several launched windows: {cands}; pass window=<title>")
    return cands[0]["hwnd"], cands[0]


def _geo(hwnd):
    l, t, r, b = win32gui.GetWindowRect(hwnd)
    cl, ct, cr, cb = win32gui.GetClientRect(hwnd)
    csx, csy = win32gui.ClientToScreen(hwnd, (0, 0))
    return {
        "window_rect": [l, t, r, b],
        "window_size": [r - l, b - t],
        "client_size": [cr - cl, cb - ct],
        "client_offset_in_window": [csx - l, csy - t],  # client origin inside a
        "client_screen_origin": [csx, csy],             # full-window screenshot
    }


def _foreground(hwnd):
    if win32gui.IsIconic(hwnd):
        win32gui.ShowWindow(hwnd, win32con.SW_RESTORE)
        time.sleep(0.15)
    if win32gui.GetForegroundWindow() == hwnd:
        return
    try:
        win32gui.SetForegroundWindow(hwnd)
    except Exception:
        # ALT-key trick: Windows blocks foreground steals from background procs
        user32 = ctypes.windll.user32
        user32.keybd_event(win32con.VK_MENU, 0, 0, 0)
        try:
            win32gui.SetForegroundWindow(hwnd)
        finally:
            user32.keybd_event(win32con.VK_MENU, 0, win32con.KEYEVENTF_KEYUP, 0)
    time.sleep(0.05)


# ------------------------------------------------------------- screenshot ---
def _shot_printwindow(hwnd, w, h):
    hwnd_dc = win32gui.GetWindowDC(hwnd)
    mfc = win32ui.CreateDCFromHandle(hwnd_dc)
    dc = mfc.CreateCompatibleDC()
    bmp = win32ui.CreateBitmap()
    bmp.CreateCompatibleBitmap(mfc, w, h)
    dc.SelectObject(bmp)
    ok = ctypes.windll.user32.PrintWindow(hwnd, dc.GetSafeHdc(), PW_RENDERFULLCONTENT)
    info, data = bmp.GetInfo(), bmp.GetBitmapBits(True)
    win32gui.DeleteObject(bmp.GetHandle())
    dc.DeleteDC()
    mfc.DeleteDC()
    win32gui.ReleaseDC(hwnd, hwnd_dc)
    img = Image.frombuffer("RGB", (info["bmWidth"], info["bmHeight"]),
                           data, "raw", "BGRX", 0, 1)
    return img, bool(ok)


def _shot_bitblt(hwnd, w, h):  # fallback: client area straight from the screen
    g = _geo(hwnd)
    cw, ch = g["client_size"]
    hdc = win32gui.GetDC(hwnd)
    mfc = win32ui.CreateDCFromHandle(hdc)
    dc = mfc.CreateCompatibleDC()
    bmp = win32ui.CreateBitmap()
    bmp.CreateCompatibleBitmap(mfc, cw, ch)
    dc.SelectObject(bmp)
    ctypes.windll.user32.BitBlt(dc.GetSafeHdc(), 0, 0, cw, ch,
                                hdc, 0, 0, 0x00CC0020)  # SRCCOPY
    info, data = bmp.GetInfo(), bmp.GetBitmapBits(True)
    win32gui.DeleteObject(bmp.GetHandle())
    dc.DeleteDC()
    mfc.DeleteDC()
    win32gui.ReleaseDC(hwnd, hdc)
    return Image.frombuffer("RGB", (info["bmWidth"], info["bmHeight"]),
                            data, "raw", "BGRX", 0, 1)


def _default_shot_path():
    d = os.path.join(tempfile.gettempdir(), "claude_shots")
    os.makedirs(d, exist_ok=True)
    return os.path.join(d, time.strftime("shot_%Y%m%d_%H%M%S") + f"_{int(time.time()*1000)%1000:03d}.png")


# ----------------------------------------------------------------- input ----
class _MOUSEINPUT(ctypes.Structure):
    _fields_ = [("dx", wintypes.LONG), ("dy", wintypes.LONG),
                ("mouseData", wintypes.DWORD), ("dwFlags", wintypes.DWORD),
                ("time", wintypes.DWORD), ("dwExtraInfo", ctypes.c_size_t)]


class _KEYBDINPUT(ctypes.Structure):
    _fields_ = [("wVk", wintypes.WORD), ("wScan", wintypes.WORD),
                ("dwFlags", wintypes.DWORD), ("time", wintypes.DWORD),
                ("dwExtraInfo", ctypes.c_size_t)]


class _HARDWAREINPUT(ctypes.Structure):
    _fields_ = [("uMsg", wintypes.DWORD), ("wParamL", wintypes.WORD),
                ("wParamH", wintypes.WORD)]


class _IUNION(ctypes.Union):
    _fields_ = [("mi", _MOUSEINPUT), ("ki", _KEYBDINPUT), ("hi", _HARDWAREINPUT)]


class _INPUT(ctypes.Structure):
    _anonymous_ = ("u",)
    _fields_ = [("type", wintypes.DWORD), ("u", _IUNION)]


MOUSE_MOVE, MOUSE_ABS, MOUSE_VIRT = 0x0001, 0x8000, 0x4000
BTN = {  # (down, up) flags
    "left": (0x0002, 0x0004),
    "right": (0x0008, 0x0010),
    "middle": (0x0020, 0x0040),
}


def _send_input(inputs):
    arr = (_INPUT * len(inputs))(*inputs)
    if ctypes.windll.user32.SendInput(len(arr), arr, ctypes.sizeof(_INPUT)) != len(arr):
        raise OSError("SendInput was blocked")


def _mouse(flags, dx=0, dy=0):
    inp = _INPUT(type=0)  # INPUT_MOUSE
    inp.mi = _MOUSEINPUT(dx, dy, 0, flags, 0, 0)
    _send_input([inp])


def _abs_px(px, py):
    u = ctypes.windll.user32
    vx, vy = u.GetSystemMetrics(SM_XVIRTUALSCREEN), u.GetSystemMetrics(SM_YVIRTUALSCREEN)
    vw, vh = u.GetSystemMetrics(SM_CXVIRTUALSCREEN), u.GetSystemMetrics(SM_CYVIRTUALSCREEN)
    return (round((px - vx) * 65535 / max(vw - 1, 1)),
            round((py - vy) * 65535 / max(vh - 1, 1)))


def _send_move_abs(px, py):
    nx, ny = _abs_px(px, py)
    _mouse(MOUSE_MOVE | MOUSE_ABS | MOUSE_VIRT, nx, ny)


VKS = {"enter": 0x0D, "esc": 0x1B, "escape": 0x1B, "space": 0x20, "tab": 0x09,
       "backspace": 0x08, "delete": 0x2E, "del": 0x2E, "insert": 0x2D,
       "home": 0x24, "end": 0x23, "pageup": 0x21, "pagedown": 0x22,
       "left": 0x25, "up": 0x26, "right": 0x27, "down": 0x28,
       "shift": 0x10, "ctrl": 0x11, "control": 0x11, "alt": 0x12}


def _vk_for(token):
    if token.lower() in VKS:
        return VKS[token.lower()], 0
    if len(token) == 1:
        res = ctypes.windll.user32.VkKeyScanW(ord(token))
        if res == -1:
            raise ValueError(f"no VK code for {token!r}")
        return res & 0xFF, (res >> 8) & 0xFF  # vk, modifiers (1 shift 2 ctrl 4 alt)
    raise ValueError(f"unknown key {token!r}; use a named key or a single char")


def _type_key(text):
    seq = []  # (vk, up?)
    for token in text.split("+"):
        if not token:
            raise ValueError("empty key token in combination")
        vk, mods = _vk_for(token)
        if mods & 1:
            seq.append((0x10, False))
        if mods & 2:
            seq.append((0x11, False))
        if mods & 4:
            seq.append((0x12, False))
        seq.append((vk, False))
        seq.append((vk, True))
        if mods & 4:
            seq.append((0x12, True))
        if mods & 2:
            seq.append((0x11, True))
        if mods & 1:
            seq.append((0x10, True))
    inputs = []
    for vk, up in seq:
        inp = _INPUT(type=1)  # INPUT_KEYBOARD
        inp.ki = _KEYBDINPUT(vk, 0, 4 if up else 0, 0, 0)  # KEYEVENTF_UNICODE? no: 4=KEYUP
        inputs.append(inp)
    _send_input(inputs)


def _to_client_point(hwnd, x, y, space):
    g = _geo(hwnd)
    if space == "window":
        return x - g["client_offset_in_window"][0], y - g["client_offset_in_window"][1]
    if space == "client":
        return x, y
    raise ValueError("space must be 'window' or 'client'")


# ------------------------------------------------------------------ tools ---
@server.tool()
def list_windows(title_filter: str = "") -> dict:
    """List visible top-level windows (hwnd, title, rect). Optional case-insensitive title substring filter."""
    wins = [w for w in _enum_windows()
            if not title_filter or title_filter.lower() in w["title"].lower()]
    return {"count": len(wins), "windows": wins}


@server.tool()
def launch_app(exe_path: str, args: list[str] = None, cwd: str = "",
               wait_for_title: str = "", timeout_s: float = 10.0) -> dict:
    """Start an exe (console output suppressed). Optionally wait until one of its
    visible windows contains wait_for_title. Returns pid and the window found."""
    if not os.path.isfile(exe_path):
        return {"error": f"no such file: {exe_path}"}
    proc = subprocess.Popen(
        [exe_path] + (args or []),
        cwd=cwd or os.path.dirname(exe_path),
        creationflags=subprocess.CREATE_NO_WINDOW,
    )
    LAUNCHED[proc.pid] = {"proc": proc, "exe": exe_path,
                          "title_hint": wait_for_title}
    deadline = time.time() + timeout_s
    win = None
    while time.time() < deadline and win is None:
        time.sleep(0.1)
        for w in _enum_windows():
            if _window_pid(w["hwnd"]) == proc.pid and w["title"] and \
               (not wait_for_title or wait_for_title.lower() in w["title"].lower()):
                win = w
                break
    return {"pid": proc.pid, "exe": exe_path,
            "window": win, "found": win is not None}


def _capture(hwnd, out_path):
    """Screenshot machinery shared by screenshot_window and click_sequence."""
    g = _geo(hwnd)
    w, h = g["window_size"]
    img, ok = _shot_printwindow(hwnd, w, h)
    method = "printwindow"
    lo, hi = img.convert("L").getextrema()
    if not ok or hi < 10:  # black frame -> GL content missed, blit from screen
        _foreground(hwnd)
        time.sleep(0.15)
        img = _shot_bitblt(hwnd, w, h)
        method = "bitblt_client"
        g["window_rect"] = [g["client_screen_origin"][0], g["client_screen_origin"][1],
                            g["client_screen_origin"][0] + g["client_size"][0],
                            g["client_screen_origin"][1] + g["client_size"][1]]
        g["window_size"] = g["client_size"]
        g["client_offset_in_window"] = [0, 0]
    path = out_path or _default_shot_path()
    img.save(path)
    return {"path": path, "method": method,
            "title": win32gui.GetWindowText(hwnd), "hwnd": hwnd, **g}


@server.tool()
def screenshot_window(window: str = "", out_path: str = "") -> dict:
    """Capture a window to a PNG (PrintWindow; captures background windows too).
    Returns the image path (view it with the Read tool), sizes, and
    client_offset_in_window so click(x, y, space='window') can target pixels
    seen in the screenshot."""
    hwnd, _ = _resolve_hwnd(window)
    return _capture(hwnd, out_path)


def _click_once(hwnd, x, y, button, space, method, double=False):
    """One click; shared by click and click_sequence."""
    cx, cy = _to_client_point(hwnd, x, y, space)
    if method == "postmessage":
        MK = {"left": 0x0001, "right": 0x0002, "middle": 0x0010}[button]
        msgs = {"left": (0x0201, 0x0202), "right": (0x0204, 0x0205),
                "middle": (0x0207, 0x0208)}[button]
        lparam = (int(cy) & 0xFFFF) << 16 | (int(cx) & 0xFFFF)
        post = ctypes.windll.user32.PostMessageW
        post(hwnd, 0x0200, 0, lparam)  # WM_MOUSEMOVE so hover state updates
        time.sleep(0.05)
        post(hwnd, msgs[0], MK, lparam)
        time.sleep(0.06)
        post(hwnd, msgs[1], 0, lparam)
        if double:
            time.sleep(0.05)
            post(hwnd, msgs[0], MK, lparam)
            time.sleep(0.06)
            post(hwnd, msgs[1], 0, lparam)
        return {"ok": True, "method": method, "client": [cx, cy], "hwnd": hwnd}
    # sendinput
    g = _geo(hwnd)
    px = g["client_screen_origin"][0] + cx
    py = g["client_screen_origin"][1] + cy
    _foreground(hwnd)
    _send_move_abs(px, py)
    time.sleep(0.05)
    for _ in range(2 if double else 1):
        _mouse(BTN[button][0])
        time.sleep(0.06)
        _mouse(BTN[button][1])
        time.sleep(0.08)
    return {"ok": True, "method": "sendinput", "screen": [px, py],
            "client": [cx, cy], "hwnd": hwnd}


@server.tool()
def click(x: float, y: float, button: str = "left", window: str = "",
          space: str = "window", method: str = "sendinput",
          double: bool = False) -> dict:
    """Click at (x, y). space='window' matches screenshot pixel coordinates;
    space='client' is relative to the client area. method='sendinput' moves the
    real cursor (activates the window); method='postmessage' posts mouse
    messages without stealing focus (works for many GLUT/Win32 apps)."""
    hwnd, _ = _resolve_hwnd(window)
    return _click_once(hwnd, x, y, button, space, method, double)


@server.tool()
def click_sequence(clicks: list, window: str = "", space: str = "window",
                   method: str = "sendinput", inter_click_ms: float = 150,
                   abort_on_window_move: bool = True, out_path: str = "") -> dict:
    """Click several points in ONE call: clicks=[{"x":158,"y":431},
    {"x":458,"y":231,"button":"right"}, ...]; optional per-click "double": true.
    Same space/method semantics as click. Sleeps inter_click_ms (clamped >=50)
    between clicks so same-spot pairs aren't read as double-clicks. If the
    window moves mid-sequence, stops there and reports instead of firing the
    rest blindly. out_path: optional screenshot after the last click."""
    if not isinstance(clicks, list) or not clicks:
        return {"error": "clicks must be a non-empty list of {x, y, button?}"}
    for c in clicks:
        if "x" not in c or "y" not in c:
            return {"error": f"every click needs x and y, got {c!r}"}
        if c.get("button", "left") not in BTN:
            return {"error": f"unknown button {c.get('button')!r}"}
    hwnd, _ = _resolve_hwnd(window)
    origin0 = tuple(_geo(hwnd)["client_screen_origin"])
    done = []
    for i, c in enumerate(clicks):
        if i:
            if abort_on_window_move:
                origin = tuple(_geo(hwnd)["client_screen_origin"])
                if origin != origin0:
                    return {"ok": False, "aborted": True, "at_click": i,
                            "reason": "window moved between clicks",
                            "client_origin_before": list(origin0),
                            "client_origin_now": list(origin),
                            "clicks_done": done, "hwnd": hwnd}
            time.sleep(max(inter_click_ms, 50) / 1000.0)
        try:
            res = _click_once(hwnd, c["x"], c["y"], c.get("button", "left"),
                              space, method, bool(c.get("double", False)))
        except Exception as e:
            return {"ok": False, "aborted": True, "at_click": i, "reason": str(e),
                    "clicks_done": done, "hwnd": hwnd}
        res["index"] = i
        done.append(res)
    out = {"ok": True, "clicks": done, "hwnd": hwnd,
           "title": win32gui.GetWindowText(hwnd)}
    if out_path:
        out["screenshot"] = _capture(hwnd, out_path)
    return out


@server.tool()
def drag(x1: float, y1: float, x2: float, y2: float, button: str = "left",
         window: str = "", space: str = "window", steps: int = 25) -> dict:
    """Press at (x1,y1), move to (x2,y2) in small steps, release. Uses SendInput
    (the window is activated). E.g. drag a graph vertex."""
    hwnd, _ = _resolve_hwnd(window)
    c1 = _to_client_point(hwnd, x1, y1, space)
    c2 = _to_client_point(hwnd, x2, y2, space)
    g = _geo(hwnd)
    p1 = (g["client_screen_origin"][0] + c1[0], g["client_screen_origin"][1] + c1[1])
    p2 = (g["client_screen_origin"][0] + c2[0], g["client_screen_origin"][1] + c2[1])
    _foreground(hwnd)
    _send_move_abs(*p1)
    time.sleep(0.08)
    _mouse(BTN[button][0])
    time.sleep(0.08)
    for i in range(1, steps + 1):
        px = p1[0] + (p2[0] - p1[0]) * i / steps
        py = p1[1] + (p2[1] - p1[1]) * i / steps
        _send_move_abs(px, py)
        time.sleep(0.012)
    time.sleep(0.05)
    _mouse(BTN[button][1])
    return {"ok": True, "from": list(p1), "to": list(p2), "hwnd": hwnd}


@server.tool()
def key(text: str, window: str = "") -> dict:
    """Send keystrokes, e.g. 'a', 'enter', 'ctrl+s', 'shift+left'. The window is
    activated first."""
    hwnd, _ = _resolve_hwnd(window)
    _foreground(hwnd)
    _type_key(text)
    return {"ok": True, "sent": text, "hwnd": hwnd}


@server.tool()
def activate_window(window: str = "") -> dict:
    """Bring a window to the foreground."""
    hwnd, _ = _resolve_hwnd(window)
    _foreground(hwnd)
    return {"ok": True, "hwnd": hwnd, "title": win32gui.GetWindowText(hwnd)}


@server.tool()
def close_window(window: str = "", pid: int = 0) -> dict:
    """Close an app politely (WM_CLOSE), force-killing it if still alive after 2s."""
    if pid:
        target_pid = pid
        hwnd = next((w["hwnd"] for w in _enum_windows()
                     if _window_pid(w["hwnd"]) == pid), None)
    else:
        hwnd, _ = _resolve_hwnd(window)
        target_pid = _window_pid(hwnd)
    closed = "already-gone"
    if hwnd:
        win32gui.PostMessage(hwnd, win32con.WM_CLOSE, 0, 0)
        closed = "wm_close"
        deadline = time.time() + 2.0
        while time.time() < deadline:
            if not any(w["hwnd"] == hwnd for w in _enum_windows()):
                break
            time.sleep(0.1)
        else:
            subprocess.run(["taskkill", "/PID", str(target_pid), "/F", "/T"],
                           capture_output=True)
            closed = "force-killed"
    elif target_pid:
        subprocess.run(["taskkill", "/PID", str(target_pid), "/F", "/T"],
                       capture_output=True)
        closed = "force-killed"
    LAUNCHED.pop(target_pid, None)
    return {"ok": True, "pid": target_pid, "how": closed}


if __name__ == "__main__":
    server.run("stdio")
