#! /usr/bin/python

import sys, random

def generate(m):
    print m
    for n in range(m): 
        print int(random.uniform(0, m)), int(random.uniform(0, m))

def help():
    print "Please provide a number"

if (__name__ == "__main__"):
    try:
        generate(int(sys.argv[1]))
    except:
        help();
     
            
            

