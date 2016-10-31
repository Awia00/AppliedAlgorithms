CC=g++
CFLAGS= -O2 -Wall -std=c++11

HEADERSKRUSKAL = graph.h disjointset.h disjointsetarray.h
KRUSKAL = mst-kruskal.cpp graph.cpp disjointset.cpp disjointsetarray.cpp

HEADERSPRIM = graph.h daryheap.h
PRIM = mst-prim.cpp graph.cpp daryheap.cpp

COMBINED = mst-combined.cpp graph.cpp daryheap.cpp disjointset.cpp disjointsetarray.cpp

CXXFLAGS = -O2 -Wall -std=c++11

default: prim

kruskal: $(HEADERSKRUSKAL) $(KRUSKAL)
	$(CC) $(CFLAGS) $(KRUSKAL) $(HEADERSKRUSKAL) -o kruskal
	
prim: $(PRIM) $(HEADERSPRIM)
	$(CXX) $(CXXFLAGS) $(PRIM) $(HEADERSPRIM) -o prim

combined: $(COMBINED) $(HEADERSPRIM) $(HEADERSKRUSKAL)
	$(CXX) $(CXXFLAGS) $(COMBINED) $(HEADERSPRIM) $(HEADERSKRUSKAL) -o combined

clean:
	$(RM) kruskal prim combined *.o *.out *.gch

