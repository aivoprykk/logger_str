# String Utilities Module
The strutil module contains two headers with the following functionality:

The `strutil` module is designed to provide efficient and easy-to-use utilities for string manipulation and numeric conversions. These utilities are essential for various applications, including logging, data processing, and user interface development. By using this module, developers can simplify their code and improve performance when working with strings and numbers.

# strbf.h
The strbf.h provides a string buffer functionality. It allows you to create a buffer, append strings to it, retrieve its contents, and clear it when done. This is useful for efficiently managing and manipulating strings in memory.

## Example Usage
Here's an example of how to use the strbf.h:
```c
#include "strbf.h"

void example_usage() {
    strbf_t buffer;
    strbf_init(&buffer); // Initialize buffer

    strbf_puts(&buffer, "Hello,"); // Append a string to the buffer
    strbf_putc(&buffer, ' ');
    strbf_putd(&buffer, 1924);
    strbf_putc(&buffer, ' ');
    strbf_puts(&buffer, "World!"); // Append another string to the buffer

    printf("%s\n", strbf_finish(&buffer)); // Print the contents of the buffer

    // Additional functions
    printf("Buffer contents: %s\n", strbf_get(&buffer)); // Get string buffer pointer
    printf("Buffer length: %zu\n", strbf_len(&buffer)); // Get string buffer length
    printf("Buffer end pointer: %p\n", strbf_cur(&buffer)); // Get string buffer end pointer

    strbf_free(&buffer); // Clear the buffer
}
```

## Functions
- strbf_init(strbf_t *buffer): Initialize a string buffer.
- strbf_puts(strbf_t *buffer, const char *str): Append a string to the buffer.
- strbf_putc(strbf_t *buffer, char c): Append a character to the buffer.
- strbf_putd(strbf_t *buffer, int num): Append an integer to the buffer.
- strbf_finish(strbf_t *buffer): Retrieve the contents of the buffer.
- strbf_free(strbf_t *buffer): Clear the buffer.
- char *strbf_get(const SB *sb): Get string buffer pointer.
- size_t strbf_len(SB *sb): Get string buffer length.
- char *strbf_cur(SB *sb): Get string buffer end pointer.

# numstr.h
The numstr provides functionality for converting numbers to strings. It includes functions for converting integers, floating-point numbers, and other numeric types to their string representations.

## Example Usage
Here's an example of how to use the numstr.h:
```c
#include "numstr.h"

void example_usage() {
    char buffer[32];

    int num = 123;
    xltoa(num, buffer); // Convert integer to string
    printf("Integer: %s\n", buffer);

    float fnum = 123.45;
    xftoa(fnum, buffer, 2); // Convert float to string with 2 decimal places
    printf("Float: %s\n", buffer);

    // Additional function
    size_t len = xint_len(num); // Get the length of the integer when converted to a string
    printf("Length of integer: %zu\n", len);
}
```

## Functions
- size_t xultoa(unsigned long value, char *dst): Convert an unsigned long integer to a string.
- size_t xltoa(long value, char *dst): Convert a long integer to a string.
- size_t xint_len(size_t value): Get the length of an integer when converted to a string.
- char *xftoa(double f, char *buf, int precision): Convert a float to a string with specified precision.
- char *xdtostrf_b(double val, const int8_t width, const uint8_t prec, char *sout, const uint8_t pad_digit): Convert a double to a string with specified width, precision, and padding.
- char *xdtostrf(double val, const int8_t width, const uint8_t prec, char *sout): Convert a double to a string with specified width and precision.
- size_t int_to_char(int32_t f, char *str): Convert an integer to a string.
- size_t uint_to_char(uint32_t f, char *str): Convert an unsigned integer to a string.
- size_t time_to_char_hm(int16_t h, int16_t m, char *str): Convert hours and minutes to a string.
- size_t time_to_char_hms(uint8_t h, uint8_t m, uint8_t s, char *str): Convert hours, minutes, and seconds to a string.
- size_t date_to_char(int16_t d, int16_t m, int16_t y, uint8_t format, char *str): Convert a date to a string with specified format.
- size_t sec_to_hms_str(uint32_t sec, char *str): Convert seconds to a string in hours, minutes, and seconds format.
- size_t f_to_char_f(double f, char *str, uint8_t fractionlen, uint8_t padlen): Convert a float to a string with specified fraction length and padding.
- size_t f_to_char(double f, char *str, uint8_t fractionlength): Convert a float to a string with specified fraction length.
- size_t f1_to_char(double f, char *str): Convert a float to a string with 1 decimal place.
- size_t f2_to_char(double f, char *str): Convert a float to a string with 2 decimal places.
- size_t f3_to_char(double f, char *str): Convert a float to a string with 3 decimal places.
- size_t concat(char *dst, size_t dlen, const char *src, size_t len): Concatenate two strings.

# Integration

## ESP-IDF
To integrate the logger_str component into an ESP-IDF project, clone the GitHub repository into the components directory of your project:
```
cd components; 
git clone https://github.com/aivoprykk/logger_str.git
```

## Standalone
To use the strutil module as a standalone project, clone the GitHub repository and build it using CMake:
```
git clone https://github.com/aivoprykk/logger_str.git
cd logger_str
mkdir build
cd build
cmake ..
make && make install
```
