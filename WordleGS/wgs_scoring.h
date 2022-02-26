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

#ifndef _GUARD_PROJECTWordleGS_FILEwgs_scoring_
#define _GUARD_PROJECTWordleGS_FILEwgs_scoring_

#include "wgs_game_types.h"


/* Lifecycle Methods */

void Scoring_Create(void);


/* Game Methods */

int Scoring_GradeWord(char *guess, char *secret, wgs_letter_status *status);

void Scoring_RecordWin(int guess_num);
void Scoring_RecordLoss(void);

int Scoring_GetMaxGuessDistribution(void);
int Scoring_GetGuessDistributionAbsolute(int guess_num);
float Scoring_GetGuessDistributionPercentage(int guess_num);

int Scoring_GetTotalPlayed(void);
int Scoring_GetWinPercentage(void);
int Scoring_GetCurrentStreak(void);
int Scoring_GetLongestStreak(void);

#endif /* define _GUARD_PROJECTWordleGS_FILEwgs_scoring_ */
