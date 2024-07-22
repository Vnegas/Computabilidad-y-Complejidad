#ifndef SOLVER_H
#define SOLVER_H

#include <algorithm>
#include <array>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <numeric>
#include <time.h>
#include <vector>

#include "geneticAlg.hpp"
#include "sudoku.hpp"

class Solver {
 public:
  Sudoku sudoku;

  Solver(const Sudoku &sudoku);
  ~Solver();
  void solve(size_t id = 0);
  void bruteForce();
  bool solveBruteForce(Sudoku &sudoku, size_t row, size_t col);
  void heuristic();
  bool isSolved();
  bool eliminateCandidates(std::vector<std::vector<std::unordered_set<int>>>& candidates, size_t row, size_t col, int num);
  bool reduceCandidates(std::vector<std::vector<std::unordered_set<int>>>& candidates);
  bool solveHeuristic(Sudoku &sudoku);
};

#endif // SOLVER_H
