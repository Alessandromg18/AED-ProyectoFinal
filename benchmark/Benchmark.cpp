//
// Created by facum on 9/06/2026.
//

#include "Benchmark.h"

#include "../database/Database.h"

#include <chrono>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace chrono;

void Benchmark::run(
    const string& csvFile
)
{

    cout << "BENCHMARK: TIEMPOS DE CONSTRUCCION Y CONSULTAS\n";

    // Construcción de la Estructura.

    auto startStruct = high_resolution_clock::now();
    Database db;

    // Aqui se lee el CSV y construye internamente el Red-Black Tree y Hash.

    db.loadCSV(csvFile);
    auto endStruct = high_resolution_clock::now();
    auto totalStructureTimeMs = duration_cast<milliseconds>(endStruct - startStruct).count();

    // "Construcción" Lineal

    auto startLinearLoad = high_resolution_clock::now();

    // Simulamos la carga lineal es decir abrimos el archivo y guardamos en un vector plano.

    std::vector<Record> rawLinearRecords;
    std::ifstream file(csvFile);
    std::string line;

    if (file.is_open()) {
        std::getline(file, line);

        while (std::getline(file, line)) {

            std::istringstream ss(line);
            std::string idStr, name, ageStr, scoreStr, category;

            if (std::getline(ss, idStr, ',') && std::getline(ss, name, ',') &&
                std::getline(ss, ageStr, ',') && std::getline(ss, scoreStr, ',') &&
                std::getline(ss, category, ',')) {

                if(idStr.empty() || ageStr.empty() || scoreStr.empty()) { continue; }

                try
                {
                    rawLinearRecords.push_back({
                        std::stoi(idStr),
                        name,
                        std::stoi(ageStr),
                        std::stod(scoreStr),
                        category
                    });
                }

                catch(const std::exception&)
                {
                    continue;
                }
            }
        }

        file.close();
    }

    auto endLinearLoad = high_resolution_clock::now();
    auto totalLinearLoadTimeMs = duration_cast<milliseconds>(endLinearLoad - startLinearLoad).count();

    // Los resultados de la fase de construccion

    auto records = db.getAllRecords();
    cout << "Registros Totales Detectados: " << records.size() << "\n\n";

    cout << "FASE DE CONSTRUCCION\n";
    cout << "Tiempo de Carga Lineal (Solo Vector en bruto):  " << totalLinearLoadTimeMs << " ms\n";
    cout << "Tiempo de Construccion Estructura (Arbol): " << totalStructureTimeMs << " ms\n";


    int ITERATIONS = (int)records.size();

    cout << "Iniciando pruebas de ID para " << ITERATIONS << " iteraciones completas...\n";

    // Búsqueda optimizada por ID (Hash)

    auto start = high_resolution_clock::now();

    for(int i = 0; i < ITERATIONS; i++)
    {
        db.findById(records[i].id);
    }
    auto end = high_resolution_clock::now();

    cout << "Busqueda por ID: " << duration_cast<microseconds>(end - start).count() << " us\n";

    // Búsqueda Lineal Normal por ID

    long long linearVisited = 0;
    start = high_resolution_clock::now();

    for(int i = 0; i < ITERATIONS; i++)
    {
        int targetId = records[i].id;
        for(const auto& r : records)
        {
            linearVisited++;
            if(r.id == targetId)
                break;
        }
    }

    end = high_resolution_clock::now();

    cout << "Busqueda por ID (Lineal): " << duration_cast<microseconds>(end - start).count() << " us\n";
    cout << "Registros revisados en Lineal: " << linearVisited << "\n\n";


    // Consulta por score para RBT

    double score = records[records.size() / 2].score;

    db.getScoreIndex().resetVisited();

    start = high_resolution_clock::now();
    db.getScoreIndex().search(score);
    end = high_resolution_clock::now();

    cout << "Consulta por score (Red-Black Tree): " << duration_cast<microseconds>(end - start).count() << " us\n";
    cout << "Nodos de la estructura visitados: "<< db.getScoreIndex().getVisited() << "\n";

    // Consulta por score para lineal.

    linearVisited = 0;
    start = high_resolution_clock::now();

    for(const auto& r : records)
    {
        linearVisited++;
        if(r.score == score)
        {}
    }
    end = high_resolution_clock::now();

    cout << "Consulta por score (Lineal): " << duration_cast<microseconds>(end - start).count() << " us\n";
    cout << "Registros revisados en Lineal: " << linearVisited << "\n\n";

    db.getScoreIndex().resetVisited();
    auto [minIt, maxIt] = minmax_element(
    records.begin(),
    records.end(),
    [](const auto& a, const auto& b)
    {
        return a.score < b.score;
    }
    );

    // Busqueda por rango para RBT

    double minScore = minIt->score;

    double maxScore = maxIt->score;

    double low = minScore + (maxScore - minScore) * 0.25;

    double high = minScore + (maxScore - minScore) * 0.75;

    start = high_resolution_clock::now();

    auto result = db.findBetween(low, high);

    end = high_resolution_clock::now();

    cout << "Consulta por rango (RBT): " << duration_cast<microseconds>(end - start).count() << " us\n";

    std::cout << "Nodos visitados: " << db.getScoreIndex().getVisited() << "\n";

    // Busqueda por rango en lineal

    linearVisited = 0;

    start = high_resolution_clock::now();

    vector<Record> linearResult;

    for(const auto& r : records)
    {
        linearVisited++;

        if(r.score >= low && r.score <= high)
        {
            linearResult.push_back(r);
        }
    }

    end = high_resolution_clock::now();

    cout << "Consulta por rango (Lineal): " << duration_cast<microseconds>(end - start).count() << " us\n";

    cout << "Registros revisados: " << linearVisited << "\n\n";

    db.getScoreIndex().resetVisited();

    // Consulta de la mediana para RBT

    start = high_resolution_clock::now();

    double median = db.medianScore();

    end = high_resolution_clock::now();

    cout << "Consulta por Mediana (RBT): " << median << " | " << duration_cast<microseconds>(end - start).count() << " us\n";

    vector<double> scores;

    cout << "Nodos visitados: " << db.getScoreIndex().getVisited() << "\n";


    scores.reserve(records.size());

    // Consulta de la mediana para Lineal

    start = high_resolution_clock::now();

    for(const auto& r : records)
    {
        scores.push_back(
            r.score
        );
    }

    sort(scores.begin(),scores.end());

    double medianLinear;

    int n = scores.size();

    if(n % 2 == 1)
    {
        medianLinear = scores[n/2];
    }

    else
    {
        medianLinear = (scores[n/2] + scores[n/2 - 1]) / 2.0;
    }

    end = high_resolution_clock::now();

    cout << "Consulta por mediana (Lineal): " << medianLinear << " | " << duration_cast<microseconds>(end - start).count() << " ms\n";

    db.getScoreIndex().resetVisited();

    // Consulta para percentil usando RBT

    start = high_resolution_clock::now();

    double p35 = db.percentileScore(35);

    end = high_resolution_clock::now();

    cout << "Consulta por Percentil 35 (RBT): " << p35 << " | " << duration_cast<microseconds>(end - start).count() << " us\n";

    cout << "Nodos visitados: " << db.getScoreIndex().getVisited() << "\n";

    // Consulta para percentil usando Lineal

    vector<double> temp = scores;

    start = high_resolution_clock::now();

    sort(temp.begin(), temp.end());

    int pos = (35 * (temp.size() - 1)) / 100;

    double p35Linear = temp[pos];

    end = high_resolution_clock::now();

    cout << "Consulta por Percentil 35 (Lineal): " << p35Linear << " | " << duration_cast<microseconds>(end - start).count() << " us\n\n";

    // Consulta para cantidad de elementos en un rango determinado en RBT

    db.getScoreIndex().resetVisited();

    start = high_resolution_clock::now();

    int countIndex = db.getScoreIndex().countRange(low,high);

    end = high_resolution_clock::now();

    cout << "Consulta para cantidad en un rango (RBT): " << countIndex << " | " << duration_cast<microseconds>(end - start).count() << " us\n";

    cout << "Nodos visitados: " << db.getScoreIndex().getVisited() << "\n";

    // Consulta para cantidad de elementos en un rango determinado en Lineal

    int countLinear = 0;

    start = high_resolution_clock::now();

    for(const auto& r : records)
    {
        if(r.score >= low && r.score <= high){
            countLinear++;
        }
    }

    end = high_resolution_clock::now();

    cout << "Consulta para cantidad en un rango  (Lineal): " << countLinear << " | " << duration_cast<microseconds>(end - start).count() << " us\n\n";

}
