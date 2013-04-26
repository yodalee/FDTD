#################################
# MACRO DEFINE
#################################

CUDA_PATH	?= /opt/cuda
CUDA_LIB	?= $(CUDA_PATH)/lib
CUDA_INC	?= $(CUDA_PATH)/include
CUDA_BIN	?= $(CUDA_PATH)/bin
CC			= gcc
CXX			= g++
NVCC		= $(CUDA_BIN)/nvcc
CXXFLAGS	= -O2 -Wall
CXXFLAGS 	= -g -Wall
CULDFLAGS   = -m32
INCLUDE 	= -I$(CUDA_INC)
CUINCLUDE   = 
TLIB 		= -L$(CUDA_LIB) -lcuda -lcudart

#################################
# suffix rule
# c++ suffixes and c suffies fule
#################################

.SUFFIXES: .cpp
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -c

.cpp :
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ -lm $(TLIB) -lg++

#################################
# File Dependencies
#################################
GOAL = FDTD
SRC = main.cpp FDTD.cpp source.cpp mesh.cpp gnuplot.cpp displayer.cpp
OBJ = $(addsuffix .o, $(basename $(SRC)))
$(GOAL): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(TLIB) -o $@ $(OBJ)

#################################
# Other function 
#################################
depend:
	makedepend $(SRC)

clean:
	rm -f $(OBJ) $(GOAL)
