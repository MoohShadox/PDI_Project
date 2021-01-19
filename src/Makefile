#------------------------------------------------------------
#
# Please define an environnement variable PATHTUTOMIP
# equal to the XXX directory where Lemon,Graphviz and Cplex have been installed
# In order to these:
# edit ~/.bashrc add "export PATHTUTOMIP=XXX"
# close your xterm and star new ones
#
#------------------------------------------------------------

CPLEXDIR      = $(PATHTUTOMIP)/cplex-12.7.1/cplex
CONCERTDIR    = $(PATHTUTOMIP)/cplex-12.7.1/concert
LEMONDIR      = $(PATHTUTOMIP)/lemon-1.3.1


SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic


# ---------------------------------------------------------------------
# Compiler selection
# ---------------------------------------------------------------------

CCC = g++ -O0


# ---------------------------------------------------------------------
# Compiler options
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -Wno-ignored-attributes

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
LEMONLIBDIR	= $(LEMONDIR)/lib

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) -L$(LEMONLIBDIR)


CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
LEMONINCDIR   = $(LEMONDIR)/include

CCLNFLAGS = -lconcert -lilocplex -lcplex -lemon -lm -lpthread

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -I$(LEMONINCDIR)

SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
HDRS = $(SRC:.cpp=.h)

all: LSP CVRP PRP
	 #rm -f *.o
	 echo "Compilation terminée : ./LSP pour lancer"


Unit_Tests/test.o : Unit_Tests/Test_LSP.cpp $(HDRS) $(OBJ)
	$(CCC) -o Unit_Tests/test.o -c Unit_Tests/Test_LSP.cpp $(CCFLAGS) $(CCLNDIRS)

Unit_Tests/test : $(OBJ) Unit_Tests/test.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o Unit_Tests/test Unit_Tests/test.o $(CCLNFLAGS)
	"Compilation des tests terminées : Unit_Tests/test pour lancer"


%.o: %.cpp %.h
	$(CCC) -o $@ -c $(CCFLAGS) $(CCLNDIRS) $<


CVRP.o : cvrp_algorithms.cpp $(HDRS) $(OBJ)
	$(CCC) -o $(OBJ) -c $(CCFLAGS) $(CCLNDIRS) cvrp_algorithms.cpp

CVRP: $(OBJ)
	$(CCC) $(CCFLAGS) $(CCLNDIRS) $(OBJ) -o CVRP $(CCLNFLAGS)

LSP.o : LSP_Solver.cpp $(HDRS) $(OBJ)
	$(CCC) -o LSP.o -c $(CCFLAGS) $(CCLNDIRS) LSP_Solver.cpp

LSP: $(OBJ)
	$(CCC) $(CCFLAGS) $(CCLNDIRS) $(OBJ) -o LSP $(CCLNFLAGS)


PRP.o : PRP_1-16_CPLEX.cpp $(HDRS) $(OBJ)
	$(CCC) -o PRP.o -c $(CCFLAGS) $(CCLNDIRS) PRP_1-16_CPLEX.cpp

PRP: $(OBJ)
	$(CCC) $(CCFLAGS) $(CCLNDIRS) $(OBJ) -o PRP $(CCLNFLAGS)

clean:
	rm -rf *.o *.dot *.lp sol.txt *.pdf

mrproper: clean
	rm -rf $(EXEC)
