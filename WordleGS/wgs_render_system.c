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

#include "wgs_alphabet_state.h"
#include "wgs_game_engine.h"
#include "wgs_game_types.h"
#include "wgs_guess_state.h"
#include "wgs_render_system.h"


#define WGS_TITLE_TEXT             "WORDLE GS"
#define WGS_LETTER_GUESS_INSET_H   4
#define WGS_LETTER_KEY_INSET_H     2
#define WGS_LETTER_GUESS_INSET_V   12


GrafPort rs_offscreen_graf_port;
Handle rs_offscreen_buffer;
LocInfo rs_offscreen_info;


void RenderSystemCreateOffscreenGrafPort(void) {
  rs_offscreen_buffer = NewHandle(32000, userid(), 0xC010, 0);
  
  HLock(rs_offscreen_buffer);

  rs_offscreen_info.portSCB = 0x00;
  rs_offscreen_info.ptrToPixImage = *rs_offscreen_buffer;
  rs_offscreen_info.boundsRect.h1 = 0;
  rs_offscreen_info.boundsRect.h2 = 320;
  rs_offscreen_info.boundsRect.v1 = 0;
  rs_offscreen_info.boundsRect.v2 = 200;
  rs_offscreen_info.width = 160;
  
  OpenPort(&rs_offscreen_graf_port);
  SetPortLoc(&rs_offscreen_info);
}

void RenderSystemDrawBackdrop(void) {
  Rect r;
  int title_width = CStringWidth(WGS_TITLE_TEXT);

  SetPort(&rs_offscreen_graf_port);

  r.h1 = 0;     r.h2 = 320;
  r.v1 = 13;    r.v2 = 200;
  
  SetSolidPenPat(0);
  PaintRect(&r);
    
  SetForeColor(15);
  SetBackColor(0);
  MoveTo(320 / 2 - title_width / 2, 22);
  DrawCString(WGS_TITLE_TEXT);
}

void CreateRenderSystem(void) {
  GrafPortPtr curr_port;

  RenderSystemCreateOffscreenGrafPort();
  
  curr_port = GetPort();
  RenderSystemDrawBackdrop();
  SetPort(curr_port);
}

void RenderSystemDrawLetterGuess(wgs_letter_state letter_state) {
  int fill_color = 1; // dark grey
  int line_color = 15; // white

  switch (letter_state.status) {
      
    case gtCorrectLetter:
      line_color = 10; // green
      fill_color = 5; // dark green
      break;
      
    case gtWrongPlaceLetter:
      line_color = 9; // yellow
      fill_color = 6; // orange
      break;
      
    case gtIncorrectLetter:
      line_color = 15; // white
      fill_color = 14; // light gray
      break;
  }

  SetSolidPenPat(fill_color);
  PaintRect(&letter_state.render_box);
  
  if (letter_state.letter != ' ') {
    SetForeColor(15);
    SetBackColor(fill_color);
    MoveTo(letter_state.render_box.h1 + WGS_LETTER_GUESS_INSET_H, letter_state.render_box.v1 + WGS_LETTER_GUESS_INSET_V);
    DrawChar(letter_state.letter);
  }
  
  SetSolidPenPat(line_color);
  FrameRect(&letter_state.render_box);
}


void RenderSystemDrawLetterKey(wgs_letter_state letter_state) {
  int fill_color = 1; // dark grey
  int line_color = 15; // white
  
  switch (letter_state.status) {
      
    case gtCorrectLetter:
      line_color = 10; // green
      fill_color = 5; // dark green
      break;
      
    case gtWrongPlaceLetter:
      line_color = 9; // yellow
      fill_color = 6; // orange
      break;
      
    case gtIncorrectLetter:
      line_color = 15; // white
      fill_color = 14; // light gray
      break;
  }

  SetSolidPenPat(fill_color);
  PaintRect(&letter_state.render_box);
  
  SetForeColor(15);
  SetBackColor(fill_color);
  MoveTo(letter_state.render_box.h1 + WGS_LETTER_KEY_INSET_H, letter_state.render_box.v1 + WGS_LETTER_GUESS_INSET_V);
  DrawChar(letter_state.letter);
  
  SetSolidPenPat(line_color);
  FrameRect(&letter_state.render_box);
}


void RenderSystemUpdate(void) {
  int row, col, i;
  GrafPortPtr curr_port = GetPort();

  SetPort(&rs_offscreen_graf_port);

  for (row=0; row<WGS_GAME_GUESSES_NUMBER_OF_ROWS; row++) {
    for (col=0; col<WGS_GAME_GUESSES_NUMBER_OF_COLS; col++) {
      wgs_letter_state letter_state = GuessState_GetLetterState(row, col);

      if (! letter_state.changed) continue;
      
      RenderSystemDrawLetterGuess(letter_state);
    }
  }

  for (i=0; i<WGS_GAME_GUESSES_NUMBER_OF_LETTERS; i++) {
    wgs_letter_state letter_state = AlphabetState_GetLetterState((char)('A' + i));
    
    if (! letter_state.changed) continue;

    RenderSystemDrawLetterKey(letter_state);
  }
  
  SetPort(curr_port);
}

GrafPort GetRenderSystemOffscreen(void) {
  return rs_offscreen_graf_port;
}

LocInfo GetRenderSystemLocInfo(void) {
  return rs_offscreen_info;
}
