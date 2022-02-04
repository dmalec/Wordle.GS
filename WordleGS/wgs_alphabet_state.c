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

#include "wgs_alphabet_state.h"

/* Constants */

#define WGS_NUMBER_OF_KEY_ROWS              3
#define WGS_NUMBER_OF_KEY_COLUMNS          10

#define WGS_LETTER_KEY_SQUARE_SIZE         16
#define WGS_LETTER_KEY_SQUARE_INSET         1

#define WGS_SCREEN_WIDTH                   320
#define WGS_SCREEN_HEIGHT                  200

#define WGS_SCREEN_MIDDLE                  (WGS_SCREEN_WIDTH / 2)


#define WGS_KEYBOARD_WIDTH                 (WGS_NUMBER_OF_KEY_COLUMNS * WGS_LETTER_KEY_SQUARE_SIZE)
#define WGS_KEYBOARD_LEFT                  (WGS_SCREEN_MIDDLE - (WGS_KEYBOARD_WIDTH / 2))
#define WGS_KEYBOARD_BOTTOM_MARGIN         2
#define WGS_KEYBOARD_TOP                   ((WGS_SCREEN_HEIGHT - WGS_KEYBOARD_BOTTOM_MARGIN) - (WGS_LETTER_KEY_SQUARE_SIZE * WGS_NUMBER_OF_KEY_ROWS))


/* Static Model */

static const char* WGS_KEYBOARD_LETTERS[] = {
  "QWERTYUIOP",
  "ASDFGHJKL ",
  "ZXCVBNM   "
};

static const short WGS_KEYBOARD_INSET[] = {
  0,
  4,
  10
};


/* State */

static wgs_letter_state wgs_game_guesses_letter_state[WGS_GAME_GUESSES_NUMBER_OF_LETTERS];


/* Local Prototypes */

void AlphabetState_CreateRects(void);


/* Lifecycle Methods */

void AlphabetState_Create(void) {
  int letter;

  for (letter=0; letter<WGS_GAME_GUESSES_NUMBER_OF_LETTERS; letter++) {
    wgs_game_guesses_letter_state[letter].letter = (char)('A' + letter);
    wgs_game_guesses_letter_state[letter].status = gtUnusedLetter;
    wgs_game_guesses_letter_state[letter].changed = TRUE;
  }
  
  AlphabetState_CreateRects();
}

void AlphabetState_CreateRects(void) {
  short row, col;
  char letter;
  short letter_idx;
  wgs_letter_state *letter_state;

  for (row=0; row<WGS_NUMBER_OF_KEY_ROWS; row++) {
    short row_inset = WGS_KEYBOARD_LEFT + WGS_KEYBOARD_INSET[row];

    for (col=0; col<WGS_NUMBER_OF_KEY_COLUMNS; col++) {
      letter = WGS_KEYBOARD_LETTERS[row][col];
      if (letter == ' ') continue;

      letter_idx = letter - 'A';
      letter_state = &(wgs_game_guesses_letter_state[letter_idx]);
      
      letter_state->render_box.h1 = row_inset + (col * WGS_LETTER_KEY_SQUARE_SIZE) + WGS_LETTER_KEY_SQUARE_INSET;
      letter_state->render_box.h2 = letter_state->render_box.h1 + WGS_LETTER_KEY_SQUARE_SIZE - (WGS_LETTER_KEY_SQUARE_INSET * 2);

      letter_state->render_box.v1 = WGS_KEYBOARD_TOP + (row * WGS_LETTER_KEY_SQUARE_SIZE) + WGS_LETTER_KEY_SQUARE_INSET;
      letter_state->render_box.v2 = letter_state->render_box.v1 + WGS_LETTER_KEY_SQUARE_SIZE - (WGS_LETTER_KEY_SQUARE_INSET * 2);
    }
  }
}

void AlphabetState_NewGame(void) {
  int letter;

  for (letter=0; letter<WGS_GAME_GUESSES_NUMBER_OF_LETTERS; letter++) {
    if (wgs_game_guesses_letter_state[letter].status != gtUnusedLetter) {
      wgs_game_guesses_letter_state[letter].status = gtUnusedLetter;
      wgs_game_guesses_letter_state[letter].changed = TRUE;
    }
  }
}

void AlphabetState_UpdateFinished(void) {
  int letter;

  for (letter=0; letter<WGS_GAME_GUESSES_NUMBER_OF_LETTERS; letter++) {
    wgs_game_guesses_letter_state[letter].changed = FALSE;
  }
}

void AlphabetState_Destroy(void) {
}


/* Game Methods */

wgs_letter_state AlphabetState_GetLetterState(char c) {
  return wgs_game_guesses_letter_state[c - 'A'];
}

void AlphabetState_MaybeUpdateLetterStatus(char c, wgs_letter_status status) {
  int letter = c - 'A';

  if (wgs_game_guesses_letter_state[letter].status == gtCorrectLetter) {
    // Once a letter has been guessed in the correct place, don't change.
    return;
  }

  if (wgs_game_guesses_letter_state[letter].status == gtWrongPlaceLetter && status == gtIncorrectLetter) {
    // If a letter had been misplaced, don't change to unused.
    return;
  }
  
  wgs_game_guesses_letter_state[letter].status = status;
  wgs_game_guesses_letter_state[letter].changed = TRUE;
}
