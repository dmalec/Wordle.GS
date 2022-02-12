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
#include "CppUTestExt/MockSupport.h"

extern "C" {
#include "wgs_dictionary.h"
}

static char dictionary_memory[] = "BABELCRAMSROOTS";
static Pointer dictionary_pointer = (Pointer)(&dictionary_memory);
static Handle dictionary_handle = (Handle)(&dictionary_pointer);
static LongWord dictionary_length = 15;

static char secrets_memory[] = "APPLEROBOT";
static Pointer secrets_pointer = (Pointer)(&secrets_memory);
static Handle secrets_handle = (Handle)(&secrets_pointer);
static LongWord secrets_length = 10;


TEST_GROUP(Dictionary) {
  void setup() {
    mock()
      .expectOneCall("GsShim_LoadFile")
      .withStringParameter("c_str_file_name", "dictionary.txt")
      .withOutputParameterReturning("file_handle", &dictionary_handle, sizeof(Handle))
      .withOutputParameterReturning("file_length", &dictionary_length, sizeof(LongWord));

    mock()
      .expectOneCall("GsShim_LoadFile")
      .withStringParameter("c_str_file_name", "secrets.txt")
      .withOutputParameterReturning("file_handle", &secrets_handle, sizeof(Handle))
      .withOutputParameterReturning("file_length", &secrets_length, sizeof(LongWord));

    Dictionary_Create();
  }

  void teardown() {
    mock().expectOneCall("DisposeHandle").withPointerParameter("handle", secrets_handle);
    mock().expectOneCall("DisposeHandle").withPointerParameter("handle", dictionary_handle);

    Dictionary_Destroy();

    mock().checkExpectations();
    mock().clear();
  }
};


TEST(Dictionary, UpdateFinished) {
  Dictionary_UpdateFinished();
}


TEST(Dictionary, GetNumberOfSecretWords) {
  UNSIGNED_LONGS_EQUAL_TEXT(2, Dictionary_GetNumberOfSecretWords(), "Secret word size as expected");
}


TEST(Dictionary, IsValidGuess) {
  mock().expectNCalls(3, "HLock").withPointerParameter("handle", dictionary_handle);
  mock().expectNCalls(3, "HUnlock").withPointerParameter("handle", dictionary_handle);
  mock().expectNCalls(2, "HLock").withPointerParameter("handle", secrets_handle);
  mock().expectNCalls(2, "HUnlock").withPointerParameter("handle", secrets_handle);

  LONGS_EQUAL_TEXT(TRUE, Dictionary_IsValidGuess("ROBOT"), "Secret words are valid guesses");
  LONGS_EQUAL_TEXT(TRUE, Dictionary_IsValidGuess("ROOTS"), "Dictionary words are valid guesses");
  LONGS_EQUAL_TEXT(FALSE, Dictionary_IsValidGuess("RENTS"), "Unknown words are not valid guesses");
}


TEST(Dictionary, GetRandomWord) {
  char word[] = "     ";

  mock().expectOneCall("HLock").withPointerParameter("handle", secrets_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", secrets_handle);
  srand(0x12345);

  Dictionary_GetRandomWord(word);
  STRCMP_CONTAINS_TEXT(word, secrets_memory, "Random secret word in secret word list");
}


TEST(Dictionary, GetWord) {
  char word[] = "     ";

  mock().expectNCalls(2, "HLock").withPointerParameter("handle", secrets_handle);
  mock().expectNCalls(2, "HUnlock").withPointerParameter("handle", secrets_handle);

  Dictionary_GetWord(0, word);
  STRNCMP_EQUAL_TEXT("APPLE", word, 5, "Secret word at 0 returned as expected");

  Dictionary_GetWord(1, word);
  STRNCMP_EQUAL_TEXT("ROBOT", word, 5, "Secret word at 1 returned as expected");
}

