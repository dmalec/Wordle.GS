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

static char dictionary_memory[] = "BABELCRAMSROOTS";
static Pointer dictionary_pointer = (Pointer)(&dictionary_memory);
static Handle dictionary_handle = (Handle)(&dictionary_pointer);
static LongWord dictionary_length = 15;

static char secrets_memory[] = "APPLEROBOT";
static Pointer secrets_pointer = (Pointer)(&secrets_memory);
static Handle secrets_handle = (Handle)(&secrets_pointer);
static LongWord secrets_length = 10;

static unsigned int sequence_memory[] = { 0, 0 };
static Pointer sequence_pointer = (Pointer)(&sequence_memory);
static Handle sequence_handle = (Handle)(&sequence_pointer);
static LongWord sequence_length = 2;

static char sequence_code[] = "TESTS";

void setupFileMocks(void) {
  int user_id = 42;

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

  mock().expectOneCall("userid").andReturnValue(user_id);

  mock()
    .expectOneCall("NewHandle")
    .withUnsignedLongIntParameter("size", sequence_length * sizeof(int))
    .withUnsignedIntParameter("user_id", user_id)
    .withUnsignedIntParameter("attributes", 0xC010)
    .withPointerParameter("location", NULL)
    .andReturnValue(sequence_handle);

  mock().expectOneCall("HUnlock").withPointerParameter("handle", sequence_handle);
}


TEST_GROUP(GameEngine_Creation) {
  void setup() {
    srand(0x1234);
    setupFileMocks();
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
    srand(0x1234);
    setupFileMocks();
    GameEngine_Create();

    mock().expectNCalls(2, "HLock").withPointerParameter("handle", sequence_handle);
    mock().expectNCalls(2, "HUnlock").withPointerParameter("handle", sequence_handle);
    mock().expectOneCall("HLock").withPointerParameter("handle", secrets_handle);
    mock().expectOneCall("HUnlock").withPointerParameter("handle", secrets_handle);

    mock().expectOneCall("GsShim_ShowProgressDialog");
    mock().expectOneCall("GsShim_HideProgressDialog");

    GameEngine_NewGame(sequence_code);
  }

  void teardown() {
    mock().expectOneCall("DisposeHandle").withPointerParameter("handle", secrets_handle);
    mock().expectOneCall("DisposeHandle").withPointerParameter("handle", dictionary_handle);
    mock().expectOneCall("DisposeHandle").withPointerParameter("handle", sequence_handle);

    GameEngine_Destroy();

    mock().checkExpectations();
    mock().clear();
  }
};

TEST(GameEngine, NextRound) {
  mock().expectOneCall("HLock").withPointerParameter("handle", sequence_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", sequence_handle);

  mock().expectOneCall("HLock").withPointerParameter("handle", secrets_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", secrets_handle);

  GameEngine_NextRound();

  ENUMS_EQUAL_INT_TEXT(InProgress, GameEngine_GetGameState(), "Game should start in 'InProgress'");
}

TEST(GameEngine, IsGameInProgress) {
  mock().expectOneCall("HLock").withPointerParameter("handle", sequence_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", sequence_handle);

  mock().expectOneCall("HLock").withPointerParameter("handle", secrets_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", secrets_handle);

  GameEngine_NextRound();

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

  mock().expectOneCall("HLock").withPointerParameter("handle", dictionary_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", dictionary_handle);

  mock().expectOneCall("HLock").withPointerParameter("handle", secrets_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", secrets_handle);

  ENUMS_EQUAL_INT_TEXT(InvalidWord, GameEngine_GuessCurrentWord(), "Invalid word returns expected enum");
}

TEST(GameEngine, GuessCurrentWord_PerfectMatch) {
  char actual_secret_word[] = "     ";

  GameEngine_GetSecretWord(actual_secret_word);

  for (int i=0; i<5; i++) {
    GuessState_AddLetterToGuess(actual_secret_word[i]);
  }

  mock().expectOneCall("HLock").withPointerParameter("handle", dictionary_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", dictionary_handle);

  mock().expectOneCall("HLock").withPointerParameter("handle", secrets_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", secrets_handle);

  ENUMS_EQUAL_INT_TEXT(ValidGuess, GameEngine_GuessCurrentWord(), "Perfect match returns expected enum");
}

TEST(GameEngine, GuessCurrentWord_AllNonMatch) {
  char guess_word[] = "CRAMS";

  for (int i=0; i<5; i++) {
    GuessState_AddLetterToGuess(guess_word[i]);
  }

  mock().expectOneCall("HLock").withPointerParameter("handle", dictionary_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", dictionary_handle);

  ENUMS_EQUAL_INT_TEXT(ValidGuess, GameEngine_GuessCurrentWord(), "All non-match returns expected enum");
}


TEST(GameEngine, GetSecretWord) {
  char expected_secret_word[] = "APPLE";
  char actual_secret_word[] = "     ";

  mock().expectOneCall("HLock").withPointerParameter("handle", sequence_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", sequence_handle);

  mock().expectOneCall("HLock").withPointerParameter("handle", secrets_handle);
  mock().expectOneCall("HUnlock").withPointerParameter("handle", secrets_handle);

  GameEngine_NextRound();

  GameEngine_GetSecretWord(actual_secret_word);
  STRNCMP_EQUAL_TEXT(expected_secret_word, actual_secret_word, 5, "Secret word should be returned unchanged");
}
