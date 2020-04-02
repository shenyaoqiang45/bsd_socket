#!/bin/bash
rm -rf Client
g++ main.cpp Client.cpp -o Client.out -std=c++11 -lpthread
