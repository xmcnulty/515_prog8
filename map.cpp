/** CS 515 Assignment 8
 * File: map.cpp
 * Name: Xavier McNulty
 * Section: 1
 * Date: 24 March 2015
 * Collaboration: 
 */

#include "map.h"

/**
 * Default constructor.
 */
template <typename KEY, typename T>
Map<KEY, T>::Map() {
    _root = new Elem(KEY(), T(), _root, 0, 0, false);
    _size = 0;
}

/**
 * Copy constructor.
 *
 * @param v    map being copied
 */
template <typename KEY, typename T>
Map<KEY, T>::Map(const Map<KEY,T> &v){
    // if empty tree
    if (v._root == v._root->left){
        _root = new Elem(KEY(), T(), _root, 0, 0, false);
        _size = 0;
    } else {
        _root = new Elem(KEY(), T(), _root, 0, 0, false);
        copyCode(_root->left, v._root->left); // to deep copy the tree without dummy nodes
        copyThread(_root, v._root); // to copy the threading; must pass dummy nodes to complete the threads
        _size = v._size;
    }
}

/**
 * Overloaded assignment operator.
 *
 * @param rhs    Map being copied
 * @return       reference to this
 */
template<typename KEY, typename T>
Map<KEY, T>& Map<KEY,T>::operator=(const Map &rhs) {
    if (this != &rhs) {
        if (&rhs != 0) {
            if(_size != 0)
                destructCode(_root->left);
            
            delete _root;
            
            // create a dummy root node
            _root = new Elem(KEY(), T(), _root, 0, 0, false);
            _size = 0;
            
            copyCode(_root->left, rhs._root->left);
            copyThread(_root, rhs._root);
            
            _size = rhs.size();
        } else {
            if(_size != 0)
                destructCode(_root->left);
            
            delete _root;
            
            // create a dummy root node
            _root = new Elem(KEY(), T(), _root, 0, 0, false);
            _size = 0;
        }
    }
    
    return * this;
}

/**
 * Destructor.
 */
template <typename KEY, typename T>
Map<KEY, T>::~Map() {
    if(_size == 0) // empty map.
        delete _root;
    else {
        destructCode(_root->left);
        delete _root;
    }
}

/**
 * Destructs the map tree.
 *
 * @param p    pointer to node being removed
 */
template <typename KEY, typename T>
void Map<KEY, T>::destructCode(Elem *& p) {
    // destroy left subtree.
    if (p->left) {
        destructCode(p->left);
    }
    
    // destroy right subtree.
    if (!p->rightThread) {
        destructCode(p->right);
    }
    
    delete p;
}

// construct a key-element map to rethread the new tree
// the map contains all nodes key values and their corresonding elem node address
// for furture lookup in setting up threads
template <typename KEY, typename T>
void Map<KEY, T>::addToMap(Elem* root, map<KEY, Elem*> &keyElemMap){
    if (root) {
        keyElemMap[root->key] = root;
        addToMap(root->left, keyElemMap);
        if (!root->rightThread)
            addToMap(root->right, keyElemMap);
    }
}

/**
 * Common copyCode
 */
template <typename KEY, typename T>
void  Map<KEY,T>::copyCode(Elem* &newRoot, Elem* origRoot){
    if (origRoot == 0)
        newRoot = 0;
    else{
        newRoot = new Elem(origRoot->key, origRoot->data, 0, 0, origRoot->height, origRoot->rightThread);
        newRoot->key = origRoot->key;
        newRoot->data = origRoot->data;
        newRoot->height = origRoot->height;
        newRoot->rightThread = origRoot->rightThread;
        copyCode(newRoot->left, origRoot->left);
        if (!origRoot->rightThread)
            copyCode(newRoot->right, origRoot->right);
    }
}

// copy threads for the tree
template <typename KEY, typename T>
void Map<KEY, T>::copyThread(Elem* &newRoot, Elem* origRoot){
    // construct the key-element map for new and orig tree
    map<KEY, Elem*> newKeyElemMap;
    map<KEY, Elem*> origKeyElemMap;
    addToMap(newRoot->left, newKeyElemMap);
    addToMap(origRoot->left, origKeyElemMap);
    
    // start at the last element in the tree, which threads to root
    typename std::map<KEY, Elem*>::reverse_iterator it = origKeyElemMap.rbegin();
    newKeyElemMap[it->first] -> rightThread = true;
    newKeyElemMap[it->first] -> right = newRoot;
    
    // then thread the rest of the tree backwardly
    it++;
    while(it != origKeyElemMap.rend()){
        if (it->second->rightThread){
            newKeyElemMap[it->first] -> rightThread = true;
            newKeyElemMap[it->first] -> right = newKeyElemMap[ origKeyElemMap[it->first]->right->key ];
        }
        it++;
    } 
}

/**
 * Overloaded [] operator.
 *
 * @param k    key being searched for
 * @return     reference to data that belongs to k
 */
template<typename KEY, typename T>
T& Map<KEY, T>::operator[](KEY k) {
    Iterator it = find(k);
    
    // insert element if not in the map.
    if (it == end()) {
        insert(k, T());
        it = find(k);
    }
    
    return it->data;
}

/**
 * find min
 */
template<typename KEY, typename T>
typename Map<KEY, T>::Elem* Map<KEY, T>::findMin(Map::Elem *node) {
    if (node == 0)
        return node;
    while (node->left)  // move to the leftmost node
        node = node->left;
    return node;
}

template<typename KEY, typename T>
int Map<KEY, T>::size() const {
    return _size;
}

/**
 * Find an element in the map.
 *
 * @param key     key of element being searched for
 * @return        Iterator to found element
 */
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::find(KEY key) const {
    Iterator it = begin();
    
    while(it != end() && it->key != key)
        it ++;
    
    return it;
}

/**
 * Insert into the tree.
 *
 * @param k    key of new element
 * @param t    data of new element
 * @return     true if the data element was inserted
 */
template <typename KEY, typename T>
bool Map<KEY, T>::insert(KEY k, T t) {
    if (_size == 0) { // insert into an empty set.
        _root->left = new Elem(k, t, 0, _root);
        _size ++;
        return true;
    } else if(find(k) != end()) {
        return false;
    } else { // use helper method.
        insert(k, t, _root->left, _root);
        return true;
    }
}

/**
 * Insert helper method.
 *
 * @param k    key for new element
 * @param t    data for new element
 * @param cur  curent pointer
 * @param lastLeft  for rightThread
 * @return true if element inserted
 */
template <typename KEY, typename T>
void Map<KEY, T>::insert(KEY k, T t, Elem *& cur, Elem * lastLeft) {
    Elem * node;
    
    if(cur->left && k < cur->key) { // insert at left.
        node = cur->left;
        insert(k, t, cur->left, cur);
    } else if(!cur->rightThread && k > cur->key) { // insert at right.
        node = cur->right;
        insert(k, t, cur->right, lastLeft);
    } else if (!cur->left && k < cur->key) { // insert at the left.
        cur->left = new Elem(k, t, 0, cur);
        node = cur->left;
        _size ++;
    } else { //  insert at the right.
        cur->rightThread = false;
        cur->right = new Elem(k, t, 0, lastLeft);
        node = cur->right;
        _size ++;
    }
    
    
    if (height(cur->left, false) - height(cur->right, cur->rightThread) == 2) { // calculate load factor
        if(k < cur->left->key) // outside case
            rotateRight(cur);
        else  // inside case
            doubleRotateRight(cur);
    }
    
    if (height(cur->left, false) - height(cur->right, cur->rightThread) == -2) { // calculate load factor
        if (k > cur->right->key) // outside case
            rotateLeft(cur);
        else  // inside case
            doubleRotateLeft(cur);
    }
    
    cur->height = max(height(cur->left, false), height(cur->right, cur->rightThread)) + 1; // update height
}

template <typename KEY, typename T>
int Map<KEY, T>::height(Elem *node, bool rThread) {
    if (node == 0 || rThread) {
        return -1;
    } else {
        return node->height;
    }
}

// output the structure of tree. The tree is output as "lying down"
// output each node's key, value and its tree height
template <typename KEY, typename T>
void Map<KEY, T>::printTree(ostream& out, int level, Elem *p) const{
    int i;
    if (p) {
        if (p->right && !p->rightThread)
            printTree(out, level+1,p->right);
        for(i=0;i<level;i++) {
            out << "\t";
        }
        out << p->key << " " << p->data << "(" << p->height << ")" << '\n';
        printTree(out, level+1,p->left);
    }
}

// outputs information in tree in inorder traversal order
template <typename KEY, typename T>
ostream& Map<KEY, T>::dump(ostream& out) const{
    if ( _size == 0) {// tree empty
        return out;
    }
    printTree(out, 0, _root->left);   // print tree structure
    return out;
}


// outputs using overloaded << operator
template<typename KEY, typename T>
ostream& operator<< (ostream& out, const Map<KEY, T>& v){
    v.dump(out);
    return out;
}

/*
 * Map iterator implementation.
 */
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::begin() const {  // return the left most (smallest) tree node
    // fill in here
    if(_size == 0) // empty tree
        return Iterator(_root);
    
    Elem * cur = _root->left;
    while(cur->left)
        cur = cur->left;
    
    return Iterator(cur);
}

/**
 * Rotate left method.
 *
 * @param node  node being rotated about
 */
template <typename KEY, typename T>
void Map<KEY, T>::rotateLeft(Elem *& node) {
    //cout << "rotating left on " << node->key << endl;
    Elem * tmp = node->right;
    if(tmp->left)
        node->right = tmp->left;
    else {
        node->rightThread = true;
        node->right = tmp;
    }
    tmp->left = node;
    node = tmp;
    
    node->left->height = max(height(node->left->left, false), height(node->left->right, node->left->rightThread)) +1;
}

/**
 * Rotate right method.
 *
 * @param node  node being rotated about
 */
template <typename KEY, typename T>
void Map<KEY, T>::rotateRight(Elem *& node) {
    //cout << "rotating right "<< node->key << endl;
    Elem * tmp = node->left;
    if(!tmp->rightThread)
        node->left = tmp->right;
    else
        node->left = 0;
    
    tmp->right = node;
    tmp->rightThread = false;
    node = tmp;
    
    if(!node->rightThread)
        node->right->height = max(height(node->right->left, false), height(node->right->right, node->right->rightThread)) +1;
    else
        node->right->height = -1;
}

/**
 * Double rotate right.
 */
template <typename KEY, typename T>
void Map<KEY, T>::doubleRotateRight(Elem *& node) {
    //cout << "Double rotate right" << endl;
    rotateLeft(node->left);
    rotateRight(node);
}

/**
 * Double rotate left.
 */
template <typename KEY, typename T>
void Map<KEY, T>::doubleRotateLeft(Elem *& node) {
    //cout << "Double rotate left" << endl;
    rotateRight(node->right);
    rotateLeft(node);
}


//------------------------Iterator methods--------------------------
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::end() const {  // return the dummy root node
    // fill in here
    return Iterator(_root);
}


template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::Iterator::operator++(int){
    if (_cur->right == 0) {
        return *this;
    }
    
    // fill in here
    if(_cur->rightThread)
        _cur = _cur->right;
    else {
        _cur = _cur->right;
        while(_cur->left)
            _cur = _cur->left;
    }
    
    return *this;
}

template <typename KEY, typename T>
typename Map<KEY, T>::Elem& Map<KEY, T>::Iterator::operator*(){
    // fill in here
    return *_cur;
}

template <typename KEY, typename T>
typename Map<KEY, T>::Elem* Map<KEY, T>::Iterator::operator->(){
    // fill in here
    return _cur;
}

/**
 * Iterator comparison.
 *
 * @param it    Iterator being compared to this
 * @return      true if the Iterators are equal
 */
template <typename KEY, typename T>
bool Map<KEY, T>::Iterator::operator==(Iterator it) {
    return (_cur == it.operator->());
}

/**
 * Iterator comparison.
 *
 * @param it    Iterator being compared to this
 * @return      true if the Iterators are equal
 */
template <typename KEY, typename T>
bool Map<KEY, T>::Iterator::operator!=(Iterator it) {
    return (_cur != it.operator->());
}
