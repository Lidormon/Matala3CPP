#ifndef __UI_DIE_HPP__
#define __UI_DIE_HPP__

#include<cstddef>

#define DICE_N 6
#define DIE_LENGTH 10
#define DIE_LINES 5

namespace ariel::ui {

constexpr char DICE[DICE_N][DIE_LINES][DIE_LENGTH] = {{
   R"(/-------\)",
   R"(|       |)",
   R"(|   *   |)",
   R"(|       |)",
   R"(\-------/)"
}, {
   R"(/-------\)",
   R"(| *     |)",
   R"(|       |)",
   R"(|     * |)",
   R"(\-------/)"
}, {
   R"(/-------\)",
   R"(| *     |)",
   R"(|   *   |)",
   R"(|     * |)",
   R"(\-------/)"
}, {
   R"(/-------\)",
   R"(| *   * |)",
   R"(|       |)",
   R"(| *   * |)",
   R"(\-------/)"
}, {
   R"(/-------\)",
   R"(| *   * |)",
   R"(|   *   |)",
   R"(| *   * |)",
   R"(\-------/)"
}, {
   R"(/-------\)",
   R"(| *   * |)",
   R"(| *   * |)",
   R"(| *   * |)",
   R"(\-------/)"
}};

/**
 * Print a die with the specified number.
 * @param num A number between 1-6.
*/
void printDie(int num);

/**
 * Print n dice stacked horizontally (in the same "line").
 * @param num A pointer to an array of numbers between 1-6.
 * @param n The amount of numbers to read from `num` as dice numbers.
 */
void printDice(int *num, size_t n);

};

#endif