#################################
# MACRO DEFINE
#################################

CUDA_PATH	?= /opt/cuda
CUDA_LIB	?= $(CUDA_PATH)/lib64
CUDA_INC	?= $(CUDA_PATH)/include
CUDA_BIN	?= $(CUDA_PATH)/bin
CC			= gcc
CXX			= g++
NVCC		= $(CUDA_BIN)/nvcc
CXXFLAGS	= -O2 -Wall
CXXFLAGS 	= -g -Wall
CULDFLAGS   = -m64
GENCODE_SM10    := -gencode arch=compute_10,code=sm_10
GENCODE_SM20    := -gencode arch=compute_20,code=sm_20
GENCODE_SM30    := -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35
GENCODE_FLAGS   := $(GENCODE_SM30)
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
$(GOAL): $(OBJ) solver.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(TLIB) -o $@ $(OBJ) solver.o

solver.o: solver.cu
	$(NVCC) $(CULDFLAGS) $(GENCODE_FLAGS) $(CUINCLUDE) -o $@ $< -c

#################################
# Other function 
#################################
depend:
	makedepend $(SRC)

clean:
	rm -f $(OBJ) $(GOAL)
