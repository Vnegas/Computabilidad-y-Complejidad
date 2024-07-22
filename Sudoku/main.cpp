#include "solver.hpp"

int main(int argc, char *argv[]) {
  size_t mode = 0;
  int K = 12; int level = 1;
  if (argc >= 2) {
    mode = atoi(argv[1]);
    level = atoi(argv[2]);
  }

  if (level == 2) {
    K = 25;
  } else if (level >= 3) {
    K = 60;
  }

  Sudoku sudoku(9);
  sudoku.fillValues();
  sudoku.removeKDigits(K);
  
  std::cout << "Tamaño de la Matriz:" << 9 << std::endl;
  std::cout << "Sudoku a resolver:" << std::endl;
  sudoku.print();
  
  Solver solver(sudoku);
  solver.solve(mode);

  return 0; 
}
