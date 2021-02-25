CC=g++
CFLAGS=-std=c++14
DEFINES=DEBUG_PARAMETERS
DEFINESTINY=TIXML_USE_STL

all:  tinyXML/tinyxml.o main.o tinyXML/tinyxmlerror.o tinyXML/tinyxmlparser.o tinyXML/tinystr.o
	$(CC) -g -o DecisionMakerLoadFunctions.out main.o tinyXML/tinyxml.o tinyXML/tinyxmlerror.o tinyXML/tinyxmlparser.o tinyXML/tinystr.o

main.o: main.cpp
	$(CC) -D$(DEFINESTINY) -D$(DEFINES) -g -c $(CFLAGS) main.cpp -o main.o


tinyXML/tinyxml.o: tinyXML/tinyxml.cpp
	$(CC) -D$(DEFINESTINY) -c tinyXML/tinyxml.cpp -o tinyXML/tinyxml.o

tinyXML/tinyxmlerror.o: tinyXML/tinyxmlerror.cpp
	$(CC) -D$(DEFINESTINY) -g -c tinyXML/tinyxmlerror.cpp -o tinyXML/tinyxmlerror.o

tinyXML/tinyxmlparser.o: tinyXML/tinyxmlparser.cpp 
	$(CC) -D$(DEFINESTINY) -g -c tinyXML/tinyxmlparser.cpp -o tinyXML/tinyxmlparser.o

tinyXML/tinystr.o: tinyXML/tinystr.cpp
	$(CC) -D$(DEFINESTINY) -g -c tinyXML/tinystr.cpp -o tinyXML/tinystr.o

clean:
	rm -f DecisionMakerLoadFunctions.out *.o *~
	-for d in ../; do (cd $$d; rm -f *.o *~ ); done
	-for d in ../data_structures; do (cd $$d; rm -f *.o *~ ); done
	-for d in ../tinyXML; do (cd $$d; rm -f *.o *~ ); done