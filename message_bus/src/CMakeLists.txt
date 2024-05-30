# CMakeList.txt: dynamic_lib 项目文件

project(message_bus VERSION 1.0.0.0)

add_definitions(-DMESSAGE_BUS_EXPORTS)
# add_definitions(-DSYNC_MESSAGE_BUS)

# add include directory
include_directories(
    # ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    # ${CMAKE_SOURCE_DIR}/dll_version/include
)

# link_directories(
# 	${CMAKE_SOURCE_DIR}/lib
# )

# get source list of the directory
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

# message(${CMAKE_CURRENT_SOURCE_DIR})
# message(${SRC_LIST})

if(MSVC)
    set(VERSIONINFO_RC "${PROJECT_SOURCE_DIR}/version_info.rc")
    configure_file("${PROJECT_SOURCE_DIR}/version_info.rc.in" "${VERSIONINFO_RC}")
endif()

# add source to this project's library dll
add_library(${PROJECT_NAME} SHARED ${SRC_LIST} ${VERSIONINFO_RC})

# target_link_libraries(${PROJECT_NAME} file_version)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})