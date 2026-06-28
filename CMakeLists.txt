cmake_minimum_required(VERSION 3.29)
project(Proyecto_Final_AED)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(Proyecto_Final_AED
        main.cpp
        models/Record.h
        hash/HashTable.h
        rbtree/RBNode.h
        rbtree/RedBlackTree.cpp
        database/Database.cpp
        benchmark/Benchmark.cpp
        visualization/RBVisualizer.cpp
        visualization/TreeLayout.cpp
        csv/CSVLoader.h
        csv/CSVLoader.cpp
        visualization/DatabaseVisualizer.h
        visualization/DatabaseVisualizer.cpp
        benchmark/QueryResult.h
        benchmark/QueryBenchmark.h
        benchmark/QueryBenchmark.cpp

)

target_link_libraries(Proyecto_Final_AED
        SFML::Graphics
        SFML::Window
        SFML::System
)

# SOLO si tienes carpeta fonts
file(COPY assets/fonts DESTINATION ${CMAKE_BINARY_DIR}/assets)