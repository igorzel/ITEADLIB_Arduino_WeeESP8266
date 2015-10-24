/**
 * @file String.cpp
 * @brief The implementation of fake String class for Arduino.
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

#include "String.h"

String::String(const std::string& str)
{
	assign(str);
}

String::String(const char* buffer)
{
	assign(buffer);
}


int String::indexOf(char c) const
{
	return find(c, 0);
}

int String::indexOf( char ch, unsigned int fromIndex ) const
{
	return find(ch, fromIndex);
}

int String::indexOf(const char* target) const
{
	return find(std::string(target), 0);
}

int String::indexOf(const String &s2) const
{
	return find(s2, 0);
}

int String::indexOf(const String &s2, unsigned int fromIndex) const
{
	return find(s2, fromIndex);
}

String String::substring(unsigned int left) const
{
	return substr((size_t)left, length());
}

String String::substring(unsigned int left, unsigned int right) const
{
	const size_t len = right - left;
	return substr((size_t)left, len);
}

long String::toInt(void) const
{
	if (length())
	{
		return atol(c_str());
	}

	return 0;
}
