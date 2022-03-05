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
#include "wgs_scoring.h"
}


TEST_GROUP(Scoring_Lifecycle) {
};

TEST(Scoring_Lifecycle, Create) {
  Scoring_Create();

  for (int i=0; i<6; i++) {
    LONGS_EQUAL_TEXT(0, Scoring_GetGuessDistributionAbsolute(i), "Win stats should start at zero");
    DOUBLES_EQUAL_TEXT(0, Scoring_GetGuessDistributionPercentage(i), 0.0001, "Win stats should start at zero");
  }
}

TEST_GROUP(Scoring) {
  void setup() {
    Scoring_Create();
  }
};

TEST(Scoring, GuessDistribution) {
  Scoring_RecordWin(2);
  LONGS_EQUAL_TEXT(1, Scoring_GetMaxGuessDistribution(), "Max distribution is 1 after 1 win");
  LONGS_EQUAL_TEXT(1, Scoring_GetGuessDistributionAbsolute(2), "Absolute guess for 2 should be 1");
  DOUBLES_EQUAL_TEXT(1.0, Scoring_GetGuessDistributionPercentage(2), 0.0001, "Guess percentage for 2 should be 100%");


  Scoring_RecordWin(2);
  LONGS_EQUAL_TEXT(2, Scoring_GetMaxGuessDistribution(), "Max distribution is 2 after 2 wins");
  LONGS_EQUAL_TEXT(2, Scoring_GetGuessDistributionAbsolute(2), "Absolute guess for 2 should be 2");
  DOUBLES_EQUAL_TEXT(1.0, Scoring_GetGuessDistributionPercentage(2), 0.0001, "Guess percentage for 2 should be 100%");

  Scoring_RecordWin(1);
  LONGS_EQUAL_TEXT(2, Scoring_GetMaxGuessDistribution(), "Max distribution remains 2 after a different guess number wins");

  LONGS_EQUAL_TEXT(1, Scoring_GetGuessDistributionAbsolute(1), "Absolute guess for 1 should be 1");
  DOUBLES_EQUAL_TEXT(0.5, Scoring_GetGuessDistributionPercentage(1), 0.0001, "Guess percentage for 1 should be 50%");

  LONGS_EQUAL_TEXT(2, Scoring_GetGuessDistributionAbsolute(2), "Absolute guess for 2 should be 2");
  DOUBLES_EQUAL_TEXT(1.0, Scoring_GetGuessDistributionPercentage(2), 0.0001, "Guess percentage for 2 should be 100%");
}

TEST(Scoring, WinPercentage) {
  LONGS_EQUAL_TEXT(0, Scoring_GetWinPercentage(), "Win percentage starts at zero");

  Scoring_RecordWin(2);
  LONGS_EQUAL_TEXT(100, Scoring_GetWinPercentage(), "Win percentage goes to 100 after a win");

  Scoring_RecordLoss();
  LONGS_EQUAL_TEXT(50, Scoring_GetWinPercentage(), "Win percentage drops to 50 after a loss");
}

TEST(Scoring, GetCurrentStreak) {
  LONGS_EQUAL_TEXT(0, Scoring_GetCurrentStreak(), "Current streak starts at zero");

  Scoring_RecordWin(2);
  LONGS_EQUAL_TEXT(1, Scoring_GetCurrentStreak(), "Current streak increments after a win");

  Scoring_RecordLoss();
  LONGS_EQUAL_TEXT(0, Scoring_GetCurrentStreak(), "Current streak resets after a loss");
}

TEST(Scoring, GetLongestStreak) {
  LONGS_EQUAL_TEXT(0, Scoring_GetLongestStreak(), "Longest streak starts at zero");

  Scoring_RecordWin(2);
  LONGS_EQUAL_TEXT(1, Scoring_GetLongestStreak(), "Longest streak increments after a win");

  Scoring_RecordLoss();
  LONGS_EQUAL_TEXT(1, Scoring_GetLongestStreak(), "Longest streak doesn't change after a loss");

  Scoring_RecordWin(2);
  Scoring_RecordWin(2);
  Scoring_RecordWin(2);
  LONGS_EQUAL_TEXT(3, Scoring_GetLongestStreak(), "Longest streak updates after a longer streak");
}

TEST(Scoring, GetTotalPlayed) {
  LONGS_EQUAL_TEXT(0, Scoring_GetTotalPlayed(), "Total played starts at zero");

  Scoring_RecordWin(2);
  LONGS_EQUAL_TEXT(1, Scoring_GetTotalPlayed(), "Total played increments after a win");

  Scoring_RecordLoss();
  LONGS_EQUAL_TEXT(2, Scoring_GetTotalPlayed(), "Total played increments after a loss");
}

TEST(Scoring, Hydration) {
  char buffer[256];
  char *data = (char *)&buffer;
  wgs_game_stats before, after;

  Scoring_RecordWin(2);
  Scoring_RecordLoss();
  Scoring_RecordWin(2);
  Scoring_RecordWin(3);
  Scoring_RecordWin(2);

  data = (char *)&buffer;
  before = Scoring_GetStats();
  Scoring_Dehydrate(&data);

  Scoring_Create();

  data = (char *)&buffer;
  Scoring_Hydrate(&data);
  after = Scoring_GetStats();

  LONGS_EQUAL_TEXT(before.guess_max_distribution, after.guess_max_distribution, "Max distribution is dehydrated/hydrated correctly");
  LONGS_EQUAL_TEXT(before.total_played, after.total_played, "Total played is dehydrated/hydrated correctly");
  LONGS_EQUAL_TEXT(before.win_percentage, after.win_percentage, "Win percentage is dehydrated/hydrated correctly");
  LONGS_EQUAL_TEXT(before.current_streak, after.current_streak, "Current streak is dehydrated/hydrated correctly");
  LONGS_EQUAL_TEXT(before.longest_streak, after.longest_streak, "Longest streak is dehydrated/hydrated correctly");

  for (int guess_num=0; guess_num<WGS_GAME_ENGINE_MAX_GUESSES; guess_num++) {
    LONGS_EQUAL_TEXT(before.guess_distribution[guess_num], after.guess_distribution[guess_num], "Absolute distribution is dehydrated/hydrated correctly");
    DOUBLES_EQUAL_TEXT(before.guess_distribution_percentage[guess_num], after.guess_distribution_percentage[guess_num], 0.001, "Percentage distribution is dehydrated/hydrated correctly");
  }
}
