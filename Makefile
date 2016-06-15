# File:          Makefile
# Author:        Logan Goldberg
# Created:       15 April 2015
# Simplified:    15 April 2015
# Updated:       27 April 2015
# Acknowledgement: adapted from an example by Henry M. Walker
#----------------------------------------------------------------------------
# Use the gcc compiler
CC = gcc

# Set preprocessor flags
CPPFLAGS=-I/home/walker/Myro/include/MyroC

# Set appropriate compiling flags
CFLAGS = -ansi -g -Wall -std=gnu99

# include the location of the MyroC shared library object file #
LIBRARY_PATH=$LIBRARY_PATH:/home/walker/Myro/lib/MyroC

# Set linker flags to include the relevant libraries
LDFLAGS=-L/home/walker/Myro/lib -lm -lMyroC -lbluetooth -ljpeg -leSpeakPackage 

#----------------------------------------------------------------------------
# build rules:
#
# Each rule takes the following form  (Note there MUST be a tab,
# as opposed to several spaces, preceeding each command.
#
# target_name:  dependency_list
#	command(s)

all: wordboard

# List program components, what they depend on, and how to compile each

wordboard: wordboard-main.c wordboard.c listStack.c
	$(CC) $(CFLAGS) -o wordboard wordboard.c listStack.c wordboard-main.c

wordboard-main.o: wordboard-main.c wordboard.h listStack.h
	$(CC) $(CFLAGS) -o wordboard-main.c wordboard.c

wordboard.o: wordboard.c wordboard.h listStack.h
	$(CC) $(CFLAGS) -o wordboard.c

listStack.o: listStack.c listStack.h
	$(CC) $(CFLAGS) -o listStack.c

#----------------------------------------------------------------------------
# cleanup rules: To invoke this command, type "make clean".
# Use this target to clean up your directory, deleting (without warning) 
#   object files, old emacs source versions, and core dumps.

clean:
	rm -f *.o *~ core*

clean-exe:
	rm -f wordboard
