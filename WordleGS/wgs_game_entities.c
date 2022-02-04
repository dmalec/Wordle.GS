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

#include "wgs_game_entities.h"

wgs_letter_guess wgs_letter_guess_entities[WGS_NUMBER_OF_GUESSES][WGS_LETTERS_IN_WORD];


void CreateLetterGuessEntities(void) {
  int row, col;
  int grid_left = (320 / 2) - (5 * WGS_LETTER_GUESS_SQUARE_SIZE) / 2;
  
  for (row=0; row<WGS_NUMBER_OF_GUESSES; row++) {
    for (col=0; col<WGS_LETTERS_IN_WORD; col++) {
      wgs_letter_guess *letter_guess = &(wgs_letter_guess_entities[row][col]);

      letter_guess->box.h1 = grid_left + (col * WGS_LETTER_GUESS_SQUARE_SIZE) + WGS_LETTER_GUESS_SQUARE_INSET;
      letter_guess->box.h2 = letter_guess->box.h1 + WGS_LETTER_GUESS_SQUARE_SIZE - (WGS_LETTER_GUESS_SQUARE_INSET * 2);

      letter_guess->box.v1 = WGS_LETTER_GUESS_SQUARE_TOP + (row * WGS_LETTER_GUESS_SQUARE_SIZE) + WGS_LETTER_GUESS_SQUARE_INSET;
      letter_guess->box.v2 = letter_guess->box.v1 + WGS_LETTER_GUESS_SQUARE_SIZE - (WGS_LETTER_GUESS_SQUARE_INSET * 2);
    }
  }
  
  ResetLetterGuessEntities();
}


void ResetLetterGuessEntities(void) {
  int row, col;

  for (row=0; row<WGS_NUMBER_OF_GUESSES; row++) {
    for (col=0; col<WGS_LETTERS_IN_WORD; col++) {
      wgs_letter_guess *letter_guess = &(wgs_letter_guess_entities[row][col]);
      
      letter_guess->state = UnusedGuess;
      letter_guess->letter = ' ';
      letter_guess->image_state = ImageDirty;
    }
  }
}
