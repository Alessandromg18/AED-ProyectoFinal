//
// Created by facum on 9/06/2026.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H
#pragma once

#include <functional>
#include <stdexcept>
#include <vector>

using namespace std;

template<typename Key>
size_t base_hash(const Key& key)
{
    return std::hash<Key>{}(key);
}

template<typename Key>
class DivisionHash
{
public:

    size_t operator()(
        const Key& key,
        size_t tableSize
    ) const
    {
        return base_hash(key) % tableSize;
    }
};

template<
    typename Key,
    typename Value
>
class HashTable
{
private:

    struct Node
    {
        Key key;
        Value value;

        Node* next;

        Node(
            const Key& k,
            const Value& v,
            Node* n = nullptr
        )
            :
            key(k),
            value(v),
            next(n)
        {}
    };

    Node** table;

    size_t capacity;

    size_t numElements;

    double maxFillFactor;

    DivisionHash<Key> hashFunction;

private:

    void initTable(size_t cap)
    {
        capacity = cap;

        table =
            new Node*[capacity];

        for(size_t i=0;i<capacity;i++)
        {
            table[i] = nullptr;
        }
    }

    void clearBuckets()
    {
        for(size_t i=0;i<capacity;i++)
        {
            Node* current =
                table[i];

            while(current)
            {
                Node* temp =
                    current;

                current =
                    current->next;

                delete temp;
            }

            table[i] = nullptr;
        }
    }

    bool needRehash() const
    {
        double ff =
            (double)(numElements + 1)
            / capacity;

        return ff > maxFillFactor;
    }

    void insertAtHead(
        const Key& key,
        const Value& value
    )
    {
        size_t index =
            hashFunction(
                key,
                capacity
            );

        Node* node =
            new Node(
                key,
                value,
                table[index]
            );

        table[index] = node;

        numElements++;
    }

    void rehash()
    {
        size_t oldCapacity =
            capacity;

        Node** oldTable =
            table;

        capacity *= 2;

        table =
            new Node*[capacity];

        for(size_t i=0;i<capacity;i++)
        {
            table[i] = nullptr;
        }

        numElements = 0;

        for(size_t i=0;i<oldCapacity;i++)
        {
            Node* current =
                oldTable[i];

            while(current)
            {
                insertAtHead(
                    current->key,
                    current->value
                );

                Node* temp =
                    current;

                current =
                    current->next;

                delete temp;
            }
        }

        delete[] oldTable;
    }

public:

    HashTable(
        size_t initialCapacity = 11,
        double maxFF = 0.75
    )
    {
        numElements = 0;

        maxFillFactor = maxFF;

        initTable(
            initialCapacity
        );
    }

    ~HashTable()
    {
        clearBuckets();

        delete[] table;
    }

    HashTable(
        const HashTable&
    ) = delete;

    HashTable& operator=(
        const HashTable&
    ) = delete;

    void insert(
        const Key& key,
        const Value& value
    )
    {
        size_t index =
            hashFunction(
                key,
                capacity
            );

        Node* current =
            table[index];

        while(current)
        {
            if(current->key == key)
            {
                current->value =
                    value;

                return;
            }

            current =
                current->next;
        }

        if(needRehash())
        {
            rehash();

            index =
                hashFunction(
                    key,
                    capacity
                );
        }

        Node* node =
            new Node(
                key,
                value,
                table[index]
            );

        table[index] = node;

        numElements++;
    }

    Value* search(
        const Key& key
    )
    {
        size_t index =
            hashFunction(
                key,
                capacity
            );

        Node* current =
            table[index];

        while(current)
        {
            if(current->key == key)
            {
                return &(current->value);
            }

            current =
                current->next;
        }

        return nullptr;
    }

    const Value* search(
        const Key& key
    ) const
    {
        size_t index =
            hashFunction(
                key,
                capacity
            );

        Node* current =
            table[index];

        while(current)
        {
            if(current->key == key)
            {
                return &(current->value);
            }

            current =
                current->next;
        }

        return nullptr;
    }

    bool contains(
        const Key& key
    ) const
    {
        return search(key)
            != nullptr;
    }

    bool remove(
        const Key& key
    )
    {
        size_t index =
            hashFunction(
                key,
                capacity
            );

        Node* current =
            table[index];

        Node* previous =
            nullptr;

        while(current)
        {
            if(current->key == key)
            {
                if(previous == nullptr)
                {
                    table[index] =
                        current->next;
                }
                else
                {
                    previous->next =
                        current->next;
                }

                delete current;

                numElements--;

                return true;
            }

            previous =
                current;

            current =
                current->next;
        }

        return false;
    }

    bool update(
        const Key& key,
        const Value& value
    )
    {
        Value* ptr =
            search(key);

        if(ptr == nullptr)
        {
            return false;
        }

        *ptr = value;

        return true;
    }

    size_t size() const
    {
        return numElements;
    }

    bool empty() const
    {
        return numElements == 0;
    }

    size_t getCapacity() const
    {
        return capacity;
    }

    double getFillFactor() const
    {
        return
            (double)numElements
            / capacity;
    }

    vector<Key> getKeys() const
    {
        vector<Key> result;

        result.reserve(
            numElements
        );

        for(size_t i=0;i<capacity;i++)
        {
            Node* current =
                table[i];

            while(current)
            {
                result.push_back(
                    current->key
                );

                current =
                    current->next;
            }
        }

        return result;
    }

    vector<Value> getValues() const
    {
        vector<Value> result;

        result.reserve(
            numElements
        );

        for(size_t i=0;i<capacity;i++)
        {
            Node* current =
                table[i];

            while(current)
            {
                result.push_back(
                    current->value
                );

                current =
                    current->next;
            }
        }

        return result;
    }

    void clear()
    {
        clearBuckets();

        numElements = 0;
    }

};

#endif //HASHTABLE_H
