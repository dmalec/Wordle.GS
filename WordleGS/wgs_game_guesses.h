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

#ifndef _GUARD_PROJECTWordleGS_FILEwgs_game_guesses_
#define _GUARD_PROJECTWordleGS_FILEwgs_game_guesses_

#include "wgs_game_types.h"

/* Lifecycle Methods */

void GameGuesses_Create(void);
void GameGuesses_NewGame(void);
void GameGuesses_UpdateFinished(void);
void GameGuesses_Destroy(void);


/* Game Methods */

void GameGuesses_AddLetterToGuess(char letter);
void GameGuesses_RemoveLetterFromGuess(void);
void GameGuesses_UpdateGuessLetterStatus(wgs_letter_status status[]);
void GameGuesses_MaybeUpdateGuessLetterStatus(int col, wgs_letter_status status);
void GameGuesses_NextGuess(void);


/* Accessors */

int GameGuesses_GetRow(void);
int GameGuesses_GetCol(void);

wgs_letter_state GameGuesses_GetGuessLetterState(int row, int col);

wgs_guess_status GameGuesses_GetGuessStatus(void);
void GameGuesses_GetGuessWord(char *word);

#endif /* define _GUARD_PROJECTWordleGS_FILEwgs_game_guesses_ */
