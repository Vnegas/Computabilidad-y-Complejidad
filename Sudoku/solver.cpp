#include "solver.hpp"

Solver::Solver(const Sudoku &sudoku) : sudoku(sudoku) {
}

void Solver::solve(size_t id) {
  if (id == 0) {
    std::cout << "Algoritmo de Fuerza Bruta:" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    this->bruteForce();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Duro: " << duration.count() << " microsegundos" << std::endl;
  } else if (id == 1) {
    std::cout << "Algoritmo de Heuristica:" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    this->heuristic();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Duro: " << duration.count() << " microsegundos" << std::endl;
  } else if (id == 2) {
    std::cout << "Algoritmo Genetico:" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    GeneticAlgorithm algoritmoGenetico(sudoku, sudoku.size);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Duro: " << duration.count() << " microsegundos" << std::endl;
  }
}


Solver::~Solver() {
}

void Solver::bruteForce() {
  solveBruteForce(this->sudoku, 0, 0);
  this->sudoku.print();
}

bool Solver::solveBruteForce(Sudoku &sudoku, size_t row, size_t col) {
  if (row == sudoku.size - 1 && col == sudoku.size) {
    return true; // Tablero completado
  }
  
  if (col == sudoku.size) {
    row++;
    col = 0;
  }

  if (sudoku.matrix[row][col] != 0) {
    return solveBruteForce(sudoku, row, col + 1); // Casilla ya llena, pasar a la siguiente
  }

  for (size_t num = 1; num <= sudoku.size; num++) {
    if (sudoku.esValidoFuerzaBruta(row, col, num)) {
      sudoku.matrix[row][col] = num;
      if (solveBruteForce(sudoku, row, col + 1)) {
        return true; // Intentar rellenar la siguiente casilla
      }
      sudoku.matrix[row][col] = 0; // Si no se puede, restaurar y probar otro número
    }
  }
  return false; // No se puede resolver con estos números, retroceder
}

/* HEURISTIC SOLVER */

void Solver::heuristic() {
  bool finished = solveHeuristic(this->sudoku);
  this->sudoku.print();
  if (!finished) {
    std::cout << "No possible solution\n";
  }
}

bool Solver::isSolved() {
  for (size_t i = 0; i < this->sudoku.size; ++i) {
    for (size_t j = 0; j < this->sudoku.size; ++j) {
      if (this->sudoku.matrix[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

bool Solver::eliminateCandidates(std::vector<std::vector<std::unordered_set<int>>>& candidates
    , size_t row, size_t col, int num) {
  // Eliminate 'num' from candidates in the same row, column, and subgrid
  for (size_t i = 0; i < this->sudoku.size; ++i) {
    if (candidates[row][i].erase(num) && candidates[row][i].size() == 1) {
      int value = *(candidates[row][i].begin());
      if (!eliminateCandidates(candidates, row, i, value))
        return false;
    }

    if (candidates[i][col].erase(num) && candidates[i][col].size() == 1) {
      int value = *(candidates[i][col].begin());
      if (!eliminateCandidates(candidates, i, col, value))
        return false;
    }
  }

  size_t sub_size = this->sudoku.size / 3;
  size_t startRow = sub_size * (row / sub_size);
  size_t startCol = sub_size * (col / sub_size);

  for (size_t i = startRow; i < startRow + sub_size; ++i) {
    for (size_t j = startCol; j < startCol + sub_size; ++j) {
      if (candidates[i][j].erase(num) && candidates[i][j].size() == 1) {
        int value = *(candidates[i][j].begin());
        if (!eliminateCandidates(candidates, i, j, value))
          return false;
      }
    }
  }
  return true;
}

bool Solver::reduceCandidates(std::vector<std::vector<std::unordered_set<int>>>& candidates) {
  bool isReduced = false;
  for (size_t i = 0; i < this->sudoku.size; ++i) {
    for (size_t j = 0; j < this->sudoku.size; ++j) {
      if (candidates[i][j].size() == 1) {
        int value = *(candidates[i][j].begin());
        if (!eliminateCandidates(candidates, i, j, value)) {
          return false;
        }
        isReduced = true;
      }
    }
  }
  return isReduced;
}

bool Solver::solveHeuristic(Sudoku &sudoku) {
  int size = this->sudoku.size;
  std::vector<std::vector<std::unordered_set<int>>> candidates(size, std::vector<std::unordered_set<int>>(size));
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      for (int k = 1; k <= size; ++k) {
        if (this->sudoku.matrix[i][j] != 0) {
          candidates[i][j].insert(k);
        } else {
          candidates[i][j].insert(this->sudoku.matrix[i][j]); // Insert the given value as the only candidate
        }
      }
    }
  }

  if (isSolved()) {
    return true; // Puzzle already solved using candidate reduction
  }

  // Backtracking to fill in the remaining empty cells
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (this->sudoku.matrix[i][j] == 0) {
        for (size_t num = 1; num <= this->sudoku.size; ++num) {
          if (this->sudoku.esValido(i, j, num)) {
            this->sudoku.matrix[i][j] = num;
            if (solveHeuristic(this->sudoku)) {
              return true;
            }
            this->sudoku.matrix[i][j] = 0;
          }
        }
        return false;
      }
    }
  }
  return true; // No solution found
}
