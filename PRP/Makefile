all: Visu_instance_prp Trans_Boudia_prp Trans_Dati_prp

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

Clean: rm *.o Visu_instance_prp Trans_Boudia_prp Trans_Dati_prp
