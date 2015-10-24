/**
 * @file ESP8266_tests.cpp
 * @brief The implementations of unit tests for class ESP8266.
 * @author Igor Zelenyi
 * @date 2015.10
 *
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <algorithm>
#include <sstream>

#include "Mock_Serial.h"
#include "Mock_Arduino.h"

#include "ESP8266.h"

using ::testing::Return;
using ::testing::Invoke;
using ::testing::InvokeArgument;
using ::testing::InvokeWithoutArgs;
using ::testing::IgnoreResult;
using ::testing::Matcher;
using ::testing::StrEq;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Assign;
using ::testing::DoAll;
using ::testing::DoDefault;
using ::testing::ByRef;

class ESP8266Test : public ::testing::Test
{
protected:
	NiceMock<Mock_Arduino>* arduinoMock;
	NiceMock<Mock_Serial>* serialMock;
	ESP8266* esp;

	virtual void SetUp()
	{
		arduinoMock = createArduinoMock();
		serialMock = createSerialMock();
		const unsigned long baud = 9600;

		EXPECT_SERIAL_DATA("", "bad data");

		esp = new ESP8266(Serial, baud);
	}

	void EXPECT_SERIAL_DATA(const std::string& expectedDataToSerial, const std::string& expectedDataFromSerial)
	{
		serialMock->requestsAnswers.push_back
		(
			Mock_Serial::RequestAnswer{expectedDataToSerial, expectedDataFromSerial, ""}
		);
	}

	void VerifySerialRequests()
	{
		for (auto iter = serialMock->requestsAnswers.begin(); iter != serialMock->requestsAnswers.end(); ++iter)
		{
			EXPECT_EQ(iter->expectedDataToSerial, iter->actualDataToSerial);
		}
	}

	virtual void TearDown()
	{
		delete esp;
		releaseSerialMock();
		releaseArduinoMock();
	}
};

class ESP8266_getVersion : public ESP8266Test {};

TEST_F(ESP8266_getVersion, ok)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+GMR\r\n", "AT+GMR\r\r\n0018000902-AI03\r\n\r\nOK\r\n");

	// running code...
	String version = esp->getVersion();

	// check
	EXPECT_EQ(std::string("0018000902-AI03"), version);
	VerifySerialRequests();
}

TEST_F(ESP8266_getVersion, receiveByParts)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+GMR\r\n", "AT+GMR\r\r\n0018000902-");
	EXPECT_SERIAL_DATA("",								   "AI03\r\n\r\n");
	EXPECT_SERIAL_DATA("",												"OK\r");
	EXPECT_SERIAL_DATA("",													 "\n");

	// running code...
	String version = esp->getVersion();

	// check
	EXPECT_EQ(std::string("0018000902-AI03"), version);
	VerifySerialRequests();
}

TEST_F(ESP8266_getVersion, timeout)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+GMR\r\n", "AT+GMR\r\r\n0018");
	EXPECT_SERIAL_DATA("",							"000902-AI03\r\n\r\nOK\r\n");

	{
		InSequence s;

		// start
		EXPECT_CALL(*arduinoMock, millis())
			.Times(1)
			.WillOnce(DoDefault())
			.RetiresOnSaturation();

		// first call
		EXPECT_CALL(*arduinoMock, millis())
			.Times(1)
			.WillOnce(DoDefault())
			.RetiresOnSaturation();

		// call after No data
		EXPECT_CALL(*arduinoMock, millis())
			.Times(1)
			.WillOnce(Return(999999))
			.RetiresOnSaturation();
	}

	// running code...
	String version = esp->getVersion();

	// check
	EXPECT_EQ(std::string("AT+GMR\r\r\n0018"), version);
	VerifySerialRequests();
}

TEST_F(ESP8266_getVersion, ignoreNulls)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	std::string expectedDataFromSerial = " AT+ GMR \r\r \n 00  18000902 -AI03 \r \n\r\nO K\r\n ";
	//									^   ^   ^	^  ^  ^^		^	 ^  ^	   ^	 ^
	std::replace(expectedDataFromSerial.begin(), expectedDataFromSerial.end(), ' ', '\0');
	EXPECT_SERIAL_DATA("AT+GMR\r\n", expectedDataFromSerial);

	// running code...
	String version = esp->getVersion();

	// check
	EXPECT_EQ(std::string("0018000902-AI03"), version);
	VerifySerialRequests();
}

class ESP8266_setOprToStation : public ESP8266Test {};

TEST_F(ESP8266_setOprToStation, okAlreadySet)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWMODE?\r\n", "AT+CWMODE?\r\r\n+CWMODE:1\r\n\r\nOK\r\n");

	// running code...
	bool result = esp->setOprToStation();

	EXPECT_TRUE(result);
	VerifySerialRequests();
}

TEST_F(ESP8266_setOprToStation, okSet)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWMODE?\r\n", "AT+CWMODE?\r\r\n+CWMODE:2\r\n\r\nOK\r\n");
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWMODE=1\r\n", "AT+CWMODE=1\r\r\n\r\nOK\r\n");

	// running code...
	bool result = esp->setOprToStation();

	EXPECT_TRUE(result);
	VerifySerialRequests();
}

TEST_F(ESP8266_setOprToStation, cannotRequest)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWMODE?\r\n", "AT+CWMODE?\r\r\nError\r\n");

	// running code...
	bool result = esp->setOprToStation();

	EXPECT_FALSE(result);
	VerifySerialRequests();
}

TEST_F(ESP8266_setOprToStation, cannotSet)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWMODE?\r\n", "AT+CWMODE?\r\r\n+CWMODE:2\r\n\r\nOK\r\n");
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWMODE=1\r\n", "AT+CWMODE=1\r\r\n\r\nError\r\n");

	// running code...
	bool result = esp->setOprToStation();

	EXPECT_FALSE(result);
	VerifySerialRequests();
}

class ESP8266_joinAP : public ESP8266Test {};

TEST_F(ESP8266_joinAP, ok)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWJAP=\"AccessPointName\",\"AccessPointPassword\"\r\n", "AT+CWJAP=\"AccessPointName\",\"AccessPointPassword\"\r\r\n\r\nOK\r\n");

	// running code...
	bool result = esp->joinAP("AccessPointName", "AccessPointPassword");

	EXPECT_TRUE(result);
	VerifySerialRequests();
}

TEST_F(ESP8266_joinAP, failed)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWJAP=\"AccessPointName\",\"AccessPointPassword\"\r\n", "AT+CWJAP=\"AccessPointName\",\"AccessPointPassword\"\r\r\n\r\nFAIL\r\n");

	// running code...
	bool result = esp->joinAP("AccessPointName", "AccessPointPassword");

	EXPECT_FALSE(result);
	VerifySerialRequests();
}

TEST_F(ESP8266_joinAP, timeout)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CWJAP=\"AccessPointName\",\"AccessPointPassword\"\r\n", "AT+CWJAP=\"AccessPointName\",\"AccessPointPassword\"\r\r\n");

	// running code...
	bool result = esp->joinAP("AccessPointName", "AccessPointPassword");

	EXPECT_FALSE(result);
	VerifySerialRequests();
}

class ESP8266_enableMUX : public ESP8266Test {};

TEST_F(ESP8266_enableMUX, ok)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CIPMUX=1\r\n", "AT+CIPMUX=1\r\r\n\r\nOK\r\n");

	// running code...
	bool result = esp->enableMUX();

	EXPECT_TRUE(result);
	VerifySerialRequests();
}

TEST_F(ESP8266_enableMUX, failed)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CIPMUX=1\r\n", "AT+CIPMUX=1\r\r\n\r\nERROR\r\n");

	// running code...
	bool result = esp->enableMUX();

	EXPECT_FALSE(result);
	VerifySerialRequests();
}

class ESP8266_startTCPServer : public ESP8266Test{};

TEST_F(ESP8266_startTCPServer, ok)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CIPSERVER=1,8888\r\n", "AT+CIPSERVER=1,8888\r\r\n\r\nOK\r\n");

	// running code...
	bool result = esp->startTCPServer(8888);

	EXPECT_TRUE(result);
	VerifySerialRequests();
}

TEST_F(ESP8266_startTCPServer, failed)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("AT+CIPSERVER=1,8888\r\n", "AT+CIPSERVER=1,8888\r\r\n\r\nERROR\r\n");

	// running code...
	bool result = esp->startTCPServer(8888);

	EXPECT_FALSE(result);
	VerifySerialRequests();
}

class ESP8266_recv : public ESP8266Test
{
public:
	std::string RecvPkg(uint8_t coming_mux_id, const std::string& data)
	{
		std::ostringstream oss;
		oss << "+IPD," << (int)coming_mux_id << "," << + data.length() << ":";
		oss << data;
		oss << "\r\n\r\nOK\r\n";

		return oss.str();
	}
};

TEST_F(ESP8266_recv, okWithId)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("", "+IPD,1,4:abcd\r\n\r\nOK\r\n");

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(4, dataSize);
	EXPECT_EQ(1, coming_mux_id);
	EXPECT_STREQ("abcd", (const char*)buffer);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, okWithoutId)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("", "+IPD,4:abcd\r\n\r\nOK\r\n");

	// running code...
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(buffer, buffer_size, timeout);

	EXPECT_EQ(4, dataSize);
	EXPECT_STREQ("abcd", (const char*)buffer);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, goodDataBadTail)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("", "+IPD,1,4:abcd\r\n\r\nError\r\n");

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(4, dataSize);
	EXPECT_EQ(1, coming_mux_id);
	EXPECT_STREQ("abcd", (const char*)buffer);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, goodDataNoTail)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("", "+IPD,1,4:abcd");

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(4, dataSize);
	EXPECT_EQ(1, coming_mux_id);
	EXPECT_STREQ("abcd", (const char*)buffer);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, okUnsufficienBuffer)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("", "+IPD,1,4:abcd\r\n\r\nOK\r\n");

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 2;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(2, dataSize);
	EXPECT_EQ(1, coming_mux_id);
	EXPECT_TRUE(strncmp("abcd", (const char*)buffer, 2) == 0);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, okUnsufficientData)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("", "+IPD,1,4:ab");

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(0, dataSize);
	EXPECT_EQ((uint8_t)-1, coming_mux_id);
	EXPECT_STREQ("ab", (char*)buffer);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, multiParts)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("", "+IPD,1,4:ab");
	EXPECT_SERIAL_DATA("",			 "cd\r\n\r");
	EXPECT_SERIAL_DATA("",					  "\nOK\r");
	EXPECT_SERIAL_DATA("",							 "\n");

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(4, dataSize);
	EXPECT_EQ(1, coming_mux_id);
	EXPECT_STREQ("abcd", (char*)buffer);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, extraData)
{
	// expectations
	EXPECT_SERIAL_DATA("", "");
	EXPECT_SERIAL_DATA("", "+IPD,1,4:abcdEXTRA\r\n\r\nOK\r\n");

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(4, dataSize);
	EXPECT_EQ(1, coming_mux_id);
	EXPECT_EQ(std::string("abcd"), std::string((char*)buffer));
	EXPECT_STREQ("abcd", (char*)buffer);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, timeoutInHeader)
{
	// expectations
	EXPECT_SERIAL_DATA("", "+IP");
	EXPECT_SERIAL_DATA("",	 "D,1,4:abcd\r\n\r\nOK\r\n");
	// Timeout here:		   ^

	{
		InSequence s;

		// start + first calls
		EXPECT_CALL(*arduinoMock, millis())
			.Times(1 + 3)
			.WillRepeatedly(DoDefault())
			.RetiresOnSaturation();

		// timeout
		EXPECT_CALL(*arduinoMock, millis())
			.Times(1)
			.WillOnce(Invoke([this]() -> unsigned long { return arduinoMock->new_fake_millis(999999); }))
			.RetiresOnSaturation();
	}

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(0, dataSize);
	EXPECT_EQ((uint8_t)-1, coming_mux_id);

	VerifySerialRequests();
}

TEST_F(ESP8266_recv, timeoutInData)
{
	// expectations
	EXPECT_SERIAL_DATA("", "+IPD,1,4:ab");
	EXPECT_SERIAL_DATA("",			 "cd\r\n\r\nOK\r\n");
	// Timeout here:				   ^

	{
		InSequence s;

		// start + first calls
		EXPECT_CALL(*arduinoMock, millis())
			.Times(1 + 11)
			.WillRepeatedly(DoDefault())
			.RetiresOnSaturation();

		// timeout
		EXPECT_CALL(*arduinoMock, millis())
			.Times(1)
			.WillOnce(Invoke([this]() -> unsigned long { return arduinoMock->new_fake_millis(999999); }))
			.RetiresOnSaturation();
	}

	// running code...
	uint8_t coming_mux_id = -1;
	const uint32_t buffer_size = 128;
	uint8_t buffer[buffer_size] = {0};
	uint32_t timeout = 100;

	uint32_t dataSize = esp->recv(&coming_mux_id, buffer, buffer_size, timeout);

	EXPECT_EQ(0, dataSize);
	EXPECT_EQ((uint8_t)-1, coming_mux_id);
	//EXPECT_EQ(std::string("abcd"), std::string((char*)buffer));

	VerifySerialRequests();
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	return ret;
}
