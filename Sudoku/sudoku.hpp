#ifndef SUDOKU_H
#define SUDOKU_H

#include <array>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <time.h>
#include <unordered_set>
#include <vector>

class Sudoku {
 public:
  // El tamano debe ser divisible entre 3. Para las cajas
  size_t size;
  size_t ** matrix;

  Sudoku(size_t tamano = 9);
  Sudoku(const Sudoku& other);  // Copy constructor
  ~Sudoku();
  bool esValido(size_t row, size_t col, size_t num);
  bool esValidoFuerzaBruta(size_t row, size_t col, size_t num);
  void print();
  void cleanMatrix();
  void fillValues();
  bool fillRemaining(size_t row, size_t col);
  void removeKDigits(int K);
  size_t randomGenerator(size_t num);
  void randomAns();
  size_t contarDuplicados();
};

#endif // SUDOKU_H
