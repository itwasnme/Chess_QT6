# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\appChess_QT6_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appChess_QT6_autogen.dir\\ParseCache.txt"
  "appChess_QT6_autogen"
  )
endif()
