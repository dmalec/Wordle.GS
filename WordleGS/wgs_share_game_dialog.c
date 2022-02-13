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
#include "wgs_game_engine.h"
#include "wgs_game_sequence.h"
#include "wgs_game_types.h"
#include "wgs_guess_state.h"
#include "wgs_render_system.h"
#include "wgs_share_game_dialog.h"


/* Constants */

#define WGS_TITLE_TEXT             "WORDLE GS"


/* State */

GrafPortPtr wgs_share_game_dialog_ptr;


/* Local Prototypes */

void ShareGameDialog_DrawContents(void);


/* Methods */

void ShareGameDialog_DrawContents(void) {
  Rect r;
  char code_word[] = "     ";
  char buffer[128];
  int title_width = CStringWidth(WGS_TITLE_TEXT);
  int buffer_screen_width;
  int row, col;
  wgs_letter_state raw_letter_state, share_letter_state;

  /* Erase Background */
  SetRect(&r, 0, 0, 320, 200);
  SetSolidPenPat(0);
  PaintRect(&r);

  /* Draw Title */
  SetForeColor(15);
  SetBackColor(0);
  MoveTo(320 / 2 - title_width / 2, 30);
  DrawCString(WGS_TITLE_TEXT);

  /* Draw Code Info */
  GameSequence_GetSequenceCode(code_word);
  sprintf(buffer, "Code: %s", code_word);
  buffer_screen_width = CStringWidth(buffer);
  MoveTo(10, 45);
  DrawCString(buffer);

  /* Draw Game Info */
  if (GameEngine_GetGameState() == Lost) {
    sprintf(buffer, "-/6");
  } else {
    sprintf(buffer, "%d/6", GuessState_GetRow() + 1);
  }
  buffer_screen_width = CStringWidth(buffer);
  MoveTo(160 - buffer_screen_width / 2, 45);
  DrawCString(buffer);

  /* Draw Word Info */
  sprintf(buffer, "Word: %d", GameSequence_GetSequenceIndex());
  buffer_screen_width = CStringWidth(buffer);
  MoveTo(310 - buffer_screen_width, 45);
  DrawCString(buffer);

  /* Draw Round */
  share_letter_state.letter = ' ';
  for (row=0; row<GuessState_GetRow() + 1; row++) {
    for (col=0; col<5; col++) {
      raw_letter_state = GuessState_GetLetterState(row, col);
      share_letter_state.status = raw_letter_state.status;
      SetRect(&share_letter_state.render_box, 120 + col * 16 + 1, 52 + row * 16 + 1, 120 + col * 16 + 14, 52 + row * 16 + 14);
      RenderSystemDrawLetterGuess(share_letter_state);
    }
  }
  
  DrawControls(wgs_share_game_dialog_ptr);
}

void ShareGameDialog_Show(void) {
  EventRecord dlg_event;
  LongWord dlg_part;
  unsigned short modal_dialog_flags[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
  Word modal_dialog_mask = Utils_GetBitmask16(modal_dialog_flags);

  wgs_share_game_dialog_ptr = NewWindow2("\pShare Game", 0, ShareGameDialog_DrawContents, NULL, 0x02, rez_window_ShareGame, rWindParam1);
  if (wgs_share_game_dialog_ptr == NULL) return;
  
  do {
    dlg_part = DoModalWindow(&dlg_event, NULL, NULL, NULL, modal_dialog_mask);
  } while (dlg_part != rez_window_ShareGame_CloseButtonId);
  
  InitCursor();
  CloseWindow(wgs_share_game_dialog_ptr);
}
