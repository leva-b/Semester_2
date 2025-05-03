# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/LB_7_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/LB_7_autogen.dir/ParseCache.txt"
  "LB_7_autogen"
  )
endif()
