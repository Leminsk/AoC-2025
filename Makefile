MAIN_SOURCE = day01_0.cpp

all : $(MAIN_SOURCE)
	g++ $(MAIN_SOURCE) -lmingw32 -o main