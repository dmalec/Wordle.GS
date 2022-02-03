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

/* State */

wgs_game_state wgs_game_engine_game_state;

static int wgs_game_engine_win_stats[WGS_GAME_ENGINE_MAX_GUESSES];


/* Lifecycle Methods */

void GameEngine_Create(void) {
  int guess_num;

  for (guess_num=0; guess_num<WGS_GAME_ENGINE_MAX_GUESSES; guess_num++) {
    wgs_game_engine_win_stats[guess_num] = 0;
  }
}

void GameEngine_NewGame(void) {
  wgs_game_engine_game_state = InProgress;
}


/* Game Methods */

wgs_game_state GameEngine_GetGameState(void) {
  return wgs_game_engine_game_state;
}

void GameEngine_SetGameState(wgs_game_state game_state) {
  wgs_game_engine_game_state = game_state;
}


void GameEngine_IncrementWinStat(int guess_num) {
  wgs_game_engine_win_stats[guess_num]++;
}

int GameEngine_GetWinStat(int guess_num) {
  return wgs_game_engine_win_stats[guess_num];
}
