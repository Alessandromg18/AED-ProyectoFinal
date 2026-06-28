//
// Created by facum on 9/06/2026.
//

#include "Database.h"

#include <algorithm>

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

std::vector<Record> Database::linearFindById(
    int id
)
{
    std::vector<Record> result;

    auto all = getAllRecords();

    for(const auto& r : all)
    {
        if(r.id == id)
        {
            result.push_back(r);
            break;
        }
    }

    return result;
}

std::vector<Record> Database::linearFindBetween(
    double low,
    double high
)
{
    std::vector<Record> result;

    auto all = getAllRecords();

    for(const auto& r : all)
    {
        if(r.score >= low && r.score <= high)
            result.push_back(r);
    }

    return result;
}

std::vector<Record> Database::linearTopK(
    int k
)
{
    auto allRecords = getAllRecords();

    std::sort(
        allRecords.begin(),
        allRecords.end(),
        [](const Record& a, const Record& b)
        {
            return a.score > b.score;
        }
    );

    if(k > allRecords.size())
        k = allRecords.size();

    return std::vector<Record>(
        allRecords.begin(),
        allRecords.begin() + k
    );
}

double Database::linearMedianScore()
{
    auto allRecords = getAllRecords();

    if(allRecords.empty())
        return 0.0;

    std::vector<double> scores;

    for(const auto& record : allRecords)
    {
        scores.push_back(
            record.score
        );
    }

    std::sort(
        scores.begin(),
        scores.end()
    );

    int n = scores.size();

    if(n % 2 == 1)
    {
        return scores[n / 2];
    }

    return (
        scores[n / 2 - 1] +
        scores[n / 2]
    ) / 2.0;
}

std::vector<Record> Database::linearBottomK(
    int k
)
{
    auto all = getAllRecords();

    std::sort(
        all.begin(),
        all.end(),
        [](const Record& a, const Record& b)
        {
            return a.score < b.score; // menor a mayor
        }
    );

    if(k > all.size())
        k = all.size();

    return std::vector<Record>(
        all.begin(),
        all.begin() + k
    );
}

double Database::linearPercentileScore(
    double p
)
{
    auto all = getAllRecords();

    if(all.empty())
        return 0.0;

    std::sort(
        all.begin(),
        all.end(),
        [](const Record& a, const Record& b)
        {
            return a.score < b.score;
        }
    );

    int index =
        static_cast<int>(
            (p / 100.0) * (all.size() - 1)
        );

    return all[index].score;
}

