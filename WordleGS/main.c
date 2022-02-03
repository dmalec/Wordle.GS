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
#include "wgs_game_entities.h"
#include "wgs_game_model.h"
#include "wgs_render_system.h"
#include "wgs_dictionary.h"

#include "wgs_game_engine.h"

BOOLEAN done;
EventRecord my_event;
int menu_num, menu_item_num;


void HandleAboutDialog(void) {
  AlertWindow(awResource, NULL, rez_alert_About);
}


void HandleQuitGame(void) {
  Word alert_result;

  if (IsGameInProgress()) {
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
    case file_Quit:
      HandleQuitGame();
      break;
      
    case 255:
      HandleNewGame();
      break;
      
    case 257:
      HandleAboutDialog();
      break;
      
  }
  
  HiliteMenu(FALSE, menu_num);
}


void InitMenus (void) {
  int height;
  MenuRecHndl menu_hand;
  
  menu_hand = NewMenu(
                     ">> Edit \\N3\r"
                     "--Undo\\N250*Zz\r"
                     "--Cut\\N251*Xx\r"
                     "--Copy\\N252*Cc\r"
                     "--Paste\\N253*Pp\r"
                     "--Clear\\N254\r"
                     ".\r");
  
  InsertMenu(menu_hand, 0);
  
  menu_hand = NewMenu(
                     ">> File \\N2\r"
                     "--New Game\\N255*Nn\r"
                     "--Quit\\N256*Qq\r"
                     ".\r");
  
  InsertMenu(menu_hand, 0);
  
  menu_hand = NewMenu(
                     ">>@\\XN1\r"
                     "--About...\\N257\r"
                     "---\\N258D\r"
                     ".\r");
  InsertMenu(menu_hand, 0);
  
  FixAppleMenu(1);
  height = FixMenuBar();
  DrawMenuBar();
}


int main (void) {
  int event;
  int i;
  char time_string[21];
  unsigned seed = 0;
  
  ReadAsciiTime(time_string);
  for (i=0; i<21; i++) {
    seed += time_string[i];
  }
  srand(seed);
  
  startdesk(320);
  InitMenus();
  InitCursor();
  
  
  CreateLetterGuessEntities();
  CreateLetterKeyEntites();
  GameEngine_Create();
  CreateRenderSystem();
  
  CreateAppWindow();
  HandleNewGame();
  
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
