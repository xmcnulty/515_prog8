CXX=g++

all: maptest1 maptest2 maptest3 maptest4 maptest5 maptest6 maptest7 maptest8


maptest1: map.h map.cpp maptest1.cpp
	$(CXX)  maptest1.cpp -o maptest1

maptest2: map.h map.cpp maptest2.cpp
	$(CXX)  maptest2.cpp -o maptest2

maptest3: map.h map.cpp maptest3.cpp
	$(CXX)  maptest3.cpp -o maptest3

maptest4: map.h map.cpp maptest4.cpp
	$(CXX)  maptest4.cpp -o maptest4

maptest5: map.h map.cpp maptest5.cpp
	$(CXX)  maptest5.cpp -o maptest5

maptest6: map.h map.cpp maptest6.cpp
	$(CXX)  maptest6.cpp -o maptest6

maptest7: map.h map.cpp maptest7.cpp
	$(CXX)  maptest7.cpp -o maptest7

maptest8: map.h map.cpp maptest8.cpp
	$(CXX)  maptest8.cpp -o maptest8

clean:
	rm -f *.o  maptest1 maptest2 maptest3 maptest4 maptest5 maptest6 maptest7 maptest8


