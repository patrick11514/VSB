#!/bin/bash

#build library
g++ -o generator.o -c generator.cpp
ar  r libgenerator.a generator.o

#build source
g++ -o main main.cpp -L. -lgenerator
