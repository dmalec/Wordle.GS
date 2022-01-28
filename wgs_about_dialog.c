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

#include "wgs_about_dialog.h"


void HandleAboutDialog(void) {
  EventRecord dlg_event;
  LongWord dlg_part;
  GrafPortPtr about_dialot_ptr;
  
  about_dialot_ptr = NewWindow2("\p About ", 0, NULL, NULL, 0x02, 1101, rWindParam1);
  if (about_dialot_ptr == NULL) return;

  do {
    dlg_part = DoModalWindow(&dlg_event, NULL, NULL, NULL, 0x401E);
  } while (dlg_part != 1);

  InitCursor();
  CloseWindow(about_dialot_ptr);
}
