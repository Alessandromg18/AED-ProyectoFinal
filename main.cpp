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
    std::cout << " MINI DATABASE SYSTEM\n";
    std::cout << "==============================\n";
    std::cout << "1. Load dataset\n";
    std::cout << "2. Start empty database\n";
    std::cout << "Option: ";

    std::cin >> startOption;

    std::string datasetPath;

    // =========================
    // DATASET SELECTION
    // =========================

    if(startOption == 1)
    {
        int datasetOption;

        std::cout << "\nChoose dataset:\n";
        std::cout << "1. 1K\n";
        std::cout << "2. 10K\n";
        std::cout << "3. 100K\n";
        std::cout << "4. 1M\n";
        std::cout << "Option: ";

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
                std::cout << "Invalid dataset option.\n";
                return 1;
        }

        try
        {
            std::cout << "\nLoading dataset...\n";

            db.loadCSV(datasetPath);

            std::cout
                << "Loaded records: "
                << db.totalRecords()
                << "\n";
        }
        catch(const std::exception& e)
        {
            std::cout
                << "Error loading CSV: "
                << e.what()
                << "\n";

            return 1;
        }
    }

    else if(startOption == 2)
    {
        std::cout
            << "\nStarting with demo database...\n";

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

        std::cout
            << "Loaded demo records: "
            << db.totalRecords()
            << "\n";
    }

    else
    {
        std::cout << "Invalid option.\n";
        return 1;
    }

    // =========================
    // EXECUTION MODE
    // =========================

    int mode;

    std::cout << "\nChoose mode:\n";
    std::cout << "1. Visualization\n";
    std::cout << "2. Benchmark\n";
    std::cout << "Option: ";

    std::cin >> mode;

    // =========================
    // VISUALIZATION
    // =========================

    if(mode == 1)
    {
        RBVisualizer viz;

        viz.setDatabase(db);

        viz.show(
            db.getScoreIndex()
        );
    }

    // =========================
    // BENCHMARK
    // =========================

    else if(mode == 2)
    {
        if(datasetPath.empty())
        {
            std::cout
                << "Benchmark requires a dataset.\n";

            return 1;
        }

        Benchmark::run(datasetPath);
    }
    else
    {
        std::cout << "Invalid mode.\n";
        return 1;
    }

    return 0;
}