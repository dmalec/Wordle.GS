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

#ifndef _GUARD_PROJECTWordleGS_FILEwgs_dictionary_
#define _GUARD_PROJECTWordleGS_FILEwgs_dictionary_

#include <types.h>


/* Lifecycle Methods */

void Dictionary_Create(void);
void Dictionary_NextRound(void);
void Dictionary_UpdateFinished(void);
void Dictionary_Destroy(void);


/* Game Methods */

unsigned int Dictionary_GetNumberOfSecretWords(void);
BOOLEAN Dictionary_IsValidGuess(char *word);
void Dictionary_GetRandomWord(char *buffer);
void Dictionary_GetWord(unsigned int index, char *buffer);


#endif /* define _GUARD_PROJECTWordleGS_FILEwgs_dictionary_ */
