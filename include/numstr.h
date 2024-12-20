#ifndef BA653534_F90C_4A44_985A_2B84835006B0
#define BA653534_F90C_4A44_985A_2B84835006B0

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t xultoa(unsigned long value, char *dst);
size_t xltoa(long value, char *dst);
size_t xint_len(size_t value);

char *xftoa(double f, char *buf, int precision);

char *xdtostrf_b(double val, const int8_t width, const uint8_t prec, char *sout, const uint8_t pad_digit);
char *xdtostrf(double val, const int8_t width, const uint8_t prec, char *sout);

size_t int_to_char(int32_t f, char *str);
size_t uint_to_char(uint32_t f, char *str);

size_t time_to_char_hm(int16_t h, int16_t m, char *str);
size_t time_to_char_hms(uint8_t h, uint8_t m, uint8_t s, char *str);
size_t date_to_char(int16_t d, int16_t m, int16_t y, uint8_t format, char *str);
size_t sec_to_hms_str(uint32_t sec, char *str);

size_t f_to_char_f(double f, char *str, uint8_t fractionlen, uint8_t padlen);
size_t f_to_char(double f, char *str, uint8_t fractionlength);
size_t f1_to_char(double f, char *str);
size_t f2_to_char(double f, char *str);
size_t f3_to_char(double f, char *str);

size_t concat(char *dst, size_t dlen, const char *src, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* BA653534_F90C_4A44_985A_2B84835006B0 */
