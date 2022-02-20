/*
 * MIT License
 *
 * Copyright (c) 2022 Dan Malec
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <types.h>

#include "wgs_utils.h"


unsigned int Utils_GetBitmask16(unsigned short bits[]) {
  int i;
  unsigned int result = 0;

  for (i=0; i<16; i++) {
    if (bits[i] == 1) {
      result |= 0x01;
    }

    if (i < 15) {
      result = result << 1;
    }
  }

  return result;
}

void Utils_StringNCopy(char *destination, char *source, unsigned int number_of_chars) {
  unsigned int i;
  BOOLEAN fill_null = FALSE;

  for (i=0; i<number_of_chars; i++) {
    if (source[i] == '\0') {
      fill_null = TRUE;
    }

    destination[i] = (fill_null) ? '\0' : source[i];
  }
}

char * Utils_StringNFindChar(char *str, unsigned int number_of_chars, char c) {
  unsigned int i;
  char *first_occurence = NULL;

  for (i=0; i<number_of_chars; i++) {
    if (str[i] == c) {
      first_occurence = &str[i];
      break;
    }
  }

  return first_occurence;
}
