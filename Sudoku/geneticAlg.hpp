#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "sudoku.hpp"

const int SIZE = 9; // Size of the Sudoku grid (9x9)

struct Cromosoma {
    std::vector<size_t> solution;
    size_t ranking;

    Cromosoma() : ranking(1000) {
    } // Default constructor
    // Constructor to initialize an individual with a solution and ranking
    Cromosoma(const std::vector<size_t>& sol, size_t rank) : solution(sol), ranking(1000) {}
};

class GeneticAlgorithm {
public:
    std::vector<Cromosoma> population; // Vector de vectores de soluciones
    size_t populationSize;

    std::vector<std::pair<size_t, size_t>> vacios; // Espacios vacios del Sudoku
    Sudoku sudoku;

    GeneticAlgorithm(Sudoku &matriz, size_t tamano = 9);

    void mezcla();
    void mutate();
    std::vector<std::pair<size_t, size_t>> generarParejas();
    size_t rank();
    void truncar();
    bool tryToFillSudokuBoard(std::vector<size_t>& solution) ;
};

#endif // GENETIC_ALGORITHM_HPP
