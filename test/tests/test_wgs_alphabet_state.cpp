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


TEST_GROUP(AlphabetState) {
};


TEST(AlphabetState, Create) {
  AlphabetState_Create();

  for (char letter='A'; letter<='Z'; letter++) {
    wgs_letter_state state = AlphabetState_GetLetterState(letter);

    LONGS_EQUAL(letter, state.letter);
    ENUMS_EQUAL_INT(gtUnusedLetter, state.status);
    LONGS_EQUAL(TRUE, state.changed);
  }
}


TEST(AlphabetState, UpdateFinished) {
  AlphabetState_Create();
  AlphabetState_UpdateFinished();

  for (char letter='A'; letter<='Z'; letter++) {
    wgs_letter_state state = AlphabetState_GetLetterState(letter);

    LONGS_EQUAL(FALSE, state.changed);
  }
}


TEST(AlphabetState, MaybeUpdateLetterStatus) {
  wgs_letter_state state;

  AlphabetState_Create();
  AlphabetState_UpdateFinished();

  AlphabetState_MaybeUpdateLetterStatus('B', gtWrongPlaceLetter);

  state = AlphabetState_GetLetterState('B');
  LONGS_EQUAL('B', state.letter);
  ENUMS_EQUAL_INT(gtWrongPlaceLetter, state.status);
  LONGS_EQUAL(TRUE, state.changed);
}


TEST(AlphabetState, MaybeUpdateLetterStatus_Sequence) {
  wgs_letter_state state;

  AlphabetState_Create();
  AlphabetState_UpdateFinished();


  AlphabetState_MaybeUpdateLetterStatus('B', gtWrongPlaceLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT(gtWrongPlaceLetter, state.status);
  LONGS_EQUAL(TRUE, state.changed);

  AlphabetState_UpdateFinished();


  AlphabetState_MaybeUpdateLetterStatus('B', gtCorrectLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT(gtCorrectLetter, state.status);
  LONGS_EQUAL(TRUE, state.changed);

  AlphabetState_UpdateFinished();


  AlphabetState_MaybeUpdateLetterStatus('B', gtWrongPlaceLetter);

  state = AlphabetState_GetLetterState('B');
  ENUMS_EQUAL_INT(gtCorrectLetter, state.status);
  LONGS_EQUAL(FALSE, state.changed);
}
