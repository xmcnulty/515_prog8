// A generic Map implemented with right-threaded BST
// BST is not balanced

#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <map> 
using namespace std;

template <typename KEY, typename T>
class Map{
private:
	struct Elem {
        Elem(KEY k, T t, Elem * l, Elem * r, int h = 0, bool rThr = true) : key(k), data(t), left(l), right(r), height(h), rightThread(rThr) {}
		KEY key;
		T data;
		Elem *left;
		Elem *right;
        int height;
		bool rightThread;  // normal right child link or a thread link
	};
	Elem *_root;  // a dummy root sentinel 
	int _size;

	// helper method for inserting record into tree.
	void insert(KEY k, T t, Elem *& cur, Elem * lastLeft);

	// helper method for print tree
	void printTree(ostream& out, int level, Elem *p) const;

	// common code for deallocation
	void destructCode(Elem *& p);

	// common code for copy tree; not required. 
	void copyCode(Elem* &newRoot, Elem* origRoot); 
    int height(Elem *, bool);
    
	// common code for deep copying threaded tree
	void addToMap(Elem* root, map<KEY, Elem*> &keyElemMap); 
	void copyThread(Elem* &newRoot, Elem* origRoot);
    
    // single right rotation
    void rotateRight(Elem *&);
    
    // single left rotation
    void rotateLeft(Elem *&);
    
    // double right rotation
    void doubleRotateRight(Elem *&);
    
    // double left rotation
    void doubleRotateLeft(Elem *&);
    
    Elem* findMin(Elem *node);

public:
	// a simple Iterator, traverse the collection in one direction
	class Iterator{
	public:
		Iterator(){}
		explicit Iterator(Elem *cur):_cur(cur) {}
		Elem& operator*();
		Elem* operator->();
		Iterator operator++(int);
		bool operator==(Iterator it);
		bool operator!=(Iterator it);
	private:
		Elem* _cur;
	};
	Iterator begin() const;
	Iterator end() const;

	// constructs empty Map
	Map();

	// copy constructor
	Map(const Map &rhs);

	// destructor
	~Map();

	// assignment operator
	Map& operator=(const Map &rhs);

	// insert an element into the Map; return true if successful
	bool insert(KEY, T);

	// remove an element from the Map; return true if successful
	bool erase(KEY);

	// return size of the Map
	int size() const;

	// return an iterator pointing to the end if the element is not found,
	// otherwise, return an iterator to the element
	Iterator find(KEY) const;

	// overloaded subscript operator
	T& operator[](KEY);

	// output the underlying BST
	ostream& dump(ostream& out) const;
};

template<typename KEY, typename T>
ostream& operator<< (ostream&, const Map<KEY, T>&);

#include "map.cpp"  // must include source file for template compilation
#endif
