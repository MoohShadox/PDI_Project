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

all: main

PRP.o: PRP.cpp PRP.h
	g++ -c PRP.cpp

Visu_instance_prp.o: Visu_instance_prp.cpp PRP.h
	g++ -c Visu_instance_prp.cpp

Trans_Boudia_prp.o: Trans_Boudia_prp.cpp PRP.h
	g++ -c Trans_Boudia_prp.cpp

Trans_Dati_prp.o: Trans_Dati_prp.cpp PRP.h
	g++ -c Trans_Dati_prp.cpp

Visu_instance_prp: Visu_instance_prp.o PRP.o
	g++ -o Visu_instance_prp Visu_instance_prp.o PRP.o

Trans_Boudia_prp: Trans_Boudia_prp.o PRP.o
	g++ -o Trans_Boudia_prp Trans_Boudia_prp.o PRP.o

Trans_Dati_prp: Trans_Dati_prp.o PRP.o
	g++ -o Trans_Dati_prp Trans_Dati_prp.o PRP.o


main: main.o PRP.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) main.o PRP.o -o main $(CCLNFLAGS)

main.o : main.cpp PRP.h
	$(CCC) -o main.o -c $(CCFLAGS) $(CCLNDIRS) main.cpp 

