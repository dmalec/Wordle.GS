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

#include <stdio.h>


extern "C" {
#include "wgs_game_sequence.h"
#include "types.h"
}

static unsigned int sequence_memory[] = { 1, 2, 3, 4, 5 };
static Pointer sequence_pointer = (Pointer)(&sequence_memory);
static Handle sequence_handle = (Handle)(&sequence_pointer);
static LongWord sequence_length = 5;

static char sequence_code[] = "TESTS";


TEST_GROUP(GameSequence) {
  void setup() {
    int user_id = 42;

    mock().expectOneCall("userid").andReturnValue(user_id);

    mock()
      .expectOneCall("NewHandle")
      .withUnsignedLongIntParameter("size", sequence_length * sizeof(int))
      .withUnsignedIntParameter("user_id", user_id)
      .withUnsignedIntParameter("attributes", 0xC010)
      .withPointerParameter("location", NULL)
      .andReturnValue(sequence_handle);

    mock().expectOneCall("HUnlock").withPointerParameter("handle", sequence_handle);

    GameSequence_Create(5);

    mock().expectOneCall("HLock").withPointerParameter("handle", sequence_handle);
    mock().expectOneCall("HUnlock").withPointerParameter("handle", sequence_handle);

    mock().expectOneCall("GsShim_ShowProgressDialog");
    mock().expectOneCall("GsShim_HideProgressDialog");

    GameSequence_NewGame(sequence_code);
  }

  void teardown() {
    mock().expectOneCall("DisposeHandle").withPointerParameter("handle", sequence_handle);

    GameSequence_Destroy();

    mock().checkExpectations();
    mock().clear();
  }
};


TEST(GameSequence, UpdateFinished) {

  GameSequence_UpdateFinished();
}

TEST(GameSequence, GetSequenceCode) {
  char actual_sequence_code[5];

  GameSequence_GetSequenceCode(actual_sequence_code);
  STRNCMP_EQUAL_TEXT(sequence_code, actual_sequence_code, 5, "Sequence code returned as expected");
}

TEST(GameSequence, GetSequenceIndex) {
  UNSIGNED_LONGS_EQUAL_TEXT(0, GameSequence_GetSequenceIndex(), "Sequence index returned as expected");

  GameSequence_NextRound();

  UNSIGNED_LONGS_EQUAL_TEXT(1, GameSequence_GetSequenceIndex(), "Sequence index returned as expected");
}

TEST(GameSequence, GetSequenceValue) {
  unsigned int actual_value;

  mock().expectNCalls(4, "HLock").withPointerParameter("handle", sequence_handle);
  mock().expectNCalls(4, "HUnlock").withPointerParameter("handle", sequence_handle);

  UNSIGNED_LONGS_EQUAL_TEXT(sequence_memory[0], GameSequence_GetSequenceValue(), "Sequence value should be as expected");

  GameSequence_NextRound();

  UNSIGNED_LONGS_EQUAL_TEXT(sequence_memory[1], GameSequence_GetSequenceValue(), "Sequence value should be as expected");

  GameSequence_NextRound();
  GameSequence_NextRound();
  GameSequence_NextRound();

  UNSIGNED_LONGS_EQUAL_TEXT(sequence_memory[4], GameSequence_GetSequenceValue(), "Sequence value should be as expected");

  GameSequence_NextRound();

  UNSIGNED_LONGS_EQUAL_TEXT(sequence_memory[0], GameSequence_GetSequenceValue(), "Sequence value should be as expected");
}
