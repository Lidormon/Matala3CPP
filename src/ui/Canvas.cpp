#include<cstdio>
#include<string>
#include "ui/Canvas.hpp"

using namespace ariel::ui;

Canvas::Canvas() : buffer{SEA_PIXEL}, bg(DEFAULT_BG), fg(DEFAULT_FG),
bold(false), r(0), c(0) {}

void Canvas::clear() {
   for (size_t i = 0; i < MAP_ROWS; i++) {
      for (size_t j = 0; j < MAP_COLS; j++) {
         buffer[i][j] = SEA_PIXEL;
      }
   }
   setCursor(0, 0);
}

void Canvas::setForeground(color_t color) {
   fg = color;
}

void Canvas::setBackground(color_t color) {
   bg = color;
}

void Canvas::setColor(color_t background, color_t foreground) {
   bg = background;
   fg = foreground;
}

void Canvas::setCursor(size_t row, size_t col) {
   r = row;
   c = col;
   if (r >= MAP_ROWS) {
      r = MAP_ROWS - 1;
      c = MAP_COLS - 1;
   } else if (c >= MAP_COLS) {
      c = MAP_COLS - 1;
   }
}

void Canvas::setBold(bool b) {
   bold = b;
}

void Canvas::moveCursor(size_t dr, size_t dc) {
   r += dr;
   c += dc;
   while (c > MAP_COLS) {
      c -= MAP_COLS;
      r++;
   }
   if (r >= MAP_ROWS) {
      r = MAP_ROWS - 1;
      c = MAP_COLS - 1;
   }
}

void Canvas::moveCursor() {
   c++;
   if (c == MAP_COLS) {
      c = 0;
      r++;
   }
   if (r == MAP_ROWS) {
      r = MAP_ROWS - 1;
      c = MAP_COLS - 1;
   }
}

void Canvas::putString(const char *str) {
   for (;*str; str++, c++) {
      buffer[r][c].bg = bg;
      buffer[r][c].fg = fg;
      buffer[r][c].bold = bold;
      buffer[r][c].chr = *str;
   }
}

void Canvas::putChar(char chr) {
   buffer[r][c].bg = bg;
   buffer[r][c].fg = fg;
   buffer[r][c].bold = bold;
   buffer[r][c].chr = chr;
   c++;
}

void Canvas::putInt(int i){
   std::string str = std::to_string(i);
   putString(str.c_str());
}

void Canvas::print() {
   for (size_t i = 0; i < MAP_ROWS; i++) {
      for (size_t j = 0; j < MAP_COLS; j++) {
         printf("%s\033[48;2;%d;%d;%dm\033[38;2;%d;%d;%dm%c\033[0m",
            buffer[i][j].bold ? "\033[1m" : "",
            buffer[i][j].bg.r, buffer[i][j].bg.g, buffer[i][j].bg.b,
            buffer[i][j].fg.r, buffer[i][j].fg.g, buffer[i][j].fg.b,
            buffer[i][j].chr
         );
      }
      // printf("\033[39;49m\n");
      printf("\n");
   }
}
