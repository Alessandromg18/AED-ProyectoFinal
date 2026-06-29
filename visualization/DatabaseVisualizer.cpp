//
// Created by facum on 26/06/2026.
//

#include "DatabaseVisualizer.h"
#include <sstream>

static std::string truncateText(const std::string& text,size_t maxLength)
{
    if(text.size() <= maxLength)
        return text;

    return text.substr(0, maxLength - 3) + "...";
}

DatabaseVisualizer::DatabaseVisualizer(sf::Font& font): font(font),startX(20.f),startY(50.f),rowHeight(30.f){}

void DatabaseVisualizer::setRecords(const std::vector<Record>& newRecords)
{
    records = newRecords;
    totalPages = (records.size() + pageSize - 1) / pageSize;
    currentPage = 0;
}

void DatabaseVisualizer::highlight(const std::vector<int>& ids)
{
    highlightedIds.clear();

    for(int id : ids)
        highlightedIds.insert(id);
}

void DatabaseVisualizer::clearHighlights()
{
    highlightedIds.clear();
}

void DatabaseVisualizer::draw(sf::RenderWindow& window)
{
    const float colId    = 80.f;
    const float colName  = 260.f;
    const float colAge   = 80.f;
    const float colScore = 100.f;
    const float colCat   = 140.f;

    sf::Text header(font);
    header.setCharacterSize(16);
    header.setFillColor(sf::Color::Black);

    float x = startX;
    float y = startY;

    header.setString("ID");
    header.setPosition({x, y});
    window.draw(header);
    x += colId;

    header.setString("NOMBRE");
    header.setPosition({x, y});
    window.draw(header);
    x += colName;

    header.setString("SCORE");
    header.setPosition({x, y});
    window.draw(header);
    x += colAge;

    header.setString("TIMESTAMP");
    header.setPosition({x, y});
    window.draw(header);
    x += colScore;

    header.setString("CATEGORIA");
    header.setPosition({x, y});
    window.draw(header);

    // Paginacion :

    int start = currentPage * pageSize;
    int end = std::min(start + pageSize, (int)records.size());

    float rowY = startY + 40.f;

    sf::Text cell(font);
    cell.setCharacterSize(14);
    cell.setFillColor(sf::Color::Black);

    for(int i = start; i < end; i++)
    {
        const auto& record = records[i];

        sf::RectangleShape rowBg({650.f, rowHeight - 4.f});
        rowBg.setPosition({startX - 5.f, rowY});

        if(highlightedIds.count(record.id))
            rowBg.setFillColor(sf::Color(200, 230, 255));

        else
            rowBg.setFillColor(sf::Color(245, 245, 245));

        window.draw(rowBg);

        float cx = startX;

        cell.setString(std::to_string(record.id));
        cell.setPosition({cx, rowY});
        window.draw(cell);
        cx += colId;

        cell.setString(truncateText(record.name, 28));
        cell.setPosition({cx, rowY});
        window.draw(cell);
        cx += colName;

        cell.setString(std::to_string(record.age));
        cell.setPosition({cx, rowY});
        window.draw(cell);
        cx += colAge;

        cell.setString(std::to_string(static_cast<int>(record.score)));
        cell.setPosition({cx, rowY});
        window.draw(cell);
        cx += colScore;

        cell.setString(record.category);
        cell.setPosition({cx, rowY});
        window.draw(cell);

        rowY += rowHeight;
    }

    sf::Text footer(font);
    footer.setCharacterSize(14);
    footer.setFillColor(sf::Color::Black);

    footer.setString(
        "Pagina " + std::to_string(currentPage + 1) +
        " / " + std::to_string(totalPages)
    );

    auto viewSize = window.getView().getSize();

    footer.setPosition({startX,viewSize.y - 30.f});
    window.draw(footer);
}

void DatabaseVisualizer::nextPage()
{
    if(currentPage < totalPages - 1)
        currentPage++;
}

void DatabaseVisualizer::prevPage()
{
    if(currentPage > 0)
        currentPage--;
}

