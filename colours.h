/* 
 * https://en.wikipedia.org/wiki/ANSI_escape_code
 *
 * EXAMPLE:
 * #include <stdio.h>
 * #include "colours.h"
 *
 * int main() {
 *   set_colour(FG_RED);
 *   set_colour(BG_GREEN);
 *   bold();
 *   printf("Yo!\n");
 *   reset();
 *
 *   return 0;
 * }
 */

enum Code {
  FG_BLACK    = 30,
  FG_RED      = 31,
  FG_GREEN    = 32,
  FG_YELLOW   = 33,
  FG_BLUE     = 34,
  FG_MAGENTA  = 35,
  FG_CYAN     = 36,
  FG_WHITE    = 37,
  BG_BLACK    = 40,
  BG_RED      = 41,
  BG_GREEN    = 42,
  BG_YELLOW   = 43,
  BG_BLUE     = 44,
  BG_MAGENTA  = 45,
  BG_CYAN     = 46,
  BG_WHITE    = 47,
};

void reset_colour(void) {
  printf("\033[0m");
}

void set_colour(const int code) {
  printf("\033[%dm", code);
}

void bold(void) {
  printf("\033[1m");
}

void italic(void) {
  printf("\033[3m");
}
