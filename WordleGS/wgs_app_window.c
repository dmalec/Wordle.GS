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
#include "wgs_dictionary.h"
#include "wgs_game_entities.h"
#include "wgs_game_model.h"
#include "wgs_game_over_dialog.h"
#include "wgs_render_system.h"


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
  announce_status = NoAnnouncement;

  ResetLetterGuessEntities();
  ResetLetterKeyEntites();

  GetRandomWord(word);
  NewSecretWord(word);
  NewGame();
  InvalidateWindow();
}


void HandleKeyPress (EventRecord event)
{
  BOOLEAN manual_update_needed = TRUE;
  char c = (char) event.message;

  if (GetGameState() != InProgress) return;
  
  if (isalpha(c)) {
    AddLetterToGuess(c);
  } else if (c == 0x08 || c == 0x7F) {
    RemoveLetterFromGuess();
  } else if (c == 0x0D) {
    wgs_guess_status status = GuessCurrentWord();
    
    if (status == InvalidWord) {
      char *guess_word = GetGuessWord();
      AlertWindow(awCString+awResource, (Pointer) &guess_word, 1);
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
    int row, col;

    for (row=0; row<WGS_NUMBER_OF_GUESSES; row++) {
      for (col=0; col<WGS_LETTERS_IN_WORD; col++) {
        wgs_letter_guess *letter_guess = &(wgs_letter_guess_entities[row][col]);

        if (letter_guess->image_state != ImageRendered) continue;
        
        PPToPort(&info, &letter_guess->box, letter_guess->box.h1, letter_guess->box.v1, modeCopy);

        RenderSystemDrawLetterGuess(letter_guess);
        
        letter_guess->image_state = ImageStatic;
      }
    }

    for (row=0; row<WGS_NUMBER_OF_KEY_ROWS; row++) {
      for (col=0; col<WGS_KEYBOARD_LEN[row]; col++) {
        wgs_letter_key *letter_key = &(wgs_letter_key_entities[row][col]);

        if (letter_key->image_state != ImageRendered) continue;
        
        PPToPort(&info, &letter_key->box, letter_key->box.h1, letter_key->box.v1, modeCopy);

        letter_key->image_state = ImageStatic;
      }
    }
  } else {
    RegionHndl vis_region = GetVisHandle();
    int x = (&(*vis_region)->rgnBBox).h1;
    int y = (&(*vis_region)->rgnBBox).v1;

    PPToPort(&info, &(*vis_region)->rgnBBox, x, y, modeCopy);
  }

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
  if (GetGameState() == InProgress) return;
  
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
  
  app_graf_port = NewWindow2("\pWordleGS", 0, DrawContents, NULL, 0x02, 1001, rWindParam1);

  return app_graf_port;
}
