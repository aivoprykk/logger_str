/**
 *  stm32tpl --  STM32 C++ Template Peripheral Library
 *  Visit https://github.com/antongus/stm32tpl for new versions
 *
 *  Copyright (c) 2011-2020 Anton B. Gusev aka AHTOXA
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 *
 *  file         : ftoa.h
 *  description  : convert double to string
 *
 */

#ifndef BA653534_F90C_4A44_985A_2B84835006B0
#define BA653534_F90C_4A44_985A_2B84835006B0

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t xultoa(uint32_t value, char *dst);
size_t xltoa(int32_t value, char *dst);
char *xftoa(double f, char *buf, int precision);
char *xdtostrf_b(double val, const int8_t width, const uint8_t prec, char *sout, const uint8_t pad_digit);
char *xdtostrf(double val, const int8_t width, const uint8_t prec, char *sout);

size_t int_to_char(int32_t f, char *str);
size_t uint_to_char(uint32_t f, char *str);

size_t time_to_char_hm(int16_t h, int16_t m, char *str);
size_t time_to_char_hms(uint8_t h, uint8_t m, uint8_t s, char *str);
size_t date_to_char(int16_t d, int16_t m, int16_t y, uint8_t format, char *str);

size_t f_to_char_f(double f, char *str, uint8_t fractionlen, uint8_t padlen);
size_t f_to_char(double f, char *str, uint8_t fractionlength);
size_t f1_to_char(double f, char *str);
size_t f2_to_char(double f, char *str);
size_t f3_to_char(double f, char *str);

#ifdef __cplusplus
}
#endif

#endif /* BA653534_F90C_4A44_985A_2B84835006B0 */
