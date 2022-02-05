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

#include <gsos.h>
#include <memory.h>
#include <misctool.h>
#include <stdio.h>

#include "wgs_dictionary.h"


/* Constants */

#define DICTIONARY_FILENAME         "dictionary.txt"
#define SECRETS_FILENAME            "secrets.txt"


/* State */

LongWord wgs_dictionary_list_count = 0;
Handle wgs_dictionary_handle;

LongWord wgs_secret_word_list_count = 0;
Handle wgs_secret_word_handle;


/* Local Prototypes */

void Dictionary_GetWordFromList(char *src, char *dst, LongWord index);
BOOLEAN Dictionary_DoesListContainWord(LongWord list_len, char *list, char *word);
void Dictionary_LoadFile(GSString255 *file_name, Handle *file_handle, LongWord *file_length);


/* Lifecycle Methods */

void Dictionary_LoadFile(GSString255 *file_name, Handle *file_handle, LongWord *file_length) {
  BOOLEAN success = TRUE;
  OpenRecGS open_file_rec;
  Handle file_contents = NULL;
  IORecGS read_file_rec;
  RefNumRecGS close_file_rev;

  *file_handle = NULL;
  *file_length = 0;

  open_file_rec.pCount = 12;
  open_file_rec.pathname = file_name;
  open_file_rec.requestAccess = 1;         // Read mode
  open_file_rec.resourceNumber = 0;        // Read data fork
  open_file_rec.optionList = NULL;
  OpenGS(&open_file_rec);
  if (toolerror() != 0) {
    SysBeep(); //FlagError(4, toolerror());
    return;
  }

  file_contents = NewHandle(open_file_rec.eof, userid(), 0xC010, NULL);
  if (toolerror() != 0) {
    SysBeep(); SysBeep(); //FlagError(4, toolerror());
    return;
  }

  read_file_rec.pCount = 4;
  read_file_rec.refNum = open_file_rec.refNum;
  read_file_rec.dataBuffer = *file_contents;
  read_file_rec.requestCount = open_file_rec.eof;
  ReadGS(&read_file_rec);
  if (toolerror() != 0) {
    SysBeep(); SysBeep(); SysBeep(); //FlagError(4, toolerror());
    return;
  }

  *file_handle = file_contents;
  *file_length = open_file_rec.eof;

  close_file_rev.pCount = 1;
  close_file_rev.refNum = open_file_rec.refNum;
  CloseGS(&close_file_rev);

  HUnlock(file_contents);
}

void Dictionary_Create(void) {
  GSString255 file_name;
  LongWord file_length;
  int i;

  file_name.length = 14;
  for (i=0; i<14; i++) {
    file_name.text[i] = DICTIONARY_FILENAME[i];
  }

  Dictionary_LoadFile(&file_name, &wgs_dictionary_handle, &file_length);
  wgs_dictionary_list_count = file_length / 5;

  file_name.length = 11;
  for (i=0; i<11; i++) {
    file_name.text[i] = SECRETS_FILENAME[i];
  }

  Dictionary_LoadFile(&file_name, &wgs_secret_word_handle, &file_length);
  wgs_secret_word_list_count = file_length / 5;
}

void Dictionary_NewGame(void) {
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

void Dictionary_GetWordFromList(char *src, char *dst, LongWord index) {
  int i;

  for (i=0; i<5; i++) {
    dst[i] = src[index * 5 + i];
  }

  dst[5] = '\0';
}

BOOLEAN Dictionary_DoesListContainWord(LongWord list_len, char *list, char *word) {
  LongWord low_idx = 0;
  LongWord high_idx = list_len - 1;
  LongWord mid_idx;
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
  int i;
  LongWord random_index = rand() % wgs_secret_word_list_count;

  HLock(wgs_secret_word_handle);
  for (i=0; i<5; i++) {
    buffer[i] = (*wgs_secret_word_handle)[random_index * 5 + i];
  }
  HUnlock(wgs_secret_word_handle);
}
