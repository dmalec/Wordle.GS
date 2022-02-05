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

#include "wgs_game_engine.h"

#include "wgs_alphabet_state.h"
#include "wgs_dictionary.h"
#include "wgs_guess_state.h"


/* State */

static wgs_game_state wgs_game_engine_game_state;

static int wgs_game_engine_win_stats[WGS_GAME_ENGINE_MAX_GUESSES];

static char wgs_game_engine_secret_word[6] = "ROBOT";


/* Local Prototypes */

void GameEngine_NewSecretWord(char *word);
int GameEngine_IndexOfLetter(char* word, char letter);


/* Lifecycle Methods */

void GameEngine_Create(void) {
  int guess_num;

  for (guess_num=0; guess_num<WGS_GAME_ENGINE_MAX_GUESSES; guess_num++) {
    wgs_game_engine_win_stats[guess_num] = 0;
  }

  AlphabetState_Create();
  Dictionary_Create();
  GuessState_Create();
}

void GameEngine_NewGame(void) {
  char secret_word[] = "     ";

  AlphabetState_NewGame();
  Dictionary_NewGame();
  GuessState_NewGame();

  wgs_game_engine_game_state = InProgress;

  Dictionary_GetRandomWord(secret_word);
  GameEngine_NewSecretWord(secret_word);
}

void GameEngine_NewSecretWord(char *secret_word) {
  int i;

  for (i=0; i<5; i++) {
    wgs_game_engine_secret_word[i] = secret_word[i];
  }
  wgs_game_engine_secret_word[5] = '\0';
}

void GameEngine_UpdateFinished(void) {
  AlphabetState_UpdateFinished();
  Dictionary_UpdateFinished();
  GuessState_UpdateFinished();
}

void GameEngine_Destroy(void) {
  AlphabetState_Destroy();
  Dictionary_Destroy();
  GuessState_Destroy();
}


/* Game Methods */

wgs_game_state GameEngine_GetGameState(void) {
  return wgs_game_engine_game_state;
}

BOOLEAN GameEngine_IsGameInProgress(void) {
  return wgs_game_engine_game_state == InProgress && (GuessState_GetRow() > 0 || GuessState_GetCol() > 0);
}


wgs_guess_status GameEngine_GuessCurrentWord(void) {
  int current_guess_row = GuessState_GetRow();
  int i, letter_index;
  int matches = 0;
  char tmp_secret[5];
  char guess_word[] = "     ";
  char c;

  if (GuessState_GetRow() >= 6) return MaxGuesses;
  if (GuessState_GetCol() < 5) return WordFilled;

  GuessState_GetGuessWord(guess_word);
  if (!Dictionary_IsValidGuess(guess_word)) return InvalidWord;

  for (i=0; i<5; i++) {
    wgs_letter_state letter_state = GuessState_GetLetterState(current_guess_row, i);

    // Mark matches in advance - this will help with properly
    // marking misplaced versus incorrect letters later.
    if (letter_state.letter == wgs_game_engine_secret_word[i]) {
      tmp_secret[i] = '+';
    } else {
      tmp_secret[i] = wgs_game_engine_secret_word[i];
    }
  }

  for (i=0; i<5; i++) {
    wgs_letter_state letter_state = GuessState_GetLetterState(current_guess_row, i);

    c = letter_state.letter;
    letter_index = c - 'A';

    if (tmp_secret[i] == '+') {
      GuessState_MaybeUpdateLetterStatus(i, gtCorrectLetter);
      AlphabetState_MaybeUpdateLetterStatus(c, gtCorrectLetter);
      matches++;
    } else {
      int letter_idx = GameEngine_IndexOfLetter(tmp_secret, c);

      if (letter_idx >= 0) {
        GuessState_MaybeUpdateLetterStatus(i, gtWrongPlaceLetter);
        AlphabetState_MaybeUpdateLetterStatus(c, gtWrongPlaceLetter);

        // Clear the letter, so it won't be double counted
        // if it appears again in the guess.
        tmp_secret[letter_idx] = ' ';
      } else {
        GuessState_MaybeUpdateLetterStatus(i, gtIncorrectLetter);
        AlphabetState_MaybeUpdateLetterStatus(c, gtIncorrectLetter);
      }
    }
  }

  if (matches >= 5) {
    wgs_game_engine_game_state = Won;
    wgs_game_engine_win_stats[current_guess_row]++;
  } else if (current_guess_row >= 5) {
    wgs_game_engine_game_state = Lost;
  } else {
    GuessState_NextGuess();
  }

  return ValidGuess;
}

int GameEngine_IndexOfLetter(char* word, char letter) {
  int i;

  for (i=0; i<5; i++) {
    if (word[i] == letter) {
      return i;
    }
  }

  return -1;
}


void GameEngine_GetSecretWord(char *word) {
  int i;

  for (i=0; i<5; i++) {
    word[i] = wgs_game_engine_secret_word[i];
  }
}


int GameEngine_GetWinStat(int guess_num) {
  return wgs_game_engine_win_stats[guess_num];
}
