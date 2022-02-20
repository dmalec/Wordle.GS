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

#include "CppUTest/TestHarness.h"

extern "C" {
#include "wgs_utils.h"
}


TEST_GROUP(Utils) {
};

TEST(Utils, GetBitmask16) {
  unsigned short all_zero[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  LONGS_EQUAL_TEXT(0, Utils_GetBitmask16(all_zero), "All zeroes generates expected mask");

  unsigned short all_ones[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  LONGS_EQUAL_TEXT(0xFFFF, Utils_GetBitmask16(all_ones), "All ones generates expected mask");

  unsigned short mixed[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
  LONGS_EQUAL_TEXT(0x4008, Utils_GetBitmask16(mixed), "Mixed generates expected mask");
}

TEST(Utils, StringNCopy) {
  char actual[32];

  Utils_StringNCopy(actual, "ROBOT", 3);
  STRNCMP_EQUAL_TEXT("ROB", actual, 3, "Partial copy works");

  Utils_StringNCopy(actual, "ROBOT", 5);
  STRNCMP_EQUAL_TEXT("ROBOT", actual, 5, "Full copy works");

  Utils_StringNCopy(actual, "ROBOT", 10);
  STRCMP_EQUAL_TEXT("ROBOT", actual, "Excess copy works");
}

TEST(Utils, StringNFindChar) {
  char *actual;
  char str[] = "ROBOT";

  actual = Utils_StringNFindChar(str, 5, 'A');
  POINTERS_EQUAL_TEXT(NULL, actual, "Non-existant letter not found");

  actual = Utils_StringNFindChar(str, 5, 'R');
  POINTERS_EQUAL_TEXT(str, actual, "First letter found");

  actual = Utils_StringNFindChar(str, 5, 'T');
  POINTERS_EQUAL_TEXT(&str[4], actual, "Last letter found");

  actual = Utils_StringNFindChar(str, 2, 'T');
  POINTERS_EQUAL_TEXT(NULL, actual, "Length limit works");
}
