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


#ifndef _GUARD_PROJECTWordleGS_FILEwgs_guess_state_
#define _GUARD_PROJECTWordleGS_FILEwgs_guess_state_

#include "wgs_game_types.h"


/* Lifecycle Methods */

void GuessState_Create(void);
void GuessState_NewGame(void);
void GuessState_UpdateFinished(void);
void GuessState_Destroy(void);


/* Game Methods */

int GuessState_GetRow(void);
int GuessState_GetCol(void);

wgs_letter_state GuessState_GetLetterState(int row, int col);
wgs_guess_status GuessState_GetGuessStatus(void);

BOOLEAN GuessState_AddLetterToGuess(char letter);
BOOLEAN GuessState_RemoveLetterFromGuess(void);
void GuessState_MaybeUpdateLetterStatus(int col, wgs_letter_status status);
void GuessState_NextGuess(void);

void GuessState_GetGuessWord(char *word);


#endif /* define _GUARD_PROJECTWordleGS_FILEwgs_guess_state_ */
