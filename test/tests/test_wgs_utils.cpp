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

TEST(Utils, DehydrateIntAndAdvancePointer) {
  char buffer[256];
  char *data = (char *)&buffer;

  Utils_DehydrateIntAndAdvancePointer(&data, 42);
  Utils_DehydrateIntAndAdvancePointer(&data, 56);

  POINTERS_EQUAL_TEXT((char *)&buffer + sizeof(int) * 2, data, "Pointer advanced as expected");

  LONGS_EQUAL_TEXT(42, *(int *)&buffer, "First value written as expected");
  LONGS_EQUAL_TEXT(56, *(((int *)&buffer) + 1), "Second value written as expected");
}

TEST(Utils, HydrateIntAndAdvancePointer) {
  int buffer[] = { 42, 56, -1 };
  char *data = (char *)&buffer;

  LONGS_EQUAL_TEXT(42, Utils_HydrateIntAndAdvancePointer(&data), "First value read as expected");
  LONGS_EQUAL_TEXT(56, Utils_HydrateIntAndAdvancePointer(&data), "Second value read as expected");

  POINTERS_EQUAL_TEXT((char *)&buffer + sizeof(int) * 2, data, "Pointer advanced as expected");
}

TEST(Utils, DehydrateFloatAndAdvancePointer) {
  char buffer[256];
  char *data = (char *)&buffer;

  Utils_DehydrateFloatAndAdvancePointer(&data, 1.345);
  Utils_DehydrateFloatAndAdvancePointer(&data, 13.37);

  POINTERS_EQUAL_TEXT((char *)&buffer + sizeof(float) * 2, data, "Pointer advanced as expected");

  DOUBLES_EQUAL_TEXT(1.345, *(float *)&buffer, 0.0001, "First value written as expected");
  DOUBLES_EQUAL_TEXT(13.37, *(((float *)&buffer) + 1), 0.0001, "Second value written as expected");
}

TEST(Utils, HydrateFloatAndAdvancePointer) {
  float buffer[] = { 1.345, 13.37, -1 };
  char *data = (char *)&buffer;

  DOUBLES_EQUAL_TEXT(1.345, Utils_HydrateFloatAndAdvancePointer(&data), 0.0001, "First value read as expected");
  DOUBLES_EQUAL_TEXT(13.37, Utils_HydrateFloatAndAdvancePointer(&data), 0.0001, "Second value read as expected");

  POINTERS_EQUAL_TEXT((char *)&buffer + sizeof(float) * 2, data, "Pointer advanced as expected");
}

TEST(Utils, DehydrateStringAndAdvancePointer) {
  char buffer[256];
  char *data = (char *)&buffer;

  Utils_DehydrateStringAndAdvancePointer(&data, "HELLO", 5);
  Utils_DehydrateStringAndAdvancePointer(&data, "WORLD", 5);

  POINTERS_EQUAL_TEXT((char *)&buffer + 10, data, "Pointer advanced as expected");

  STRNCMP_EQUAL_TEXT("HELLOWORLD", buffer, 10, "Values written as expected");
}

TEST(Utils, HydrateStringAndAdvancePointer) {
  char buffer[] = "HELLOWORLD";
  char str[256];
  char *data = (char *)&buffer;

  Utils_HydrateStringAndAdvancePointer(&data, str, 5);
  STRNCMP_EQUAL_TEXT("HELLO", str, 5, "First value read as expected");

  Utils_HydrateStringAndAdvancePointer(&data, str, 5);
  STRNCMP_EQUAL_TEXT("WORLD", str, 5, "Second value read as expected");

  POINTERS_EQUAL_TEXT((char *)&buffer + 10, data, "Pointer advanced as expected");
}
