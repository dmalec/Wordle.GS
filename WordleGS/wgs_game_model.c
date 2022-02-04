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

#include "wgs_alphabet_state.h"
#include "wgs_dictionary.h"
#include "wgs_game_engine.h"
#include "wgs_game_model.h"
#include "wgs_guess_state.h"


char secret_word[6] = "ROBOT";


void NewGame(void) {
}


wgs_guess_status GuessCurrentWord(void) {
  int current_guess_row = GuessState_GetRow();
  int i, letter_index;
  int matches = 0;
  char tmp_secret[5];
  char guess_word[] = "     ";
  char c;
  
  if (GuessState_GetCol() < 5) return WordFilled;

  if (GuessState_GetRow() >= 6) return MaxGuesses;
  
  GuessState_GetGuessWord(guess_word);

  if (!IsValidGuess(guess_word)) return InvalidWord;
  
  for (i=0; i<5; i++) {
    wgs_letter_state letter_state = GuessState_GetLetterState(current_guess_row, i);

    // Mark matches in advance - this will help with properly
    // marking misplaced versus incorrect letters later.
    if (letter_state.letter == secret_word[i]) {
      tmp_secret[i] = '+';
    } else {
      tmp_secret[i] = secret_word[i];
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
      int letter_idx = IndexOfLetter(tmp_secret, c);

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
    GameEngine_IncrementWinStat(current_guess_row);
    GameEngine_SetGameState(Won);
  } else if (current_guess_row >= 5) {
    GameEngine_SetGameState(Lost);
  } else {
    GuessState_NextGuess();
  }
  
  return ValidGuess;
}


BOOLEAN IsGameInProgress(void) {
  return GameEngine_GetGameState() == InProgress && (GuessState_GetRow() > 0 || GuessState_GetCol() > 0);
}


char *GetSecretWord(void) {
  return secret_word;
}


void NewSecretWord(char *word) {
  int i;
  
  for (i=0; i<5; i++) {
    secret_word[i] = word[i];
  }
  secret_word[5] = '\0';
}

int IndexOfLetter(char* word, char letter) {
  int i;
  
  for (i=0; i<5; i++) {
    if (word[i] == letter) {
      return i;
    }
  }
  
  return -1;
}