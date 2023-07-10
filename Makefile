COMPILER          = gcc
FLAGS             = -Wall -pedantic -std=c99 -fsanitize=address
DEBUG_COMPILER    = -g
EXECUTABLE_MON    = bikeSharingMON
EXECUTABLE_NYC    = bikeSharingNYC
OBJECT_FILES_ADT  = bikeSharingADT.o
OBJECT_FILES_TABLE = htmlTable.o
OBJECT_FILES_MON  = bikeSharingMON.o
OBJECT_FILES_NYC  = bikeSharingNYC.o
QUERIES           = query1.csv query2.csv query3.csv query1.html query2.html query3.html

all: bikeSharingMON bikeSharingNYC

allNoObject: bikeSharingMON bikeSharingNYC cleanObjects

bikeSharingNYC: $(OBJECT_FILES_NYC) $(OBJECT_FILES_ADT) $(OBJECT_FILES_TABLE)
	$(COMPILER) -o $(EXECUTABLE_NYC) $(OBJECT_FILES_NYC) $(OBJECT_FILES_ADT) $(OBJECT_FILES_TABLE) $(FLAGS)

bikeSharingMON: $(OBJECT_FILES_MON) $(OBJECT_FILES_ADT) $(OBJECT_FILES_TABLE)
	$(COMPILER) -o $(EXECUTABLE_MON) $(OBJECT_FILES_MON) $(OBJECT_FILES_ADT) $(OBJECT_FILES_TABLE) $(FLAGS)

debug: FLAGS += $(DEBUG_COMPILER)
debug: all

bikeSharingMON.o: bikeSharingMON.c
	$(COMPILER) -c bikeSharingMON.c -o $(OBJECT_FILES_MON) $(FLAGS)

bikeSharingNYC.o: bikeSharingNYC.c
	$(COMPILER) -c bikeSharingNYC.c -o $(OBJECT_FILES_NYC) $(FLAGS)

bikeSharingADT.o: bikeSharingADT.c
	$(COMPILER) -c bikeSharingADT.c -o $(OBJECT_FILES_ADT) $(FLAGS)

htmlTable.o: htmlTable.c
	$(COMPILER) -c htmlTable.c -o $(OBJECT_FILES_TABLE) $(FLAGS)

deleteQueries:
	rm -rf $(QUERIES)

deleteObjects:
	rm -rf $(OBJECT_FILES_MON) $(OBJECT_FILES_NYC) $(OBJECT_FILES_ADT) $(OBJECT_FILES_TABLE)

deleteExecutables:
	rm -rf $(EXECUTABLE_MON) $(EXECUTABLE_NYC)

clean: deleteQueries deleteObjects deleteExecutables
