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

all: PRP1
	 #rm -f *.o
	 echo "Compilation terminée : ./LSP pour lancer"

%.o: %.cpp %.h
	$(CCC) -o $@ -c $(CCFLAGS) $(CCLNDIRS) $<

PRP1.o : PRP_1-16_CPLEX.cpp $(HDRS) $(OBJ)
	$(CCC) -o PRP2.o -c $(CCFLAGS) $(CCLNDIRS) PRP_1-16_CPLEX.cpp

PRP1: $(OBJ)
	$(CCC) $(CCFLAGS) $(CCLNDIRS) $(OBJ) -o PRP1 $(CCLNFLAGS)

clean:
	rm -rf *.o *.dot *.lp sol.txt *.pdf

mrproper: clean
	rm -rf $(EXEC)
