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

#include "wgs_engine/wgs_game_engine.h"

#include "wgs_engine/wgs_alphabet_state.h"
#include "wgs_engine/wgs_dictionary.h"
#include "wgs_engine/wgs_game_guesses.h"

static char wgs_game_engine_secret_word[] = "DEBUG";

static wgs_game_state wgs_game_engine_game_state;

static wgs_game_engine_stats[7] = { 0, 0, 0, 0, 0, 0, 0 };


/* Lifecycle Methods */

void GameEngine_Create(void) {
  Dictionary_Create();
  AlphabetState_Create();
  GameGuesses_Create();
}

void GameEngine_NewGame(void) {
  AlphabetState_NewGame();
  GameGuesses_NewGame();
  
  GetRandomWord(wgs_game_engine_secret_word);
  
  wgs_game_engine_game_state = InProgress;
}

void GameEngine_UpdateFinished(void) {
  AlphabetState_UpdateFinished();
  GameGuesses_UpdateFinished();
}

void GameEngine_Destroy(void) {
  Dictionary_Destroy();
  AlphabetState_Destroy();
  GameGuesses_Destroy();
}


/* Game Methods */

int GameEngine_IndexOfLetter(char* word, char letter) {
  int i;
  
  for (i=0; i<5; i++) {
    if (word[i] == letter) {
      return i;
    }
  }
  
  return -1;
}

void GameEngine_GuessCurrentWord(void) {
  char guess_word[5];
  char tmp_secret[5];
  int i;
  int matches = 0;
  wgs_guess_status guess_status;
  
  guess_status = GameGuesses_GetGuessStatus();
  if (guess_status != WordFilled) return;
  // TODO: Warn user
  
  GameGuesses_GetGuessWord(guess_word);
  if (!IsValidGuess(guess_word)) return;
  // TODO: Warn user

  // Mark letters which are in the correct place in advance.
  // This is part of making sure that the hint colors align with
  // the actual number of letters in the secret word.
  for (i=0; i<5; i++) {
    if (guess_word[i] == wgs_game_engine_secret_word[i]) {
      tmp_secret[i] = '+';
    } else {
      tmp_secret[i] = wgs_game_engine_secret_word[i];
    }
  }
  
  // Update the guess word based on the pre-marked secret word.
  for (i=0; i<5; i++) {
    
    if (tmp_secret[i] == '+') {
      AlphabetState_MaybeUpdateLetterStatus(guess_word[i], gtCorrectLetter);
      GameGuesses_MaybeUpdateGuessLetterStatus(i, gtCorrectLetter);
      matches++;
    } else {
      int letter_idx = GameEngine_IndexOfLetter(tmp_secret, guess_word[i]);

      if (letter_idx >= 0) {

        AlphabetState_MaybeUpdateLetterStatus(guess_word[i], gtWrongPlaceLetter);
        GameGuesses_MaybeUpdateGuessLetterStatus(i, gtWrongPlaceLetter);

        // Clear the letter, so it won't be double counted
        // if it appears again in the guess.
        tmp_secret[letter_idx] = ' ';
      } else {
        AlphabetState_MaybeUpdateLetterStatus(guess_word[i], gtIncorrectLetter);
        GameGuesses_MaybeUpdateGuessLetterStatus(i, gtIncorrectLetter);
      }
    }
  }

  if (matches >= 5) {
    wgs_game_engine_game_state = Won;
    wgs_game_engine_stats[GameGuesses_GetRow]++;
  } else if (GameGuesses_GetGuessStatus() == MaxGuesses) {
    wgs_game_engine_game_state = Lost;
  } else {
    GameGuesses_NextGuess();
  }
}
