//
// Created by facum on 9/06/2026.
//

#ifndef RECORD_H
#define RECORD_H

#pragma once
#include <string>

struct Record {

    int id;   // Debe ser unico para la tabla hash
    std::string name;
    int age;
    double score;
    std::string category;

    Record() = default;

    Record(int id,const std::string& name,int age,double score,const std::string& category): id(id),name(name),
          age(age),score(score),category(category){}

};

#endif //RECORD_H
