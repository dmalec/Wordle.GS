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
#include <gsos.h>
#include <memory.h>
#include <misctool.h>
#include <quickdraw.h>
#include <resources.h>
#include <window.h>

#include "main.h"
#include "wgs_gs_shim.h"


/* State */

static GrafPortPtr wgs_shim_progress_dialog_ptr;


/* Methods */

void GsShim_LoadFile(char *c_str_file_name, Handle *file_handle, LongWord *file_length) {
  BOOLEAN success = TRUE;
  OpenRecGS open_file_rec;
  Handle file_contents = NULL;
  IORecGS read_file_rec;
  RefNumRecGS close_file_rev;
  GSString255 file_name;

  file_name.length = 0;
  while (file_name.length < 255 && c_str_file_name[file_name.length] != '\0') {
    file_name.text[file_name.length] = c_str_file_name[file_name.length];
    file_name.length++;
  }

  *file_handle = NULL;
  *file_length = 0;

  open_file_rec.pCount = 12;
  open_file_rec.pathname = &file_name;
  open_file_rec.requestAccess = 1;         // Read mode
  open_file_rec.resourceNumber = 0;        // Read data fork
  open_file_rec.optionList = NULL;
  OpenGS(&open_file_rec);
  if (toolerror() != 0) {
    SysBeep(); //FlagError(4, toolerror());
    return;
  }

  file_contents = NewHandle(open_file_rec.eof, userid(), 0xC010, NULL);
  if (toolerror() != 0) {
    SysBeep(); SysBeep(); //FlagError(4, toolerror());
    return;
  }

  read_file_rec.pCount = 4;
  read_file_rec.refNum = open_file_rec.refNum;
  read_file_rec.dataBuffer = *file_contents;
  read_file_rec.requestCount = open_file_rec.eof;
  ReadGS(&read_file_rec);
  if (toolerror() != 0) {
    SysBeep(); SysBeep(); SysBeep(); //FlagError(4, toolerror());
    return;
  }

  *file_handle = file_contents;
  *file_length = open_file_rec.eof;

  close_file_rev.pCount = 1;
  close_file_rev.refNum = open_file_rec.refNum;
  CloseGS(&close_file_rev);

  HUnlock(file_contents);
}

void GsShim_ShowProgressDialog(void) {
  EventRecord dlg_event;
  unsigned short modal_dialog_flags[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
  Word modal_dialog_mask = Utils_GetBitmask16(modal_dialog_flags);

  wgs_shim_progress_dialog_ptr = NewWindow2("\pProgress", 0, NULL, NULL, 0x02, rez_window_Progress, rWindParam1);
  if (wgs_shim_progress_dialog_ptr == NULL) return;
  
  DoModalWindow(&dlg_event, NULL, NULL, NULL, modal_dialog_mask);
}

void GsShim_UpdateProgressDialog(unsigned int mercury_value, unsigned int mercury_scale) {
  CtlRecHndl thermometer_control_handle;
  double percentage = (double)mercury_value / (double)mercury_scale * 100.0;
  
  thermometer_control_handle = GetCtlHandleFromID(wgs_shim_progress_dialog_ptr, rez_window_Progress_ThermometerId);
  SetCtlValue((int)percentage, thermometer_control_handle);
}

void GsShim_HideProgressDialog(void) {
  InitCursor();
  CloseWindow(wgs_shim_progress_dialog_ptr);
}
