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
#include <misctool.h>
#include <quickdraw.h>
#include <resources.h>
#include <stdio.h>
#include <time.h>

#include "wgs_app_window.h"

#include "wgs_game_engine.h"
#include "wgs_alphabet_state.h"
#include "wgs_guess_state.h"

#include "main.h"
#include "wgs_dictionary.h"
#include "wgs_game_model.h"
#include "wgs_game_over_dialog.h"
#include "wgs_render_system.h"

#define WGS_LETTERS_IN_WORD            5
#define WGS_NUMBER_OF_GUESSES          6

#define SQUARE_SIZE 20
#define SQUARE_INSET 2

#define KEY_SIZE 16
#define KEY_INSET 1


typedef enum { None, Pending } wgs_drawing_statue;
wgs_drawing_statue drawing_status = None;

typedef enum { NoAnnouncement, ResultAnnounced } wgs_announce_state;
wgs_announce_state announce_status = NoAnnouncement;

GrafPortPtr app_graf_port;
GrafPort offscreen_graf_port;


void InvalidateWindow(void) {
  GrafPortPtr curr_port = GetPort();
  Rect r;
  
  SetPort(app_graf_port);
  GetPortRect(&r);
  InvalRect(&r);
  SetPort(curr_port);
}


void HandleNewGame(void) {
  char word[16];
  Word alert_result;

  if (IsGameInProgress()) {
    alert_result = AlertWindow(awResource, NULL, rez_alert_VerifyNewGame);
    if (alert_result == rez_alert_VerifyNewGame_Cancel) {
      return;
    }
  }

  GameEngine_NewGame();

  announce_status = NoAnnouncement;

  GetRandomWord(word);
  NewSecretWord(word);
  NewGame();
  InvalidateWindow();
}


void HandleKeyPress (EventRecord event)
{
  BOOLEAN manual_update_needed = TRUE;
  char c = (char) event.message;

  if (GameEngine_GetGameState() != InProgress) return;
  
  if (isalpha(c)) {
    GuessState_AddLetterToGuess(c);
  } else if (c == 0x08 || c == 0x7F) {
    GuessState_RemoveLetterFromGuess();
  } else if (c == 0x0D) {
    wgs_guess_status status = GuessCurrentWord();

    if (status == InvalidWord) {
      char guess_word[] = "     ";
      char *ptr = guess_word;

      GuessState_GetGuessWord(guess_word);
      AlertWindow(awCString+awResource, (Pointer)&ptr, rez_alert_UnknownWord);
      manual_update_needed = false;
    }
  }
  
  if (manual_update_needed) {
    drawing_status = Pending;
    InvalidateWindow();
  }
}


void DrawContents (void)
{
  LocInfo info = GetRenderSystemLocInfo();
  
  RenderSystemUpdate();
  SetPort(app_graf_port);

  if (drawing_status == Pending) {
    int row, col, i;

    for (row=0; row<WGS_NUMBER_OF_GUESSES; row++) {
      for (col=0; col<WGS_LETTERS_IN_WORD; col++) {
        wgs_letter_state letter_state = GuessState_GetLetterState(row, col);

        if (! letter_state.changed) continue;
        
        PPToPort(&info, &letter_state.render_box, letter_state.render_box.h1, letter_state.render_box.v1, modeCopy);

        RenderSystemDrawLetterGuess(letter_state);
      }
    }

    for (i=0; i<WGS_GAME_GUESSES_NUMBER_OF_LETTERS; i++) {
      wgs_letter_state letter_state = AlphabetState_GetLetterState((char)('A' + i));

      if (! letter_state.changed) continue;

      PPToPort(&info, &letter_state.render_box, letter_state.render_box.h1, letter_state.render_box.v1, modeCopy);

      RenderSystemDrawLetterKey(letter_state);
    }
  } else {
    RegionHndl vis_region = GetVisHandle();
    int x = (&(*vis_region)->rgnBBox).h1;
    int y = (&(*vis_region)->rgnBBox).v1;

    PPToPort(&info, &(*vis_region)->rgnBBox, x, y, modeCopy);
  }

  GameEngine_UpdateFinished();
  drawing_status = None;
}


void ShowEndGame(void) {
  GrafPortPtr dlg_ptr = NewWindow2("\p Game Won ", 0, NULL, NULL, 0x02, 1002, rWindParam1);
  EventRecord dlg_event;
  LongWord dlg_part;
  
  if (dlg_ptr == NULL) return;
  
  do {
    dlg_part = DoModalWindow(&dlg_event, NULL, NULL, NULL, 0x401E);
  } while (dlg_part != 1 && dlg_part != 2);
  
  InitCursor();
  CloseWindow(dlg_ptr);
  
  if (dlg_part == 1) {
    HandleNewGame();
  }
}


void AppTick(void) {

  if (drawing_status == Pending) return;
  if (GameEngine_GetGameState() == InProgress) return;
  
  if (announce_status == NoAnnouncement) {
    wgs_game_over_response response = HandleGameOverDialog();
    announce_status = ResultAnnounced;
    
    if (response == NewGameRequested) {
      HandleNewGame();
    }
  }
}


GrafPortPtr CreateAppWindow(void) {
  offscreen_graf_port = GetRenderSystemOffscreen();
  
  app_graf_port = NewWindow2("\pWordleGS", 0, DrawContents, NULL, 0x02, rez_window_Wordle, rWindParam1);

  return app_graf_port;
}
