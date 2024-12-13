# Find the Balloon header
#
#  BALLOON_INCLUDE_DIR - Where to find Balloon header files
#  BALLOON_FOUND       - True if Balloon found.

find_path(BALLOON_INCLUDE_DIR Balloon.h
        HINTS $ENV{BALLOON_DIR} ${BALLOON_DIR}
        PATH_SUFFIXES include
        DOC "Where the Balloon headers can be found"
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Balloon DEFAULT_MSG BALLOON_INCLUDE_DIR)

add_library(Balloon INTERFACE)
target_include_directories(Balloon INTERFACE ${BALLOON_INCLUDE_DIR})