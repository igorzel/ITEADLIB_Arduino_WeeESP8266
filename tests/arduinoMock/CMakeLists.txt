set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(arduinoMock_SOURCES
	Mock_Arduino.cpp
	Mock_Arduino.h
	Mock_Serial.cpp
	Mock_Serial.h
	String.cpp
	String.h
)

if(MSVC)
foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	if(${flag_var} MATCHES "/MD")
		string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	endif(${flag_var} MATCHES "/MD")
	if(${flag_var} MATCHES "/MDd")
		string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
	endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)
endif(MSVC)

add_library(libarduinoMock STATIC ${arduinoMock_SOURCES})

target_link_libraries(libarduinoMock
	libgtest
	libgmock
#	${GTEST_LIBRARIES}
#	${GMOCK_LIBRARIES}
)

target_include_directories(libarduinoMock PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
