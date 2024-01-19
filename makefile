all: zal

zal: main.cpp
	g++ -Wall -o program main.cpp -lncurses
