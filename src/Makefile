#################################
# MACRO DEFINE
#################################

CC			= gcc
CXX			= g++
CXXFLAGS 	= -g -Wall
#CXXFLAGS	= -O2 -Wall
INCLUDE 	= 
TLIB 		= 

#################################
# suffix rule
# c++ suffixes and c suffies fule
#################################

.SUFFIXES: .cpp
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

.cpp :
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ -lm $(TLIB) -lg++

#################################
# File Dependencies
#################################
GOAL = FDTD
SRC = main.cpp FDTD.cpp source.cpp mesh.cpp gnuplot.cpp
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
