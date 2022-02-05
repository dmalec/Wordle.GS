# The MIT License (MIT)
#
# Copyright (c) 2013 James Grenning
# Modified by Dan Malec for use with Wordle GS
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

COMPONENT_NAME = wordle_gs

PROJECT_HOME_DIR = ..
CPPUTEST_HOME = ./cpputest

SRC_FILES += $(PROJECT_HOME_DIR)/WordleGS/wgs_alphabet_state.c
SRC_FILES += $(PROJECT_HOME_DIR)/WordleGS/wgs_game_engine.c
SRC_FILES += $(PROJECT_HOME_DIR)/WordleGS/wgs_guess_state.c
SRC_FILES += $(PROJECT_HOME_DIR)/WordleGS/wgs_utils.c

TEST_SRC_DIRS += tests

# Turn on CppUMock
CPPUTEST_USE_EXTENSIONS = Y

INCLUDE_DIRS += $(CPPUTEST_HOME)/include
INCLUDE_DIRS += $(CPPUTEST_HOME)/include/Platforms/Gcc

INCLUDE_DIRS += shim
INCLUDE_DIRS += $(PROJECT_HOME_DIR)/WordleGS

# Some flags to quiet clang
ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang"), 1)
CPPUTEST_WARNINGFLAGS += -Wno-unknown-warning-option
CPPUTEST_WARNINGFLAGS += -Wno-covered-switch-default
CPPUTEST_WARNINGFLAGS += -Wno-reserved-id-macro
CPPUTEST_WARNINGFLAGS += -Wno-keyword-macro
CPPUTEST_WARNINGFLAGS += -Wno-documentation
CPPUTEST_WARNINGFLAGS += -Wno-missing-noreturn
endif

CPPUTEST_WARNINGFLAGS += -Wno-unused-parameter

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
