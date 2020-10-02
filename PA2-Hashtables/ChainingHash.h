/*
 *  Separate chaining hashtable
 */

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

// Standard library includes
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <iterator>

// Custom project includes
#include "Hash.h"

// Namespaces to include
using std::vector;
using std::list;
using std::pair;
using std::iterator;
using std::cout;
using std::endl;

//
// Separate chaining based hash table - inherits from Hash
//
template<typename K, typename V>
class ChainingHash : public Hash<K,V> {
private:
    int _tableSize;
    vector<list<pair<K,V>>> _hashTable; //need have a pair in the list
public:
    ChainingHash(int n = 11) 
    {
        _tableSize = n;
        _hashTable.clear();
        _hashTable.resize(n);
    }

    ~ChainingHash() 
    {
        this->clear();
    }

//empty() checks to see if the hash is empty
    bool empty() 
    {
        list<pair<K,V>> *hashList;//does this make it faster
        //have to check every bucket
        for(int i = 0; i < _tableSize;i++)
        {
            hashList = &_hashTable[i];
            if(hashList->size() > 0)
            {
                //if the list's size > 0 for this bucket then the whole hashTable can not be empty
                return false;
            }
        }
        return true;
    }

//size() returns the amount of elements in the hash
    int size() 
    {
        list<pair<K,V>> hashList;
        int  elements = 0;

        //have to iterate through _hashTable and check the size of the list in every bucket
        for(int i = 0; i < _tableSize; i++)
        {
            hashList = _hashTable[i];
            elements = elements + (int)hashList.size();
        }
        return elements;    
    }

//at() returns the value that is with the key, gets called when [] is used
    V& at(const K& key) 
    {
    
        int index = 0, i = 0;
        list<pair<K,V>> hashList;
        pair<K,V> listPair;
        
        index = hash(key);
        hashList = _hashTable[index]; 
        listPair = hashList.front();        
        while(listPair.first != key && (int)hashList.size() != 0 && i < (int)hashList.size())
        {
            //while loop will run unless the listPair.first = key
            //  the list is empty
            //  i is greater than the size of the list meaning we checked the whole list and the key was not found
            listPair = hashList.front();
            if(listPair.first != key)
            {
                //delete the front and move it to the back of the list if the .first is not equal to the key. //try to find something faster like the find() function.
                hashList.pop_front();
                hashList.push_back(listPair);
            }
            i++;
        }
        if(listPair.first == key)
        { 
            return hashList.front().second;
        }
        //if the key is not found throw 
        throw std::out_of_range("Key not in hash");
    
    }

//operator[] calls the at() function
    V& operator[](const K& key) 
    {
          return at(key);
    }        

//count() returns the number of elements with the key k
    int count(const K& key) 
    {
        list<pair<K,V>> hashList;
        pair<K,V> listPair1, listPair2;
        int elements = 0, index = 0, i = 0;
        
        index = hash(key);
         
        hashList = _hashTable[index];
        while(i < (int)hashList.size())
        //this while loop runs until the end of the list
            //try to use find to have this be quicker run time
        //for tests do not need loop but technically i do to find the proper count of the number of elements with the key k. But should only be one?
        {
            listPair2 = hashList.front();
            hashList.pop_front();
            hashList.push_back(listPair2);
            if(listPair2.first == key)    
            {
                elements = elements + 1;
            }
            i++;            
        }
        
//        elements = (int)hashList.size();
        return elements; 
    }

//emplace() puts the pair into the hashTable
    void emplace(K key, V value) 
    {
        int index = 0, i = 0;
        list<pair<K,V>> hashList;
        pair<K,V> listPair, temp;
        float lambda = 0;
        
        listPair.first = key;
        listPair.second = value;
        index = hash(key);
        hashList = _hashTable[index];
        lambda = load_factor(); //get the load factor

        if(lambda >= 1.0) //checks to see if we need to rehash
        {
            rehash();
        }
        while(hashList.front().first != key && (int)hashList.size() != 0 && i < (int)hashList.size())
        {
            //checks to see if there is already a value with the same key
            temp = hashList.front();
            if(temp.first != key)
            {
                hashList.pop_front();
                hashList.push_back(temp);
            }
            i++;
        }
        if(hashList.front().first == key)
        {
            hashList.pop_front();
        }

        hashList.push_back(listPair); 
        _hashTable[index] = hashList; //finally puts values in hashTable
    }

//insert() calls emplace()
    void insert(const std::pair<K, V>& pair) 
    {
        V value = pair.second;
        K key = pair.first;
        emplace(key, value);
    }

//erase() will go hash to the bucket and then go through the list to find the correct key and erase it
    void erase(const K& key) 
    {
        int index = 0, i = 0;
        list<pair<K,V>> hashList;
        pair<K,V> listPair;
        
        index = hash(key);
        hashList = _hashTable[index];
        listPair = hashList.front();
        while(listPair.first != key && (int)hashList.size() != 0 && i < (int)hashList.size())
        {
            //goes through to find the correct key
            listPair = hashList.front();
            if(listPair.first != key)
            {
                hashList.pop_front();
                hashList.push_back(listPair);
            }
            i++;
        }
        //look into using find and swapping the front and found iterator values
        if(hashList.front().first == key)
        {
             hashList.pop_front();
        }
        _hashTable[index] = hashList;
    }

//clear() will clear will clear the whole _hashTable
    void clear() 
    {
        for( auto &hTable : _hashTable)
        {
            hTable.clear();
        }
    }

//bucket_count() returns the amount of buckets allocated to the hashTable
    int bucket_count() 
    {
        return _tableSize; 
    }

//returns the amount of elements that are in a certain bucket
    int bucket_size(int n) 
    {
        list<pair<K,V>> hashList;
        int numberElements = 0;

        hashList = _hashTable[n]; //get the list of the bucket
        numberElements = (int)hashList.size(); //get the size of the list
        return numberElements;//return the size        
    }

//bucket() returns the bucket number of the key
    int bucket(const K& key) 
    {
        throw std::out_of_range("Key not in hash");
        return hash(key);
    }

//load_factor() finds the load facotr of the hashTable, calls size()
    float load_factor() 
    {
        int elements = size();
        float lambda = 0;
        
        lambda = ((float)elements / (float)_tableSize);
        return lambda;
    }

//rehash() calls rehash(int n)
    void rehash() 
    {
        rehash(_tableSize);
    }

//rehash(int n) rehashs the hashTable and resizes it to double the tablesize and then the next prime
    void rehash(int n) 
    {
        vector<list<pair<K,V>>> oldHash = _hashTable;
        list<pair<K,V>> hashList;
        pair<K,V>   listPair;
        K key;
        V value;
        
        _tableSize = findNextPrime(2 * _tableSize);
        _hashTable.resize(_tableSize);
        
        clear();//delete old hashTable 
        for(int index = 0; index < (int)oldHash.size(); index++)
        {
            //searches through oldHash to find valid pairs to put into new hash
            hashList = oldHash[index];
            while(hashList.size() != 0)
            {
                listPair = hashList.front();
                hashList.pop_front();
                key = listPair.first;
                value = listPair.second;
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

    int hash(const K& key) {
        return (key % _tableSize);
    }

};

#endif //__CHAINING_HASH_H
