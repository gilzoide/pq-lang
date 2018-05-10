cmake_minimum_required(VERSION 2.8)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_SOVERSION ${PROJECT_VERSION_MAJOR})

if(${CMAKE_VERSION} VERSION_GREATER 3.0)
	cmake_policy(SET CMP0048 NEW)
	project(pq-lang VERSION ${PROJECT_VERSION})
else()
	project(pq-lang)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wno-parentheses")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

# Extra CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(Judy REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PEGA_TEXTO REQUIRED pega-texto>=2)
pkg_check_modules(LIBJIT REQUIRED libjit)

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
file(GLOB headers "include/pq/*.h")
configure_file("include/pq.h" "pq.h" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/pq.h" DESTINATION "include")
install(FILES ${headers} DESTINATION "include/${CMAKE_PROJECT_NAME}")
