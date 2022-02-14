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
#include "wgs_game_over_dialog.h"
#include "wgs_share_game_dialog.h"

GrafPortPtr game_over_dialog_ptr;

void GameOverDialogDrawContents (void) {
  char buffer[128];
  char secret_word[] = "     ";
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
  
  if (GameEngine_GetGameState() == Won) {
    MoveTo(8, 10);
    DrawCString("Congratulations!\0");
    MoveTo(8, 28);
    DrawCString("You guessed the word!\0");
  } else if (GameEngine_GetGameState() == Lost) {
    MoveTo(8, 10);
    DrawCString("The word was:\0");
    MoveTo(8, 28);
    GameEngine_GetSecretWord(secret_word);
    DrawCString(secret_word);
  } else {
    char code_word[] = "     ";

    MoveTo(8, 10);
    DrawCString("WORDLE GS!\0");

    GameSequence_GetSequenceCode(code_word);
    sprintf(buffer, "%s  %d  %d/6", code_word, GameSequence_GetSequenceIndex(), GuessState_GetRow() + 1);

    MoveTo(8, 28);
    DrawCString(buffer);
  }
  
  for (i=0; i<6; i++) {
    sprintf(buffer, "%d", i+1);
    MoveTo(8, 50 + i * 14);
    DrawCString(buffer);

    stat = GameEngine_GetWinStat(i);
    if (stat > max_wins) {
      max_wins = stat;
    }
  }

  SetSolidPenPat(0);

  for (i=0; i<6; i++) {
    r.h1 = 18;
    r.v1 = 50 + i * 14 - 10;

    r.h2 = r.h1 + 240;
    r.v2 = r.v1 + 13;

    FrameRect(&r);
  }

  SetSolidPenPat(10); // greem

  for (i=0; i<6; i++) {
    stat = GameEngine_GetWinStat(i);
    if (stat == 0) {
      offset = 0;
    } else {
      offset = 230 * ((float)stat / (float)max_wins) - 2;
    }

    r.h1 = 19;
    r.v1 = 50 + i * 14 - 9;

    r.h2 = r.h1 + 10 + offset;
    r.v2 = r.v1 + 11;

    FrameRect(&r);
  }

  SetSolidPenPat(5);

  for (i=0; i<6; i++) {
    stat = GameEngine_GetWinStat(i);
    if (stat == 0) {
      offset = 0;
    } else {
      offset = 230 * ((float)stat / (float)max_wins) - 2;
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
    stat = GameEngine_GetWinStat(i);
    if (stat == 0) {
      offset = 0;
    } else {
      offset = 230 * ((float)stat / (float)max_wins) - 2;
    }
    
    h = 20 + offset;
    sprintf(buffer, "%d", stat);
    MoveTo(h, 50 + i * 14);
    DrawCString(buffer);
  }

  DrawControls(game_over_dialog_ptr);
}

wgs_game_over_response HandleGameOverDialog(void) {
  EventRecord dlg_event;
  LongWord dlg_part;
  unsigned short modal_dialog_flags[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
  Word modal_dialog_mask = Utils_GetBitmask16(modal_dialog_flags);

  game_over_dialog_ptr = NewWindow2("\pGame Over", 0, GameOverDialogDrawContents, NULL, 0x02, rez_window_GameOver, rWindParam1);
  if (game_over_dialog_ptr == NULL) return;
  
  if (GameEngine_GetGameState() == InProgress) {
    CtlRecHndl share_game_button_handle = GetCtlHandleFromID(game_over_dialog_ptr, rez_window_GameOver_ShareGameButtonId);
    CtlRecHndl new_game_button_handle = GetCtlHandleFromID(game_over_dialog_ptr, rez_window_GameOver_NewGameButtonId);

    if (share_game_button_handle != NULL) {
      HideControl(share_game_button_handle);
    }
    if (new_game_button_handle != NULL) {
      HideControl(new_game_button_handle);
    }
  }

  do {
    dlg_part = DoModalWindow(&dlg_event, NULL, NULL, NULL, modal_dialog_mask);
    
    if (dlg_part == rez_window_GameOver_ShareGameButtonId) {
      ShareGameDialog_Show();
    }
  } while (dlg_part != rez_window_GameOver_NewGameButtonId && dlg_part != rez_window_GameOver_CloseButtonId);
  
  InitCursor();
  CloseWindow(game_over_dialog_ptr);
  
  if (dlg_part == rez_window_GameOver_NewGameButtonId) {
    return NewGameRequested;
  } else {
    return CloseGameOver;
  }
}
