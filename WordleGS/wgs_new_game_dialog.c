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

#include <control.h>
#include <quickdraw.h>
#include <resources.h>
#include <window.h>

#include "main.h"
#include "wgs_new_game_dialog.h"
#include "wgs_utils.h"

/* State */

static Str255 wgs_new_game_dialog_code;


/* Methods */

wgs_new_game_dialog_response NewGameDialog_Show(void) {
  GrafPortPtr wgs_new_game_dialog_ptr;
  EventRecord dialog_event;
  LongWord dialog_part;
  unsigned short modal_dialog_flags[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
  Word modal_dialog_mask = Utils_GetBitmask16(modal_dialog_flags);
  wgs_new_game_dialog_response response = NewGameDialog_Cancel;

  wgs_new_game_dialog_ptr = NewWindow2("\pNew Game", 0, NULL, NULL, 0x02, rez_window_NewGame, rWindParam1);
  if (wgs_new_game_dialog_ptr == NULL) return;

  do {
    dialog_part = DoModalWindow(&dialog_event, NULL, NULL, NULL, modal_dialog_mask);
  } while (dialog_part != rez_window_NewGame_OKButtonId && dialog_part != rez_window_NewGame_CancelButtonId);

  if (dialog_part == rez_window_NewGame_OKButtonId) {
    GetLETextByID(wgs_new_game_dialog_ptr, rez_window_NewGame_GameCodeId, &wgs_new_game_dialog_code);
    response = NewGameDialog_NewGame;
  }

  InitCursor();
  CloseWindow(wgs_new_game_dialog_ptr);
  
  return response;
}

void NewGameDialog_GetCode(Str255 *code) {
  int i;

  code->textLength = wgs_new_game_dialog_code.textLength;

  for (i=0; i<wgs_new_game_dialog_code.textLength; i++) {
    code->text[i] = wgs_new_game_dialog_code.text[i];
  }
}
