#include "convert.h"

static void reverse(char *start, char *end) {
  while (start < end) {
    char tmp = *start;
    *start = *end;
    *end = tmp;
    start++;
    end--;
  }
}

static char *write_unsigned_decimal(unsigned long value, char *output) {
  char *cursor = output;

  if (value == 0) {
    *cursor++ = '0';
    *cursor = '\0';
    return output;
  }

  while (value > 0) {
    *cursor++ = (char)('0' + (value % 10));
    value /= 10;
  }

  reverse(output, cursor - 1);
  *cursor = '\0';
  return output;
}

static char *write_signed_decimal(long value, char *output) {
  unsigned long magnitude;

  if (value < 0) {
    *output++ = '-';
    magnitude = (unsigned long)(-(value + 1)) + 1;
  } else {
    magnitude = (unsigned long)value;
  }

  write_unsigned_decimal(magnitude, output);
  return output - ((value < 0) ? 1 : 0);
}

static char *write_unsigned_hex(unsigned long value, char *output) {
  char *cursor = output;

  if (value == 0) {
    *cursor++ = '0';
    *cursor = '\0';
    return output;
  }

  while (value > 0) {
    unsigned long nibble = value & 0xFUL;
    *cursor++ = (char)((nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10)));
    value >>= 4;
  }

  reverse(output, cursor - 1);
  *cursor = '\0';
  return output;
}

static char *write_signed_hex(long value, char *output) {
  unsigned long magnitude;

  if (value < 0) {
    *output++ = '-';
    magnitude = (unsigned long)(-(value + 1)) + 1;
  } else {
    magnitude = (unsigned long)value;
  }

  write_unsigned_hex(magnitude, output);
  return output - ((value < 0) ? 1 : 0);
}

void itoc(int input, char *output) {
  write_signed_decimal((long)input, output);
}

void stoc(short input, char *output) {
  write_signed_decimal((long)input, output);
}

void ltoc(long input, char *output) { write_signed_decimal(input, output); }

void uitoc(unsigned int input, char *output) {
  write_unsigned_decimal((unsigned long)input, output);
}

void ustoc(unsigned short input, char *output) {
  write_unsigned_decimal((unsigned long)input, output);
}

void ultoc(unsigned long input, char *output) {
  write_unsigned_decimal(input, output);
}

void itoh(int input, char *output) { write_signed_hex((long)input, output); }

void stoh(short input, char *output) { write_signed_hex((long)input, output); }

void ltoh(long input, char *output) { write_signed_hex(input, output); }

void uitohex(unsigned int input, char *output) {
  write_unsigned_hex((unsigned long)input, output);
}

void ustohex(unsigned short input, char *output) {
  write_unsigned_hex((unsigned long)input, output);
}

void ultohex(unsigned long input, char *output) {
  write_unsigned_hex(input, output);
}
