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

    cout << "\n==================================================\n";
    cout << "BENCHMARK: TIEMPOS DE CONSTRUCCION Y CONSULTAS\n";
    cout << "==================================================\n\n";

    // ---------------------------------------------------------
    // 1. COMPARATIVA DE CONSTRUCCIÓN (RBT/HASH VS LINEAL)
    // ---------------------------------------------------------

    // A. Construcción de la Estructura Indexada (Carga + Árbol/Hash)
    auto startStruct = high_resolution_clock::now();
    Database db;
    db.loadCSV(csvFile); // Lee el CSV y construye internamente el Red-Black Tree y Hash
    auto endStruct = high_resolution_clock::now();
    auto totalStructureTimeMs = duration_cast<milliseconds>(endStruct - startStruct).count();

    // B. "Construcción" Lineal (Solo lectura cruda del CSV a memoria sin indexar)
    auto startLinearLoad = high_resolution_clock::now();

    // Simulamos la carga lineal: abrimos el archivo y guardamos en un vector plano
    std::vector<Record> rawLinearRecords;
    std::ifstream file(csvFile);
    std::string line;
    if (file.is_open()) {
        // Omitimos la cabecera si existe
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string idStr, name, ageStr, scoreStr, category;

            // Ajusta este parseo según el orden exacto de las columnas de tu CSV
            if (std::getline(ss, idStr, ',') &&
    std::getline(ss, name, ',') &&
    std::getline(ss, ageStr, ',') &&
    std::getline(ss, scoreStr, ',') &&
    std::getline(ss, category, ','))
            {
                if(
                    idStr.empty() ||
                    ageStr.empty() ||
                    scoreStr.empty()
                )
                {
                    continue;
                }

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

    // Mostramos los resultados de la fase de preparación
    auto records = db.getAllRecords();
    cout << "Registros Totales Detectados: " << records.size() << "\n\n";

    cout << "--- FASE DE PREPARACION / CONSTRUCCION ---\n";
    cout << "Tiempo de Carga Lineal (Solo Vector en bruto):  " << totalLinearLoadTimeMs << " ms\n";
    cout << "Tiempo de Construccion Estructura (Arbol/Hash): " << totalStructureTimeMs << " ms\n";
    cout << "Nota: El Arbol tarda mas inicialmente porque clasifica y ordena los datos.\n\n";

    // ---------------------------------------------------------
    // 2. BUSQUEDA POR ID: EJECUTAR LA TOTALIDAD DE LOS DATOS
    // ---------------------------------------------------------
    // Eliminamos el min(1000, ...) para procesar absolutamente TODOS los registros
    int ITERATIONS = (int)records.size();

    cout << "Iniciando pruebas de ID para " << ITERATIONS << " iteraciones completas...\n";

    // A. Búsqueda optimizada por ID (Hash / Estructura primaria)
    auto start = high_resolution_clock::now();
    for(int i = 0; i < ITERATIONS; i++)
    {
        db.findById(records[i].id);
    }
    auto end = high_resolution_clock::now();

    cout << "Busqueda por ID (Indexada): "
         << duration_cast<microseconds>(end - start).count() << " us\n";

    // B. Búsqueda Lineal Normal por ID (Para comparar eficiencias)
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

    cout << "Busqueda por ID (Lineal): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
    cout << "Registros revisados en Lineal: " << linearVisited << "\n\n";


    // ---------------------------------------------------------
    // 3. CONSULTA POR SCORE (EQUALITY QUERY: RBT VS LINEAL)
    // ---------------------------------------------------------
    double score = records[records.size() / 2].score;

    db.getScoreIndex().resetVisited();

    start = high_resolution_clock::now();
    db.getScoreIndex().search(score);
    end = high_resolution_clock::now();

    cout << "Equality Query (Red-Black Tree): "
         << duration_cast<microseconds>(end - start).count() << " us\n";
    cout << "Nodos de la estructura visitados: "
         << db.getScoreIndex().getVisited() << "\n";

    // Comparativa lineal para la misma consulta de Score
    linearVisited = 0;
    start = high_resolution_clock::now();

    for(const auto& r : records)
    {
        linearVisited++;
        if(r.score == score)
        {
            // Encontrado linealmente
        }
    }
    end = high_resolution_clock::now();

    cout << "Equality Query (Lineal): "
         << duration_cast<microseconds>(end - start).count() << " us\n";
    cout << "Registros revisados en Lineal: " << linearVisited << "\n\n";

    db.getScoreIndex().resetVisited();
    cout << "==============================\n";

    auto [minIt, maxIt] = minmax_element(
    records.begin(),
    records.end(),
    [](const auto& a, const auto& b)
    {
        return a.score < b.score;
    }
);

    double minScore = minIt->score;
    double maxScore = maxIt->score;

    double low =
        minScore +
        (maxScore - minScore) * 0.25;

    double high =
        minScore +
        (maxScore - minScore) * 0.75;

    start =
        high_resolution_clock::now();

    auto result = db.findBetween(low, high);

    end =
        high_resolution_clock::now();

    cout
        << "Range Query (RBT): "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    std::cout
        << "Nodos visitados: "
        << db.getScoreIndex()
               .getVisited()
        << "\n";
    linearVisited = 0;

    start =
        high_resolution_clock::now();

    vector<Record> linearResult;

    for(const auto& r : records)
    {
        linearVisited++;

        if(r.score >= low && r.score <= high)
        {
            linearResult.push_back(r);
        }
    }

    end =
        high_resolution_clock::now();

    cout
        << "Range Query (Lineal): "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    cout
        << "Registros revisados: "
        << linearVisited
        << "\n\n";

    db.getScoreIndex()
    .resetVisited();

    start =
    high_resolution_clock::now();

    double median =
        db.medianScore();

    end =
        high_resolution_clock::now();

    cout
        << "Mediana (RBT): "
        << median
        << " | "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";
    vector<double> scores;

    cout
        << "Nodos visitados: "
        << db.getScoreIndex()
            .getVisited()
        << "\n";


    scores.reserve(
        records.size()
    );

    start =
        high_resolution_clock::now();

    for(const auto& r : records)
    {
        scores.push_back(
            r.score
        );
    }

    sort(
        scores.begin(),
        scores.end()
    );

    double medianLinear;

    int n =
        scores.size();

    if(n % 2 == 1)
    {
        medianLinear =
            scores[n/2];
    }
    else
    {
        medianLinear =
            (
                scores[n/2]
                +
                scores[n/2 - 1]
            ) / 2.0;
    }

    end =
        high_resolution_clock::now();

    cout
        << "Mediana (Lineal): "
        << medianLinear
        << " | "
        << duration_cast<milliseconds>(
               end - start
           ).count()
        << " ms\n";

    db.getScoreIndex()
    .resetVisited();

    start =
        high_resolution_clock::now();

    double p90 =
        db.percentileScore(90);

    end =
        high_resolution_clock::now();

    cout
        << "Percentil 90 (RBT): "
        << p90
        << " | "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    cout
        << "Nodos visitados: "
        << db.getScoreIndex()
               .getVisited()
        << "\n";

    vector<double> temp = scores;

    start = high_resolution_clock::now();

    sort(temp.begin(), temp.end());

    int pos = (90 * (temp.size() - 1)) / 100;
    double p90Linear = temp[pos];

    end = high_resolution_clock::now();

    cout
        << "Percentil 90 (Lineal): "
        << p90Linear
        << " | "
        << duration_cast<milliseconds>(
               end - start
           ).count()
        << " ms\n\n";

    db.getScoreIndex()
    .resetVisited();

    start =
        high_resolution_clock::now();

    int countIndex =
        db.getScoreIndex()
            .countRange(
                low,
                high
            );

    end =
        high_resolution_clock::now();

    cout
        << "Count Range (RBT): "
        << countIndex
        << " | "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    cout
        << "Nodos visitados: "
        << db.getScoreIndex()
               .getVisited()
        << "\n";

    int countLinear = 0;

    start =
        high_resolution_clock::now();

    for(const auto& r : records)
    {
        if(
            r.score >= low &&
            r.score <= high
        )
        {
            countLinear++;
        }
    }

    end =
        high_resolution_clock::now();

    cout
        << "Count Range (Lineal): "
        << countLinear
        << " | "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n\n";


}
