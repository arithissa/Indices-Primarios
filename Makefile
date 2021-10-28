objects =   indicesprimarios.o main.o

all :   $(objects)
	gcc -o  alg $(objects)

run :   alg
	./alg

indicesprimarios.o :   indicesprimarios.h
main.o  :   indicesprimarios.h

clean   :
	rm *.o alg
