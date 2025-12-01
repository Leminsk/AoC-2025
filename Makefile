MAIN_SOURCE = day01_0.cpp

all : $(MAIN_SOURCE)
	g++ $(MAIN_SOURCE) -LC:/msys64/mingw64/lib -lmingw32 -o main