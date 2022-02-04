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
#include "wgs_alphabet_state.h"
}


TEST_GROUP(AlphabetState_Creation) {
};

TEST(AlphabetState_Creation, Create) {
  AlphabetState_Create();

  for (char letter='A'; letter<='Z'; letter++) {
    wgs_letter_state state = AlphabetState_GetLetterState(letter);

    LONGS_EQUAL_TEXT(letter, state.letter, "Letter should be populated");
    ENUMS_EQUAL_INT_TEXT(gtUnusedLetter, state.status, "Status should start as unused");
    LONGS_EQUAL_TEXT(TRUE, state.changed, "Changed should start as true");
  }
}


TEST_GROUP(AlphabetState) {
  void setup() {
    AlphabetState_Create();
    AlphabetState_UpdateFinished();
  }
};

TEST(AlphabetState, UpdateFinished) {
  for (char letter='A'; letter<='Z'; letter++) {
    wgs_letter_state state = AlphabetState_GetLetterState(letter);

    LONGS_EQUAL_TEXT(FALSE, state.changed, "Changed should startbe false after update");
  }
}


TEST(AlphabetState, MaybeUpdateLetterStatus) {
  wgs_letter_state state;

  AlphabetState_MaybeUpdateLetterStatus('B', gtWrongPlaceLetter);

  state = AlphabetState_GetLetterState('B');
  LONGS_EQUAL_TEXT('B', state.letter, "Letter shouldn't be changed");
  ENUMS_EQUAL_INT_TEXT(gtWrongPlaceLetter, state.status, "Status should change");
  LONGS_EQUAL_TEXT(TRUE, state.changed, "Changed should be true");
}


TEST(AlphabetState, MaybeUpdateLetterStatus_CorrectLetter) {
  wgs_letter_state state;

  AlphabetState_MaybeUpdateLetterStatus('B', gtCorrectLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT_TEXT(gtCorrectLetter, state.status, "Status should change");
  LONGS_EQUAL_TEXT(TRUE, state.changed, "Changed should be true");

  AlphabetState_UpdateFinished();


  AlphabetState_MaybeUpdateLetterStatus('B', gtWrongPlaceLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT_TEXT(gtCorrectLetter, state.status, "Status should not change");
  LONGS_EQUAL_TEXT(FALSE, state.changed, "Changed should be false");

  AlphabetState_UpdateFinished();


  AlphabetState_MaybeUpdateLetterStatus('B', gtIncorrectLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT_TEXT(gtCorrectLetter, state.status, "Status should not change");
  LONGS_EQUAL_TEXT(FALSE, state.changed, "Changed should be false");
}


TEST(AlphabetState, MaybeUpdateLetterStatus_WrongPlaceLetter) {
  wgs_letter_state state;

  AlphabetState_Create();
  AlphabetState_UpdateFinished();


  AlphabetState_MaybeUpdateLetterStatus('B', gtWrongPlaceLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT_TEXT(gtWrongPlaceLetter, state.status, "Status should change");
  LONGS_EQUAL_TEXT(TRUE, state.changed, "Changed should be true");

  AlphabetState_UpdateFinished();


  AlphabetState_MaybeUpdateLetterStatus('B', gtIncorrectLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT_TEXT(gtWrongPlaceLetter, state.status, "Status should not change");
  LONGS_EQUAL_TEXT(FALSE, state.changed, "Changed should be false");

  AlphabetState_UpdateFinished();


  AlphabetState_MaybeUpdateLetterStatus('B', gtCorrectLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT_TEXT(gtCorrectLetter, state.status, "Status should change");
  LONGS_EQUAL_TEXT(TRUE, state.changed, "Changed should be true");

  AlphabetState_UpdateFinished();
}
