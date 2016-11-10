CXXFLAGS = -O2 -Wall -std=c++11

default: all

all: hash hyperloglog

hash: hashlib.cpp hash.cpp
hyperloglog: hashlib.cpp hyperloglog.cpp

clean:
	$(RM) hash