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

#include <ctype.h>

#include "wgs_guess_state.h"


/* Constants */

#define WGS_LETTER_GUESS_SQUARE_SIZE        20
#define WGS_LETTER_GUESS_SQUARE_INSET        2

#define WGS_SCREEN_WIDTH                   320

#define WGS_SCREEN_MIDDLE                  (WGS_SCREEN_WIDTH / 2)

#define WGS_GUESS_WIDTH                    (WGS_GAME_GUESSES_NUMBER_OF_COLS * WGS_LETTER_GUESS_SQUARE_SIZE)
#define WGS_GUESS_LEFT                     (WGS_SCREEN_MIDDLE - (WGS_GUESS_WIDTH / 2))
#define WGS_LETTER_GUESS_SQUARE_TOP         25


/* State */

static int wgs_guess_state_current_row = 0;
static int wgs_guess_state_current_col = 0;

static wgs_letter_state wgs_guess_state_letter_state[WGS_GAME_GUESSES_NUMBER_OF_ROWS][WGS_GAME_GUESSES_NUMBER_OF_COLS];


/* Local Prototypes */

void GuessState_CreateRects(void);


/* Lifecycle Methods */

void GuessState_Create(void) {
  int row, col;

  for (row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      wgs_guess_state_letter_state[row][col].letter = ' ';
      wgs_guess_state_letter_state[row][col].status = gtUnusedLetter;
      wgs_guess_state_letter_state[row][col].changed = TRUE;
    }
  }

  wgs_guess_state_current_row = 0;
  wgs_guess_state_current_col = 0;

  GuessState_CreateRects();
}

void GuessState_CreateRects(void) {
  int row, col;
  wgs_letter_state *letter_state;

  for (row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {

      letter_state = &(wgs_guess_state_letter_state[row][col]);

      letter_state->render_box.h1 = (short)(WGS_GUESS_LEFT + (col * WGS_LETTER_GUESS_SQUARE_SIZE) + WGS_LETTER_GUESS_SQUARE_INSET);
      letter_state->render_box.h2 = (short)(letter_state->render_box.h1 + WGS_LETTER_GUESS_SQUARE_SIZE - (WGS_LETTER_GUESS_SQUARE_INSET * 2));

      letter_state->render_box.v1 = (short)(WGS_LETTER_GUESS_SQUARE_TOP + (row * WGS_LETTER_GUESS_SQUARE_SIZE) + WGS_LETTER_GUESS_SQUARE_INSET);
      letter_state->render_box.v2 = (short)(letter_state->render_box.v1 + WGS_LETTER_GUESS_SQUARE_SIZE - (WGS_LETTER_GUESS_SQUARE_INSET * 2));
    }
  }
}

void GuessState_NextRound(void) {
  int row, col;

  for (row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      if (wgs_guess_state_letter_state[row][col].letter != gtUnusedLetter) {
        wgs_guess_state_letter_state[row][col].letter = ' ';
        wgs_guess_state_letter_state[row][col].status = gtUnusedLetter;
        wgs_guess_state_letter_state[row][col].changed = TRUE;
      }
    }
  }

  wgs_guess_state_current_row = 0;
  wgs_guess_state_current_col = 0;
}

void GuessState_UpdateFinished(void) {
  int row, col;

  for (row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      wgs_guess_state_letter_state[row][col].changed = FALSE;
    }
  }
}

void GuessState_Destroy(void) {
}


/* Game Methods */

int GuessState_GetRow(void) {
  return wgs_guess_state_current_row;
}

int GuessState_GetCol(void) {
  return wgs_guess_state_current_col;
}


wgs_letter_state GuessState_GetLetterState(int row, int col) {
  return wgs_guess_state_letter_state[row][col];
}

wgs_guess_status GuessState_GetGuessStatus(void) {

  if (wgs_guess_state_current_row >= 6) return MaxGuesses;

  if (wgs_guess_state_current_col < 5) return WordIncomplete;

  return WordFilled;
}

BOOLEAN GuessState_AddLetterToGuess(char letter) {
  if (wgs_guess_state_current_col >= 5) return FALSE;

  wgs_guess_state_letter_state[wgs_guess_state_current_row][wgs_guess_state_current_col].letter = (char)toupper(letter);
  wgs_guess_state_letter_state[wgs_guess_state_current_row][wgs_guess_state_current_col].changed = TRUE;

  wgs_guess_state_current_col++;

  return TRUE;
}

BOOLEAN GuessState_RemoveLetterFromGuess(void) {
  if (wgs_guess_state_current_col <= 0) return FALSE;

  wgs_guess_state_current_col--;

  wgs_guess_state_letter_state[wgs_guess_state_current_row][wgs_guess_state_current_col].letter = ' ';
  wgs_guess_state_letter_state[wgs_guess_state_current_row][wgs_guess_state_current_col].changed = TRUE;

  return TRUE;
}

void GuessState_MaybeUpdateLetterStatus(int col, wgs_letter_status status) {
  if (wgs_guess_state_letter_state[wgs_guess_state_current_row][col].status != status) {
    wgs_guess_state_letter_state[wgs_guess_state_current_row][col].status = status;
    wgs_guess_state_letter_state[wgs_guess_state_current_row][col].changed = TRUE;
  }
}

void GuessState_NextGuess(void) {
  wgs_guess_state_current_row++;
  wgs_guess_state_current_col = 0;
}


void GuessState_GetGuessWord(char *word) {
  int i;

  // TODO: factor out word copy
  for (i=0; i<5; i++) {
    word[i] = wgs_guess_state_letter_state[wgs_guess_state_current_row][i].letter;
  }
}
