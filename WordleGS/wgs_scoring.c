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

#include "wgs_game_types.h"
#include "wgs_scoring.h"
#include "wgs_utils.h"


/* State */

static int wgs_scoring_win_count;
static int wgs_scoring_loss_count;
static int wgs_scoring_current_streak;
static int wgs_scoring_longest_streak;
static int wgs_scoring_guess_distribution[WGS_GAME_ENGINE_MAX_GUESSES];
static int wgs_scoring_guess_max_distribution;
static float wgs_scoring_guess_distribution_percentage[WGS_GAME_ENGINE_MAX_GUESSES];


/* Lifecycle Methods */

void Scoring_Create(void) {
  int guess_num;

  wgs_scoring_win_count = 0;
  wgs_scoring_loss_count = 0;
  wgs_scoring_current_streak = 0;
  wgs_scoring_longest_streak = 0;
  wgs_scoring_guess_max_distribution = 0;

  for (guess_num=0; guess_num<WGS_GAME_ENGINE_MAX_GUESSES; guess_num++) {
    wgs_scoring_guess_distribution[guess_num] = 0;
    wgs_scoring_guess_distribution_percentage[guess_num] = 0;
  }
}


/* Game Methods */

int Scoring_GradeWord(char *guess, char *secret, wgs_letter_status *status) {
  int i;
  char tmp_secret[5];
  int matches = 0;

  /*
     Mark perfect matches in advance. This will help with marking non-matching
     letters later. Specifically, it helps avoid marking a letter as misplaced
     when all occurences of the letter in the secret word have already been
     accounted for.
  */

  for (i=0; i<5; i++) {
    if (guess[i] == secret[i]) {
      tmp_secret[i] = '+';
    } else {
      tmp_secret[i] = secret[i];
    }
  }

  for (i=0; i<5; i++) {
    if (tmp_secret[i] == '+') {
      status[i] = gtCorrectLetter;
      matches++;
    } else {
      char *letter_location = Utils_StringNFindChar(tmp_secret, 5, guess[i]);

      if (letter_location != NULL) {
        status[i] = gtWrongPlaceLetter;

        // Clear the letter, so it won't be double counted
        // if it appears again in the guess.
        *letter_location = ' ';
      } else {
        status[i] = gtIncorrectLetter;
      }
    }
  }

  return matches;
}

void Scoring_RecordWin(int guess_num) {
  int guess_num_idx;

  wgs_scoring_guess_distribution[guess_num]++;
  wgs_scoring_win_count++;
  wgs_scoring_current_streak++;
  
  if (wgs_scoring_current_streak > wgs_scoring_longest_streak) {
    wgs_scoring_longest_streak = wgs_scoring_current_streak;
  }

  if (wgs_scoring_guess_distribution[guess_num] > wgs_scoring_guess_max_distribution) {
    wgs_scoring_guess_max_distribution = wgs_scoring_guess_distribution[guess_num];
  }

  for (guess_num_idx=0; guess_num_idx<WGS_GAME_ENGINE_MAX_GUESSES; guess_num_idx++) {
    wgs_scoring_guess_distribution_percentage[guess_num_idx] = (float)wgs_scoring_guess_distribution[guess_num_idx] / (float)wgs_scoring_guess_max_distribution;
  }
}

void Scoring_RecordLoss(void) {
  wgs_scoring_loss_count++;
  wgs_scoring_current_streak = 0;
}

int Scoring_GetMaxGuessDistribution(void) {
  return wgs_scoring_guess_max_distribution;
}

int Scoring_GetGuessDistributionAbsolute(int guess_num) {
  return wgs_scoring_guess_distribution[guess_num];
}

float Scoring_GetGuessDistributionPercentage(int guess_num) {
  return wgs_scoring_guess_distribution_percentage[guess_num];
}

int Scoring_GetTotalPlayed(void) {
  return wgs_scoring_win_count + wgs_scoring_loss_count;
}

int Scoring_GetWinPercentage(void) {
  int game_total = wgs_scoring_win_count + wgs_scoring_loss_count;
  
  if (game_total == 0) {
    return 0;
  } else {
    return (int)((float)wgs_scoring_win_count / (float)game_total * 100);
  }
}

int Scoring_GetCurrentStreak(void) {
  return wgs_scoring_current_streak;
}

int Scoring_GetLongestStreak(void) {
  return wgs_scoring_longest_streak;
}
