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

#include <gsos.h>
#include <misctool.h>
#include <types.h>

#include "wgs_gs_utils.h"

void GSUtils_Test(void) {
  GSString255 filename;
  char buffer[] = "HELLO GS WORLD!";

  filename.length = 3;
  filename.text[0] = 'T';
  filename.text[1] = 'M';
  filename.text[2] = 'P';
  
  GsUtils_SaveFile(filename, buffer, 15);
}

void GsUtils_SaveFile(GSString255 filename, Pointer data, LongWord bytes) {
  CreateRecGS create_file_rec;
  IORecGS write_file_rec;
  NameRecGS destroy_file_rec;
  OpenRecGS open_file_rec;
  RefNumRecGS close_file_rec;

  destroy_file_rec.pCount = 1;
  destroy_file_rec.pathname = &filename;
  DestroyGS(&destroy_file_rec);

  create_file_rec.pCount = 5;
  create_file_rec.pathname = &filename;
  create_file_rec.access = 0xC3;
  create_file_rec.fileType = 0x04;
  create_file_rec.auxType = 0;
  create_file_rec.storageType = 1;
  CreateGS(&create_file_rec);
  if (toolerror() != 0) {
    SysBeep(); //FlagError(4, toolerror());
    return;
  }

  open_file_rec.pCount = 3;
  open_file_rec.pathname = &filename;
  open_file_rec.requestAccess = 2;         // Write mode
  OpenGS(&open_file_rec);
  if (toolerror() != 0) {
    SysBeep(); //FlagError(4, toolerror());
    return;
  }
  
  write_file_rec.pCount = 4;
  write_file_rec.refNum = open_file_rec.refNum;
  write_file_rec.dataBuffer = data;
  write_file_rec.requestCount = bytes;
  WriteGS(&write_file_rec);
  if (toolerror() != 0) {
    SysBeep(); //FlagError(4, toolerror());
  }
  
  close_file_rec.pCount = 1;
  close_file_rec.refNum = open_file_rec.refNum;
  CloseGS(&close_file_rec);
}
