#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
  const unsigned char* bytes = (const unsigned char*)data;
  for (size_t i = 0; i < length; i++)
    if (putchar(bytes[i]) == EOF) return false;
  return true;
}

namespace {
constexpr char HEX_CHARS_UPPER[] = "0123456789ABCDEF";
constexpr char HEX_CHARS_LOWER[] = "0123456789abcdef";

int printf_unsigned(unsigned long long num, int radix, bool lower = false) {
  char buffer[32];
  int pos = 0;
  do {
    auto rem = num % radix;
    num /= radix;
    buffer[pos++] = lower ? HEX_CHARS_LOWER[rem] : HEX_CHARS_UPPER[rem];
  } while (num > 0);
  int str_len = pos;
  while (--pos >= 0)
    if (!putchar(buffer[pos])) return -1;
  return str_len;
}

int printf_signed(long long num, int radix, bool lower = false) {
  bool less_than_zero = num < 0;
  if (less_than_zero) {
    if (!putchar('-')) return -1;
    num = -num;
  }
  return printf_unsigned(num, radix, lower);
}
}  // namespace

int printf(const char* __restrict format, ...) {
  va_list parameters;
  va_start(parameters, format);

  int written_total = 0;

  while (*format != '\0') {
    size_t maxrem = INT_MAX - written_total;
    if (format[0] != '%' || format[1] == '%') {
      if (format[0] == '%') format++;
      size_t amount = 1;
      while (format[amount] && format[amount] != '%') amount++;
      if (maxrem < amount) {
        // TODO: Set errno to EOVERFLOW.
        return -1;
      }
      if (!print(format, amount)) return -1;
      format += amount;
      written_total += amount;
      continue;
    }
    const char* format_begun_at = format++;

    if (*format == 'c') {  // char
      format++;
      char c = (char)va_arg(parameters, int /* char promotes to int */);
      if (!maxrem) {
        // TODO: Set errno to EOVERFLOW.
        return -1;
      }
      if (!print(&c, sizeof(c))) return -1;
      written_total++;
    } else if (*format == 's') {  // print string
      format++;
      const char* str = va_arg(parameters, const char*);
      size_t len = strlen(str);
      if (maxrem < len) {
        // TODO: Set errno to EOVERFLOW.
        return -1;
      }
      if (!print(str, len)) return -1;
      written_total += len;
    } else if (*format == 'd' || *format == 'i') {  // decimal signed integer
      format++;
      int val = va_arg(parameters, int);
      int written = printf_signed(val, 10);
      if (written == -1) return -1;
      written_total += written;
    } else if (*format == 'u') {  // decimal unsigned integer
      format++;
      unsigned int val = va_arg(parameters, unsigned int);
      int written = printf_unsigned(val, 10);
      if (written == -1) return -1;
      written_total += written;
    } else if (*format == 'o') {  // unsigned octal integer
      format++;
      unsigned int val = va_arg(parameters, unsigned int);
      int written = printf_unsigned(val, 8);
      if (written == -1) return -1;
      written_total += written;
    } else if (*format == 'x') {  // unsigned hexadecimal integer(lower)
      format++;
      unsigned int val = va_arg(parameters, unsigned int);
      int written = printf_unsigned(val, 16, true);
      if (written == -1) return -1;
      written_total += written;
    } else if (*format == 'X') {  // unsigned hexadecimal integer(upper)
      format++;
      unsigned int val = va_arg(parameters, unsigned int);
      int written = printf_unsigned(val, 16);
      if (written == -1) return -1;
      written_total += written;
    } else {
      format = format_begun_at;
      size_t len = strlen(format);
      if (maxrem < len) {
        // TODO: Set errno to EOVERFLOW.
        return -1;
      }
      if (!print(format, len)) return -1;
      written_total += len;
      format += len;
    }
  }

  va_end(parameters);
  return written_total;
}
