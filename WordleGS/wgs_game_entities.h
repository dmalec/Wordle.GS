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

#ifndef _GUARD_PROJECTWordleGS_FILEwgs_game_entities_
#define _GUARD_PROJECTWordleGS_FILEwgs_game_entities_

#include <quickdraw.h>

#define WGS_LETTERS_IN_WORD            5
#define WGS_NUMBER_OF_GUESSES          6

#define WGS_LETTER_GUESS_SQUARE_TOP   25
#define WGS_LETTER_GUESS_SQUARE_SIZE  20
#define WGS_LETTER_GUESS_SQUARE_INSET  2


typedef enum {
  ImageDirty,
  ImageRendered,
  ImageCopied,
  ImageStatic
} wgs_image_state;

typedef enum {
  UnusedGuess,
  CorrectGuess,
  WrongPlaceGuess,
  IncorrectGuess
} wgs_guess_state;

typedef struct wgs_letter_guess {
  Rect box;
  char letter;
  wgs_guess_state state;
  wgs_image_state image_state;
} wgs_letter_guess;

extern wgs_letter_guess wgs_letter_guess_entities[WGS_NUMBER_OF_GUESSES][WGS_LETTERS_IN_WORD];

void CreateLetterGuessEntities(void);
void ResetLetterGuessEntities(void);

#endif /* define _GUARD_PROJECTWordleGS_FILEwgs_game_entities_ */
