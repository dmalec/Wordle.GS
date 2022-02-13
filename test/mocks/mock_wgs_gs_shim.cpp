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

#include "CppUTestExt/MockSupport.h"

extern "C" {
#include "wgs_gs_shim.h"
}


void GsShim_LoadFile(char *c_str_file_name, Handle *file_handle, LongWord *file_length) {
  mock()
    .actualCall("GsShim_LoadFile")
    .withStringParameter("c_str_file_name", c_str_file_name)
    .withOutputParameter("file_handle", file_handle)
    .withOutputParameter("file_length", file_length);
}

void GsShim_ShowProgressDialog(void) {
  mock().actualCall("GsShim_ShowProgressDialog");
}

void GsShim_UpdateProgressDialog(unsigned int mercury_value, unsigned int mercury_scale) {
  mock()
    .actualCall("GsShim_UpdateProgressDialog")
    .withUnsignedLongIntParameter("mercury_value", mercury_value)
    .withUnsignedLongIntParameter("mercury_scale", mercury_scale);

}

void GsShim_HideProgressDialog(void) {
  mock().actualCall("GsShim_HideProgressDialog");
}
