# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Shuriken_lite_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Shuriken_lite_autogen.dir\\ParseCache.txt"
  "Shuriken_lite_autogen"
  )
endif()
