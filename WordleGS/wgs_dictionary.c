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

#include "wgs_dictionary.h"
#include "wgs_gs_shim.h"


/* Constants */

#define DICTIONARY_FILENAME         "dictionary.txt\0"
#define SECRETS_FILENAME            "secrets.txt\0"


/* State */

static LongWord wgs_dictionary_list_count = 0;
static Handle wgs_dictionary_handle;

static LongWord wgs_secret_word_list_count = 0;
static Handle wgs_secret_word_handle;


/* Local Prototypes */

void Dictionary_GetWordFromList(char *src, char *dst, LongWord index);
BOOLEAN Dictionary_DoesListContainWord(LongWord list_len, char *list, char *word);


/* Lifecycle Methods */

void Dictionary_Create(void) {
  LongWord file_length;

  GsShim_LoadFile(DICTIONARY_FILENAME, &wgs_dictionary_handle, &file_length);
  wgs_dictionary_list_count = file_length / 5;

  GsShim_LoadFile(SECRETS_FILENAME, &wgs_secret_word_handle, &file_length);
  wgs_secret_word_list_count = file_length / 5;
}

void Dictionary_NextRound(void) {
}

void Dictionary_UpdateFinished(void) {
}

void Dictionary_Destroy(void) {
  if (wgs_dictionary_handle != NULL) {
    DisposeHandle(wgs_dictionary_handle);
  }
  if (wgs_secret_word_handle != NULL) {
    DisposeHandle(wgs_secret_word_handle);
  }
}


/* Game Methods */

unsigned int Dictionary_GetNumberOfSecretWords(void) {
  return wgs_secret_word_list_count;
}

void Dictionary_GetWordFromList(char *src, char *dst, LongWord index) {
  int i;

  for (i=0; i<5; i++) {
    dst[i] = src[index * 5 + i];
  }

  dst[5] = '\0';
}

BOOLEAN Dictionary_DoesListContainWord(LongWord list_len, char *list, char *word) {
  long low_idx = 0;
  long high_idx = list_len - 1;
  long mid_idx;
  int compare_result;
  char list_word[6];

  while (low_idx <= high_idx) {
    mid_idx = (low_idx + high_idx) / 2;

    Dictionary_GetWordFromList(list, list_word, mid_idx);
    compare_result = strcmp(word, list_word);

    if (compare_result == 0) {
      return TRUE;
    } else if (compare_result < 0) {
      high_idx = mid_idx - 1;
    } else {
      low_idx = mid_idx + 1;
    }
  }

  return FALSE;
}

BOOLEAN Dictionary_IsValidGuess(char *word) {
  BOOLEAN result;

  HLock(wgs_dictionary_handle);
  result = Dictionary_DoesListContainWord(wgs_dictionary_list_count, *wgs_dictionary_handle, word);
  HUnlock(wgs_dictionary_handle);

  if (result == TRUE) {
    return result;
  }

  HLock(wgs_secret_word_handle);
  result = Dictionary_DoesListContainWord(wgs_secret_word_list_count, *wgs_secret_word_handle, word);
  HUnlock(wgs_secret_word_handle);

  return result;
}

void Dictionary_GetRandomWord(char *buffer) {
  unsigned int random_index = rand() % wgs_secret_word_list_count;

  Dictionary_GetWord(random_index, buffer);
}

void Dictionary_GetWord(unsigned int index, char *buffer) {
  int i;

  HLock(wgs_secret_word_handle);
  for (i=0; i<5; i++) {
    buffer[i] = (*wgs_secret_word_handle)[index * 5 + i];
  }
  HUnlock(wgs_secret_word_handle);
}
