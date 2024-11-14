# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\TestStepsTool_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TestStepsTool_autogen.dir\\ParseCache.txt"
  "TestStepsTool_autogen"
  )
endif()
