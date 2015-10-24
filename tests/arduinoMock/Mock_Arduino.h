/**
 * @file Mock_Arduino.h
 * @brief The definition of Arduino mockups.
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

#ifndef ESPDUINO_MOCK_ARDUINO_H_
#define ESPDUINO_MOCK_ARDUINO_H_

#include <stdint.h>
#include <gmock/gmock.h>
#include <string>

class Mock_Arduino
{
  public:
	//void pinMode(uint8_t, uint8_t) {};
	MOCK_METHOD2(pinMode, void (uint8_t, uint8_t));
	MOCK_METHOD2(digitalWrite, void (uint8_t, uint8_t));
	MOCK_METHOD1(digitalRead, int (int));

	MOCK_METHOD0(millis, unsigned long ());
	MOCK_METHOD1(delay, void (unsigned long));

	MOCK_METHOD1(pgm_read_byte, uint8_t (const uint8_t* address));

	Mock_Arduino();

	unsigned long millisValue;

	unsigned long fake_millis();
	unsigned long new_fake_millis(unsigned long newValue);
};

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);

unsigned long millis();
void delay(unsigned long);

uint8_t pgm_read_byte(const uint8_t* address);

using ::testing::NiceMock;

NiceMock<Mock_Arduino>* createArduinoMock();
void releaseArduinoMock();

#endif
