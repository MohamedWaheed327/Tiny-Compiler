# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\LexicalAnalyzer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LexicalAnalyzer_autogen.dir\\ParseCache.txt"
  "LexicalAnalyzer_autogen"
  )
endif()
