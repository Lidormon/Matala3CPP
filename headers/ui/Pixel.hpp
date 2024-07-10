#ifndef __UI_PIXEL_HPP__
#define __UI_PIXEL_HPP__

namespace ariel::ui {

struct color_t {
   unsigned char r,g,b;
};

/**Represents a TUI "pixel" element represented by a character.*/
struct pixel_t {
   /**Background and Foreground colors.*/
   color_t bg, fg;
   /**The character to draw.*/
   char chr;
   /**Whether to draw the character as bold.*/
   bool bold;
};

/**The default background color.*/
constexpr color_t DEFAULT_BG = {0,0,0};
/**The default foreground color.*/
constexpr color_t DEFAULT_FG = {248,248,248};
/**The pixel that represents the sea.*/
constexpr pixel_t SEA_PIXEL = {
   {248,248,248}, {0,0,248}, '~', false
};

};

#endif