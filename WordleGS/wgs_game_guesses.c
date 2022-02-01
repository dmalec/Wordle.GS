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

#include <assert.h>
#include "wgs_game_guesses.h"


static int wgs_game_guesses_current_row = 0;
static int wgs_game_guesses_current_col = 0;

static wgs_letter_state wgs_game_guesses_word_state[WGS_GAME_GUESSES_NUMBER_OF_ROWS][WGS_GAME_GUESSES_NUMBER_OF_COLS];
static wgs_letter_state wgs_game_guesses_letter_state[WGS_GAME_GUESSES_NUMBER_OF_LETTERS];


/* Lifecycle Methods */

void GameGuesses_Create(void) {
  int row, col, letter;

  for (row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      wgs_game_guesses_word_state[row][col].letter = ' ';
      wgs_game_guesses_word_state[row][col].status = gtUnusedLetter;
      wgs_game_guesses_word_state[row][col].changed = TRUE;
    }
  }

  for (letter=0; letter<WGS_GAME_GUESSES_NUMBER_OF_LETTERS; letter++) {
    wgs_game_guesses_letter_state[letter].letter = (char)('A' + letter);
    wgs_game_guesses_letter_state[letter].status = gtUnusedLetter;
    wgs_game_guesses_letter_state[letter].changed = TRUE;
  }

  wgs_game_guesses_current_row = 0;
  wgs_game_guesses_current_col = 0;
}


void GameGuesses_NewGame(void) {
  int row, col, letter;

  for (row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      if (wgs_game_guesses_word_state[row][col].letter != gtUnusedLetter) {
        wgs_game_guesses_word_state[row][col].letter = ' ';
        wgs_game_guesses_word_state[row][col].status = gtUnusedLetter;
        wgs_game_guesses_word_state[row][col].changed = TRUE;
      }
    }
  }

  for (letter=0; letter<WGS_GAME_GUESSES_NUMBER_OF_LETTERS; letter++) {
    if (wgs_game_guesses_letter_state[letter].status != gtUnusedLetter) {
      wgs_game_guesses_letter_state[letter].status = gtUnusedLetter;
      wgs_game_guesses_letter_state[letter].changed = TRUE;
    }
  }

  wgs_game_guesses_current_row = 0;
  wgs_game_guesses_current_col = 0;
}


void GameGuesses_Destroy(void) {
}


/* Game Methods */

void GameGuesses_AddLetterToGuess(char letter) {
  wgs_game_guesses_word_state[wgs_game_guesses_current_row][wgs_game_guesses_current_col].letter = letter;
  wgs_game_guesses_word_state[wgs_game_guesses_current_row][wgs_game_guesses_current_col].changed = TRUE;

  wgs_game_guesses_current_col++;
}

void GameGuesses_RemoveLetterFromGuess(void) {
  wgs_game_guesses_current_col--;

  wgs_game_guesses_word_state[wgs_game_guesses_current_row][wgs_game_guesses_current_col].letter = ' ';
  wgs_game_guesses_word_state[wgs_game_guesses_current_row][wgs_game_guesses_current_col].changed = TRUE;
}

void GameGuesses_SubmitGuess(void) {
  assert(wgs_game_guesses_current_row < WGS_GAME_GUESSES_NUMBER_OF_ROWS);

  wgs_game_guesses_current_row++;
  wgs_game_guesses_current_col = 0;
}


/* Accessors */

int GameGuesses_GetRow(void) {
  return wgs_game_guesses_current_row;
}

int GameGuesses_GetCol(void) {
  return wgs_game_guesses_current_col;
}

wgs_letter_state GameGuesses_GetGuessLetterStatus(int row, int col) {
  return wgs_game_guesses_word_state[row][col];
}

wgs_letter_state GameGuesses_GetLetterStatus(char c) {
  return wgs_game_guesses_letter_state[c - 'A'];
}
