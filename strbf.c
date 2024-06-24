#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "strbf.h"
#include "logger_common.h"
#include "str.h"

#define isset(x) (x)
#define is_spacing(x) (*(x) == ' ' || *(x) == '\t' || *(x) == '\r' || *(x) == '\n')
#define is_spacing_safe(x) (isset(x) && is_spacing(x))

#define SB strbf_t
#ifndef BUFSIZ
#define BUFSIZ 512
#endif

SB *strbf_init(SB *sb) {
  assert(sb);
  sb->start = calloc(sizeof(char), BUFSIZ / 2);
  sb->cur = sb->start;
  sb->end = sb->start + BUFSIZ / 2 - 1;
  sb->max = 0;
  return sb;
}

SB *strbf_inits(SB *sb, char *str, size_t len) {
  assert(sb && str);
  memset(str, 0, len);
  sb->start = str;
  sb->cur = sb->start;
  sb->end = 0;
  sb->max = sb->start + len - 1;
  return sb;
}

SB *strbf_reset(SB *sb) {
  assert(sb);
  if (!sb->start)
    strbf_init(sb);
  else {
    memset(sb->start, 0, (sb->max ? sb->max : sb->end) - sb->start);
    sb->cur = sb->start;
  }
  return sb;
}

/* sb and need may be evaluated multiple times. */
#define sb_need(sb, need)                                                      \
  do {                                                                         \
    if ((sb)->end - (sb)->cur < (need))                                        \
      sb_grow(sb, need);                                                       \
  } while (0)

static void sb_grow(SB *sb, size_t need) {
  assert(sb && sb->start);
  assert(!sb->max || sb->max - sb->cur > need);
  if (sb->max)
    return;
  size_t length = sb->cur - sb->start;
  size_t alloc = sb->end - sb->start;

  do {
    alloc *= 2;
  } while (alloc < length + need);

  // sb->start = (char*) realloc(sb->start, alloc + 1);
  char *data = calloc(sizeof(char), (int)alloc + 1);
  memcpy(data, sb->start, length);
  free(sb->start);
  sb->start = data;
  sb->cur = sb->start + length;
  sb->end = sb->start + alloc;
}

void strbf_put(SB *sb, const char *bytes, size_t count) {
  assert(sb && sb->start);
  if (bytes && count) {
    sb_need(sb, count);
    memcpy(sb->cur, bytes, count);
    sb->cur += count;
  }
}

void strbf_putu(SB *sb, const uint8_t *bytes, size_t count) {
  assert(sb && sb->start);
  if (bytes && count) {
    sb_need(sb, count);
    memcpy(sb->cur, bytes, count);
    sb->cur += count;
  }
}

/*
#define strbf_putc(sb, c) do {         \
        if ((sb)->cur >= (sb)->end) \
            sb_grow(sb, 1);         \
        *(sb)->cur++ = (c);         \
    } while (0)
*/

void strbf_putc(SB *sb, const char c) {
  assert(sb && sb->start);
  if (sb->cur >= sb->end)
    sb_grow(sb, 1);
  *sb->cur++ = c;
}

SB *strbf_puts(SB *sb, const char *str) {
  if (str)
    strbf_put(sb, str, strlen(str));
  return sb;
}

SB *strbf_puts_v(SB *sb, int argc, ...) {
  assert(sb);
  va_list ptr;
  va_start(ptr, argc);
  const char *str = 0;
  register int i = 0;
  for (; i < argc; ++i) {
    str = va_arg(ptr, const char *);
    strbf_puts(sb, str);
  }
  return sb;
}

SB *strbf_sprintf(SB *sb, const char *fmt, ...) {
  assert(sb);
  va_list ptr;
  va_start(ptr, fmt);
  size_t len = vsnprintf(0, 0, fmt, ptr); // get the length
  va_end(ptr);
  va_start(ptr, fmt);
  sb_need(sb, len);
  vsnprintf(sb->cur, len + 1, fmt, ptr);
  sb->cur += len;
  va_end(ptr);
  return sb;
}

void strbf_putl(SB *sb, long val) {
  char i[16] = {0}, *p = i;
  size_t len = xltoa(val, p);
  strbf_put(sb, p, len);
}

void strbf_putul(SB *sb, unsigned long val) {
  char i[16] = {0}, *p = i;
  size_t len = xltoa(val, p);
  strbf_put(sb, p, len);
}

void strbf_putf(SB *sb, float val) {
  char i[16] = {0}, *p = i;
  xftoa(val, p, 16);
  strbf_put(sb, p, strlen(p));
}

void strbf_putd_b(SB *sb, double val, const int8_t width, const uint8_t perc, const uint8_t mark) {
  char i[16] = {0}, *p = i;
  xdtostrf_b(val, width,  perc, p, mark);
  strbf_put(sb, p, strlen(p));
}

void strbf_putd(SB *sb, double val, const int8_t width, const uint8_t perc) {
  strbf_putd_b(sb, val, width, perc, ' ');
}

/*
    path related string manipulations
*/

static SB *_put_pathsep(SB *sb, char sep) {
  assert(sb);
  if (!sep)
    sep = '/';
  if (sb->start && sb->cur && sb->cur > sb->start && *(sb->cur - 1) != sep)
    strbf_putc(sb, sep);
  return sb;
}

SB *strbf_put_pathsep(SB *sb) {
  assert(sb);
  _put_pathsep(sb, 0);
  return sb;
}

SB *strbf_put_urisep(SB *sb) {
  assert(sb);
  _put_pathsep(sb, '/');
  return sb;
}

SB *_put_path(SB *sb, const char *str, char sep) {
  assert(sb);
  if (str) {
    if (*str == sep) {
      if (sb->cur > sb->start && *(sb->cur - 1) == sep)
        --sb->cur;
    } else {
      _put_pathsep(sb, sep);
    }
    strbf_puts(sb, str);
  }
  return sb;
}

SB *_put_path_n(SB *sb, const char *str, char sep, size_t len) {
  assert(sb);
  if (str) {
    if (*str == sep) {
      if (sb->cur > sb->start && *(sb->cur - 1) == sep)
        --sb->cur;
    } else {
      _put_pathsep(sb, sep);
    }
    strbf_put(sb, str, len);
  }
  return sb;
}

SB *strbf_put_path(SB *sb, const char *str) { return _put_path(sb, str, '/'); }

SB *strbf_put_path_n(SB *sb, const char *str, size_t len) { return _put_path_n(sb, str, '/', len); }

SB *strbf_put_path_at(SB *sb, const char *str, size_t len) {
  assert(sb);
  strbf_shape(sb, len);
  return strbf_put_path(sb, str);
}

SB *strbf_put_uri(SB *sb, const char *str) { return _put_path(sb, str, '/'); }

SB *strbf_put_uri_at(SB *sb, const char *str, size_t len) {
  assert(sb);
  strbf_shape(sb, len);
  return strbf_put_uri(sb, str);
}

SB *strbf_put_path_v(SB *sb, int argc, ...) {
  assert(sb);
  va_list ptr;
  va_start(ptr, argc);
  const char *str = 0;
  register int i = 0;
  for (; i < argc; ++i) {
    str = va_arg(ptr, const char *);
    strbf_put_path(sb, str);
  }
  return sb;
}

SB *strbf_put_uri_v(SB *sb, int argc, ...) {
  assert(sb);
  va_list ptr;
  va_start(ptr, argc);
  const char *str = 0;
  register int i = 0;
  for (; i < argc; ++i) {
    str = va_arg(ptr, const char *);
    strbf_put_uri(sb, str);
  }
  return sb;
}

static SB *_pop_path(SB *sb, char sep) {
  assert(sb);
  if (sb->cur) {
    if (!sep)
      sep = '/';
    char *cur = sb->cur - 1;
    if (cur && *cur == sep) {
      --cur;
    }
    while (cur && cur > sb->start && *cur != sep)
      --cur;
    if (cur > sb->start && *cur == sep) {
      strbf_pop(sb, sb->cur - cur);
    }
  }
  return sb;
}

SB *strbf_pop_path(SB *sb) { return _pop_path(sb, 0); }

SB *strbf_pop_url(SB *sb) { return _pop_path(sb, '/'); }

static SB *_insert_pathsep(SB *sb, char sep, size_t at) {
  assert(sb);
  if (!sep)
    sep = '/';
  char *a = sb->start + at;
  assert(a >= sb->start && a <= sb->cur);
  if (a > sb->start && *(a - 1) != sep)
    strbf_insertc(sb, sep, at++);

  return sb;
}

SB *strbf_insert_pathsep(SB *sb, size_t at) {
  assert(sb);
  _insert_pathsep(sb, 0, at);
  return sb;
}

SB *strbf_insert_urisep(SB *sb, size_t at) {
  assert(sb);
  _insert_pathsep(sb, '/', at);
  return sb;
}

static SB *_insert_path(SB *sb, const char *str, size_t at, char sep) {
  assert(sb);
  if (str) {
    char *a = sb->start + at, *b = a;
    assert(a >= sb->start && a <= sb->cur);
    if (*str == sep) {
      if (a > sb->start && *(a - 1) == sep) {
        a -= 1;
        --at;
      }
    } else {
      if (a > sb->start && *(a) == sep) {
        a += 1;
        ++at;
      }
    }
    if (a != b)
      _insert_pathsep(sb, sep, at);
    strbf_inserts(sb, str, at);
  }
  return sb;
}

SB *strbf_insert_path(SB *sb, const char *str, size_t at) {
  return _insert_path(sb, str, at, '/');
}

SB *strbf_insert_uri(SB *sb, const char *str, size_t at) {
  return _insert_path(sb, str, at, '/');
}

void strbf_concat(SB *sb, const char *str, size_t count) {
  assert(sb && sb->cur);
  strbf_put(sb, str, count);
  *sb->cur = 0;
}

void strbf_concatc(SB *sb, const char c) {
  assert(sb && sb->cur);
  strbf_putc(sb, c);
  *sb->cur = 0;
}

void strbf_concats(SB *sb, const char *str) {
  if (str) {
    assert(sb && sb->cur);
    strbf_put(sb, str, strlen(str));
    *sb->cur = 0;
  }
}

void strbf_insert(SB *sb, const char *str, size_t after, size_t count) {
  if (str) {
    assert(sb && sb->cur);
    if (!count)
      count = strlen(str);
    sb_need(sb, count);
    memmove(sb->start + after + count, sb->start + after,
            sb->cur - sb->start + after);
    memcpy(sb->start + after, str, count);
    sb->cur += count;
    //*sb->cur=0;
  }
}

void strbf_insertc(SB *sb, const char str, size_t after) {
  if (str) {
    assert(sb && sb->cur);
    if (sb->cur >= sb->end)
      sb_grow(sb, 1);
    memmove(sb->start + after + 1, sb->start + after,
            sb->cur - sb->start + after);
    *(sb->start + after) = str;
    sb->cur += 1;
    //*sb->cur=0;
  }
}

void strbf_inserts(SB *sb, const char *str, size_t after) {
  if (str) {
    strbf_insert(sb, str, after, strlen(str));
  }
}

void strbf_prepend(SB *sb, const char *str, size_t count) {
  if (str) {
    assert(sb && sb->cur);
    if (!count)
      count = strlen(str);
    sb_need(sb, count);
    memmove(sb->start + count, sb->start, sb->cur - sb->start);
    memcpy(sb->start, str, count);
    sb->cur += count;
    //*sb->cur=0;
  }
}

void strbf_prependc(SB *sb, const char c) {
  assert(sb && sb->start);
  if (sb->cur >= sb->end)
    sb_grow(sb, 1);
  memmove(sb->start + 1, sb->start, sb->cur - sb->start);
  *sb->start = c;
  sb->cur += 1;
  //*sb->cur=0;
}

void strbf_prepends(SB *sb, const char *str) {
  if (str) {
    strbf_prepend(sb, str, strlen(str));
  }
}

void strbf_shift(SB *sb, size_t count) {
  if (count) {
    assert(sb && sb->start);
    memmove(sb->start, sb->start + count, sb->cur - sb->start - count);
    sb->cur -= count;
  }
}

void strbf_pop(SB *sb, size_t count) {
  if (count) {
    assert(sb && sb->start);
    sb->cur -= count;
  }
}

strbf_t *strbf_shape(SB *sb, size_t count) {
    assert(sb && sb->start);
    sb->cur = sb->start + count;
    *sb->cur = 0;
  return sb;
}

void strbf_trim(SB *sb) {
  assert(sb && sb->start);
  if (sb->start) {
    // Right trim
    while (sb->cur && is_spacing((sb->cur - 1))) {
      --sb->cur;
    }
    // Left trim
    if (sb->start && is_spacing(sb->start)) {
      int i;
      for (i = 0; is_spacing((sb->start + i)); i++)
        ;
      memmove(sb->start, sb->start + i, sb->cur - sb->start - i);
      sb->cur -= i;
    }
  }
}

char *strbf_finish(SB *sb) {
  assert(sb && sb->start);
  *sb->cur = 0;
  assert(sb->start <= sb->cur && strlen(sb->start) == (size_t)(sb->cur - sb->start));
  return sb->start;
}

char *strbf_finish_url(SB *sb) {
  _put_pathsep(sb, '/');
  return strbf_finish(sb);
}

char *strbf_get(const SB *sb) {
  assert(sb && sb->start);
  return sb->start;
}

size_t strbf_len(SB *sb) {
  assert(sb && sb->start);
  return sb->cur - sb->start;
}

char *strbf_cur(SB *sb) {
  assert(sb && sb->start);
  return sb->cur;
}

void strbf_free(SB *sb) {
  if (sb && sb->start && !sb->max)
    free(sb->start);
}

#undef SB
