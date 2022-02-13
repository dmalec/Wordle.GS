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
#include <desk.h>
#include <event.h>
#include <menu.h>
#include <misctool.h>
#include <orca.h>
#include <quickdraw.h>
#include <resources.h>
#include <stdio.h>
#include <stdlib.h>
#include <window.h>

#include "main.h"
#include "wgs_app_window.h"
#include "wgs_game_engine.h"
#include "wgs_game_over_dialog.h"
#include "wgs_help_dialog.h"
#include "wgs_new_game_dialog.h"
#include "wgs_render_system.h"

BOOLEAN done;
EventRecord my_event;
int menu_num, menu_item_num;

/* Prototypes */

void HandleNextRound(void);


/* Implementation */

void HandleAboutDialog(void) {
  AlertWindow(awResource, NULL, rez_alert_About);
}

void HandleNewGame(void) {
  Str255 code_str255;
  char code_c_string[] = "     ";
  int i;
  unsigned seed = 0;
  
  if (GameEngine_IsGameInProgress()) {
    Word alert_result = AlertWindow(awResource, NULL, rez_alert_VerifyNewGame);
    if (alert_result == rez_alert_VerifyNewGame_Cancel) {
      return;
    }
  }

  if (NewGameDialog_Show() == NewGameDialog_Cancel) {
    return;
  }

  NewGameDialog_GetCode(&code_str255);
  
  for (i=0; i<5; i++) {
    code_c_string[i] = code_str255.text[i];
  }
  
  GameEngine_NewGame(code_c_string);
  AppWindow_NextRound();
}

void HandleNextWord(void) {  
  if (GameEngine_IsGameInProgress()) {
    Word alert_result = AlertWindow(awResource, NULL, rez_alert_VerifyNextWord);
    if (alert_result == rez_alert_VerifyNextWord_Cancel) {
      return;
    }
  }

  HandleNextRound();
}

void HandleViewStats(void) {
  wgs_game_over_response response = HandleGameOverDialog();

  if (response == NewGameRequested) {
    HandleNextRound();
  }
}

void HandleNextRound(void) {
  GameEngine_NextRound();
  AppWindow_NextRound();
}

void HandleQuitGame(void) {
  Word alert_result;

  if (GameEngine_IsGameInProgress()) {
    alert_result = AlertWindow(awResource, NULL, rez_alert_VerifyQuitGame);
    if (alert_result == rez_alert_VerifyNewGame_Cancel) {
      return;
    }
  }

  done = TRUE;
}


void HandleMenu (void) {
  int i;
  
  switch (menu_item_num) {
    case rez_menuitem_QuitId:
      HandleQuitGame();
      break;
      
    case rez_menuitem_NewGameId:
      HandleNewGame();
      break;
      
    case rez_menuitem_AboutId:
      HandleAboutDialog();
      break;

    case rez_menuitem_HelpId:
      HelpDialog_Show();
      break;

    case rez_menuitem_NextWordId:
      HandleNextWord();
      break;

    case rez_menuitem_ViewStatsId:
      HandleViewStats();
      break;
  }
  
  HiliteMenu(FALSE, menu_num);
}


void InitMenus (void) {
  int height;
  MenuBarRecHndl menu_bar_handle;

  menu_bar_handle = NewMenuBar2(refIsResource, rez_menubar_Main, NULL);
  SetSysBar(menu_bar_handle);
  SetMenuBar(NULL);
  
  FixAppleMenu(rez_menu_Apple);
  height = FixMenuBar();
  DrawMenuBar();
}


int main (void) {
  int event;
  int i;
  char time_string[21];
  unsigned seed = 0;
  Rect background;
  
  ReadAsciiTime(time_string);
  for (i=0; i<21; i++) {
    seed += time_string[i];
  }
  srand(seed);
  
  startdesk(320);

  SetRect(&background, 0, 0, 320, 200);
  SetSolidPenPat(0);
  PaintRect(&background);

  InitMenus();
  InitCursor();

  GameEngine_Create();
  GameEngine_NewGame(NULL);
  
  CreateRenderSystem();
  CreateAppWindow();
  AppWindow_NextRound();
  
  PenNormal();
  my_event.wmTaskMask = 0x001F7FFF;
  
  done = FALSE;
  while (!done) {
    event = TaskMaster(everyEvent, &my_event);
    
    switch (event) {
      case wInSpecial:
      case wInMenuBar:
        menu_num = (int) (my_event.wmTaskData >> 16);
        menu_item_num = (int) my_event.wmTaskData;
        
        HandleMenu();
        break;
        
      case keyDownEvt:
        HandleKeyPress(my_event);
        break;
        
    }
    
    AppTick();
  }
  
  GameEngine_Destroy();
  
  enddesk();
}
