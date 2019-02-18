LDIR = $(HOME)/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220/
CDIR = $(HOME)/buildroot/output/host/bin/

obj-m +=k_animal_sort.o

all:
	make ARCH=arm CROSS_COMPILE=$(CDIR)arm-buildroot-linux-uclibcgnueabihf- -C $(LDIR) M=$(shell pwd) modules
clean:
	make -C $(LDIR) M=$(shell pwd) clean