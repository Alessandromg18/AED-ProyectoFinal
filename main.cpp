#include <iostream>
#include <limits>
#include <string>

#include "benchmark/Benchmark.h"
#include "database/Database.h"
#include "visualization/RBVisualizer.h"

static bool readOption(int minOption, int maxOption, int& option) {
    if(!(std::cin >> option)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    if(option < minOption || option > maxOption) {
        return false;
    }

    return true;
}

int main() {

    Database db;

    int startOption = 0;

    std::cout << "==============================\n";
    std::cout << " SISTEMA DE BASE DE DATOS MINI\n";
    std::cout << "==============================\n";
    std::cout << "Escriba una opcion:\n";
    std::cout << "1. Cargar dataset\n";
    std::cout << "2. Empezar con un dataset vacio\n";
    std::cout << "Opcion: ";

    if(!readOption(1, 2, startOption)) {
        std::cout << "Opcion invalida.\n";
        return 1;
    }

    std::string datasetPath;

    // =========================
    // DATASET SELECTION
    // =========================

    if(startOption == 1) {
        int datasetOption = 0;

        std::cout << "\nElige tu dataset:\n";
        std::cout << "1. 1K datos\n";
        std::cout << "2. 10K datos\n";
        std::cout << "3. 100K datos\n";
        std::cout << "4. 1M datos\n";
        std::cout << "Opcion: ";

        if(!readOption(1, 4, datasetOption)) {
            std::cout << "Opcion invalida.\n";
                return 1;
        }

        switch(datasetOption) {
            case 1:
                datasetPath = "../data/dataset_1000.csv";
                break;

            case 2:
                datasetPath = "../data/dataset_10000.csv";
                break;

            case 3:
                datasetPath = "../data/dataset_100000.csv";
                break;

            case 4:
                datasetPath = "../data/dataset_1000000.csv";
                break;
        }

        try {
            std::cout << "\nCargando dataset...\n";

            db.loadCSV(datasetPath);

            std::cout
                << "Registros cargados: "
                << db.totalRecords()
                << "\n";
        }
        catch(const std::exception& e) {
            std::cout
                << "Error cargando CSV: "
                << e.what()
                << "\n";

            return 1;
        }
    } 
    else if(startOption == 2) {
        std::cout << "\nCreando un dataset nuevo...\n";

        db.insertRecord({
            1, "Alice", 20, 85.5, "A"
        });

        db.insertRecord({
            2, "Bob", 21, 72.0, "B"
        });

        db.insertRecord({
            3, "Carol", 19, 91.0, "A"
        });

        db.insertRecord({
            4, "David", 22, 65.5, "C"
        });

        db.insertRecord({
            5, "Eva", 20, 88.0, "B"
        });

        db.insertRecord({
            6, "Frank", 23, 78.5, "A"
        });

        db.insertRecord({
            7, "Grace", 21, 95.0, "C"
        });

        db.insertRecord({
            8, "Henry", 24, 60.0, "B"
        });

        db.insertRecord({
            9, "Ivy", 20, 85.5, "A"
        });

        db.insertRecord({
            10, "Jack", 22, 72.0, "C"
        });

        std::cout << "Registros cargados: " << db.totalRecords() << "\n";
    }
    else {
        std::cout << "Opcion invalida.\n";
        return 1;
    }

    // =========================
    // EXECUTION MODE
    // =========================

    int mode = 0;

    std::cout << "\nEsocger modo:\n";
    std::cout << "1. Visualizacion\n";
    std::cout << "2. Benchmark\n";
    std::cout << "Opcion: ";

    if(!readOption(1, 2, mode)) {
        std::cout << "Opcion invalida.\n";
        return 1;
    }

    // =========================
    // VISUALIZATION
    // =========================

    if(mode == 1) {
        RBVisualizer viz;

        viz.setDatabase(db);

        viz.show(db.getScoreIndex());
    }
    // =========================
    // BENCHMARK
    // =========================
    else if(mode == 2) {
        if(datasetPath.empty()) {
            std::cout
                << "Benchmark requiere un dataset.\n";

            return 1;
        }
        Benchmark::run(datasetPath);
    }
    else {
        std::cout << "Opcion invalida.\n";
        return 1;
    }

    return 0;
}