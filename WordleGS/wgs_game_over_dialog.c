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
#include <font.h>
#include <quickdraw.h>
#include <resources.h>
#include <types.h>
#include <window.h>

#include "main.h"
#include "wgs_game_engine.h"
#include "wgs_game_over_dialog.h"
#include "wgs_share_game_dialog.h"


/* Constants */

#define DIALOG_WIDTH                  280
#define DIALOG_HEIGHT                 157

#define DIALOG_H_CENTER               (DIALOG_WIDTH / 2)

#define STATS_H                       (DIALOG_H_CENTER + 8)

#define THERMOMETER_H                 18
#define THERMOMETER_WIDTH             (DIALOG_H_CENTER - 8 - THERMOMETER_H)


/* Structs */

typedef struct wgs_game_over_dialog_guess_thermometer {
  Point guess_number_point;
  Rect thermometer_outline;
  Rect thermometer_fill_perimeter;
  Rect thermometer_fill_inside;
  Point guess_value_point;
} wgs_game_over_dialog_guess_thermometer;


/* State */

GrafPortPtr game_over_dialog_ptr;
static wgs_game_over_dialog_guess_thermometer guess_distribution[6];


/* Methods */

void GameOverDialog_DrawInt(int val) {
  char buffer[32];

  sprintf(buffer, "%d", val);
  DrawCString(buffer);
}

void GameOverDialog_BuildGuessDistribution(wgs_game_stats game_stats) {
  int guess_number;
  int base_line;
  int fill_line;
  int distribution_value_width;
  char buffer[128];

  for (guess_number=0; guess_number<6; guess_number++) {
    base_line = 50 + guess_number * 14;

    if (game_stats.guess_distribution[guess_number] == 0) {
      fill_line = 0;
    } else {
      fill_line = (THERMOMETER_WIDTH - 10) * game_stats.guess_distribution_percentage[guess_number] - 2;
    }

    sprintf(buffer, "%d", game_stats.guess_distribution[guess_number]);
    distribution_value_width = CStringWidth(buffer);

    SetPt(&guess_distribution[guess_number].guess_number_point, THERMOMETER_H - 10, base_line);
    SetPt(&guess_distribution[guess_number].guess_value_point, THERMOMETER_H + 2 + fill_line - distribution_value_width + 8, base_line);
    SetRect(&guess_distribution[guess_number].thermometer_outline, THERMOMETER_H, base_line - 10, THERMOMETER_H + THERMOMETER_WIDTH, base_line + 3);
    SetRect(&guess_distribution[guess_number].thermometer_fill_perimeter, THERMOMETER_H + 1, base_line - 9, THERMOMETER_H + 11 + fill_line, base_line + 2);
    SetRect(&guess_distribution[guess_number].thermometer_fill_inside, THERMOMETER_H + 2, base_line - 8, THERMOMETER_H + 10 + fill_line, base_line + 1);
  }
}

void GameOverDialogDrawContents(void) {
  char buffer[128];
  char secret_word[] = "     ";
  int i;
  int title_width;
  Rect r;
  wgs_game_stats game_stats;
  FontID font_id;

  game_stats = GameEngine_GetStats();
  GameOverDialog_BuildGuessDistribution(game_stats);
  GameEngine_GetSecretWord(secret_word);

  /* Draw labels and lines in black on white */
  SetForeColor(0);
  SetSolidPenPat(0);
  SetBackColor(15);

  /* Draw banner text */
  if (GameEngine_GetGameState() == Won) {
    sprintf(buffer, "You successfully guessed %s", secret_word);
  } else if (GameEngine_GetGameState() == Lost) {
    sprintf(buffer, "The word was actually: %s", secret_word);
  } else {
    char code_word[] = "     ";

    GameSequence_GetSequenceCode(code_word);
    sprintf(buffer, "Code: %s   Word: %d   Guess: %d/6", code_word, GameSequence_GetSequenceIndex() + 1, GuessState_GetRow() + 1);
  }
  title_width = CStringWidth(buffer);
  MoveTo(DIALOG_H_CENTER - (title_width / 2), 10);
  DrawCString(buffer);

  /* Draw section headers */

  /* switch to bold */
  font_id.fidLong = FMGetCurFID();
  font_id.fidRec.fontStyle = font_id.fidRec.fontStyle | 0x1;
  InstallFont(font_id, 0);

  MoveTo(THERMOMETER_H, 34);
  DrawCString("Distribution");

  MoveTo(180, 34);
  DrawCString("Stats");

  /* switch to plain */
  font_id.fidLong = FMGetCurFID();
  font_id.fidRec.fontStyle = font_id.fidRec.fontStyle & 0xE;
  InstallFont(font_id, 0);

  /* Draw horizontal divider */
  MoveTo(DIALOG_H_CENTER, 40);
  LineTo(DIALOG_H_CENTER, 123);

  /* Draw stats */
  MoveTo(STATS_H, 64);
  sprintf(buffer, "Played: %d", game_stats.total_played);
  DrawCString(buffer);

  MoveTo(STATS_H, 78);
  sprintf(buffer, "Win %%: %d", game_stats.win_percentage);
  DrawCString(buffer);

  MoveTo(STATS_H, 92);
  sprintf(buffer, "Streak: %d", game_stats.current_streak);
  DrawCString(buffer);

  MoveTo(STATS_H, 106);
  sprintf(buffer, "Max Streak: %d", game_stats.longest_streak);
  DrawCString(buffer);

  /* Draw distribution labels */
  for (i=0; i<6; i++) {
    MoveTo(guess_distribution[i].guess_number_point.h, guess_distribution[i].guess_number_point.v);
    GameOverDialog_DrawInt(i+1);
  }

  /* Draw outline of thermometers */
  for (i=0; i<6; i++) {
    FrameRect(&guess_distribution[i].thermometer_outline);
  }

  /* Draw outline of thermometer fill in light green */
  SetSolidPenPat(10);
  for (i=0; i<6; i++) {
    FrameRect(&guess_distribution[i].thermometer_fill_perimeter);
  }

  /* Draw interior of thermometer fill in dark green */
  SetSolidPenPat(5);
  for (i=0; i<6; i++) {
    PaintRect(&guess_distribution[i].thermometer_fill_inside);
  }

  /* Draw distribution value in white on dark green */
  SetForeColor(15);
  SetBackColor(5);
  for (i=0; i<6; i++) {
    MoveTo(guess_distribution[i].guess_value_point.h, guess_distribution[i].guess_value_point.v);
    GameOverDialog_DrawInt(game_stats.guess_distribution[i]);
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
