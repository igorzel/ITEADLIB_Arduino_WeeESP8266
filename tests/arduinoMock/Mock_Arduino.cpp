/**
 * @file Mock_Arduino.h
 * @brief The implementation of Arduino mockups.
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

#include <Mock_Arduino.h>

using ::testing::NiceMock;
using ::testing::Invoke;


static NiceMock<Mock_Arduino>* arduinoMock = nullptr;


NiceMock<Mock_Arduino>* createArduinoMock()
{
  if (!arduinoMock)
  {
	arduinoMock = new NiceMock<Mock_Arduino>;
  }

  return arduinoMock;
}

void releaseArduinoMock()
{
  if (arduinoMock)
  {
	delete arduinoMock;
	arduinoMock = nullptr;
  }
}

Mock_Arduino::Mock_Arduino()
{
	millisValue = 1000;

	ON_CALL(*this, millis())
		.WillByDefault(Invoke(this, &Mock_Arduino::fake_millis));
}

unsigned long Mock_Arduino::fake_millis()
{
	return millisValue++;
}

unsigned long Mock_Arduino::new_fake_millis(unsigned long newValue)
{
	millisValue = newValue;
	return millisValue;
}

void pinMode(uint8_t a, uint8_t b)
{
	arduinoMock->pinMode(a, b);
}

void digitalWrite(uint8_t a, uint8_t b)
{
	arduinoMock->digitalWrite(a, b);
}

int digitalRead(uint8_t a)
{
	return arduinoMock->digitalRead(a);
}

unsigned long millis()
{
	return arduinoMock->millis();
}

void delay(unsigned long t)
{
	arduinoMock->delay(t);
}

uint8_t pgm_read_byte(const uint8_t* address)
{
	return arduinoMock->pgm_read_byte(address);
}
