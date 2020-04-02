#!/bin/bash
rm -rf Server
g++ main.cpp Server.cpp -o Server.out -std=c++11 -lpthread
