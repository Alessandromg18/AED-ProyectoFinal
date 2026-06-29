//
// Created by facum on 9/06/2026.
//

#ifndef HASHSET_H
#define HASHSET_H

#pragma once

#include <functional>
#include <vector>

class HashSetInt {

private:

    struct Node {

        int value;
        Node* next;
        Node(int value,Node* next = nullptr): value(value),next(next){}

    };

    Node** table;

    size_t capacity;

    size_t numElements;

private:

    size_t hash(int value) const {return std::hash<int>{}(value) % capacity;}

    void initialize(size_t cap) {

        capacity = cap;
        table = new Node*[capacity];

        for(size_t i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }

    void rehash() {

        size_t oldCapacity = capacity;

        Node** oldTable = table;

        initialize(capacity * 2);

        numElements = 0;

        for(size_t i = 0; i < oldCapacity; i++) {

            Node* current = oldTable[i];

            while(current) {

                insert(current->value);

                Node* temp = current;

                current = current->next;

                delete temp;
            }
        }

        delete[] oldTable;
    }

public:

    HashSetInt(size_t cap = 11) : numElements(0)
    {
        initialize(cap);
    }

    ~HashSetInt() {

        for(size_t i = 0; i < capacity; i++) {

            Node* current = table[i];

            while(current) {

                Node* temp = current;

                current = current->next;

                delete temp;
            }
        }

        delete[] table;
    }

    bool contains(int value) const {

        size_t idx = hash(value);

        Node* current = table[idx];

        while(current) {
            if(current->value == value)
                return true;

            current = current->next;
        }

        return false;
    }

    void insert(int value) {

        if(contains(value))
            return;

        if((double)(numElements + 1)/ capacity > 0.75)
        {
            rehash();
        }

        size_t idx = hash(value);

        table[idx] =new Node( value,table[idx] );

        numElements++;
    }

    bool remove(int value) {

        size_t idx = hash(value);

        Node* current = table[idx];

        Node* prev = nullptr;

        while(current) {

            if(current->value == value) {

                if(prev == nullptr)
                    table[idx] = current->next;
                else
                    prev->next = current->next;

                delete current;

                numElements--;

                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    size_t size() const {
        return numElements;
    }

    bool empty() const {
        return numElements == 0;
    }

    std::vector<int> toVector() const;
};

// FUNCION QUE FALTABA PONER

inline std::vector<int> HashSetInt::toVector() const
{
    std::vector<int> result;

    for(size_t i = 0; i < capacity; i++)
    {
        Node* current = table[i];

        while(current)
        {
            result.push_back(current->value);

            current = current->next;
        }
    }
    return result;
}

#endif //HASHSET_H
