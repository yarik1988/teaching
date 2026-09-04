# AGENTS.md — semester_1/graphics (SFML)

Notes for AI agents working in this folder. Repo-wide rules (toolchain paths,
exe mirror layout, win-gui automation) live in the root `AGENTS.md`.
Everything here was verified by actually running the code (2026-09-04), except
where explicitly marked otherwise.

Exes follow the root mirror rule: `cmake-build-debug/semester_1/graphics/<TARGET>.exe`
(note: the graph app's target is mixed-case `Graph_SFML`).

Stack: SFML 3.0.2 + imgui master + ImGui-SFML master, all FetchContent'd into
`cmake-build-debug/_deps/` (sources of truth: `_deps/sfml-src/src/SFML/Graphics/RenderTarget.cpp`,
`_deps/imgui-sfml-src/imgui-SFML.cpp`). Libraries are built static, but the
exes are **not** fully static — see below.

## SFML + ImGui-SFML apps must call resetGLStates() before the main loop

Any app that mixes SFML drawing with ImGui-SFML (`Graph.cpp`, `Screener`)
must call `window.resetGLStates()` once after `ImGui::SFML::Init(window)`,
before the event loop. The one-line fix in `Graph.cpp` carries a longer
comment; the mechanism, confirmed against both library sources:

- SFML applies its GL state lazily on the **first** `draw()` call
  (`RenderTarget::initialize()` sets `glStatesSet = false`; `setupDraw()` only
  then runs `resetGLStates()`).
- `ImGui::SFML::Render()` wraps its drawing in `window.pushGLStates()` /
  `popGLStates()` every frame — i.e. `glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS)`
  … `glPopClientAttrib()`.
- If no SFML `draw()` happened before the first ImGui render — an app that
  draws nothing until user input, like Graph_SFML with an empty graph — the
  first push saves the *pristine* state, `resetGLStates()` runs **inside** the
  push, and the pop restores vertex/color client arrays **disabled**. SFML's
  internal draw cache believes they are still enabled and never re-enables
  them, so every later `draw()` calls `glDrawArrays` with no enabled arrays.
- Result: ImGui renders fine, app logic runs, canvas stays silently blank.

Symptom signature: clicks/sliders work (state changes prove events flow), no
SFML content ever appears. The freeglut apps in `../graphics_freeglut/` use the
ImGui GLUT/OpenGL2 backends with no SFML state cache and are unaffected.

Diagnostic that nails it: draw any SFML primitive unconditionally on frame 1
(a red rectangle) — if everything starts rendering, it's this bug, not
coordinates/colors/input.

## The exes are NOT fully static — DLL imports break bare-bash launches

Despite the `-static;-static-libgcc;-static-libstdc++` link options in
`CMakeLists.txt`, the exes import `libstdc++-6.dll` + `libwinpthread-1.dll`
(check: `".../CLion 2025.2.1/bin/mingw/bin/objdump" -p <exe> | grep "DLL Name"`).

- Launching from bare git-bash fails (exit 127, DLL load error, no output).
  CLion runs and win-gui `launch_app` work (their environments find the DLLs).
- To run from bash AND capture stdout/stderr (printf debugging — `launch_app`
  suppresses the console):

  ```bash
  cd cmake-build-debug/semester_1/graphics
  PATH="/c/Program Files/JetBrains/CLion 2025.2.1/bin/mingw/bin:$PATH" \
    ./Graph_SFML.exe > sfml_debug.log 2>&1   # as a background task
  ```

- Rebuilding while the app runs fails at link time (Windows file lock) —
  `close_window` before `cmake --build`.

## Graph_SFML (port of ../graphics_freeglut/Graph.cpp)

- Window "Graph application", 800x800 client at screen (700,100).
- Coordinate model: graph space is 0..100 (`gl_width`/`gl_height`);
  `drawGraph` scales by `window.getSize()/100` px/unit, `find_node` converts
  mouse pixels back. The `Resized` handler updates the view, so resizing the
  window rescales the drawing (unlike the GLUT original, which only reshapes).
- `imgui.ini` next to the exe persists the MAIN TOOLBAR position **and its
  collapsed/expanded state**. A toolbar collapsed in a previous run hides all
  buttons — if clicks "do nothing", take a screenshot first and click ▶ to
  expand (or delete the file).
- Verified working via win-gui sendinput clicks: Create ordinary graph (nodes
  + blue edges), Create oriented graph (green arrowheads), slider click-jump,
  toolbar collapse/expand, left-click vertex selection (red dot).
- NOT yet verified by an agent after the resetGLStates fix: Find shortest path,
  right-click deselect, Delete selected vertices (owner was testing).
- Click coordinates depend on the persisted toolbar position — always
  screenshot first (root AGENTS.md workflow), don't reuse stale coords.
