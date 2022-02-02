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
#include "wgs_game_guesses.h"
}


TEST_GROUP(GameGuesses_Creation) {
  void setup() {
    GameGuesses_Create();
  }

  void teardown() {
    GameGuesses_Destroy();
  }
};


TEST(GameGuesses_Creation, Create) {
  for (int row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (int col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      wgs_letter_state state = GameGuesses_GetGuessLetterState(row, col);

      LONGS_EQUAL(' ', state.letter);
      ENUMS_EQUAL_INT(gtUnusedLetter, state.status);
      LONGS_EQUAL(TRUE, state.changed);
    }
  }

  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(0, GameGuesses_GetCol());
}

TEST_GROUP(GameGuesses) {
  void setup() {
    GameGuesses_Create();
    GameGuesses_UpdateFinished();
  }

  void teardown() {
    GameGuesses_Destroy();
  }
};


TEST(GameGuesses, UpdateFinished) {
  for (int row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (int col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      wgs_letter_state state = GameGuesses_GetGuessLetterState(row, col);

      LONGS_EQUAL(FALSE, state.changed);
    }
  }
}


TEST(GameGuesses, AddLetterToGuess) {
  GameGuesses_AddLetterToGuess('R');
  LONGS_EQUAL('R', GameGuesses_GetGuessLetterState(0, 0).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 0).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(1, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_AddLetterToGuess('O');
  LONGS_EQUAL('O', GameGuesses_GetGuessLetterState(0, 1).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 1).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(2, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_AddLetterToGuess('B');
  LONGS_EQUAL('B', GameGuesses_GetGuessLetterState(0, 2).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 2).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(3, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_AddLetterToGuess('O');
  LONGS_EQUAL('O', GameGuesses_GetGuessLetterState(0, 3).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 3).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(4, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_AddLetterToGuess('T');
  LONGS_EQUAL('T', GameGuesses_GetGuessLetterState(0, 4).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 4).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(5, GameGuesses_GetCol());
}

TEST(GameGuesses, RemoveLetterFromGuess) {
  GameGuesses_AddLetterToGuess('R');
  GameGuesses_AddLetterToGuess('O');
  GameGuesses_AddLetterToGuess('B');
  GameGuesses_AddLetterToGuess('O');
  GameGuesses_AddLetterToGuess('T');
  LONGS_EQUAL('T', GameGuesses_GetGuessLetterState(0, 4).letter);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(5, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GameGuesses_GetGuessLetterState(0, 4).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 4).changed);
  LONGS_EQUAL('O', GameGuesses_GetGuessLetterState(0, 3).letter);
  LONGS_EQUAL(FALSE, GameGuesses_GetGuessLetterState(0, 3).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(4, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GameGuesses_GetGuessLetterState(0, 3).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 3).changed);
  LONGS_EQUAL('B', GameGuesses_GetGuessLetterState(0, 2).letter);
  LONGS_EQUAL(FALSE, GameGuesses_GetGuessLetterState(0, 2).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(3, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GameGuesses_GetGuessLetterState(0, 2).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 2).changed);
  LONGS_EQUAL('O', GameGuesses_GetGuessLetterState(0, 1).letter);
  LONGS_EQUAL(FALSE, GameGuesses_GetGuessLetterState(0, 1).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(2, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GameGuesses_GetGuessLetterState(0, 1).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 1).changed);
  LONGS_EQUAL('R', GameGuesses_GetGuessLetterState(0, 0).letter);
  LONGS_EQUAL(FALSE, GameGuesses_GetGuessLetterState(0, 0).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(1, GameGuesses_GetCol());

  GameGuesses_UpdateFinished();

  GameGuesses_RemoveLetterFromGuess();
  LONGS_EQUAL(' ', GameGuesses_GetGuessLetterState(0, 0).letter);
  LONGS_EQUAL(TRUE, GameGuesses_GetGuessLetterState(0, 0).changed);
  LONGS_EQUAL(' ', GameGuesses_GetGuessLetterState(0, 1).letter);
  LONGS_EQUAL(FALSE, GameGuesses_GetGuessLetterState(0, 1).changed);
  LONGS_EQUAL(0, GameGuesses_GetRow());
  LONGS_EQUAL(0, GameGuesses_GetCol());
}

TEST(GameGuesses, GetGuessStatus) {
  GameGuesses_AddLetterToGuess('R');
  ENUMS_EQUAL_INT_TEXT(WordIncomplete, GameGuesses_GetGuessStatus(), "Guess status is as expected");

  GameGuesses_AddLetterToGuess('O');
  GameGuesses_AddLetterToGuess('B');
  GameGuesses_AddLetterToGuess('O');
  GameGuesses_AddLetterToGuess('T');
  ENUMS_EQUAL_INT_TEXT(WordFilled, GameGuesses_GetGuessStatus(), "Guess status is as expected");

  GameGuesses_RemoveLetterFromGuess();
  ENUMS_EQUAL_INT_TEXT(WordIncomplete, GameGuesses_GetGuessStatus(), "Guess status is as expected");

  GameGuesses_AddLetterToGuess('T');
  ENUMS_EQUAL_INT_TEXT(WordFilled, GameGuesses_GetGuessStatus(), "Guess status is as expected");
}

TEST(GameGuesses, GetGuessStatus_MaxGuesses) {
  ENUMS_EQUAL_INT_TEXT(WordIncomplete, GameGuesses_GetGuessStatus(), "Guess status is as expected");

  for (int i=0; i<WGS_GAME_GUESSES_NUMBER_OF_ROWS; i++) {
    GameGuesses_AddLetterToGuess('R');
    GameGuesses_AddLetterToGuess('O');
    GameGuesses_AddLetterToGuess('B');
    GameGuesses_AddLetterToGuess('O');
    GameGuesses_AddLetterToGuess('T');
    GameGuesses_NextGuess();
  }

  ENUMS_EQUAL_INT_TEXT(MaxGuesses, GameGuesses_GetGuessStatus(), "Guess status is as expected");
}

TEST(GameGuesses, GetGuessWord) {
  char word[] = "     ";

  GameGuesses_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("     ", word, 5, "Guess word as expected");

  GameGuesses_AddLetterToGuess('R');
  GameGuesses_AddLetterToGuess('O');
  GameGuesses_AddLetterToGuess('B');
  GameGuesses_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("ROB  ", word, 5, "Guess word as expected");

  GameGuesses_AddLetterToGuess('O');
  GameGuesses_AddLetterToGuess('T');
  GameGuesses_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("ROBOT", word, 5, "Guess word as expected");

  GameGuesses_NextGuess();
  GameGuesses_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("     ", word, 5, "Guess word as expected");

  GameGuesses_AddLetterToGuess('A');
  GameGuesses_AddLetterToGuess('P');
  GameGuesses_AddLetterToGuess('P');
  GameGuesses_AddLetterToGuess('L');
  GameGuesses_AddLetterToGuess('E');
  GameGuesses_GetGuessWord(word);
  STRNCMP_EQUAL_TEXT("APPLE", word, 5, "Guess word as expected");
}

TEST(GameGuesses, UpdateGuessLetterStatus) {
  wgs_letter_status status[] = { gtCorrectLetter, gtWrongPlaceLetter, gtIncorrectLetter, gtCorrectLetter, gtWrongPlaceLetter };

  GameGuesses_UpdateGuessLetterStatus(status);

  for (int i=0; i<WGS_GAME_GUESSES_NUMBER_OF_COLS; i++) {
    ENUMS_EQUAL_INT_TEXT(status[i], GameGuesses_GetGuessLetterState(GameGuesses_GetRow(), i).status, "Guess status as expected");
    ENUMS_EQUAL_INT_TEXT(TRUE, GameGuesses_GetGuessLetterState(GameGuesses_GetRow(), i).changed, "Guess changed as expected");
  }
}
