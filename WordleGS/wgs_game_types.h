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

#ifndef _GUARD_PROJECTWordleGS_FILEwgs_game_types_
#define _GUARD_PROJECTWordleGS_FILEwgs_game_types_

#include <types.h>


/* Constants */

#define WGS_GAME_GUESSES_NUMBER_OF_LETTERS            26

#define WGS_GAME_ENGINE_MAX_GUESSES                    7

#define WGS_GAME_GUESSES_NUMBER_OF_ROWS                6
#define WGS_GAME_GUESSES_NUMBER_OF_COLS                5


/* Enums */

typedef enum {
  InProgress,
  Won,
  Lost
} wgs_game_state;

typedef enum {
  WordIncomplete,
  WordFilled,
  MaxGuesses,
  InvalidWord,
  ValidGuess
} wgs_guess_status;

typedef enum {
  gtUnusedLetter,
  gtCorrectLetter,
  gtWrongPlaceLetter,
  gtIncorrectLetter
} wgs_letter_status;


/* Structs */

typedef struct wgs_letter_state {
  char letter;
  wgs_letter_status status;
  Rect render_box;
  BOOLEAN changed;
} wgs_letter_state;

typedef struct wgs_game_stats {
  int guess_max_distribution;
  int guess_distribution[WGS_GAME_ENGINE_MAX_GUESSES];
  float guess_distribution_percentage[WGS_GAME_ENGINE_MAX_GUESSES];
  int total_played;
  int win_percentage;
  int current_streak;
  int longest_streak;
} wgs_game_stats;

#endif /* define _GUARD_PROJECTWordleGS_FILEwgs_game_types_ */
