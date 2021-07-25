# Project: Project1
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++
CC   = gcc
RES  = 
OBJ  = algorithm.o datastructures.o GeneString.o MutSolver.o SamplesBySite.o simulator.o $(RES)
LINKOBJ  = algorithm.o datastructures.o GeneString.o MutSolver.o SamplesBySite.o simulator.o $(RES)
#INCS =  -I"C:/Dev-Cpp/include"  -I"D:/code/rho-infer/boost_1_47_0" 
#CXXINCS =  -I"C:/Dev-Cpp/lib/gcc/mingw32/3.4.2/include"  -I"C:/Dev-Cpp/include/c++/3.4.2/backward"  -I"C:/Dev-Cpp/include/c++/3.4.2/mingw32"  -I"C:/Dev-Cpp/include/c++/3.4.2"  -I"C:/Dev-Cpp/include"  -I"D:/code/rho-infer/boost_1_47_0" 
BIN  = sc-sample
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

algorithm.o: algorithm.cpp
	$(CPP) -c algorithm.cpp -o algorithm.o $(CXXFLAGS)

datastructures.o: datastructures.cpp
	$(CPP) -c datastructures.cpp -o datastructures.o $(CXXFLAGS)

GeneString.o: GeneString.cpp
	$(CPP) -c GeneString.cpp -o GeneString.o $(CXXFLAGS)

MutSolver.o: MutSolver.cpp
	$(CPP) -c MutSolver.cpp -o MutSolver.o $(CXXFLAGS)

SamplesBySite.o: SamplesBySite.cpp
	$(CPP) -c SamplesBySite.cpp -o SamplesBySite.o $(CXXFLAGS)

simulator.o: simulator.cpp
	$(CPP) -c simulator.cpp -o simulator.o $(CXXFLAGS)
