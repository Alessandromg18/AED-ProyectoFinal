#include <iostream>
#include <string>

#include "benchmark/Benchmark.h"
#include "database/Database.h"
#include "visualization/RBVisualizer.h"

int main()
{
    Database db;

    int startOption;

    std::cout << "==============================\n";
    std::cout << " Sistema de Mini Base de Datos\n";
    std::cout << "==============================\n";
    std::cout << "1. Cargar dataset\n";
    std::cout << "2. Empezar con un dataset vacio\n";
    std::cout << "Opcion: ";

    std::cin >> startOption;

    std::string datasetPath;

    // Eleccion del dataset

    if(startOption == 1)
    {
        int datasetOption;

        std::cout << "\nElegi tu dataset:\n";
        std::cout << "1. 1K\n";
        std::cout << "2. 10K\n";
        std::cout << "3. 100K\n";
        std::cout << "4. 1M\n";
        std::cout << "Opcion: ";

        std::cin >> datasetOption;

        switch(datasetOption)
        {
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

            default:
                std::cout << "Opcion invalida.\n";
                return 1;
        }

        try
        {
            std::cout << "\nCargando el dataset...\n";

            db.loadCSV(datasetPath);

            std::cout << "Registros cargados: " << db.totalRecords() << "\n";
        }
        catch(const std::exception& e)
        {
            std::cout << "ERROR AL CARGAR EL CSV: " << e.what() << "\n";
            return 1;
        }
    }

    else if(startOption == 2)
    {
        std::cout
            << "\nComenzando con un pequeño dataset...\n";

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

        std::cout << "Registros cargados: "<< db.totalRecords()<< "\n";
    }

    else
    {
        std::cout << "Opcion invalida.\n";
        return 1;
    }

    // Eleccion del modo en que queremos usar nuestro dataset

    int mode;

    std::cout << "\nElegi un modo:\n";
    std::cout << "1. Visualizacion\n";
    std::cout << "2. Benchmark\n";
    std::cout << "Opcion: ";

    std::cin >> mode;


    // Visualizacion

    if(mode == 1)
    {
        RBVisualizer viz;

        viz.setDatabase(db);

        viz.show(
            db.getScoreIndex()
        );
    }

    // BENCHMARK

    else if(mode == 2)
    {
        if(datasetPath.empty())
        {
            std::cout
                << "Benchmark requiere un dataset cargado.\n";

            return 1;
        }

        Benchmark::run(datasetPath);
    }
    else
    {
        std::cout << "Modo invalido.\n";
        return 1;
    }

    return 0;
}