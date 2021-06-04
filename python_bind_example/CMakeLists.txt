cmake_minimum_required(VERSION 3.5)

cm_find_package(CM)
include(CMDeploy)
include(CMSetupVersion)
include(FindPythonLibs)

cm_project(actions WORKSPACE_NAME ${CMAKE_WORKSPACE_NAME} LANGUAGES ASM C CXX)
# This is supposed to be handled with cm_project directive, but somehow it is
# not working, so enabling these languages explicitly
enable_language(C CXX ASM)

cm_find_package(Python 3.7 REQUIRED COMPONENTS Interpreter Development)
cm_find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem system python${Python_VERSION_MAJOR}${Python_VERSION_MINOR})

cm_setup_version(VERSION 0.1.0)

# get header files; only needed by CMake generators,
# e.g., for creating proper Xcode projects
set(${CURRENT_PROJECT_NAME}_HEADERS)

# list cpp files excluding platform-dependent files
list(APPEND ${CURRENT_PROJECT_NAME}_SOURCES
     src/actions.cpp)

python_add_module(${CURRENT_PROJECT_NAME}
                  ${${CURRENT_PROJECT_NAME}_HEADERS}
                  ${${CURRENT_PROJECT_NAME}_SOURCES})

set_target_properties(${CURRENT_PROJECT_NAME} PROPERTIES
                      LINKER_LANGUAGE CXX
                      EXPORT_NAME ${CURRENT_PROJECT_NAME}
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED TRUE)

target_link_libraries(${CURRENT_PROJECT_NAME} PUBLIC
                      ${PYTHON_LIBRARIES}
                      ${Boost_LIBRARIES})

target_include_directories(${CURRENT_PROJECT_NAME} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           ${PYTHON_INCLUDE_DIRS}
                           ${Boost_INCLUDE_DIRS})

if(APPLE OR NOT ${CMAKE_TARGET_ARCHITECTURE} STREQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR})
    set_target_properties(${CURRENT_PROJECT_NAME} PROPERTIES
                          XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${APPLE_CODE_SIGN_IDENTITY}"
                          XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM}")
endif()

cm_deploy(TARGETS ${CURRENT_PROJECT_NAME}
          INCLUDE include
          NAMESPACE ${CMAKE_WORKSPACE_NAME}::)
