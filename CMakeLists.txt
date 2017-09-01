cmake_minimum_required(VERSION 2.8)
set(PROJECT_VERSION 0.1.0)
if(${CMAKE_VERSION} VERSION_GREATER 3.0)
	cmake_policy(SET CMP0048 NEW)
	project(pq-lang VERSION ${PROJECT_VERSION})
else()
	project(pq-lang)
endif()

set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

# Extra CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(LLVM REQUIRED)
find_package(Judy REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PEGA_TEXTO REQUIRED pega-texto)

# Build stuff
add_subdirectory(src)
# add_subdirectory(pkg-config)

# Test
# option(ENABLE_TESTS "Enable tests to be built and run with `make test`" OFF)
# if(ENABLE_TESTS)
	# enable_testing()
	# add_subdirectory(test)
# endif()

# Examples
# option(COMPILE_EXAMPLES "Compile the examples" OFF)
# if(COMPILE_EXAMPLES)
	# add_subdirectory(examples)
# endif()

# Install headers
# file(GLOB headers "include/pq/*.h")
# configure_file("include/pq.h" "pq.h" @ONLY)
# install(FILES "${CMAKE_CURRENT_BINARY_DIR}/pq-lang.h" DESTINATION "include")
# install(FILES ${headers} DESTINATION "include/${CMAKE_PROJECT_NAME}")
