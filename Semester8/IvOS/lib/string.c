#include "string.h"

void *memcpy(void *destination, const void *source, size_t size) {
  unsigned char *out = (unsigned char *)destination;
  const unsigned char *in = (const unsigned char *)source;

  for (size_t i = 0; i < size; i++) {
    out[i] = in[i];
  }

  return destination;
}

void *memset(void *destination, int value, size_t size) {
  unsigned char *out = (unsigned char *)destination;

  for (size_t i = 0; i < size; i++) {
    out[i] = (unsigned char)value;
  }

  return destination;
}

int memcmp(const void *left, const void *right, size_t size) {
  const unsigned char *a = (const unsigned char *)left;
  const unsigned char *b = (const unsigned char *)right;

  for (size_t i = 0; i < size; i++) {
    if (a[i] != b[i]) {
      return (int)a[i] - (int)b[i];
    }
  }

  return 0;
}

size_t strlen(const char *str) {
  size_t length = 0;
  while (str[length] != '\0') {
    length++;
  }
  return length;
}

int strcmp(const char *left, const char *right) {
  while (*left != '\0' && *left == *right) {
    left++;
    right++;
  }

  return (unsigned char)*left - (unsigned char)*right;
}

int strncmp(const char *left, const char *right, size_t size) {
  while (size > 0 && *left != '\0' && *left == *right) {
    left++;
    right++;
    size--;
  }

  if (size == 0) {
    return 0;
  }

  return (unsigned char)*left - (unsigned char)*right;
}

char *strchr(const char *str, int value) {
  while (*str != '\0') {
    if (*str == (char)value) {
      return (char *)str;
    }
    str++;
  }

  if (value == '\0') {
    return (char *)str;
  }

  return NULL;
}

char *strncpy(char *destination, const char *source, size_t size) {
  size_t i = 0;
  for (; i < size && source[i] != '\0'; i++) {
    destination[i] = source[i];
  }

  for (; i < size; i++) {
    destination[i] = '\0';
  }

  return destination;
}
