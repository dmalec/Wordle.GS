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
#include "wgs_game_engine.h"
}


TEST_GROUP(GameEngine_Creation) {
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
    GameEngine_Create();
  }
};

TEST(GameEngine, NewGame) {
  GameEngine_SetGameState(Won);

  GameEngine_NewGame();

  ENUMS_EQUAL_INT_TEXT(InProgress, GameEngine_GetGameState(), "Game should start in 'InProgress'");
}

TEST(GameEngine, SetGameState) {
  GameEngine_SetGameState(Lost);

  ENUMS_EQUAL_INT_TEXT(Lost, GameEngine_GetGameState(), "Game state should be set to 'Lost'");
}

TEST(GameEngine, GetWinStatIncrementWinStat) {
  LONGS_EQUAL_TEXT(0, GameEngine_GetWinStat(0), "Win stat should start at zero");

  GameEngine_IncrementWinStat(0);
  LONGS_EQUAL_TEXT(1, GameEngine_GetWinStat(0), "Win stat should increment by one");
}
