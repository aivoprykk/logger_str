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
 *  file         : ftoa.c
 *  description  : convert double to string
 *
 */

#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "str.h"
#include "logger_common.h"

#define MAX_PRECISION (10)
static const double rounders[MAX_PRECISION + 1] = {
    0.5,           // 0
    0.05,          // 1
    0.005,         // 2
    0.0005,        // 3
    0.00005,       // 4
    0.000005,      // 5
    0.0000005,     // 6
    0.00000005,    // 7
    0.000000005,   // 8
    0.0000000005,  // 9
    0.00000000005  // 10
};

static const char digits[201] = "0001020304050607080910111213141516171819"
                                "2021222324252627282930313233343536373839"
                                "4041424344454647484950515253545556575859"
                                "6061626364656667686970717273747576777879"
                                "8081828384858687888990919293949596979899";

long xint_len(int32_t value) {
    int l = !value;
    while (value) {
        l++;
        value /= 10;
    }
    return l;
}

char *xftoa(double f, char *buf, int precision) {
    char *ptr = buf;
    char *p = ptr;
    char *p1;
    char c;
    long intPart;

    // check precision bounds
    if (precision > MAX_PRECISION)
        precision = MAX_PRECISION;

    // sign stuff
    if (f < 0) {
        f = -f;
        *ptr++ = '-';
    }

    if (precision < 0)  // negative precision == automatic precision guess
    {
        if (f < 1.0)
            precision = 6;
        else if (f < 10.0)
            precision = 5;
        else if (f < 100.0)
            precision = 4;
        else if (f < 1000.0)
            precision = 3;
        else if (f < 10000.0)
            precision = 2;
        else if (f < 100000.0)
            precision = 1;
        else
            precision = 0;
    }

    // round value according the precision
    if (precision)
        f += rounders[precision];

    // integer part...
    intPart = f;
    f -= intPart;

    if (!intPart)
        *ptr++ = '0';
    else {
        // save start pointer
        p = ptr;

        // convert (reverse order)
        while (intPart) {
            *p++ = '0' + intPart % 10;
            intPart /= 10;
        }

        // save end pos
        p1 = p;

        // reverse result
        while (p > ptr) {
            c = *--p;
            *p = *ptr;
            *ptr++ = c;
        }

        // restore end pos
        ptr = p1;
    }

    // decimal part
    if (precision) {
        // place decimal point
        *ptr++ = '.';

        // convert
        while (precision--) {
            f *= 10.0;
            c = f;
            *ptr++ = '0' + c;
            f -= c;
        }
    }

    // terminating zero
    *ptr = 0;

    return buf;
}

size_t xultoa(uint32_t value, char *dst) {
    assert(dst);
    size_t length = xint_len(value);
    size_t next = length - 1;
    uint32_t i = 0;
    while (value >= 100) {
        i = (value % 100) * 2;
        value /= 100;
        dst[next] = digits[i + 1];
        dst[next - 1] = digits[i];
        next -= 2;
    }
    if (value < 10) {
        dst[next] = '0' + value;
    } else {
        i = value * 2;
        dst[next] = digits[i + 1];
        dst[next - 1] = digits[i];
    }
    dst[length] = 0;
    return length;
}

size_t xltoa(int32_t value, char *dst) {
    assert(dst);
    size_t length = xint_len(value);
    size_t next = length - 1;
    int32_t i = 0;
    if (value < 0) {
        value = -value;
        length++;
        next++;
        *dst = '-';
    }
    while (value >= 100) {
        i = (value % 100) * 2;
        value /= 100;
        dst[next] = digits[i + 1];
        dst[next - 1] = digits[i];
        next -= 2;
    }
    if (value < 10) {
        dst[next] = '0' + value;
    } else {
        i = value * 2;
        dst[next] = digits[i + 1];
        dst[next - 1] = digits[i];
    }
    dst[length] = 0;
    return length;
}

char *xdtostrf(double number, const int8_t width, const uint8_t prec, char *s) {
    return xdtostrf_b(number, width, prec, s, ' ');
}

char *xdtostrf_b(double number, const int8_t width, const uint8_t prec, char *s, const uint8_t pad_digit) {
    uint8_t negative = 0;

    if (isnan(number)) {
        memcpy(s, "nan", 3);
        s[3] = 0;
        return s;
    }
    if (isinf(number)) {
        memcpy(s, "inf", 3);
        s[3] = 0;
        return s;
    }
    char *out = s;

    int fillme = width;  // how many cells to fill for the integer part
    
    if (prec > 0 && pad_digit==' ') {
        fillme -= (prec + 1);
    }

    // Handle negative numbers
    if (number < 0.0) {
        negative = 1;
        fillme--;
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    // I optimized out most of the divisions
    double rounding = 2.0;
    for (uint8_t i = 0; i < prec; ++i)
        rounding *= 10.0;
    rounding = 1.0 / rounding;

    number += rounding;

    // Figure out how big our number really is
    double tenpow = 1.0;
    int digitcount = 1;
    while (number >= 10.0 * tenpow) {
        tenpow *= 10.0;
        digitcount++;
    }

    number /= tenpow;
    fillme -= digitcount;

    if (fillme && pad_digit && pad_digit == ' ') {
        // Pad unused cells with spaces preceeded by minus
        while (fillme-- > 0) {
            *out++ = pad_digit;
        }
    }

    // Handle negative sign
    if (negative)
        *out++ = '-';

    if (fillme && pad_digit && pad_digit == '0') {
        // Pad unused cells with zeroes followed by minus
        while (fillme-- > 0) {
            *out++ = pad_digit;
        }
    }

    // Print the digits, and if necessary, the decimal point
    digitcount += prec;
    int8_t digit = 0;
    while (digitcount-- > 0) {
        digit = (int8_t)number;
        if (digit > 9)
            digit = 9;  // insurance
        *out++ = (char)('0' | digit);
        if ((digitcount == prec) && (prec > 0)) {
            *out++ = '.';
        }
        number -= digit;
        number *= 10.0;
    }

    // make sure the string is terminated
    *out = 0;
    return s;
}

#define MSTRF(fn) if(f > 0){p+=fn(f, p);}else{*p++ = '0';*p = 0;}return p-str;

size_t int_to_char(int32_t f, char *str) {
    char *p = str;MSTRF(xltoa);
}

size_t uint_to_char(uint32_t f, char *str) {
    char *p = str;MSTRF(xultoa);
}

size_t uint_to_char_pad_zero(uint8_t f, char *str) {
    char *p = str;
    if(f<10) *p++ = '0';
    MSTRF(xultoa);
}

size_t time_to_char_hm(int16_t h, int16_t m, char *str) {
    char *p = str;
    for(uint8_t i=0;i<2;++i){
        if(i>0) *p++ = ':';
        p+=uint_to_char_pad_zero((i == 0) ? h : m, p);
    }
    return p-str;
}

size_t time_to_char_hms(uint8_t h, uint8_t m, uint8_t s, char *str) {
    char *p = str;
    uint8_t t;
    p+=time_to_char_hm(h, m, p);
    *p++ = ':';
    p+=uint_to_char_pad_zero(s, p);
    return p-str;
}

size_t date_to_char(int16_t d, int16_t m, int16_t y, uint8_t format, char *str) {
    char *p = str;
    uint16_t t;
    char separator = (format==1) ? '-' : '.';
    for(uint8_t i=0;i<3;++i){
        if(format==1)
            t = (i == 0) ? y : (i == 1) ? m : d;
        else
            t = (i == 0) ? d : (i == 1) ? m : y;
        if(i>0) *p++ = separator;
        if((format==1 && i==0) || (format==0 && i==2)) { // year
            if(!t) memcpy(p, "0000", 4), p+=4;
            else p+=uint_to_char((t>99 && t<=1900) ? t+1900 : t, p);
        }
        else if(t<10) p+=uint_to_char_pad_zero(t, p);
        else p+=uint_to_char(t, p);        
    }
    return p-str;
}

size_t f_to_char_f(double f, char *str, uint8_t fractionlen, uint8_t padlen) {
    bool neg = (f<0); 
    uint32_t full = (uint32_t)(neg?-f:f);
    char *p = str;
    size_t len=0;
    uint8_t i=0;
    uint32_t frac=1;
    if(neg) {
        *p='-';++p;++len;
        if(padlen>0)--padlen;
    }
    while (i<padlen){
        if(f>=1||frac>1){
            if(frac > f) {*p='0';++p;++len;}
        }
        frac *= 10;
        ++i;
    }
    double fr = ((neg?-f:f) - full); // fractional part
    switch
    (fractionlen){
        case 0: 
            if(fr>=0.5) ++full;
        break;
        case 1: 
            if(fr>=0.95) {
                ++full; // round up integer part
                fr=0;
            }
            else if(fr < 0.1 && fr>=0.05) fr=0.1;
        break;
        case 2: 
            if(fr>=0.995) {
                ++full; // round up integer part
                fr=0;
            }
            else if(fr < 0.1 && fr>=0.095) fr=0.1;
            else if(fr < 0.01 && fr>=0.005)  fr=0.01;
        break;
        case 3: 
            if(fr>=0.9995) {
                ++full; // round up integer part
                fr=0;
            }
            else if(fr < 0.1 && fr>=0.095) fr=0.1;
            else if(fr < 0.01 && fr>=0.0095)  fr=0.01;
            else if(fr < 0.001 && fr>=0.0005)  fr=0.001;
        break;
        default: break;
    }

    len += uint_to_char(full, p); p=str+len;
    if(fractionlen){
        *p = '.';++p;++len;
        i = 0;
        while (i<fractionlen){
            fr *= 10;
            if(i<(fractionlen-1) && fr<1) {*p='0';++p;++len;}
            ++i;
        }
        frac = (uint32_t)fr;
        fr -= frac;
        if(fr>=0.5){
            //if(fractionlen==1) frac=0;
            //else 
            ++frac;
        }
        len += uint_to_char(frac, p);
    }
    // printf("f_to_char_f converted, orig:%.03f formatted:%s\n", f, str);
    // if (len>5 && f < 100)
    // printf("nop -----------------------------------------------------------\n");
    return len;
}

size_t f_to_char(double f, char *str, uint8_t fractionlen) {
    return f_to_char_f(f,str,fractionlen, 0);
}

size_t f1_to_char(double f, char *str) {
    return f_to_char(f,str,1);
}

size_t f2_to_char(double f, char *str) {
    return f_to_char(f, str, 2);
}

size_t f3_to_char(double f, char *str) {
    return f_to_char(f, str, 3);
}

size_t concat(char *dst, size_t dlen, const char *src, size_t len) {
    memcpy(dst+dlen, src, len);
    dlen+=len;
    dst[dlen] = 0;
    return dlen;
}

size_t sec_to_hms_str(uint32_t sec, char *str)
{
    uint16_t h = sec / 3600;
    uint16_t m = (sec / 60) % 60;
    uint16_t s = sec % 60;
    char *p = str;
    size_t len = 8;
    memcpy(p, "00:00:00", len), *(p+len)=0;
    // printf ("sec_to_hms_str: %d %d %d\n", h, m, s);
    if(h>0){
        if(h<10) ++p;
        p += xultoa(h, p), *p=':';
    }
    if(m>0){
        p = str+(len-5);
        if(m<10) ++p;
        p += xultoa(m, p), *p=':';
    }
    if(s>0){
        p = str+(len-2);
        if(s<10) ++p;
        p += xultoa(s, p);
    }
    // printf ("sec_to_hms_str: %s\n", str);
    return len;
}