set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ESP8266_tests_SOURCES
	ESP8266_tests.cpp
#	GMockTestHelper.h
#	GMockTestHelper.cpp
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


add_executable(ESP8266_tests ${ESP8266_tests_SOURCES})

target_link_libraries(ESP8266_tests
	libgtest
	libgmock
	libarduinoMock
	libESP8266
)

add_test(NAME ESP8266_tests COMMAND ESP8266_tests)
