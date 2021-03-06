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
#include "wgs_game_sequence.h"
#include "wgs_guess_state.h"
#include "wgs_scoring.h"
#include "wgs_utils.h"


/* State */

static wgs_game_state wgs_game_engine_game_state;

static char wgs_game_engine_secret_word[6] = "ROBOT";


/* Local Prototypes */

void GameEngine_NewSecretWord(char *word);


/* Lifecycle Methods */

void GameEngine_Create(void) {
  AlphabetState_Create();
  Dictionary_Create();
  GameSequence_Create(Dictionary_GetNumberOfSecretWords());
  GuessState_Create();
  Scoring_Create();
}

void GameEngine_NewGame(char code_word[]) {
  if (code_word == NULL) {
    char dict_code_word[6];

    Dictionary_GetRandomWord(dict_code_word);
    GameSequence_NewGame(dict_code_word);

  } else {
    GameSequence_NewGame(code_word);
  }

  GameEngine_NextRound();
}

void GameEngine_NextRound(void) {
  unsigned int secret_word_index;
  char secret_word[] = "     ";

  AlphabetState_NextRound();
  Dictionary_NextRound();
  secret_word_index = GameSequence_GetSequenceValue();
  GameSequence_NextRound();
  GuessState_NextRound();

  wgs_game_engine_game_state = InProgress;

  Dictionary_GetWord(secret_word_index, secret_word);
  GameEngine_NewSecretWord(secret_word);
}

void GameEngine_NewSecretWord(char *secret_word) {
  Utils_StringNCopy(wgs_game_engine_secret_word, secret_word, 5);
  wgs_game_engine_secret_word[5] = '\0';
}

void GameEngine_UpdateFinished(void) {
  AlphabetState_UpdateFinished();
  Dictionary_UpdateFinished();
  GameSequence_UpdateFinished();
  GuessState_UpdateFinished();
}

void GameEngine_Destroy(void) {
  AlphabetState_Destroy();
  Dictionary_Destroy();
  GameSequence_Destroy();
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
  wgs_letter_status guess_grade[5];
  char guess_word[] = "     ";
  int i;
  int matches;

  if (GuessState_GetRow() >= 6) return MaxGuesses;
  if (GuessState_GetCol() < 5) return WordFilled;

  GuessState_GetGuessWord(guess_word);
  if (!Dictionary_IsValidGuess(guess_word)) return InvalidWord;

  matches = Scoring_GradeWord(guess_word, wgs_game_engine_secret_word, guess_grade);
  for (i=0; i<5; i++) {
    GuessState_MaybeUpdateLetterStatus(i, guess_grade[i]);
    AlphabetState_MaybeUpdateLetterStatus(guess_word[i], guess_grade[i]);
  }

  if (matches >= 5) {
    wgs_game_engine_game_state = Won;
    Scoring_RecordWin(current_guess_row);
  } else if (current_guess_row >= 5) {
    wgs_game_engine_game_state = Lost;
    Scoring_RecordLoss();
  } else {
    GuessState_NextGuess();
  }

  return ValidGuess;
}

void GameEngine_GetSecretWord(char *word) {
  Utils_StringNCopy(word, wgs_game_engine_secret_word, 5);
}

wgs_game_stats GameEngine_GetStats(void) {
  int guess_num;
  wgs_game_stats game_stats;

  game_stats.guess_max_distribution = Scoring_GetMaxGuessDistribution();

  for (guess_num=0; guess_num<WGS_GAME_ENGINE_MAX_GUESSES; guess_num++) {
    game_stats.guess_distribution[guess_num] = Scoring_GetGuessDistributionAbsolute(guess_num);
    game_stats.guess_distribution_percentage[guess_num] = Scoring_GetGuessDistributionPercentage(guess_num);
  }

  game_stats.total_played = Scoring_GetTotalPlayed();
  game_stats.win_percentage = Scoring_GetWinPercentage();
  game_stats.current_streak = Scoring_GetCurrentStreak();
  game_stats.longest_streak = Scoring_GetLongestStreak();

  return game_stats;
}
