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

#include "wgs_game_entities.h"
#include "wgs_render_system.h"

#define WGS_TITLE_TEXT             "WORDLE GS"
#define WGS_LETTER_GUESS_INSET_H   4
#define WGS_LETTER_GUESS_INSET_V   (WGS_LETTER_GUESS_SQUARE_SIZE - 8)


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

void RenderSystemDrawLetterGuess(wgs_letter_guess *letter_guess) {
  int fill_color = 1; // dark grey
  int line_color = 15; // white

  switch (letter_guess->state) {
      
    case CorrectGuess:
      line_color = 10; // green
      fill_color = 5; // dark green
      break;
      
    case WrongPlaceGuess:
      line_color = 9; // yellow
      fill_color = 6; // orange
      break;
      
    case IncorrectGuess:
      line_color = 15; // white
      fill_color = 14; // light gray
      break;
  }

  SetSolidPenPat(fill_color);
  PaintRect(&letter_guess->box);
  
  if (letter_guess->letter != ' ') {
    SetForeColor(15);
    SetBackColor(fill_color);
    MoveTo(letter_guess->box.h1 + WGS_LETTER_GUESS_INSET_H, letter_guess->box.v1 + WGS_LETTER_GUESS_INSET_V);
    putchar(letter_guess->letter);
  }
  
  SetSolidPenPat(line_color);
  FrameRect(&letter_guess->box);
}


void RenderSystemDrawLetterKey(wgs_letter_key *letter_key) {
  int fill_color = 1; // dark grey
  int line_color = 15; // white
  
  if (letter_key->letter == ' ') return;

  switch (letter_key->state) {
      
    case CorrectKey:
      line_color = 10; // green
      fill_color = 5; // dark green
      break;
      
    case WrongPlaceKey:
      line_color = 9; // yellow
      fill_color = 6; // orange
      break;
      
    case IncorrectKey:
      line_color = 15; // white
      fill_color = 14; // light gray
      break;
  }

  SetSolidPenPat(fill_color);
  PaintRect(&letter_key->box);
  
  SetForeColor(15);
  SetBackColor(fill_color);
  MoveTo(letter_key->box.h1 + WGS_LETTER_GUESS_INSET_H, letter_key->box.v1 + WGS_LETTER_GUESS_INSET_V);
  putchar(letter_key->letter);
  
  SetSolidPenPat(line_color);
  FrameRect(&letter_key->box);
}


void RenderSystemUpdate(void) {
  int row, col;
  GrafPortPtr curr_port = GetPort();

  SetPort(&rs_offscreen_graf_port);

  for (row=0; row<WGS_NUMBER_OF_GUESSES; row++) {
    for (col=0; col<WGS_LETTERS_IN_WORD; col++) {
      wgs_letter_guess *letter_guess = &(wgs_letter_guess_entities[row][col]);

      if (letter_guess->image_state != ImageDirty) continue;
      
      RenderSystemDrawLetterGuess(letter_guess);
      
      letter_guess->image_state = ImageRendered;
    }
  }

  for (row=0; row<WGS_NUMBER_OF_KEY_ROWS; row++) {
    for (col=0; col<WGS_KEYBOARD_LEN[row]; col++) {
      wgs_letter_key *letter_key = &(wgs_letter_key_entities[row][col]);

      if (letter_key->image_state != ImageDirty) continue;
      
      RenderSystemDrawLetterKey(letter_key);
      
      letter_key->image_state = ImageRendered;
    }
  }

  SetPort(curr_port);
}

GrafPort GetRenderSystemOffscreen(void) {
  return rs_offscreen_graf_port;
}

LocInfo GetRenderSystemLocInfo(void) {
  return rs_offscreen_info;
}
