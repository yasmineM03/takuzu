#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int test_dummy(void) { return EXIT_SUCCESS; }

void usage(int argc, char* argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int test_game_is_over(void)
{
  game g = game_default_solution();
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  game g1 = game_default();
  if (g1 == NULL) {
    game_delete(g);
    exit(EXIT_FAILURE);
  }
  bool test = game_is_over(g);
  bool test1 = !game_is_over(g1);
  game_set_square(g, 0, 0, S_ONE);
  bool test2 = !game_is_over(g);
  square tab[] = {S_IMMUTABLE_ZERO, S_ZERO, S_ONE,  S_ONE,  S_ZERO, S_ONE, S_ONE,  S_ZERO, S_ZERO,
                  S_ZERO,           S_ONE,  S_ONE,  S_ZERO, S_ONE,  S_ONE, S_ZERO, S_ONE,  S_ONE,
                  S_ZERO,           S_ZERO, S_ONE,  S_ZERO, S_ZERO, S_ONE, S_ONE,  S_ONE,  S_ZERO,
                  S_ZERO,           S_ONE,  S_ZERO, S_ZERO, S_ONE};

  game g2 = game_new_ext(4, 8, tab, false, true);
  if (g2 == NULL) {
    game_delete(g);
    game_delete(g1);
    return EXIT_FAILURE;
  }
  bool test3 = game_is_over(g2);
  game g3 = game_new_ext(4, 4, tab, true, false);
  bool test4 = game_is_over(g3);
  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  if (!test || !test1 || !test2) {
    return EXIT_FAILURE;
  }
  if (test3 == true || test4 == true) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int test_game_restart(void)
{
  game g = game_new_empty_ext(6, 6, false, false);
  game g2 = game_new_empty_ext(6, 6, false, false);
  game_play_move(g, 0, 0, S_ZERO);
  game_play_move(g, 0, 1, S_ZERO);
  game_restart(g);
  if (!game_equal(g, g2)) {
    game_delete(g);
    game_delete(g2);
    return EXIT_FAILURE;
  }
  game_undo(g);
  if (!game_equal(g, g2)) {
    game_delete(g);
    game_delete(g2);
    return EXIT_FAILURE;
  }
  game_delete(g);
  game_delete(g2);
  return EXIT_SUCCESS;
}

int test_game_play_move(void)
{
  game g = game_default();
  game g1 = game_default();
  game_play_move(g, 0, 0, S_ONE);
  game_set_square(g1, 0, 0, S_ONE);
  if (game_equal(g, g1) == false) {
    game_delete(g);
    game_delete(g1);
    return EXIT_FAILURE;
  }
  game_play_move(g, 0, 0, S_ZERO);
  game_set_square(g1, 0, 0, S_ZERO);
  if (game_equal(g, g1) == false) {
    game_delete(g);
    game_delete(g1);
    return EXIT_FAILURE;
  }
  game_play_move(g, 0, 0, S_EMPTY);
  game_set_square(g1, 0, 0, S_EMPTY);
  if (game_equal(g, g1) == false) {
    game_delete(g);
    game_delete(g1);
    return EXIT_FAILURE;
  }
  game_delete(g);
  game_delete(g1);
  return EXIT_SUCCESS;
}

int test_game_check_move(void)
{
  game g = game_default();
  bool test1 = game_check_move(g, DEFAULT_SIZE + 1, 1, S_ZERO);
  bool test8 = game_check_move(g, 1, DEFAULT_SIZE + 1, S_ZERO);  // must be EXIT_FAILURE
  bool test9 = game_check_move(g, -1, 0, S_ZERO);                // EXIT_FAILURE

  bool test10 = game_check_move(g, 0, -1, S_EMPTY);                             // EXIT_FAILURE
  bool test2 = game_check_move(g, DEFAULT_SIZE - 1, DEFAULT_SIZE - 1, S_ZERO);  // EXIT_FAILURE
  bool test6 = game_check_move(g, DEFAULT_SIZE - 1, DEFAULT_SIZE - 1, S_ONE);   // EXIT_FAILURE
  bool test7 = game_check_move(g, DEFAULT_SIZE - 1, DEFAULT_SIZE - 1,
                               S_EMPTY);           // must be EXIT_FAILURE bc it is an immutable square
  bool test3 = game_check_move(g, 0, 0, S_ZERO);   // must be EXIT_SUCCESS
  bool test4 = game_check_move(g, 0, 0, S_ONE);    // must be EXIT_SUCCESS
  bool test5 = game_check_move(g, 0, 0, S_EMPTY);  // must be EXIT_SUCCESS
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_check_move(g, i, j, S_ZERO) == false) {
        if (game_get_square(g, i, j) != S_IMMUTABLE_ONE && game_get_square(g, i, j) != S_IMMUTABLE_ZERO) {
          game_delete(g);
          return EXIT_FAILURE;
        }
      }
    }
  }
  if (test1 == 0 && test8 == 0 && test9 == 0 && test10 == 0 && test2 == 0 && test6 == 0 && test7 == 0 && test3 == 1 &&
      test4 == 1 && test5 == 1) {
    game_delete(g);
    return EXIT_SUCCESS;
  }
  game_delete(g);
  return EXIT_FAILURE;
}

int test_game_print(void)
{
  game g = game_default();
  game_print(g);
  if (g == NULL) {
    game_delete(g);
    return EXIT_FAILURE;
  }
  game_delete(g);
  return EXIT_SUCCESS;
}

int test_game_is_wrapping(void)
{
  game g = game_new_empty_ext(6, 2, false, false);
  bool ok = true;

  ok = ok && !game_is_wrapping(g);
  game_delete(g);

  g = game_new_empty_ext(12, 24, true, false);
  ok = ok && game_is_wrapping(g);
  game_delete(g);

  g = game_new_empty_ext(2, 2, true, true);
  ok = ok && game_is_wrapping(g);
  game_delete(g);

  g = game_new_empty_ext(8, 6, false, true);
  ok = ok && !game_is_wrapping(g);
  game_delete(g);

  if (ok == true) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int test_game_load(void)
{
  bool ok = true;
  game g = game_load("default.txt");
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  ok = ok && (game_get_square(g, 0, 0) == S_EMPTY);
  ok = ok && (game_get_square(g, 1, 0) == S_EMPTY);
  ok = ok && (game_get_square(g, 0, 1) == S_IMMUTABLE_ONE);
  ok = ok && (game_get_square(g, 5, 5) == S_IMMUTABLE_ZERO);
  game_delete(g);
  if (ok == true) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int test_game_save(void)
{
  game g = game_default();
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  game g1 = game_default_solution();
  if (g1 == NULL) {
    game_delete(g);
    exit(EXIT_FAILURE);
  }
  char* name = "first.txt";
  char* name1 = "second.txt";
  game_save(g, name);
  game_save(g1, name1);
  game gbis = game_load("first.txt");
  if (gbis == NULL) {
    game_delete(g);
    game_delete(g1);
    exit(EXIT_FAILURE);
  }
  game g1bis = game_load("second.txt");
  if (g1bis == NULL) {
    game_delete(g);
    game_delete(g1);
    game_delete(gbis);
    exit(EXIT_FAILURE);
  }
  if (game_equal(g, gbis) && game_equal(g1, g1bis)) {
    game_delete(g);
    game_delete(g1);
    game_delete(gbis);
    game_delete(g1bis);
    return EXIT_SUCCESS;
  }
  game_delete(g);
  game_delete(g1);
  game_delete(gbis);
  game_delete(g1bis);
  return EXIT_FAILURE;
}

int main(int argcount, char* argv[])
{
  int test = 0;
  if (argcount == 2) {
    if (strcmp(argv[1], "dummy") == 0) {
      test = test_dummy();
    } else if (strcmp(argv[1], "game_play_move") == 0) {
      test = test_game_play_move();
    } else if (strcmp(argv[1], "game_restart") == 0) {
      test = test_game_restart();
    } else if (strcmp(argv[1], "game_check_move") == 0) {
      test = test_game_check_move();
    } else if (strcmp(argv[1], "game_is_over") == 0) {
      test = test_game_is_over();
    } else if (strcmp(argv[1], "game_print") == 0) {
      test = test_game_print();
    } else if (strcmp(argv[1], "game_is_wrapping") == 0) {
      test = test_game_is_wrapping();
    } else if (strcmp(argv[1], "game_load") == 0) {
      test = test_game_load();
    } else if (strcmp(argv[1], "game_save") == 0) {
      test = test_game_save();
    } else {
      test = EXIT_FAILURE;
    }
    if (test == EXIT_FAILURE) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
