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

#include "wgs_game_over_dialog.h"
#include "wgs_game_model.h"

GrafPortPtr game_over_dialog_ptr;

void GameOverDialogDrawContents (void) {
  int i, h, offset;
  int max_wins = 0;
  int stat;
  Rect r;
  
  // Erase Background
  GetPortRect(&r);
  SetSolidPenPat(15);
  PaintRect(&r);

  SetForeColor(0);
  SetBackColor(15);
  
  if (GetGameState() == Won) {
    MoveTo(8, 10);
    DrawCString("Congratulations!\0");
    MoveTo(8, 28);
    DrawCString("You guessed the word!\0");
  } else if (GetGameState() == Lost) {
    MoveTo(8, 10);
    DrawCString("The word was:\0");
    MoveTo(8, 28);
    DrawCString(GetSecretWord());
  }
  
  for (i=0; i<6; i++) {
    MoveTo(8, 50 + i * 14);
    printf("%d", i);
    
    stat = GetGuessNumberWon(i);
    if (stat > max_wins) {
      max_wins = stat;
    }
  }

  SetSolidPenPat(5);

  for (i=0; i<6; i++) {
    stat = GetGuessNumberWon(i);
    if (stat == 0) {
      offset = 0;
    } else {
      offset = 230 * ((float)stat / (float)max_wins);
    }
    
    r.h1 = 20;
    r.v1 = 50 + i * 14 - 8;

    r.h2 = r.h1 + 8 + offset;
    r.v2 = r.v1 + 9;

    PaintRect(&r);
  }

  SetForeColor(15);
  SetBackColor(5);

  for (i=0; i<6; i++) {
    stat = GetGuessNumberWon(i);
    if (stat == 0) {
      offset = 0;
    } else {
      offset = 230 * ((float)stat / (float)max_wins);
    }
    
    h = 20 + offset;
    MoveTo(h, 50 + i * 14);
    printf("%d", stat);
  }

  DrawControls(game_over_dialog_ptr);
}

wgs_game_over_response HandleGameOverDialog(void) {
  EventRecord dlg_event;
  LongWord dlg_part;

  game_over_dialog_ptr = NewWindow2("\pGame Over", 0, GameOverDialogDrawContents, NULL, 0x02, 1002, rWindParam1);
  if (game_over_dialog_ptr == NULL) return;
  
  do {
    dlg_part = DoModalWindow(&dlg_event, NULL, NULL, NULL, 0x401E);
  } while (dlg_part != 1 && dlg_part != 2);
  
  InitCursor();
  CloseWindow(game_over_dialog_ptr);
  
  if (dlg_part == 1) {
    return NewGameRequested;
  } else {
    return CloseGameOver;
  }
}
