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


static wgs_letter_state wgs_game_guesses_letter_state[WGS_GAME_GUESSES_NUMBER_OF_LETTERS];


/* Lifecycle Methods */

void AlphabetState_Create(void) {
  int letter;

  for (letter=0; letter<WGS_GAME_GUESSES_NUMBER_OF_LETTERS; letter++) {
    wgs_game_guesses_letter_state[letter].letter = (char)('A' + letter);
    wgs_game_guesses_letter_state[letter].status = gtUnusedLetter;
    wgs_game_guesses_letter_state[letter].changed = TRUE;
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

void AlphabetState_MaybeUpdateLetterStatus(char c, wgs_letter_status status) {
  int letter = c - 'A';

  if (wgs_game_guesses_letter_state[letter].status == gtCorrectLetter) {
    // Once a letter has been guessed in the correct place, don't change.
    return;
  }

  wgs_game_guesses_letter_state[letter].status = status;
  wgs_game_guesses_letter_state[letter].changed = TRUE;
}


/* Accessors */

wgs_letter_state AlphabetState_GetLetterState(char c) {
  return wgs_game_guesses_letter_state[c - 'A'];
}
