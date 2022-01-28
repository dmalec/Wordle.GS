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

#include "wgs_game_model.h"

#include "wgs_dictionary.h"
#include "wgs_game_entities.h"

int current_guess_row = 0;
int current_guess_col = 0;

char secret_word[6] = "ROBOT";

wgs_game_state game_state = InProgress;

char* guesses[] = {
  "     ",
  "     ",
  "     ",
  "     ",
  "     ",
  "     "
};

wgs_square_state letter_guesses[] = {
  Unknown, Unknown, Unknown, Unknown, Unknown, Unknown, Unknown, Unknown, Unknown, Unknown,
  Unknown, Unknown, Unknown, Unknown, Unknown, Unknown, Unknown, Unknown, Unknown, Unknown,
  Unknown, Unknown, Unknown, Unknown, Unknown, Unknown
};

int stats[7] = { 0, 0, 0, 0, 0, 0, 0 };

wgs_square_state guess_square_status[][] = {
  { Unknown, Unknown, Unknown, Unknown, Unknown},
  { Unknown, Unknown, Unknown, Unknown, Unknown},
  { Unknown, Unknown, Unknown, Unknown, Unknown},
  { Unknown, Unknown, Unknown, Unknown, Unknown},
  { Unknown, Unknown, Unknown, Unknown, Unknown},
  { Unknown, Unknown, Unknown, Unknown, Unknown}
};


void NewGame(void) {
  int i, row, col;

  current_guess_row = 0;
  current_guess_col = 0;
  game_state = InProgress;

  for (row=0; row<6; row++) {
    for (col=0; col<5; col++) {
      guesses[row][col] = ' ';
      guess_square_status[row][col] = Unknown;
    }
  }
  
  for (i=0; i<26; i++) {
    letter_guesses[i] = Unknown;
  }
}


void AddLetterToGuess(char letter) {
  if (!isalpha(letter)) return;
  
  if (current_guess_col >= 5) return;
  
  guesses[current_guess_row][current_guess_col] = toupper(letter);
  
  wgs_letter_guess_entities[current_guess_row][current_guess_col].letter = toupper(letter);
  wgs_letter_guess_entities[current_guess_row][current_guess_col].image_state = ImageDirty;
  
  current_guess_col++;
}


void RemoveLetterFromGuess(void) {
  if (current_guess_col <= 0) return;
  
  current_guess_col--;
  guesses[current_guess_row][current_guess_col] = ' ';

  wgs_letter_guess_entities[current_guess_row][current_guess_col].letter = ' ';
  wgs_letter_guess_entities[current_guess_row][current_guess_col].image_state = ImageDirty;
}


wgs_guess_status GuessCurrentWord(void) {
  int i, letter_index;
  int matches = 0;
  char c;
  
  if (current_guess_col < 5) return WordFilled;

  if (current_guess_row >= 6) return MaxGuesses;
  
  if (!IsValidGuess(guesses[current_guess_row])) return InvalidWord;
  
  for (i=0; i<5; i++) {
    c = guesses[current_guess_row][i];
    letter_index = c - 'A';
    
    if (c == secret_word[i]) {
      letter_guesses[letter_index] = Correct;
      guess_square_status[current_guess_row][i] = Correct;
      wgs_letter_guess_entities[current_guess_row][i].state = CorrectGuess;
      UpdateLetterKey(c, CorrectKey);
      matches++;
    } else if (ContainsLetter(c)) {
      letter_guesses[letter_index] = WrongPlace;
      guess_square_status[current_guess_row][i] = WrongPlace;
      wgs_letter_guess_entities[current_guess_row][i].state = WrongPlaceGuess;
      UpdateLetterKey(c, WrongPlaceKey);
    } else {
      letter_guesses[letter_index] = UnusedLetter;
      guess_square_status[current_guess_row][i] = UnusedLetter;
      wgs_letter_guess_entities[current_guess_row][i].state = IncorrectGuess;
      UpdateLetterKey(c, IncorrectKey);
    }
    
    wgs_letter_guess_entities[current_guess_row][i].image_state = ImageDirty;
  }
  
  if (matches >= 5) {
    stats[current_guess_row]++;
    game_state = Won;
  } else if (current_guess_row >= 5) {
    game_state = Lost;
  } else {
    current_guess_col = 0;
    current_guess_row++;
  }
  
  return ValidGuess;
}


char GetGuessSquareLetter(int row, int col) {
  return guesses[row][col];
}


wgs_square_state GetGuessSquareStatus(int row, int col) {
  return guess_square_status[row][col];
}


wgs_square_state GetLetterStatus(char c) {
  return letter_guesses[c - 'A'];
}


wgs_game_state GetGameState(void) {
  return game_state;
}


int GetGuessRow(void) {
  return current_guess_row;
}


int GetGuessCol(void) {
  return current_guess_col;
}


char *GetSecretWord(void) {
  return secret_word;
}

char *GetGuessWord(void) {
  return guesses[current_guess_row];
}

void NewSecretWord(char *word) {
  int i;
  
  for (i=0; i<5; i++) {
    secret_word[i] = word[i];
  }
  secret_word[5] = '\0';
}

int GetGuessNumberWon(int guess_num) {
  return stats[guess_num];
}

int ContainsLetter(char letter) {
  int i;
  
  for (i=0; i<5; i++) {
    if (secret_word[i] == letter) {
      return 1;
    }
  }
  
  return 0;
}
