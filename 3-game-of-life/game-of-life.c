#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void show(void *universe_pre, int width, int height);
void evolve(void *universe_pre, int width, int height);
void game(int width, int height);

int main(int argc, char **argv) {
  int width = 0, height = 0;

  if (argc > 1) {
    width = atoi(argv[1]);
  }

  if (argc > 2) {
    height = atoi(argv[2]);
  }

  if (width <= 0) {
    width = 30;
  }

  if (height <= 0) {
    height = 30;
  }

  game(width, height);

  return 0;
}

void show(void *universe_pre, int width, int height) {
  int(*universe)[width] = universe_pre;

  printf("\033[H");

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      printf(universe[i][j] ? "\033[07m \033[m" : "  ");
    }

    printf("\033[E");
  }

  fflush(stdout);
}

void evolve(void *universe_pre, int width, int height) {
  unsigned(*universe)[width] = universe_pre;
  unsigned universe_next[height][width];

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int steps = 0;

      for (int x = i - 1; x <= i + 1; x++) {
        for (int y = j - 1; y <= j + 1; y++) {
          if (universe[(x + height) % height][(y + width) % width]) {
            steps++;
          }
        }
      }

      if (universe[i][j]) {
        steps--;
      }

      universe_next[i][j] = (steps == 3 || (steps == 2 && universe[i][j]));
    }
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      universe[i][j] = universe_next[i][j];
    }
  }
}

void game(int width, int height) {
  unsigned universe[height][width];

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      /* Initial configuration */
      universe[j][i] = rand() < RAND_MAX / 10 ? 1 : 0;
    }
  }

  while (1) {
    show(universe, width, height);
    evolve(universe, width, height);
    usleep(200000);
  }
}