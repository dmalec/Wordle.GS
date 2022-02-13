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

#include <memory.h>

#include "wgs_game_sequence.h"
#include "wgs_gs_shim.h"
#include "wgs_utils.h"

/* State */

static Handle wgs_game_sequence_memory = NULL;
static unsigned int wgs_game_sequence_size = 0;
static unsigned int wgs_game_sequence_index = 0;
static char  wgs_game_sequence_code_word[] = "     ";

/* Lifecycle Methods */

void GameSequence_Create(unsigned int size) {
  wgs_game_sequence_size = size;
  wgs_game_sequence_memory = NewHandle(size * sizeof(unsigned int), userid(), 0xC010, 0);
  HUnlock(wgs_game_sequence_memory);
}

void GameSequence_NewGame(char code_word[]) {
  unsigned int sequence_number;
  int i;
  unsigned int *data;
  unsigned seed = 0;

  GsShim_ShowProgressDialog();

  for (i=0; i<5; i++) {
    wgs_game_sequence_code_word[i] = code_word[i];
    seed += code_word[i];
  }
  srand(seed);
  
  HLock(wgs_game_sequence_memory);
  data = (unsigned int *)*wgs_game_sequence_memory;

  // Use the "inside-out" variant of the Durstenfeld implemenmtation of a Fisher-Yates shuffle
  // to reorder the list.
  data[0] = 0;
  for (sequence_number=1; sequence_number<wgs_game_sequence_size; sequence_number++) {
    unsigned int random_index = rand() % sequence_number;

    if (random_index != sequence_number) {
      data[sequence_number] = data[random_index];
    }

    data[random_index] = sequence_number;
    
    if (sequence_number % 100 == 0) {
      GsShim_UpdateProgressDialog(sequence_number, wgs_game_sequence_size);
    }
  }
  HUnlock(wgs_game_sequence_memory);

  GsShim_HideProgressDialog();

  wgs_game_sequence_index = 0;
}

void GameSequence_NextRound(void) {
  wgs_game_sequence_index++;
  
  if (wgs_game_sequence_index >= wgs_game_sequence_size) {
    wgs_game_sequence_index = 0;
  }
}

void GameSequence_UpdateFinished(void) {
}

void GameSequence_Destroy(void) {
  if (wgs_game_sequence_memory != NULL) {
    DisposeHandle(wgs_game_sequence_memory);
  }
}

/* Game Methods */

void GameSequence_GetSequenceCode(char code_word[]) {
  Utils_StringNCopy(code_word, wgs_game_sequence_code_word, 5);
}

unsigned int GameSequence_GetSequenceIndex(void) {
  return wgs_game_sequence_index;
}

unsigned int GameSequence_GetSequenceValue(void) {
  unsigned int result = 0;
  unsigned int *data;

  HLock(wgs_game_sequence_memory);
  data = (unsigned int *)*wgs_game_sequence_memory;
  result = data[wgs_game_sequence_index];
  HUnlock(wgs_game_sequence_memory);

  return result;
}
