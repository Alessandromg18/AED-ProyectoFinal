//
// Created by facum on 9/06/2026.
//

#include "Database.h"
#include "../csv/CSVLoader.h"

Database::Database()
{
}

bool Database::contains(
    int id
)
{
    return records.contains(id);
}

Record* Database::findById(
    int id
)
{
    return records.search(id);
}

bool Database::insertRecord(
    const Record& record
)
{
    if(
        records.contains(
            record.id
        )
    )
    {
        return false;
    }

    records.insert(
        record.id,
        record
    );

    scoreIndex.insert(
        record.score,
        record.id
    );

    return true;
}

bool Database::deleteRecord(
    int id
)
{
    Record* record =
        records.search(id);

    if(record == nullptr)
        return false;

    scoreIndex.remove(
        record->score,
        id
    );

    records.remove(id);

    return true;
}

bool Database::updateScore(
    int id,
    double newScore
)
{
    Record* record =
        records.search(id);

    if(record == nullptr)
        return false;

    scoreIndex.remove(
        record->score,
        id
    );

    record->score =
        newScore;

    scoreIndex.insert(
        newScore,
        id
    );

    return true;
}

int Database::totalRecords()
{
    return scoreIndex.totalSize();
}

double Database::medianScore()
{
    return scoreIndex.median();
}

double Database::percentileScore(
    double p
)
{
    return scoreIndex.percentile(p);
}

std::vector<Record>
Database::findBetween(
    double low,
    double high
)
{
    std::vector<Record> result;

    std::vector<int> ids =
        scoreIndex.rangeQuery(
            low,
            high
        );

    for(int id : ids)
    {
        Record* record =
            records.search(id);

        if(record)
        {
            result.push_back(
                *record
            );
        }
    }

    return result;
}

std::vector<Record>
Database::topK(
    int k
)
{
    std::vector<Record> result;

    int total =
        scoreIndex.totalSize();

    for(
        int i = total;
        i > total - k && i > 0;
        i--
    )
    {
        double score =
            scoreIndex.select(i);

        RBNode* node =
            scoreIndex.search(score);

        if(node == nullptr)
            continue;

        std::vector<int> ids =
            node->rowIds.toVector();

        for(int id : ids)
        {
            Record* record =
                records.search(id);

            if(record)
            {
                result.push_back(
                    *record
                );
            }
        }
    }

    return result;
}

std::vector<Record>
Database::bottomK(
    int k
)
{
    std::vector<Record> result;

    int total =
        scoreIndex.totalSize();

    for(
        int i = 1;
        i <= k &&
        i <= total;
        i++
    )
    {
        double score =
            scoreIndex.select(i);

        RBNode* node =
            scoreIndex.search(score);

        if(node == nullptr)
            continue;

        std::vector<int> ids =
            node->rowIds.toVector();

        for(int id : ids)
        {
            Record* record =
                records.search(id);

            if(record)
            {
                result.push_back(
                    *record
                );
            }
        }
    }

    return result;
}

void Database::loadCSV(
    const std::string& filename
)
{
    clear();

    auto records =
        CSVLoader::load(filename);

    for(
        const auto& record :
        records
    )
    {
        insertRecord(
            record
        );
    }
}

std::vector<Record>
Database::getAllRecords() const
{
    return records.getValues();
}

