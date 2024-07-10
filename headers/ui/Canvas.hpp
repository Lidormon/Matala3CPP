#ifndef __UI_CANVAS_HPP__
#define __UI_CANVAS_HPP__

#include<cstddef>
#include "ui/Pixel.hpp"

#define MAP_ROWS 21
#define MAP_COLS 40

namespace ariel::ui {

/**Represents canvas*/
class Canvas {
   pixel_t buffer[MAP_ROWS][MAP_COLS];
   color_t bg, fg;
   bool bold;
   size_t r, c;

public:
   /**Initialize a new canvas.*/
   Canvas();

   /**
    * Clear the canvas with the sea colors and pixels,
    * unset bold for all pixels and reset the cursor position to (0,0).
    */
   void clear();

   /**
    * Set the foreground color of the "Pencil", all future pixels will be
    * drawn with this color(until changed).
    * @param color An rgb color to change to.
    */
   void setForeground(color_t color);

   /**
    * Set the background color of the "Pencil", all future pixels will be
    * drawn with this color(until changed).
    * @param color An rgb color to change to.
    */
   void setBackground(color_t color);

   /**
    * A shortcut to use both `setForeground` and `setBackground` functions.
    * Set the foreground and background color of the "Pencil", all future
    * pixels will be drawn with this color(until changed).
    * @param background An rgb color to change the background to.
    * @param foreground An rgb color to change the foreground to.
    */
   void setColor(color_t background, color_t foreground);

   /**
    * Set the cursor position for next writes.
    * @param row The row to set the cursor to.
    * @param col The column in the row to set the cursor to.
    */
   void setCursor(size_t row, size_t col);

   /**
    * Set or unset bold from the "Pencil".
    * @param bold Whether the next characters should be written in bold or no.
    */
   void setBold(bool bold = true);

   /**
    * Move the cursor in relation to the current cursor position.
    * If the cursor moves past the last character in a row, it moves to the
    * first character of the next row.
    * If the cursor moves past the last row, it's instead moved to the last
    * column of the last row.
    * @param dr The amount rows to move down(negative for up).
    * @param dc The amount columns to move down(negative for up).
    */
   void moveCursor(size_t dr, size_t dc);

   /**
    * Move the cursor one character forward. On the last character in the row,
    * moves to the first character of the next row.
    * When moving past the last row, the curser stays on the last character
    * of the last row instead.
   */
   void moveCursor();

   /**Put a null terminated string at the current cursor postion.*/
   void putString(const char *str);

   /**Put a character at the current cursor position.*/
   void putChar(char chr);

   /**Put an integer at the current cursor position.*/
   void putInt(int i);

   /**Print the contents of the canvas to the the terminal.*/
   void print();
};

};

#endif