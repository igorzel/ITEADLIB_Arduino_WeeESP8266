/**
 * @file Mock_Serial.h
 * @brief The definition of Arduino Serial mockups.
 * @author Igor Zelenyi
 * @date 2015.10
 *
 * @par Copyright:
 * Copyright (c) Igor Zelenyi \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef ESPDUINO_MOCK_SERIAL_H_
#define ESPDUINO_MOCK_SERIAL_H_

#include <gmock/gmock.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <vector>
#include <functional>

#include "Mock_Arduino.h"
#include "String.h"

template<typename T>
class TO_STR
{
public:
	static std::string getValue(T val)
	{
		std::ostringstream oss;
		oss << val;

		return oss.str();
	}
};

template<>
class TO_STR<uint8_t>
{
public:
	static std::string getValue(uint8_t val)
	{
		std::ostringstream oss;
		oss << (int)val;

		return oss.str();
	}
};


class Mock_Serial
{
public:
	MOCK_METHOD1(begin, void(unsigned long));
	MOCK_METHOD0(isReady, bool());

	MOCK_METHOD0(available, int());
	MOCK_METHOD0(read, uint8_t());
	MOCK_METHOD1(write, size_t(uint8_t));

	MOCK_METHOD0(flush, void());

	MOCK_METHOD1(find, bool(char*));

	MOCK_METHOD1(print, size_t(const char*));
	MOCK_METHOD1(print, size_t(String));
	MOCK_METHOD1(print, size_t(int));
	MOCK_METHOD1(print, size_t(uint8_t));
	MOCK_METHOD1(print, size_t(uint32_t));

	MOCK_METHOD1(println, size_t(const char*));
	MOCK_METHOD1(println, size_t(int));
	MOCK_METHOD1(println, size_t(uint8_t));
	MOCK_METHOD1(println, size_t(uint32_t));

	Mock_Serial();

	struct RequestAnswer
	{
		std::string expectedDataToSerial;
		std::string expectedDataFromSerial;
		std::string actualDataToSerial;
	};

	std::vector<RequestAnswer> requestsAnswers;

protected:
	int fake_available();
	uint8_t fake_read();

	int requestsAnswersIndex = 0;

	template<typename T>
	size_t fake_print(T p, bool endLine = false)
	{
		std::ostringstream oss;
		oss << TO_STR<T>::getValue(p);

		if (endLine)
		{
			oss << "\r\n";
		}

		requestsAnswers[requestsAnswersIndex].actualDataToSerial += oss.str();
	}
};

using ::testing::NiceMock;

NiceMock<Mock_Serial>* createSerialMock();
void releaseSerialMock();

class Stream
{
public:
	static void begin(unsigned long speed);

	static int available();
	static uint8_t read();
	static size_t write(uint8_t ch);

	static void flush();

	static bool find(char *target);

	static size_t print(const char* buffer);
	static size_t print(String str);
	static size_t print(int a);
	static size_t print(uint8_t a);
	static size_t print(uint32_t a);

	static size_t println(const char* buffer);
	static size_t println(int a);
	static size_t println(uint8_t a);
	static size_t println(uint32_t);
};

class HardwareSerial : public Stream
{
public:
	operator bool();
};

extern HardwareSerial Serial;

#endif
