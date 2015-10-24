/**
 * @file String.h
 * @brief The definition of fake String class for Arduino.
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

#ifndef ESPDUINO_STRING_H_
#define ESPDUINO_STRING_H_

#include <stdint.h>
#include <string>

#define F(str) str


class String : public std::string
{
public:
	String() {};
	String(const std::string& str);
	String(const char* buffer);

	int indexOf(char ch) const;
	int indexOf(char ch, unsigned int fromIndex) const;
	int indexOf(const char* target) const;
	int indexOf(const String &str) const;
	int indexOf(const String &str, unsigned int fromIndex) const;

	String substring(unsigned int left) const;
	String substring(unsigned int left, unsigned int right) const;

	long toInt(void) const;
};


#endif
