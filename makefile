ONE = one
TWO = two
CC = g++

PREF_ONE = ./first/
PREF_TWO = ./spu/
PREF_OBJ_ONE = ./obj/
PREF_OBJ_TWO = ./obj_two/
PREF_ALL_OBJ = ./all_obj/

ONE_CPP = $(wildcard $(PREF_ONE)*.cpp)
TWO_CPP = $(wildcard $(PREF_TWO)*.cpp)
ALL_CPP = $(wildcard *.cpp)
ONE_OBJ = $(patsubst $(PREF_ONE)%.cpp, $(PREF_OBJ_ONE)%.o, $(ONE_CPP))
TWO_OBJ = $(patsubst $(PREF_TWO)%.cpp, $(PREF_OBJ_TWO)%.o, $(TWO_CPP))
ALL_OBJ = $(patsubst %.cpp, $(PREF_ALL_OBJ)%.o, $(ALL_CPP))

$(ONE) : $(ONE_OBJ) $(ALL_OBJ)
	$(CC) $(ONE_OBJ) $(ALL_OBJ) -o $(ONE)

$(TWO) : $(TWO_OBJ) $(ALL_OBJ)
	$(CC) $(TWO_OBJ) $(ALL_OBJ) -o $(TWO)

$(PREF_OBJ_ONE)%.o : $(PREF_ONE)%.cpp
	$(CC) -c $< -o $@

$(PREF_OBJ_TWO)%.o : $(PREF_TWO)%.cpp
	$(CC) -c $< -o $@

$(PREF_ALL_OBJ)%.o : %.cpp
	$(CC) -c $< -o $@

cleanOne : 
	rm $(ONE) $(PREF_OBJ_ONE)*.o

cleanTwo : 
	rm $(TWO) $(PREF_OBJ_TWO)*.o