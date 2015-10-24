cmake_minimum_required(VERSION 2.8)

project(ITEADLIB_Arduino_WeeESP8266)

enable_testing()

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)



# Download and install GoogleTest
ExternalProject_Add(
	gtest
	URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
	# Disable install step
	INSTALL_COMMAND ""
)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set gtest properties
ExternalProject_Get_Property(gtest source_dir binary_dir)
set_target_properties(libgtest PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#	"INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")


# Download and install GoogleMock
ExternalProject_Add(
	gmock
	URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gmock
	# Disable install step
	INSTALL_COMMAND ""
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gmock)

# Set gmock properties
ExternalProject_Get_Property(gmock source_dir binary_dir)
set_target_properties(libgmock PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/libgmock.a"
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#	"INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")


set(libESP8266_SOURCES
	ESP8266.h
	ESP8266.cpp
)

add_definitions(-DUNIT_TEST)

add_library(libESP8266 STATIC ${libESP8266_SOURCES})

target_link_libraries(libESP8266
	libgtest
	libgmock
	libarduinoMock
)

target_include_directories(libESP8266 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(tests)
