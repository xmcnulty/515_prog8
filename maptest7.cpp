// tests cases for map::insert() method and iterator

//#include "map.h"
#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include <utility>

using namespace std;

int main(){
	map<int, int> m1;
	//map<int, int>::iterator it;

    for(int i=1; i<=10000000; i++) {
		m1.insert(pair<int, int>(i,0));
        
//        if (i % 10000 == 0) {
//            cout << "Inserting " << i << endl;
//        }
    }
    
    cout << "Done inserting" << endl;
//	it = m1.begin();
//	int key = 1; 
//	while(it != m1.end()){
//		assert(*it== key);
//		it++;
//		key++;
//	}
}




