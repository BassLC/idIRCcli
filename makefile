#OBJS specifies which files to compile as part of the project
OBJS=*.cpp

#CC specifies which compiler we're using
CC=clang++

#C++ version
VV=--std=c++14

# WARNINGS specifies all the warnings
WARNINGS=-Wall -Wextra -Wpedantic

#FLAGS specifies the flags wre passing
FLAGS= -pthreads -o 


#This is the target that compiles our executable
debug :
	$(CC) $(VV) $(WARNINGS) $(OBJS) -g $(FLAGS) debug 

main : 
	$(CC) $(VV) $(WARNINGS) $(OBJS) -O3 $(FLAGS) idIRCcli
