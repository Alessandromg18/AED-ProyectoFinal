//
// Created by facum on 9/06/2026.
//

#include "CSVLoader.h"

#include <fstream>
#include <stdexcept>

static std::vector<std::string>
parseCSVLine(const std::string& line)
{
    std::vector<std::string> fields;

    std::string current;

    bool insideQuotes = false;

    for(char c : line)
    {
        if(c == '"')
        {
            insideQuotes = !insideQuotes;
        }
        else if(c == ',' && !insideQuotes)
        {
            fields.push_back(current);
            current.clear();
        }
        else
        {
            current += c;
        }
    }

    fields.push_back(current);

    return fields;
}

std::vector<Record>CSVLoader::load(const std::string& filename)
{
    std::vector<Record> records;

    std::ifstream file(filename);

    if(!file.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo CSV");
    }

    std::string line;

    std::getline(file,line);

    while(std::getline(file,line))
    {
        if(line.empty())
            continue;

        auto fields = parseCSVLine(line);

        if(fields.size() != 5)
        {
            continue;
        }
        try
        {
            Record record(std::stoi(fields[0]),fields[1],std::stoi(fields[2]),std::stod(fields[3]),
                fields[4]);

            records.push_back(record);
        }
        catch(...)
        {
            // Para ignorar filas que pueden ser corruptas
        }
    }

    return records;
}