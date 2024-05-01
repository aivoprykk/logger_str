#ifndef A3AAFBC2_CD6B_4EA0_8931_09A55CC00C85
#define A3AAFBC2_CD6B_4EA0_8931_09A55CC00C85

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <stdint.h>
#include <stddef.h>

#define SB strbf_t
    
    typedef struct strbf_s {
        char * cur;
        char * end;
        char * start;
        char * max;
    } strbf_t;
    
    /**
     * @brief Initialize string buffer
     *     If sb is NULL, allocate new buffer
     *     If sb is not NULL, reset buffer
     * @param sb - pointer to string buffer
     * @return pointer to string buffer
     * */    
    SB * strbf_init(SB *sb);

    /**
     * @brief Initialize string buffer with string
     *     If sb is NULL, allocate new buffer
     *     If sb is not NULL, reset buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @param len - length of string
     * @return pointer to string buffer
     * */
    SB * strbf_inits(SB *sb, char * str, size_t len);

    /**
     * @brief Reset string buffer
     * @param sb - pointer to string buffer
     * @return pointer to string buffer
     * */
    SB * strbf_reset(SB *sb);

    /**
     * @brief Put count bytes into string buffer
     * @param sb - pointer to string buffer
     * @param size - new size of buffer
     * */
    void strbf_put(SB *sb, const char *bytes, size_t count);

    /**
     * @brief Put count unsigned bytes into string buffer
     * @param sb - pointer to string buffer
     * @param size - new size of buffer
     * */
    void strbf_putu(SB *sb, const uint8_t * str, size_t count);

    /**
     * @brief Put char into string buffer
     * @param sb - pointer to string buffer
     * @param size - new size of buffer
     * */
    void strbf_putc(SB *sb, const char str);

    /**
     * @brief Put string into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @return pointer to string buffer
     * */
    SB * strbf_puts(SB *sb, const char *str);

    /**
     * @brief Put string into string buffer
     * @param sb - pointer to string buffer
     * @param argc - number of strings
     * @param ... - arguments
     * @return pointer to string buffer
     * */
    SB * strbf_puts_v(SB *sb, int argc, ...);
    
    /**
     * @brief Put arguments into string buffer according to format
     * @param sb - pointer to string buffer
     * @param fmt - format
     * @param ... - arguments
     * @return pointer to string buffer
    */
    SB *strbf_sprintf(SB *sb, const char *fmt, ...);

    /**
     * @brief Put int32_t into string buffer
     * @param sb - pointer to string buffer
     * @param val - long value
     * */
    void strbf_putl(SB *sb, long val);
    #define strbf_putn strbf_putl

    /**
     * @brief Put uint32_t into string buffer
     * @param sb - pointer to string buffer
     * @param val - unsigned long value
     * */
    void strbf_putul(SB *sb, uint32_t val);

    /**
     * @brief Put float into string buffer
     * @param sb - pointer to string buffer
     * @param val - float value
     * */
    void strbf_putf(SB *sb, float val);

    /**
     * @brief Put double into string buffer
     * @param sb - pointer to string buffer
     * @param val - double value
     * @param width - width of integer field
     * @param perc - precision
     * @param mark - mark to use for decimal point
     * */
    void strbf_putd_b(SB *sb, double val, const int8_t width, const uint8_t perc, const uint8_t mark);

    /**
     * @brief Put double into string buffer
     * @param sb - pointer to string buffer
     * @param val - double value
     * @param width - width of integer field
     * @param perc - precision
     * */
    void strbf_putd(SB *sb, double val, const int8_t width, const uint8_t perc);

    /**
     * @brief Put path separator into string buffer
     * @param sb - pointer to string buffer
     * */
    SB * strbf_put_pathsep(SB *sb);

    /**
     * @brief Put path into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @return pointer to string buffer
     * */
    SB * strbf_put_path(SB *sb, const char *str);

    /**
     * @brief Put path into string buffer
     * @param sb - pointer to string buffer
     * @param argc - number of strings
     * @param ... - arguments
     * @return pointer to string buffer
     * */
    SB * strbf_put_path_v(SB *sb, int argc, ...);

    /**
     * @brief Put path into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @param len - length of string
     * @return pointer to string buffer
     * */
    SB * strbf_put_path_at(SB *sb, const char *str, size_t len);

    /**
     * @brief Pop path from string buffer
     * @param sb - pointer to string buffer
     * @return pointer to string buffer
     * */
    SB * strbf_pop_path(SB *sb);

    /**
     * @brief Put path into string buffer
     * @param sb - pointer to string buffer
     * @param len - length of string
     * @return pointer to string buffer
     * */
    SB * strbf_put_path_n(SB *sb, const char *str, size_t len);

    /**
     * @brief Put uri separator into string buffer
     * @param sb - pointer to string buffer
     * */
    SB * strbf_put_urisep(SB *sb);

    /**
     * @brief Put uri into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @return pointer to string buffer
     * */
    SB * strbf_put_uri(SB *sb, const char *str);

    /**
     * @brief Put uri into string buffer
     * @param sb - pointer to string buffer
     * @param argc - number of strings
     * @param ... - arguments
     * @return pointer to string buffer
     * */
    SB * strbf_put_uri_v(SB *sb, int argc, ...);

    /**
     * @brief Put uri into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @param len - length of string
     * @return pointer to string buffer
     * */
    SB * strbf_put_uri_at(SB *sb, const char *str, size_t len);
    SB * strbf_pop_uri(SB *sb);

    /**
     * @brief Put string into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @param count - length of string
     * @return pointer to string buffer
     * */
    void strbf_concat(SB *sb, const char *str, size_t count);

    /**
     * @brief Put char into string buffer
     * @param sb - pointer to string buffer
     * @param c - char
     * @return pointer to string buffer
     * */
    void strbf_concatc(SB *sb, const char c);

    /**
     * @brief Put string into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @return pointer to string buffer
     * */
    void strbf_concats(SB *sb, const char *str);

    /** insert string into string */
    /**
     * @brief Insert string into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @param after - position
     * @param count - length of string
     * */
    void strbf_insert(SB *sb, const char*str, size_t after, size_t count);

    /**
     * @brief Insert char into string buffer
     * @param sb - pointer to string buffer
     * @param c - char
     * @param after - position
     * */
    void strbf_insertc(SB *sb, const char c, size_t after);

    /**
     * @brief Insert string into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @param after - position
     * */
    void strbf_inserts(SB *sb, const char *str, size_t after);

    /**
     * @brief Insert path separator into string buffer
     * @param sb - pointer to string buffer
     * @param at - position
     * */
    SB * strbf_insert_pathsep(SB *sb, size_t at);

    /**
     * @brief Insert uri separator string buffer
     * @param sb - pointer to string buffer
     * @param at - position
     * */
    SB * strbf_insert_urisep(SB *sb, size_t at);

    /**
     * @brief Insert path into string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @param at - position
     * @return pointer to string buffer
     * */
    SB * strbf_insert_path(SB *sb, const char *str, size_t at);
    
    /**
     * @brief Prepend path into beginning of string buffer
     * @param sb - pointer to string buffer
     * @param bytes - string
     * @param count - length of string
     * @return pointer to string buffer
     * */
    void strbf_prepend(SB *sb, const char *bytes, size_t count);

    /**
     * @brief Prepend char into beginning of string buffer
     * @param sb - pointer to string buffer
     * @param c - char
     * @return pointer to string buffer
     * */
    void strbf_prependc(SB *sb, const char c);

    /**
     * @brief Prepend string into beginning of string buffer
     * @param sb - pointer to string buffer
     * @param str - string
     * @return pointer to string buffer
     * */
    void strbf_prepends(SB *sb, const char *str);
    
    /**
     * @brief Trim string buffer
     * @param sb - pointer to string buffer
     * @return pointer to string buffer
     * */
    void strbf_trim(SB *sb);
    
    /**
     * @brief Cut schars from beginning of string buffer
     * @param sb - pointer to string buffer
     * @param count - length of string
     * @return pointer to string buffer
     * */
    void strbf_shift(SB *sb, size_t count);
    
    /**
     * @brief Cut schars from end of string buffer
     * @param sb - pointer to string buffer
     * @param count - length of string
     * @return pointer to string buffer
     * */
    void strbf_pop(SB *sb, size_t count);

    /**
     * @brief Cut schars from end of string buffer
     * @param sb - pointer to string buffer
     * @param count - length of string
     * @return pointer to string buffer
     * */
    SB * strbf_shape(SB *sb, size_t count);

    /**
     * @brief Finish string buffer, end string with '\0'
     * @param sb - pointer to string buffer
     * @return pointer to string buffer
     * */
    char *strbf_finish(SB *sb);

    /**
     * @brief Finish url buffer, end string with '\0'
     * @param sb - pointer to string buffer
     * @return pointer to string buffer
     * */
    char *strbf_finish_url(SB *sb);

    /**
     * @brief Get string buffer pointer
     * @param sb - pointer to string buffer
     * @return pointer to string buffer
     * */
    char *strbf_get(const SB *sb);

    /**
     * @brief Get string buffer length
     * @param sb - pointer to string buffer
     * @return length of string buffer
     * */
    size_t strbf_len(SB *sb);


    /**
     * @brief Get string buffer end pointer
     * @param sb - pointer to string buffer
     * @return end pointer of string buffer
     * */
    char * strbf_cur(SB *sb);

    /**
     * @brief Free string buffer
     * @param sb - pointer to string buffer
     * */
    void strbf_free(SB *sb);
    
#undef SB
    
#ifdef __cplusplus
}
#endif


#endif /* A3AAFBC2_CD6B_4EA0_8931_09A55CC00C85 */
