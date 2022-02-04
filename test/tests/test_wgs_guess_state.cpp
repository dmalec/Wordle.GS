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
#include "wgs_guess_state.h"
}


TEST_GROUP(GuessState_Creation) {
  void setup() {
    GuessState_Create();
  }

  void teardown() {
    GuessState_Destroy();
  }
};


TEST(GuessState_Creation, Create) {
  for (int row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (int col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      wgs_letter_state state = GuessState_GetLetterState(row, col);

      LONGS_EQUAL(' ', state.letter);
      ENUMS_EQUAL_INT(gtUnusedLetter, state.status);
      LONGS_EQUAL(TRUE, state.changed);
    }
  }

  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(0, GuessState_GetCol());
}

TEST_GROUP(GuessState) {
  void setup() {
    GuessState_Create();
    GuessState_UpdateFinished();
  }

  void teardown() {
    GuessState_Destroy();
  }
};


TEST(GuessState, UpdateFinished) {
  for (int row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (int col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      wgs_letter_state state = GuessState_GetLetterState(row, col);

      LONGS_EQUAL(FALSE, state.changed);
    }
  }
}


TEST(GuessState, AddLetterToGuess) {
  GuessState_AddLetterToGuess('R');
  LONGS_EQUAL('R', GuessState_GetLetterState(0, 0).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 0).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(1, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_AddLetterToGuess('O');
  LONGS_EQUAL('O', GuessState_GetLetterState(0, 1).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 1).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(2, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_AddLetterToGuess('B');
  LONGS_EQUAL('B', GuessState_GetLetterState(0, 2).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 2).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(3, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_AddLetterToGuess('O');
  LONGS_EQUAL('O', GuessState_GetLetterState(0, 3).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 3).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(4, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_AddLetterToGuess('T');
  LONGS_EQUAL('T', GuessState_GetLetterState(0, 4).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 4).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(5, GuessState_GetCol());
}

TEST(GuessState, RemoveLetterFromGuess) {
  GuessState_AddLetterToGuess('R');
  GuessState_AddLetterToGuess('O');
  GuessState_AddLetterToGuess('B');
  GuessState_AddLetterToGuess('O');
  GuessState_AddLetterToGuess('T');
  LONGS_EQUAL('T', GuessState_GetLetterState(0, 4).letter);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(5, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GuessState_GetLetterState(0, 4).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 4).changed);
  LONGS_EQUAL('O', GuessState_GetLetterState(0, 3).letter);
  LONGS_EQUAL(FALSE, GuessState_GetLetterState(0, 3).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(4, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GuessState_GetLetterState(0, 3).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 3).changed);
  LONGS_EQUAL('B', GuessState_GetLetterState(0, 2).letter);
  LONGS_EQUAL(FALSE, GuessState_GetLetterState(0, 2).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(3, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GuessState_GetLetterState(0, 2).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 2).changed);
  LONGS_EQUAL('O', GuessState_GetLetterState(0, 1).letter);
  LONGS_EQUAL(FALSE, GuessState_GetLetterState(0, 1).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(2, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GuessState_GetLetterState(0, 1).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 1).changed);
  LONGS_EQUAL('R', GuessState_GetLetterState(0, 0).letter);
  LONGS_EQUAL(FALSE, GuessState_GetLetterState(0, 0).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(1, GuessState_GetCol());

  GuessState_UpdateFinished();

  GuessState_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GuessState_GetLetterState(0, 0).letter);
  LONGS_EQUAL(TRUE, GuessState_GetLetterState(0, 0).changed);
  LONGS_EQUAL(' ', GuessState_GetLetterState(0, 1).letter);
  LONGS_EQUAL(FALSE, GuessState_GetLetterState(0, 1).changed);
  LONGS_EQUAL(0, GuessState_GetRow());
  LONGS_EQUAL(0, GuessState_GetCol());
}

TEST(GuessState, RemoveLetterFromGuess_Overflow) {
  ENUMS_EQUAL_INT_TEXT(TRUE, GuessState_AddLetterToGuess('R'), "Letter added as expected");

  ENUMS_EQUAL_INT_TEXT(TRUE, GuessState_RemoveLetterFromGuess(), "Letter removed as expected");
  ENUMS_EQUAL_INT_TEXT(FALSE, GuessState_RemoveLetterFromGuess(), "Letter refused to remove as expected");
}


TEST(GuessState, AddLetterToGuess_Capitalization) {
  GuessState_AddLetterToGuess('r');
  LONGS_EQUAL('R', GuessState_GetLetterState(0, 0).letter);
}

TEST(GuessState, AddLetterToGuess_Overflow) {
  ENUMS_EQUAL_INT_TEXT(TRUE, GuessState_AddLetterToGuess('R'), "Letter added as expected");
  ENUMS_EQUAL_INT_TEXT(TRUE, GuessState_AddLetterToGuess('O'), "Letter added as expected");
  ENUMS_EQUAL_INT_TEXT(TRUE, GuessState_AddLetterToGuess('B'), "Letter added as expected");
  ENUMS_EQUAL_INT_TEXT(TRUE, GuessState_AddLetterToGuess('O'), "Letter added as expected");
  ENUMS_EQUAL_INT_TEXT(TRUE, GuessState_AddLetterToGuess('T'), "Letter added as expected");
  ENUMS_EQUAL_INT_TEXT(FALSE, GuessState_AddLetterToGuess('S'), "Letter refused to add as expected");
}

TEST(GuessState, GetGuessStatus) {
  GuessState_AddLetterToGuess('R');
  ENUMS_EQUAL_INT_TEXT(WordIncomplete, GuessState_GetGuessStatus(), "Guess status is as expected");

  GuessState_AddLetterToGuess('O');
  GuessState_AddLetterToGuess('B');
  GuessState_AddLetterToGuess('O');
  GuessState_AddLetterToGuess('T');
  ENUMS_EQUAL_INT_TEXT(WordFilled, GuessState_GetGuessStatus(), "Guess status is as expected");

  GuessState_RemoveLetterFromGuess();
  ENUMS_EQUAL_INT_TEXT(WordIncomplete, GuessState_GetGuessStatus(), "Guess status is as expected");

  GuessState_AddLetterToGuess('T');
  ENUMS_EQUAL_INT_TEXT(WordFilled, GuessState_GetGuessStatus(), "Guess status is as expected");
}

TEST(GuessState, GetGuessStatus_MaxGuesses) {
  ENUMS_EQUAL_INT_TEXT(WordIncomplete, GuessState_GetGuessStatus(), "Guess status is as expected");

  for (int i=0; i<WGS_GAME_GUESSES_NUMBER_OF_ROWS; i++) {
    GuessState_AddLetterToGuess('R');
    GuessState_AddLetterToGuess('O');
    GuessState_AddLetterToGuess('B');
    GuessState_AddLetterToGuess('O');
    GuessState_AddLetterToGuess('T');
    GuessState_NextGuess();
  }

  ENUMS_EQUAL_INT_TEXT(MaxGuesses, GuessState_GetGuessStatus(), "Guess status is as expected");
}

TEST(GuessState, GetGuessWord) {
  char word[] = "     ";

  GuessState_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("     ", word, 5, "Guess word as expected");

  GuessState_AddLetterToGuess('R');
  GuessState_AddLetterToGuess('O');
  GuessState_AddLetterToGuess('B');
  GuessState_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("ROB  ", word, 5, "Guess word as expected");

  GuessState_AddLetterToGuess('O');
  GuessState_AddLetterToGuess('T');
  GuessState_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("ROBOT", word, 5, "Guess word as expected");

  GuessState_NextGuess();
  GuessState_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("     ", word, 5, "Guess word as expected");

  GuessState_AddLetterToGuess('A');
  GuessState_AddLetterToGuess('P');
  GuessState_AddLetterToGuess('P');
  GuessState_AddLetterToGuess('L');
  GuessState_AddLetterToGuess('E');
  GuessState_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("APPLE", word, 5, "Guess word as expected");
}

TEST(GuessState, MaybeUpdateLetterStatus) {
  wgs_letter_state state;

  GuessState_MaybeUpdateLetterStatus(0, gtWrongPlaceLetter);

  state = GuessState_GetLetterState(0, 0);
  ENUMS_EQUAL_INT_TEXT(gtWrongPlaceLetter, state.status, "Status should change");
  LONGS_EQUAL_TEXT(TRUE, state.changed, "Changed should be true");
}

TEST(GuessState, MaybeUpdateLetterStatus_StatusMatches) {
  wgs_letter_state state;

  GuessState_MaybeUpdateLetterStatus(0, gtCorrectLetter);

  state = GuessState_GetLetterState(0, 0);
  ENUMS_EQUAL_INT_TEXT(gtCorrectLetter, state.status, "Status should change");
  LONGS_EQUAL_TEXT(TRUE, state.changed, "Changed should be true");

  GuessState_UpdateFinished();


  GuessState_MaybeUpdateLetterStatus(0, gtCorrectLetter);

  state = GuessState_GetLetterState(0, 0);
  ENUMS_EQUAL_INT_TEXT(gtCorrectLetter, state.status, "Status should not change");
  LONGS_EQUAL_TEXT(FALSE, state.changed, "Changed should be false");
}
