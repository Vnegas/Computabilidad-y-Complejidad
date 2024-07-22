#include "geneticAlg.hpp"

GeneticAlgorithm::GeneticAlgorithm(Sudoku &matriz, size_t tamano) : populationSize(tamano) {
    std::cout << "Initializing Genetic Algorithm\n";
    sudoku = matriz;
    size_t vaciosNum = 0 ;
    // Almacena todos los espacios vacios
    for (size_t i = 0; i < tamano; i++) {
        for (size_t j = 0; j < tamano; j++) {
            if (sudoku.matrix[i][j] == 0) {
                vacios.push_back(std::make_pair(i, j));
                vaciosNum++;
            }
        }
    }
    population.resize(100);
    // Initialize population
    // Inicializa la poblacion con vectores de posibles soluciones
    for (auto &cromosoma : population) {
        cromosoma.solution.clear(); // Clear the existing vector content
        Sudoku prueba = sudoku;
        for (size_t i = 0; i < vaciosNum; ++i) {
            //size_t randomNumber = prueba.randomGenerator(prueba.size);
            size_t randomNumber = prueba.randomGenerator(tamano);
            while (randomNumber> this->populationSize)
            {
               randomNumber = sudoku.randomGenerator(sudoku.size);
            }
            for (size_t h = 1; h <= tamano; h++){
                 if (prueba.esValido(vacios[i].first,vacios[i].second, h))
                 {
                     prueba.matrix[vacios[i].first][vacios[i].second] = h;
                     randomNumber = h;
                     break;
                 }
                
             }
            
            cromosoma.solution.push_back(randomNumber);
        }
    }
    bool encontrado = false;
    size_t solucionFinal = 300;
    size_t contador = 0;
    // Actualiza populationRanking con los rankings de cada cromosoma en population
    size_t indiceSolucion = rank();
    if (indiceSolucion != 300) {
        encontrado = true;
        solucionFinal = indiceSolucion;
    }
    while (encontrado == false && contador < 20000) {
        //std::cout << "Iteration: " << contador << "\n";
        // Combina padres y crea hijos
        mezcla();
        // Les da ranking a los que no tienen
        size_t a = rank();
        if (a != 300) {
            encontrado = true;
            solucionFinal = a;
        }
        // Elimina los peores cromosomas
        truncar();
        // Muta las soluciones
        mutate();
        ++contador;
    }
    if (contador > 19998){
        size_t lowestRank = 10000;  
        Sudoku mejor;
        
        for (size_t index = 0; index < population.size(); ++index) {
            Sudoku prueba = sudoku;

            for (size_t i = 0; i < population[index].solution.size(); ++i) {
                size_t row = vacios[i].first;
                size_t col = vacios[i].second;
                prueba.matrix[row][col] = population[index].solution[i];
            }

            size_t duplicados = prueba.contarDuplicados();
            population[index].ranking = duplicados;

            if (duplicados < lowestRank) {
                lowestRank = duplicados;
                mejor = prueba;
            }
        }
        std::cout << "\nEl mejor resultado encontrado tiene ranking: " << lowestRank << "\n";
        mejor.print();
    } else {
        std::cout << "\nIntentos: " << contador << "\n";
        if (solucionFinal != 300) {
            // Llena con matriz de solucion
            Sudoku prueba = sudoku;
            for (size_t i = 0; i < population[solucionFinal].solution.size(); ++i) {
                size_t row = vacios[i].first;
                size_t col = vacios[i].second;
                prueba.matrix[row][col] = population[solucionFinal].solution[i];
            }
            prueba.print();
        } else {
            std::cout << "\nNo se encontro solucion.\n";
        }
    }
    
    
}

void GeneticAlgorithm::mutate() {
    for (auto& cromosoma : population) {
        // Randomly decide whether to apply mutation
        if (rand() % 100 < 30) {
            // Randomly choose a position to mutate
            size_t mutatePos = rand() % cromosoma.solution.size();

            // Generate a new random number to replace the current value
            size_t newRandomNumber = sudoku.randomGenerator(sudoku.size);
            while (newRandomNumber> this->populationSize)
            {
               newRandomNumber = sudoku.randomGenerator(sudoku.size);
            }
            

            // Apply mutation
            cromosoma.solution[mutatePos] = newRandomNumber;
            //std::cout << "\nMuto\n";
        }
    }
}


void GeneticAlgorithm::mezcla() {
    std::vector<std::pair<size_t, size_t>> parejas = generarParejas();

    for (auto &pareja : parejas) {
        size_t padre1 = pareja.first;
        size_t padre2 = pareja.second;

        std::vector<size_t> &sol1 = population[padre1].solution;
        std::vector<size_t> &sol2 = population[padre2].solution;
        std::vector<size_t> hijoSol(sol1.size());

        // Randomly select parts from both parents
        for (size_t i = 0; i < sol1.size(); ++i) {
            if (std::rand() % 2 == 0) {
                hijoSol[i] = sol1[i];
            } else {
                hijoSol[i] = sol2[i];
            }
        }

        // Create a child with the new solution
        Cromosoma hijo(hijoSol, 1);

        // Add the child to the population
        population.push_back(hijo);
    }
}





std::vector<std::pair<size_t, size_t>> GeneticAlgorithm::generarParejas() {
        std::vector<std::pair<size_t, size_t>> uniquePairs;
        size_t vectorSize = 100;
        // Set to keep track of chosen indices
        std::unordered_set<size_t> chosenIndices;

        // Seed for random number generation
        std::srand(static_cast<unsigned>(std::time(0)));

        while (chosenIndices.size() < vectorSize) {
            // Generate two random indices
            size_t index1 = std::rand() % vectorSize;
            size_t index2 = std::rand() % vectorSize;

            // Ensure the indices are unique and not chosen before
            while (index1 == index2 || chosenIndices.count(index1) || chosenIndices.count(index2)) {
                index1 = std::rand() % vectorSize;
                index2 = std::rand() % vectorSize;
            }

            // Add the indices to the set of chosen indices
            chosenIndices.insert(index1);
            chosenIndices.insert(index2);

            // Add the pair to the vector of unique pairs
            uniquePairs.emplace_back(index1, index2);
        }

        return uniquePairs;
    }

size_t GeneticAlgorithm::rank() {
  size_t solucion = 300;
  size_t index = 0;
  size_t lowestRank = 10000;  // Initialize with the maximum possible value
  // Va por todos los cromosomas
  for (auto& cromosoma : population) {
    Sudoku prueba = sudoku;
    // Llena una matriz con una solución posible
    for (size_t i = 0; i < cromosoma.solution.size(); ++i) {
        size_t row = vacios[i].first;
        size_t col = vacios[i].second;
        prueba.matrix[row][col] = cromosoma.solution[i];
    }
    size_t duplicados = prueba.contarDuplicados();
    cromosoma.ranking = duplicados;

    if (duplicados == 0) {
        solucion = index;
        lowestRank = 0;
        break; // No need to continue once the first zero ranking is found
    }

    if (duplicados < lowestRank) {
        lowestRank = duplicados;
    }

    ++index;
  }

  //  std::cout << "Lowest Rank in Iteration: " << lowestRank << "\n";
  //  std::cout << "Lowest Rank Solution: ";
  //  for (size_t i = 0; i < population[lowestRankUser].solution.size(); i++)
  //  {
  //     std::cout << population[lowestRankUser].solution[i] << ", ";
  //  }
  //  std::cout <<"\n";
  return solucion;
}

    
void GeneticAlgorithm::truncar() {
  // Partially sort the population based on ranking
  std::partial_sort(population.begin(), population.begin() + 100, population.end(),
                  [](const Cromosoma& a, const Cromosoma& b) {
                      return a.ranking < b.ranking;
                  });

  // Erase the rest of the individuals
  population.erase(population.begin() + 100, population.end());
}
