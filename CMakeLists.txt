cmake_minimum_required(VERSION 3.25)
project(commonplusplus VERSION 1.0.0 LANGUAGES CXX)

# Day-to-day use goes through the Makefile; this file exists for vcpkg (and any CMake consumer).
# Header-only by design: keep compiled code out of src/common.
option(COMMONPP_REFLECT "Use reflect-cpp and install the common/reflect headers" ON)
option(COMMONPP_SANDBOX "Build the sandbox executable" ${PROJECT_IS_TOP_LEVEL})

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
if(COMMONPP_REFLECT)
    find_package(reflectcpp CONFIG REQUIRED)
endif()

# ---- Library: header-only, so the target only carries usage requirements ---------------------
add_library(commonplusplus INTERFACE)
add_library(commonplusplus::commonplusplus ALIAS commonplusplus)

target_compile_features(commonplusplus INTERFACE cxx_std_23)
# glm must be configured the same way in every TU, so these reach every consumer
target_compile_definitions(commonplusplus INTERFACE GLM_FORCE_SWIZZLE GLM_ENABLE_EXPERIMENTAL)
target_include_directories(commonplusplus INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(commonplusplus INTERFACE fmt::fmt spdlog::spdlog glm::glm)
if(COMMONPP_REFLECT)
    target_link_libraries(commonplusplus INTERFACE reflectcpp::reflectcpp)
endif()

# ---- Sandbox: smoke test, also sees experimental/ ---------------------------------------------
if(COMMONPP_SANDBOX)
    add_executable(sandbox sandbox/main.cpp)
    target_compile_options(sandbox PRIVATE -Wall -Wextra)
    target_include_directories(sandbox PRIVATE experimental)
    target_link_libraries(sandbox PRIVATE commonplusplus::commonplusplus)
endif()

# ---- Install: headers (never experimental/) and the CMake package config ----------------------
set(COMMONPP_HEADER_EXCLUDES)
if(NOT COMMONPP_REFLECT)
    list(APPEND COMMONPP_HEADER_EXCLUDES PATTERN "reflect" EXCLUDE)
endif()
install(DIRECTORY src/common DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" ${COMMONPP_HEADER_EXCLUDES})

install(TARGETS commonplusplus EXPORT commonplusplus-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(COMMONPP_CMAKE_DIR ${CMAKE_INSTALL_DATADIR}/commonplusplus)
install(EXPORT commonplusplus-targets NAMESPACE commonplusplus:: DESTINATION ${COMMONPP_CMAKE_DIR})
configure_package_config_file(cmake/commonplusplus-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/commonplusplus-config.cmake
    INSTALL_DESTINATION ${COMMONPP_CMAKE_DIR})
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/commonplusplus-config-version.cmake
    COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/commonplusplus-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/commonplusplus-config-version.cmake
    DESTINATION ${COMMONPP_CMAKE_DIR})
