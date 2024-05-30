# CMakeList.txt: dynamic_lib 项目文件

project(dir_opt VERSION 1.0.0.0)

# add include directory
include_directories(
    # ${CMAKE_SOURCE_DIR}/inc
    # ${CMAKE_SOURCE_DIR}/export/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# get source list of the directory
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

# message(${CMAKE_CURRENT_SOURCE_DIR})


if(MSVC)
    set(VERSIONINFO_RC "${CMAKE_CURRENT_SOURCE_DIR}/version_info.rc")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version_info.rc.in" "${VERSIONINFO_RC}")
endif()

# add source to this project's library dll
add_library(${PROJECT_NAME} STATIC ${SRC_LIST} ${VERSIONINFO_RC})