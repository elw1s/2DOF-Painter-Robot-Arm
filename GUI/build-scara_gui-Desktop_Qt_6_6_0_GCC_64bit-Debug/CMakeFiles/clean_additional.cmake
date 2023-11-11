# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/scara_gui_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/scara_gui_autogen.dir/ParseCache.txt"
  "scara_gui_autogen"
  )
endif()
