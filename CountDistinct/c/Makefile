CXXFLAGS = -O2 -Wall -std=c++11

default: all

all: histogram hash hyperloglog

histogram: hashlib.cpp histogram.cpp
hash: hashlib.cpp hash.cpp
hyperloglog: hashlib.cpp hyperloglog.cpp

clean:
	$(RM) histogram hash hyperloglog
