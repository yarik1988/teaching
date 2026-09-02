# Small patches applied to third-party sources fetched by FetchContent.
#
# ImGui-SFML's cmake/FindImGui.cmake contains a leftover debug statement
#   message(WARNING "Found: ...")
# that fires on every configure even when ImGui is found successfully
# (the module already prints a proper "Found ImGui vX.Y" STATUS message).
# This script deletes that line. If upstream changes or removes it, the
# replacement below simply doesn't match and the script is a no-op.

file(READ "${PATCH_FILE}" contents)

string(REPLACE
        "message(WARNING \"Found: \${IMGUI_DIR} \${IMGUI_SEARCH_PATH} \${IMGUI_INCLUDE_DIR} \${MY_FILES}\")\n"
        ""
        contents "${contents}")

file(WRITE "${PATCH_FILE}" "${contents}")