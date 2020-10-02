/*
 *  Yet another AVL tree edition
 * 
 *  Contributors:
 *      Aaron S. Crandall <acrandal@wsu.edu> - 2018
 * 
 */

#ifndef __AVLTree_H
#define __AVLTree_H

#include <ostream>
#include <algorithm>     // Includes a max(a,b) function
#include <exception>
#include "AVLNode.h"

// AVLTree class
// ******************PUBLIC OPERATIONS*********************
// bool empty( )          --> Test for empty tree
// int size( )            --> Quantity of elements in tree
// int height( )          --> Height of the tree (nullptr == -1)
// void add( x )          --> Insert new value x
// void remove( x )       --> Remove node with value x
// void clear ( )         --> Removes all elements from tree
// bool contains( x )     --> Return true if x is present
// T findMin( )           --> Return smallest item value
// T findMax( )           --> Return largest item value
// void printPreOrder( )  --> Print tree in pre order to an ostream
// void ~AVLTree( )       --> Big Five Destructor
// AVLTree( )             --> Basic constructor


template <typename T>
class AVLTree
{
private:
    AVLNode<T> * _root;

public:
    AVLTree() : _root(nullptr) {}    // initializes root to nullptr

    /* Returns true if the tree is empty */
    bool empty()   
    {
        return this->_root == nullptr; 
        // TODO: Properly return t/f if the tree is empty or not
    }

    void add(const T & newVal) 
    {
        // TODO: implement adding a value to the tree
       addHelper(newVal, this->_root);
    }

    void remove(const T & remVal) 
    {
        // TODO: Implemente properly removing a node and rebalancing the tree
        removeHelper(remVal, this->_root);
    }
    

    void clear() 
    {
        // TODO: Delete all elements in the tree

        clearHelper(this->_root);
    }

    bool contains(const T & findVal) 
    {
        // TODO: Return t/f if a given value is currently in the tree
    
        return containsHelper(findVal, this->_root);
    }

    // TODO: Return the current height of the tree
    // Remember! A null pointer is height -1
    int height() 
    {
        return height(this->_root); //this calls the private height function that returns the height of the tree or -1 if nullptr
    }

    // Return the minimum *value* in the tree (not a tree node)
    // this is to allow the tests to fail - should definitely be changed!
    T findMin()
    {
        T min;
        min = findMinHelper(this->_root);
        return min; 
    }

    // Return the maximum *value* in the tree (not a tree node)
    // this is to allow the tests to fail - should definitely be changed!
    T findMax() 
    {
        T max;  
        max = findMaxHelper(this->_root); //calls the findMaxHelper function
        return max;
    }

    // Prints the tree to ostream out in preorder traversal
    void printPreOrder(std::ostream& out) 
    {
        printPreOrder(this->_root, out);
    }

    int size() 
    {
        // TODO: Return the size of the tree (number of nodes in the tree)
        int value = 0;
        sizeHelper(this->_root, value); //this calls the sizeHelper function that is private to protect private data members
        return value;
    }

// *************** Private /internal function implementation ******* //

private:
    // Should put your various internal balancing, rotating, and calculating functions here

    // Freebie: neat little function to safely get a node's height
    int height( AVLNode<T> * t ) const
    {
        return t == nullptr ? -1 : t->height;
    }

    // Traverses the tree and prints in pre order
    // This is used a lot in the testing system, changing it would be troublesome
    void printPreOrder(AVLNode<T> * & t, std::ostream& out)
    {
        if( t != nullptr )
        {
            out << t->val << " ";
            printPreOrder(t->left, out);
            printPreOrder(t->right, out);
        }
    }
    //this function is a helper function for the size public funciton
    //It does a post order traversal and counts all the nodes in the tree
    //returns the amount of nodes that are in the tree
    int sizeHelper(AVLNode<T> *& t, int  &value)
    {
        if( t != nullptr)
        {
            sizeHelper(t->left, value);
            sizeHelper(t->right, value);
            value++;
        }
    }
    //this function is a helper function for the findMin public function
    //It iterates through the tree to the farthest left node in the tree and returns the value in the node
    T findMinHelper(AVLNode<T> *& t)
    {
        T minVal;
        AVLNode<T> *Cur = t;
        
        if(t != nullptr)
        {
            while(Cur->left != nullptr)
            {
                Cur = Cur->left;
            }
            minVal = Cur->val;
            return minVal;
        }
    }
    
    //this function is a helper function for the findMax public function
    //It iterates through the tree to the farthest right node in the tree and returns the val in that node
    T findMaxHelper(AVLNode<T> *&t)
    {
        T maxVal;
        AVLNode<T> *Cur = t;
                
        if(t != nullptr)
        {
            while(Cur->right != nullptr)
            {
                Cur = Cur->right;
            }
            maxVal = Cur->val;
            return maxVal;
        }
    }
    //this function is called by many funcitons to balance the tree, (add, remove call it)
    //It calls all of the rotates to balance the tree accordingly
    //returns nothing, but it will change the t node because it is passed by reference
    void balance(AVLNode<T> *& t)
    {
        if(t == nullptr)
        {
            return;
        }
        if(height(t->left) - height(t->right) > 1)
        {
            if(height(t->left->left) > height(t->left->right))
            {
                rotateWLeftChild(t); //single left rotate
            }
            else
            {
                doubleWLeftChild(t); //right left double rotate
            }
        }
        else if(height(t->right) - height(t->left) > 1)
        {
            if(height(t->right->right) > height(t->right->left))
            {
                rotateWRightChild(t); //single right rotate
            }
            else
            {
                doubleWRightChild(t); //left right double rotate
            }
        }
        
        t->height = std::max(height(t->left), height(t->right)) + 1; //adjusting the height of the node
    }
    
    //this is a single left rotate function
    //it is called by the balance function
    //it will update the root node of the subtree to a balanced subtree with a single left rotate
    void rotateWLeftChild(AVLNode<T> *& k2)
    {
        //single left rotate
        AVLNode<T> *k1 = k2->left; //set k1 to the left kid of the root(k2)
        k2->left = k1->right;   //set the left pointer of the root to left pointer of its kid
        k1->right = k2; //set the right pointer of the kid to the root
        k2->height = std::max(height(k2->left), height(k2->right))+1; //asjusting the height of k2
        k1->height = std::max(height(k1->left), k2->height)+1; //adjusting the height of k1
        k2 = k1; //set the root to the original kid
    }
    
    //this is a single right rotate function
    //it is called by the balance function
    //it will update the root node of the subtree to a balanced subtree with a single right rotate
    void rotateWRightChild(AVLNode<T> *& k2)
    {
        //single right rotate
        AVLNode<T> *k1 = k2->right; //set k1 to the right kid of the root(k2)
        k2->right = k1->left; //set the right pointer of the root to left pointer of its kid
        k1->left = k2; //set the left pointer of the kid to the root
        k2->height = std::max(height(k2->left), height(k2->right))+1; //adjusting the height k2
        k1->height = std::max(k2->height, height(k1->right))+1; //adjusting the height of k1
        k2 = k1; //set the root to the kid
    }

    //this is a right left double rotate function
    //it is called by the balance function
    //it calls the left and right rotates to do the double rotate
    //it will update the root node of the subtree to a balanced subtree with a right left double rotate
    void doubleWLeftChild(AVLNode<T> *& k3)
    {
        rotateWRightChild(k3->left);
        rotateWLeftChild(k3);
    }

    //this is a left right double rotate function
    //it is called by the balance function
    //it calls the left and right rotates to do the double rotate
    //it will update the root node of the subtree to a balanced subtree with a left right double rotate
    void doubleWRightChild(AVLNode<T> *& k3)
    {
        rotateWLeftChild(k3->right);
        rotateWRightChild(k3);
    }
    
    //this functino is a helper function for the public add function
    //this function recursively adds a new node and then balances the tree after the new node is inserted
    void addHelper(const T & newVal, AVLNode<T> *& t)
    {
        if(t == nullptr) //no kids, insert in tree
        {
            t = new AVLNode<T>(newVal);
        }
        else if(newVal<t->val) //newVal is smaller move left
        {
            addHelper(newVal, t->left);
        }
        else if(newVal > t->val) //newVal is bigger move right
        {
            addHelper(newVal, t->right);
        }
        balance(t); //balance tree after adding new node
    }

    //this is a helper function for the remove public function
    //this function deletes a specific node and then balances the tree after the removal
    void removeHelper(const T & remVal, AVLNode<T> *& t)
    { 
        if(t == nullptr)
        {
            return;
        }
        if(remVal < t->val)
        {
            removeHelper(remVal, t->left);
        }
        else if(remVal > t->val)
        {
            removeHelper(remVal, t->right);
        }
        else if(t->left != nullptr && t->right != nullptr) //if value is found and it has two kids then take the value of the smallest node in the right subtree
        {    
            t->val = findMinHelper(t->right);
            removeHelper(t->val, t->right);
            
        }
        else
        {//deletes node
            AVLNode<T> *old = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete old;
        }
        balance(t); //balances the tree after removal
    }

    //helper function that deletes all nodes and then sets the root back to nullptr
    void clearHelper(AVLNode<T> *&t)
    { 
        if(t != nullptr)
        {
            clearHelper(t->left);
            clearHelper(t->right);
            delete t;
        }
        t = nullptr;
    }
    
    //this function is a helper function for the public contains function
    //this function searchs a tree to see if a specific value exists in it
    //returns true or false 
    bool containsHelper(const T & findVal, AVLNode<T> *& t)
    {
        if( t == nullptr) //if value is not found return false
        {
            return false;
        }
        if( findVal < t->val) //findVal is less than val in node? move left
        {
            containsHelper(findVal, t->left);
        }
        else if(findVal > t->val) //findVal is greater than val in node? move right
        {
            containsHelper(findVal, t->right);
        }
        else if(findVal ==  t->val) //once val is found in tree return true
        {
            return true;
        }
    }
};

//THIS CODE PASSES ALL OF THE BASIC AND STRESS TESTS FOR MAKE TEST
//THIS CODE PASSES THE TEST FOR MAKE HUGETEST

#endif
