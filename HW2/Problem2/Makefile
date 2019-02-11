ifeq ($(PLATFORM),HOST)
	CC = gcc
	OP = file_basic_gcc
	SRC = file_basic.c
else
	CC= arm-linux-gcc
	OP = file_basic_arm
	SRC = file_basic.c
endif

all:
	$(CC) $(SRC) -o $(OP) 
