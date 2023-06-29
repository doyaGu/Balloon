# Backward-cpp
add_subdirectory(backward-cpp EXCLUDE_FROM_ALL)

# PhysicsFS
set(PHYSFS_BUILD_SHARED OFF CACHE BOOL "")
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "")
set(PHYSFS_BUILD_DOCS OFF CACHE BOOL "")
set(PHYSFS_DISABLE_INSTALL ON CACHE BOOL "")
add_subdirectory(physfs EXCLUDE_FROM_ALL)
set_target_properties(physfs-static PROPERTIES FOLDER "Dependencies")

# MinHook
add_subdirectory(minhook EXCLUDE_FROM_ALL)
set_target_properties(minhook PROPERTIES FOLDER "Dependencies")

# yyjson
add_subdirectory(yyjson EXCLUDE_FROM_ALL)
set_target_properties(yyjson PROPERTIES FOLDER "Dependencies")

# sv
add_library(sv STATIC)
set(SV_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/sv)
file(GLOB SV_HEADERS ${SV_SOURCE_DIR}/src/*.h)
file(GLOB SV_SOURCES ${SV_SOURCE_DIR}/src/*.c)
target_sources(sv PRIVATE ${SV_SOURCES} ${SV_HEADERS})
target_include_directories(sv PUBLIC $<BUILD_INTERFACE:${SV_SOURCE_DIR}/include>)
set_target_properties(sv PROPERTIES FOLDER "Dependencies")

# utf8.h
add_library(utf8 INTERFACE)
target_include_directories(utf8 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/utf8.h)

# itoa
add_library(itoa INTERFACE)
target_include_directories(itoa INTERFACE ${CMAKE_CURRENT_LIST_DIR}/itoa/include)
