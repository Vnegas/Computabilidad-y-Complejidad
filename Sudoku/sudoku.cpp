#include "sudoku.hpp"

Sudoku::Sudoku(size_t tamano) {
  // Dynamically allocate memory for the matrix
  srand (time(NULL));
  if (tamano % 3 != 0) {
    std::cerr << "Error: Sudoku size must be divisible by 3." << std::endl;
    exit(EXIT_FAILURE);
  }

  this->size = tamano;
  this->matrix = new size_t*[this->size];

  for (size_t i = 0; i < this->size; ++i) {
    this->matrix[i] = new size_t[this->size];
  }

  for (size_t i = 0; i < this->size; i++){
    for (size_t j = 0; j < this->size; j++){
      this->matrix[i][j] = 0;
    }
  }
}

// Copy constructor
Sudoku::Sudoku(const Sudoku& other) {
  this->size = other.size;

  // Allocate memory for the matrix
  this->matrix = new size_t*[this->size];
  for (size_t i = 0; i < this->size; ++i) {
    this->matrix[i] = new size_t[this->size];
  }

  // Copy values from the other matrix
  for (size_t i = 0; i < this->size; ++i) {
    for (size_t j = 0; j < this->size; ++j) {
      this->matrix[i][j] = other.matrix[i][j];
    }
  }
}

Sudoku::~Sudoku() {
  // Deallocate memory for the matrix
  // for (size_t i = 0; i < this->size; ++i) {
  //   delete[] this->matrix[i];
  // }
  // delete[] this->matrix;
}

bool Sudoku::esValidoFuerzaBruta(size_t row, size_t col, size_t num) {
  for (size_t i = 0; i < this->size; ++i) {
    if (this->matrix[row][i] == num || this->matrix[i][col] == num) {
      return false;
    }
  }

  size_t sub_size = sqrt(size);
  size_t startRow = sub_size * (row / sub_size);
  size_t startCol = sub_size * (col / sub_size);

  for (size_t i = startRow; i < startRow + sub_size; ++i) {
    for (size_t j = startCol; j < startCol + sub_size; ++j) {
      if (this->matrix[i][j] == num) {
        return false;
      }
    }
  }
  return true;
}

bool Sudoku::esValido(size_t row, size_t col, size_t num) {
  for (size_t i = 0; i < this->size; ++i) {
    if (this->matrix[row][i] == num && i != col) {
      return false;
    }
  }
  for (size_t i = 0; i < this->size; ++i) {
    if (this->matrix[i][col] == num && i != row) {
      return false;
    }
  }
  size_t sub_size = size/3;
  size_t startRow = sub_size * (row / sub_size);
  size_t startCol = sub_size * (col / sub_size);

  for (size_t i = startRow; i < startRow + sub_size; ++i) {
    for (size_t j = startCol; j < startCol + sub_size; ++j) {
      if (this->matrix[i][j] == num && i != row && j != col) {
        return false;
      }
    }
  }
  return true;
}

void Sudoku::print() {
  size_t width = 0;
  size_t number = this->size;
  // Count the digits using a loop
  while (number > 0) {
    number /= 10;
    width++;
  }
  std::cout << std::endl;
  size_t len = 0;
  for (size_t i = 0; i < this->size; i++) {
    for (size_t j = 0; j < this->size; j++) {
      if (j + 1 == this->size / 3 || j + 1 == (this->size / 3) * 2) {
        std::cout << std::setw(width) << " " << this->matrix[i][j] << "  | ";
        len += width + 4;
      } else {
        if (j == this->size - 1) {
          std::cout << std::setw(width) << " " << this->matrix[i][j];
          len += 3;
        } else {
          std::cout << std::setw(width) << " " << this->matrix[i][j] << " "; // << " | ";
          len += width + 2;
        }
      }
    }
    std::cout << std::endl;
    if (i != this->size-1) {
      for (size_t j = 0; j < len; j++) {
        if ((i + 1) % 3 == 0) {
          std::cout << "-";
        } else {
          //std::cout << "-";
        }
      }
      if ((i + 1) % 3 == 0) {
        std::cout << "-" << std::endl;
      } else {
        //std::cout << "-" << std::endl;
      }
    }
    len = 0;
  }
  std::cout << std::endl;
}

void Sudoku::cleanMatrix() {
  for (size_t i = 0; i < this->size; ++i) {
    for (size_t j = 0; j < this->size; ++j) {
      this->matrix[i][j] = 0;
    }
  }
}

// Sudoku Generator
void Sudoku::fillValues() {
  // Fill blocks
  fillRemaining(0,0);
}

// Random generator
size_t Sudoku::randomGenerator(size_t num) {
  return static_cast<size_t>(floor(static_cast<float>(rand()) / double(RAND_MAX) * num + 1));
}

// A recursive function to fill the remaining matrix
bool Sudoku::fillRemaining(size_t row, size_t col) {
  if (row == size - 1 && col == size) {
    return true; // Tablero completado
  }

  if (col == size) {
    row++;
    col = 0;
  }

  if (this->matrix[row][col] != 0) {
    return fillRemaining(row, col + 1); // Casilla ya llena, pasar a la siguiente
  }

  size_t *randomNums = new size_t[size];
  for (size_t i = 0; i < size; ++i) {
    randomNums[i] = i + 1;
  }
  srand(time(0));
  for (size_t i = size - 1; i > 0; --i) {
    size_t j = rand() % (i + 1);
    std::swap(randomNums[i], randomNums[j]);
  }

  for (size_t k = 0; k < size; ++k) {
    size_t num = randomNums[k];
    if (esValido(row, col, num)) {
      this->matrix[row][col] = num;
      if (fillRemaining(row, col + 1)) {
        delete[] randomNums;
        return true; // Intentar rellenar la siguiente casilla
      }
      this->matrix[row][col] = 0; // Si no se puede, restaurar y probar otro número
    }
  }

  delete[] randomNums;
  return false; // No se puede resolver con estos números
}

// Remove the K number of digits to complete the game
void Sudoku::removeKDigits(int K) {
  size_t count = K;
  while (count != 0) {
    int i = randomGenerator(size) - 1;
    int j = randomGenerator(size) - 1;
    if (matrix[i][j] != 0) {
      count--;
      matrix[i][j] = 0;
    }
  }
}

void Sudoku::randomAns() {
  
  for (size_t i = 0; i < this->size; i++){
    for (size_t j = 0; j < this->size; j++){
      if (this->matrix[i][j] == 0)
      {
        this->matrix[i][j] = randomGenerator(this->size);
      }
    }
  }
}

size_t Sudoku::contarDuplicados() {
  size_t invalidCells = 0;
    // Check duplicates in rows and columns
    for (size_t i = 0; i < this->size; ++i) {
        for (size_t j = 0; j < this->size; ++j) {
            if (this->matrix[i][j] != 0 && !this->esValido(i, j, this->matrix[i][j])) {
                ++invalidCells;
            }
        }
    }

    return invalidCells;
    
}