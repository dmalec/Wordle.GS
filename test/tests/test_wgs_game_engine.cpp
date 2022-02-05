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
#include "wgs_game_engine.h"
#include "wgs_guess_state.h"
#include "wgs_dictionary.h"
}


// Mock Dictionary Functions

void Dictionary_Create(void) {
  mock().actualCall("Dictionary_Create");
}

void Dictionary_NewGame(void) {
  mock().actualCall("Dictionary_NewGame");
}

void Dictionary_UpdateFinished(void) {
  mock().actualCall("Dictionary_UpdateFinished");
}

void Dictionary_Destroy(void) {
  mock().actualCall("Dictionary_Destroy");
}

void Dictionary_GetRandomWord(char *secret_word) {
  mock().actualCall("Dictionary_GetRandomWord").withOutputParameter("secret_word", secret_word);
}

BOOLEAN Dictionary_IsValidGuess(char *guess_word) {
  mock().actualCall("Dictionary_IsValidGuess");

  return (BOOLEAN)(mock().intReturnValue());
}

// Unit Tests

TEST_GROUP(GameEngine_Creation) {
  void setup() {
    mock().expectOneCall("Dictionary_Create");
  }

  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }
};



TEST(GameEngine_Creation, Create) {
  GameEngine_Create();

  for (int guess_num=0; guess_num<WGS_GAME_ENGINE_MAX_GUESSES; guess_num++) {
    LONGS_EQUAL_TEXT(0, GameEngine_GetWinStat(guess_num), "All win stats should start at zero");
  }

  ENUMS_EQUAL_INT_TEXT(InProgress, GameEngine_GetGameState(), "Game should start in 'InProgress'");
}


TEST_GROUP(GameEngine) {
  void setup() {
    mock().expectOneCall("Dictionary_Create");
    GameEngine_Create();
  }

  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }
};

TEST(GameEngine, NewGame) {
  char secret_word[] = "     ";

  mock().expectOneCall("Dictionary_NewGame");
  mock().expectOneCall("Dictionary_GetRandomWord").withOutputParameterReturning("secret_word", secret_word, 5);

  GameEngine_NewGame();

  ENUMS_EQUAL_INT_TEXT(InProgress, GameEngine_GetGameState(), "Game should start in 'InProgress'");
}

TEST(GameEngine, IsGameInProgress) {
  LONGS_EQUAL_TEXT(FALSE, GameEngine_IsGameInProgress(), "Game starts not in progress");

  GuessState_AddLetterToGuess('Q');
  LONGS_EQUAL_TEXT(TRUE, GameEngine_IsGameInProgress(), "Game in progress when words on board");

  GuessState_RemoveLetterFromGuess();
  LONGS_EQUAL_TEXT(FALSE, GameEngine_IsGameInProgress(), "Game in progress when board is empty");
}


TEST(GameEngine, GuessCurrentWord_GuardConditions_Guesses) {
  ENUMS_EQUAL_INT_TEXT(WordFilled, GameEngine_GuessCurrentWord(), "Not enough characters returns expected enum");

  for (int i=0; i<6; i++) {
    GuessState_NextGuess();
  }
  ENUMS_EQUAL_INT_TEXT(MaxGuesses, GameEngine_GuessCurrentWord(), "No guesses left returns expected enum");
}

TEST(GameEngine, GuessCurrentWord_GuardConditions_Word) {
  GuessState_AddLetterToGuess('Q');
  GuessState_AddLetterToGuess('Q');
  GuessState_AddLetterToGuess('Q');
  GuessState_AddLetterToGuess('Q');
  GuessState_AddLetterToGuess('Q');

  mock().expectOneCall("Dictionary_IsValidGuess").andReturnValue(FALSE);
  ENUMS_EQUAL_INT_TEXT(InvalidWord, GameEngine_GuessCurrentWord(), "Invalid word returns expected enum");
}


TEST(GameEngine, GetSecretWord) {
  char expected_secret_word[] = "WORDS";
  char actual_secret_word[] = "     ";

  mock().expectOneCall("Dictionary_NewGame");
  mock().expectOneCall("Dictionary_GetRandomWord").withOutputParameterReturning("secret_word", expected_secret_word, 5);

  GameEngine_NewGame();
  GameEngine_GetSecretWord(actual_secret_word);
  STRNCMP_EQUAL_TEXT(expected_secret_word, actual_secret_word, 5, "Secret word should be returned unchanged");
}
