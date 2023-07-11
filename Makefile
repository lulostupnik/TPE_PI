COMPILER          = gcc
FLAGS             = -Wall -pedantic -std=c99 -fsanitize=address
DEBUG_COMPILER    = -g
EXECUTABLE_MON    = bikeSharingMON
EXECUTABLE_NYC    = bikeSharingNYC
QUERIES           = query1.csv query2.csv query3.csv query1.html query2.html query3.html

all: bikeSharingMON bikeSharingNYC

bikeSharingNYC: bikeSharingNYC.c bikeSharingADT.c htmlTable.c
	$(COMPILER) -o $(EXECUTABLE_NYC) bikeSharingNYC.c bikeSharingADT.c htmlTable.c $(FLAGS)

bikeSharingMON: bikeSharingMON.c bikeSharingADT.c htmlTable.c
	$(COMPILER) -o $(EXECUTABLE_MON) bikeSharingMON.c bikeSharingADT.c htmlTable.c $(FLAGS)

debug: FLAGS += $(DEBUG_COMPILER)
debug: all

deleteQueries:
	rm -rf $(QUERIES)

deleteExecutables:
	rm -rf $(EXECUTABLE_MON) $(EXECUTABLE_NYC)
	
clean: deleteQueries deleteExecutables
