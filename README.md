# str Module

This module provides a string buffer (`strbf`) functionality. It allows you to create a buffer, append strings to it, retrieve its contents, and clear it when done.

## Example Usage

Here's an example of how to use the `strbf` module:

#include "strbf.h"

void example_usage() {
    strbf_t buffer;
    strbf_init(&buffer, 128); // Initialize the buffer with a capacity of 128

    strbf_append(&buffer, "Hello, "); // Append a string to the buffer
    strbf_append(&buffer, "World!"); // Append another string to the buffer

    printf("%s\n", strbf_get(&buffer)); // Print the contents of the buffer

    strbf_clear(&buffer); // Clear the buffer
}
