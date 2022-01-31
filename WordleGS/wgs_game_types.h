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

#define WGS_GAME_GUESSES_NUMBER_OF_ROWS       6
#define WGS_GAME_GUESSES_NUMBER_OF_COLS       5

#define WGS_GAME_GUESSES_NUMBER_OF_LETTERS   26


typedef enum {
  gtUnusedLetter,
  gtCorrectLetter,
  gtWrongPlaceLetter,
  gtIncorrectLetter
} wgs_letter_status;

typedef struct wgs_letter_state {
  char letter;
  wgs_letter_status status;
  BOOLEAN changed;
} wgs_letter_state;

#endif /* define _GUARD_PROJECTWordleGS_FILEwgs_game_types_ */
