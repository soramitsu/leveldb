include(CMakeFindDependencyMacro)

find_dependency(Threads)
find_dependency(crc32c CONFIG REQUIRED)
find_dependency(Snappy CONFIG REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/leveldbTargets.cmake")
