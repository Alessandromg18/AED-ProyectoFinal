//
// Created by facum on 9/06/2026.
//

#ifndef CSVLOADER_H
#define CSVLOADER_H

#pragma once

#include <string>
#include <vector>

#include "../models/Record.h"

class CSVLoader {

public:

    static std::vector<Record> load(
        const std::string& filename
    );

};

#endif //CSVLOADER_H
