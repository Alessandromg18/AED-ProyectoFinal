//
// Created by facum on 26/06/2026.
//

#ifndef DATABASEVISUALIZER_H
#define DATABASEVISUALIZER_H
#pragma once

#include "../models/Record.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>
class DatabaseVisualizer
{
private:

    sf::Font& font;

    std::vector<Record> records;
    std::unordered_set<int> highlightedIds;

    float startX;
    float startY;
    float rowHeight;

    int currentPage = 0;
    int pageSize = 7;
    int totalPages = 0;

public:

    DatabaseVisualizer(sf::Font& font);

    void setRecords(const std::vector<Record>& newRecords);

    void highlight(const std::vector<int>& ids);
    void clearHighlights();

    void draw(sf::RenderWindow& window);

    void nextPage();
    void prevPage();
};

#endif //DATABASEVISUALIZER_H
