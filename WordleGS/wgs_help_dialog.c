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
#include <memory.h>
#include <quickdraw.h>
#include <resources.h>
#include <window.h>

#include "main.h"
#include "wgs_game_types.h"
#include "wgs_help_dialog.h"
#include "wgs_render_system.h"
#include "wgs_utils.h"


/* State */

static GrafPortPtr wgs_help_dialog_ptr;


/* Local Prototypes */

void HelpDialog_DrawContents(void);
Handle HelpDialog_LoadCStringResourceOrNull(int resource_id);
void HelpDialog_ReleaseCStringResource(int resource_id, Handle handle);

/* UI Methods */

Handle HelpDialog_LoadCStringResourceOrNull(int resource_id) {
  Handle handle = NULL;

  handle = LoadResource(rCString, resource_id);
  if (toolerror() == 0) {
    HLock(handle);
    return handle;
  }

  return NULL;
}

void HelpDialog_ReleaseCStringResource(int resource_id, Handle handle) {
  if (handle == NULL) return;

  ReleaseResource(2, rCString, resource_id);
}

void HelpDialog_DrawWord(int x, int y, char *word, int highlight_idx, wgs_letter_status highlight) {
  // TODO: Consolidate letter drawing code
  wgs_letter_state letter_state;
  int i;
  Rect r;

  r.h1 = x;
  r.h2 = r.h1 + 20 * 5;
  r.v1 = y;
  r.v2 = r.v1 + 20;

  SetSolidPenPat(0);
  PaintRect(&r);

  for (i=0; i<5; i++) {
    letter_state.letter = word[i];
    letter_state.status = (i == highlight_idx) ? highlight : gtUnusedLetter;
    letter_state.render_box.h1 = x + (i * 20) + 2;
    letter_state.render_box.h2 = letter_state.render_box.h1 + 16;
    letter_state.render_box.v1 = y + 2;
    letter_state.render_box.v2 = letter_state.render_box.v1 + 16;

    RenderSystemDrawLetterGuess(letter_state);
  }

}

void HelpDialog_DrawContents(void) {
  HelpDialog_DrawWord(8, 44, "APPLE", 0, gtCorrectLetter);
  HelpDialog_DrawWord(8, 80, "MOUSE", 1, gtWrongPlaceLetter);
  HelpDialog_DrawWord(8, 116, "DRIVE", 2, gtIncorrectLetter);

  SetForeColor(0);
  SetBackColor(15);

  DrawControls(wgs_help_dialog_ptr);
}

void HelpDialog_Show(void) {
  EventRecord dialog_event;
  LongWord dialog_part;
  unsigned short modal_dialog_flags[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
  Word modal_dialog_mask = Utils_GetBitmask16(modal_dialog_flags);

  wgs_help_dialog_ptr = NewWindow2("\pHelp", 0, HelpDialog_DrawContents, NULL, 0x02, rez_window_Help, rWindParam1);
  if (wgs_help_dialog_ptr == NULL) return;

  do {
    dialog_part = DoModalWindow(&dialog_event, NULL, NULL, NULL, modal_dialog_mask);
  } while (dialog_part != rez_window_Help_CloseButtonId);

  InitCursor();
  CloseWindow(wgs_help_dialog_ptr);

  wgs_help_dialog_ptr = NULL;
}
