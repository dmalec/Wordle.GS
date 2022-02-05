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

#include "wgs_game_engine.h"

#include "wgs_alphabet_state.h"
#include "wgs_dictionary.h"
#include "wgs_guess_state.h"


/* State */

static wgs_game_state wgs_game_engine_game_state;

static int wgs_game_engine_win_stats[WGS_GAME_ENGINE_MAX_GUESSES];

static char wgs_game_engine_secret_word[6] = "ROBOT";


/* Local Prototypes */

void GameEngine_NewSecretWord(char *word);


/* Lifecycle Methods */

void GameEngine_Create(void) {
  int guess_num;

  for (guess_num=0; guess_num<WGS_GAME_ENGINE_MAX_GUESSES; guess_num++) {
    wgs_game_engine_win_stats[guess_num] = 0;
  }

  AlphabetState_Create();
  Dictionary_Create();
  GuessState_Create();
}

void GameEngine_NewGame(void) {
  char secret_word[] = "     ";

  AlphabetState_NewGame();
  Dictionary_NewGame();
  GuessState_NewGame();

  wgs_game_engine_game_state = InProgress;

  Dictionary_GetRandomWord(secret_word);
  GameEngine_NewSecretWord(secret_word);
}

void GameEngine_NewSecretWord(char *secret_word) {
  int i;

  for (i=0; i<5; i++) {
    wgs_game_engine_secret_word[i] = secret_word[i];
  }
  wgs_game_engine_secret_word[5] = '\0';
}

void GameEngine_UpdateFinished(void) {
  AlphabetState_UpdateFinished();
  Dictionary_UpdateFinished();
  GuessState_UpdateFinished();
}

void GameEngine_Destroy(void) {
  AlphabetState_Destroy();
  Dictionary_Destroy();
  GuessState_Destroy();
}


/* Game Methods */

wgs_game_state GameEngine_GetGameState(void) {
  return wgs_game_engine_game_state;
}

void GameEngine_SetGameState(wgs_game_state game_state) {
  wgs_game_engine_game_state = game_state;
}

BOOLEAN GameEngine_IsGameInProgress(void) {
  return wgs_game_engine_game_state == InProgress && (GuessState_GetRow() > 0 || GuessState_GetCol() > 0);
}


void GameEngine_GetSecretWord(char *word) {
  int i;

  for (i=0; i<5; i++) {
    word[i] = wgs_game_engine_secret_word[i];
  }
}


void GameEngine_IncrementWinStat(int guess_num) {
  wgs_game_engine_win_stats[guess_num]++;
}

int GameEngine_GetWinStat(int guess_num) {
  return wgs_game_engine_win_stats[guess_num];
}
