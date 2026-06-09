cmake_minimum_required(VERSION 3.29)
project(Proyecto_Final_AED)

set(CMAKE_CXX_STANDARD 26)

add_executable(Proyecto_Final_AED main.cpp
        models/Record.h
        hash/HashTable.h
        rbtree/RBNode.h
        rbtree/RedBlackTree.h
        rbtree/RedBlackTree.cpp
        database/Database.h
        database/Database.cpp
        hash/HashSet.h
        rbtree/Color.h
        benchmark/Benchmark.h
        benchmark/Benchmark.cpp
        visualization/TreeLayout.h
        visualization/RBVisualizer.cpp
        visualization/RBVisualizer.h
        csv/CSVLoader.h
        csv/CSVLoader.cpp)
