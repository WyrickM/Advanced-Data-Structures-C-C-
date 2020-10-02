/*
 *  Brute force detector implementation
 * 
 *  Aaron S. Crandall <acrandal@wsu.edu> - 2019
 * 
 */

#ifndef __HASH_DETECTOR_H
#define __HASH_DETECTOR_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>        // a C++11 STL std::unordered_map is a hash table
using std::unordered_map;
using std::vector;
using std::string;

/*
 *  Function to return a list of the strings appearing more than one time in an input vector
 */
std::vector<std::string> find_duplicates_with_hashtable(std::vector<std::string> & strings) {
    std::vector<std::string> dups;

    // MA TODO: Implement the duplicate finder using a hash table (unordered_map)
    
    unordered_map<string, int> hashTable;
    unordered_map<string, int> dupsHash;
    dupsHash.clear();
    hashTable.clear();

    for(unsigned int indexV = 0; indexV < strings.size(); indexV++) //for loop that inserts the strings from the vector in the hash tables
    {

        //find finds iterator where the specific key is, if theo key is not in the hash table it returns the end of the hash table
        //if statement that checks to see if a string is already in the first hashTable. If string is already in the hashTable insert it into the dups vector
        if(hashTable.find(strings[indexV]) != hashTable.end()) 
        {    
            dups.push_back(strings[indexV]); //insert duplicate string at end of dups vector
            if(dupsHash.find(strings[indexV]) != hashTable.end()) //if there are duplicate strings in the dups vector delete the duplicates in dups vector
            {
                dups.erase(dups.end()); //deletes the last string in the vector which will be a duplicate of a dupllicate
            }
            else
            {
                dupsHash[strings[indexV]] = indexV; //allows me to check if there is a duplicate of a duplicate
            }
        }
        else
        {
            hashTable[strings[indexV]] = indexV; //inserts string into hashtable and allows me to check for duplicates
        }
    }
    return dups;
    // Note on return here: it's returning to the caller via a move constructor
    // The vector (dups) lives on the stack, but the stored array contents live on the heap
    // When dups is returned, a new vector shall be instantiated using a move constructor
    // The new vector's move constructor is called with dups as the && other object
}

#endif
