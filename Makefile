COMPILER			=	gcc
FLAGS 				= 	-Wall -pedantic -std=c99 -fsanitize=address
DEBUG_COMPILER 		= 	-g
EXECUTABLE_MON		=	bikeSharingMon.c
EXECUTABLE_NYC		=	bikeSharingNYC.c
OBJECT_FILES_ADT	= 	adt.o
OBJECT_FILES_MON 	= 	testMON.o
OBJECT_FILES_NYC 	= 	testNYC.o
QUERIES 			= 	query1.csv query2.csv query3.csv query1.html query2.html query3.html

all: MON NYC
allNoObject: MON NYC cleanObjects
NYC: $(OBJECT_FILES_NYC) $(OBJECT_FILES_ADT)
	$(COMPILER) -o $(EXECUTABLE_NYC) $(OBJECT_FILES_NYC) $(OBJECT_FILES_ADT) $(FLAGS)
MON: $(OBJECT_FILES_MON) $(OBJECT_FILES_ADT)
	$(COMPILER) -o $(EXECUTABLE_MON) $(OBJECT_FILES_MON) $(OBJECT_FILES_ADT) $(FLAGS)
debug: COMPILER+=$(DEBUG_COMPILER)
debug: all
testMON.o: testMON.c
	$(COMPILER) -c testMON.c $(FLAGS)
testNYC.o: testNYC.c
	$(COMPILER) -c testNYC.c $(FLAGS)
adt.o: adt.c
	$(COMPILER) -c adt.c $(FLAGS)
cleanAll: cleanQueries cleanObjects cleanExecutable
cleanQueries:
	rm -rf $(QUERIES)
cleanObjects:
	rm -rf $(OBJECT_FILES_MON) $(OBJECT_FILES_NYC) $(OBJECT_FILES_ADT) 
cleanExecutable:
	rm -rf $(EXECUTABLE_MON) $(EXECUTABLE_NYC)