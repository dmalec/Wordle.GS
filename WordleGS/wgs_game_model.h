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

#ifndef _GUARD_PROJECTWordleGS_FILEwgs_game_model_
#define _GUARD_PROJECTWordleGS_FILEwgs_game_model_

typedef enum { Unknown, Correct, WrongPlace, UnusedLetter } wgs_square_state;

typedef enum { InProgress, Won, Lost } wgs_game_state;

typedef enum { WordFilled, MaxGuesses, InvalidWord, ValidGuess } wgs_guess_status;

void NewGame(void);

void AddLetterToGuess(char letter);
void RemoveLetterFromGuess(void);

wgs_guess_status GuessCurrentWord(void);

char GetGuessSquareLetter(int row, int col);
wgs_square_state GetGuessSquareStatus(int row, int col);

wgs_square_state GetLetterStatus(char c);

wgs_game_state GetGameState(void);
int GetGuessRow(void);
int GetGuessCol(void);
char *GetSecretWord(void);
char *GetGuessWord(void);
void NewSecretWord(char *word);

int GetGamesWon(void);
int GetGuessNumberWon(int guess_num);

int IndexOfLetter(char* word, char letter);

#endif /* define _GUARD_PROJECTWordleGS_FILEwgs_game_model_ */