#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <vector>
#include <stdexcept>
#include "Hash.h"

using std::vector;
using std::pair;
using std::cout;
using std::cin;
using std::endl;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState {
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template<typename K, typename V>
class ProbingHash : public Hash<K,V> {
private:
    // Needs a table and a size.
    // Table should be a vector of std::pairs for lazy deletion    
    vector<pair<pair<K,V>,EntryState>> _hashTable; 
    // this is a pair of pairs to hold data and then the EntryState
    int _tableSize;

public:
    ProbingHash(int n = 11) 
    {
        _tableSize = n;
        _hashTable.clear();
        _hashTable.resize(n);
        pair<pair<K,V>,EntryState> elementPair;
        for(int index = 0; index <= _tableSize; index++)
        {
            elementPair.second = EMPTY;
            _hashTable.assign(index,elementPair);
        }
    }

    ~ProbingHash() 
    {
        _hashTable.clear();
    }

//empty() tests to see if the hash is empty
    bool empty() 
    {
        pair<pair<K,V>,EntryState> elementPair;
        //have to check every bucket's EntryState
        for(int index = 0; index < _tableSize;index++)
        {
            elementPair = _hashTable[index];
            if(elementPair.second == VALID)
            {
                return false;
            }
        }
        return true;
    }

//size() returns the amount of non-deleted elements in the hash
    int size() 
    {
        int size = 0;
        pair<pair<K,V>,EntryState> element;
        //have to iterate through _hashTable and check every bucket
        for(int index = 0; index < _tableSize; index++)
        {
            element = _hashTable[index];
            if(element.second == VALID)
            {
                size++;
            }
        }
        return size; 
    }

//at() this returns the value that is with the key, gets called when [] is used
    V& at(const K& key) 
    {
        int index = 0;
        pair<pair<K,V>,EntryState> element;

        index = hash(key);
        for(; 
            _hashTable[index].first.first != key && _hashTable[index].second != EMPTY; ++index %= _tableSize)
        {
            //pass
            //this loop checks to make sure that the bucket has the correct key, using the collision resolution loop to check
        }

        if(_hashTable[index].first.first == key &&
           _hashTable[index].second == VALID) //cheching to make sure that the item is VALID
        {
            return _hashTable[index].first.second;
        }
        throw std::out_of_range("Key not in hash");
    }

//operator[] calls at() to be an overloaded operator
    V& operator[](const K& key) 
    {
        return at(key);
    }

//count() returns the the number of elements with the key k
    int count(const K& key) 
    {
        int elements = 0;
        int index = 0;
         
        index = hash(key); //need to get the index of the key
        
       for(;_hashTable[index].first.first != key && _hashTable[index].second != EMPTY; ++index %= _tableSize)
        {
            //this loops checks to make sure that the bucket has the key
        }
        elements = bucket_size(index);
        return elements;
    }

//emplace() puts the pair into the hashTable and then sets the EntryState to VALID
    void emplace(K key, V value)
    {
        int index = 0;
        pair<pair<K,V>,EntryState> elementPair;
        float lambda = 0;

        index = hash(key);
        elementPair = _hashTable[index];
        lambda = load_factor(); //get the load factor

        if(lambda >= .5) //checking the load factor in case i need to rehash
        {
            rehash();
        }
        
        if(elementPair.second == EMPTY) //checks to see if the entryState of the current bucket is EMPTY
        {
            elementPair = _hashTable[index];
            elementPair.first.second = value;
            elementPair.first.first = key;
            elementPair.second = VALID;
        }
        else
        { //collision resolution
            for(;_hashTable[index].first.first != key && _hashTable[index].second != EMPTY; ++index %= _tableSize)
            {
                //pass
            }
            elementPair = _hashTable[index];
            elementPair.first.second = value;
            elementPair.first.first = key;
            elementPair.second = VALID;
        }
                
        _hashTable[index] = elementPair; //finally puts values into hashTable
    }

//insert() calls emplace
    void insert(const std::pair<K, V>& pair) 
    {
        K key;
        V value;
        
        key = pair.first;
        value = pair.second;
        emplace(key,value);
    }

//erase() will go through the hashTable and find the specific key and set the EntryState of that pair to DELETED. This is lazy deletion, so i don't use data
    void erase(const K& key) 
    {
        int index = 0;
        index = hash(key);
        for(;_hashTable[index].first.first != key && _hashTable[index].second != EMPTY; ++index %= _tableSize)
        {
        }
        _hashTable[index].second = DELETED;
    }

    void clear() 
    {
        //this will clear every bucket in hash table and set the entry state to EMPTY so I can emplace elements on it. Do not need to change 
        pair<pair<K,V>,EntryState> elementPair;
        for(int index = 0; index < _tableSize; index++)
        {
            elementPair.second = EMPTY;
            _hashTable[index] =  elementPair;
        }
        
    }

    int bucket_count() 
    {
        return _tableSize;
        //returns the number of buckets allocated to hashTable (hashTable max size)
    }

    int bucket_size(int n) 
    {
        int numberElements = 0;
        if(_hashTable[n].second == VALID)
        {
            numberElements = 1;
        }
        return numberElements;
        //checks if indexed value is VALID if it is bucket size is 1, if not bucket size is 0 
    }

    int bucket(const K& key) 
    {
        throw std::out_of_range("Key not in hash");
        return hash(key);
        //returns the bucket number of key 
    }

    float load_factor() 
    {
        int elements = 0;
        float lambda = 0.000000;

        elements = size(); //grabs the quantity of elements in hash
        
        lambda = ((float)elements / (float)_tableSize); //finds load factor
        
        return lambda;
    }

    void rehash() 
    {
        rehash(_tableSize);
    }

    void rehash(int n) 
    {
        vector<pair<pair<K,V>,EntryState>> oldHash = _hashTable;
        pair<pair<K,V>,EntryState> elementPair;
        K key;
        V value;

        _tableSize = findNextPrime(2 * _tableSize); 
        _hashTable.resize(_tableSize); //set hashTable size to new size which is double and then the next prime
        clear();//delete the old hashTable so i can rehash it
        for(int index = 0; index < (int)oldHash.size(); index++)
        {
            //searchs through oldHash to find valid pairs to put into new hash
            elementPair = oldHash[index];
            if(elementPair.second == VALID)
            {
                key = elementPair.first.first;
                value = elementPair.first.second;
                emplace(key, value);//inserts valid values into the hash
            }
        }
    }

private:
    int findNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    int isPrime(int n)
    {
        for (int i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    int hash(const K& key) 
    {
        return (key % _tableSize);
    }
//to cut down on runtime try to make less copys and make things pointers
    
};

#endif //__PROBING_HASH_H
