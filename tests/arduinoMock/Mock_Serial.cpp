/**
 * @file Mock_Serial.cpp
 * @brief The implementation of Arduino Serial mockups.
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

#include <Mock_Serial.h>
#include <assert.h>

using ::testing::NiceMock;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::Matcher;
using ::testing::_;


HardwareSerial Serial;

static NiceMock<Mock_Serial>* mockSerial = nullptr;

Mock_Serial::Mock_Serial()
{
	ON_CALL(*this, available())
		.WillByDefault(Invoke(this, &Mock_Serial::fake_available));

	ON_CALL(*this, read())
		.WillByDefault(Invoke(this, &Mock_Serial::fake_read));

#define ON_PRINT_CALL(T) \
	ON_CALL(*this, print(Matcher<T>(_))) \
		.WillByDefault(Invoke([this](T p) -> size_t { fake_print(p); }));

#define ON_PRINTLN_CALL(T) \
	ON_CALL(*this, println(Matcher<T>(_))) \
		.WillByDefault(Invoke([this](T p) -> size_t { fake_print(p, true); }));


	ON_PRINT_CALL(const char*);
	ON_PRINT_CALL(String);
	ON_PRINT_CALL(int);
	ON_PRINT_CALL(uint8_t);
	ON_PRINT_CALL(uint32_t);

	ON_PRINTLN_CALL(const char*);
	ON_PRINTLN_CALL(int);
	ON_PRINTLN_CALL(uint8_t);
	ON_PRINTLN_CALL(uint32_t);
}

int Mock_Serial::fake_available()
{
	int dataSize = 0;

	if (requestsAnswersIndex < requestsAnswers.size())
	{
		dataSize = requestsAnswers[requestsAnswersIndex].expectedDataFromSerial.length();

		if (dataSize == 0)
		{
			requestsAnswersIndex++;
		}
	}

	return dataSize;
}

uint8_t Mock_Serial::fake_read()
{
	assert(!requestsAnswers[requestsAnswersIndex].expectedDataFromSerial.empty());

	uint8_t ch = 0;

	if (!requestsAnswers[requestsAnswersIndex].expectedDataFromSerial.empty())
	{
		ch = requestsAnswers[requestsAnswersIndex].expectedDataFromSerial[0];

		requestsAnswers[requestsAnswersIndex].expectedDataFromSerial.erase(0, 1);
	}

	return ch;
}

NiceMock<Mock_Serial>* createSerialMock()
{
	if (!mockSerial)
	{
		mockSerial = new NiceMock<Mock_Serial>;
	}

	return mockSerial;
}

void releaseSerialMock()
{
	delete mockSerial;
	mockSerial = nullptr;
}

void Stream::begin(unsigned long speed)
{
	mockSerial->begin(speed);
}

int Stream::available()
{
	return mockSerial->available();
}

uint8_t Stream::read()
{
	return mockSerial->read();
}

size_t Stream::write(uint8_t ch)
{
	return mockSerial->write(ch);
}

void Stream::flush()
{
	mockSerial->flush();
}

bool Stream::find(char *target)
{
	return mockSerial->find(target);
}

size_t Stream::print(const char* buffer)
{
	return mockSerial->print(buffer);
}

size_t Stream::print(String str)
{
	return mockSerial->print(str);
}

size_t Stream::print(int a)
{
	return mockSerial->print(a);
}

size_t Stream::print(uint8_t a)
{
	return mockSerial->print(a);
}

size_t Stream::print(uint32_t a)
{
	return mockSerial->print(a);
}

size_t Stream::println(const char* buffer)
{
	return mockSerial->println(buffer);
}

size_t Stream::println(int a)
{
	return mockSerial->println(a);
}

size_t Stream::println(uint8_t a)
{
	return mockSerial->println(a);
}

size_t Stream::println(uint32_t a)
{
	return mockSerial->println(a);
}

HardwareSerial::operator bool()
{
	return mockSerial->isReady();
}
