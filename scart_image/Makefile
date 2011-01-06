###############################################################
#####
##### Makefile for betty_scart
##### Created at 30.8.2007 02:26 am 
#####
##### scart_terminal V0.1 by telekatz - telekatz@gmx.de
#####
###############################################################

###############################################################
#####
##### PATHS (default installation)
#####
##### You can put your path-config into Makefile.local 
##### to override these defaults
#####
###############################################################



SDCCBASE=F:/Tools/SDCC
INCLUDEPATH=$(SDCCBASE)/include
SDCCPATH=/usr/bin

PACKER   = $(SDCCPATH)/packihx
IMGMAKER = ./makebin
CC=$(SDCCPATH)/sdcc

-include Makefile.local

# List C source files here.
SRC = main.c serial.c cc1100.c isp.asm

                
# Define all object files.

# Object files needed when boot loader is included.
OBJ_BOOT = main.rel  serial.rel cc1100.rel isp.rel

# Object files needed without the boot loader.
OBJ = main.rel  serial.rel cc1100.rel 

# necessary binary tools which we create ourself
BIN = flashtool bin2c makebin 


# Compiler flags.
FLAGS = --iram-size 0x100
FLAGS += --model-small
FLAGS += --code-size 8192
FLAGS += -I$(INCLUDEPATH)


# Default target.
all: $(BIN) betty_flasher betty_scart.bin 

# Only create betty_scart.bin, not the boot loader stuff
scart: $(BIN) betty_scart.bin 

# --------------------------------------------------------------------------
# Creates firmware for betty to flash scart_image to scart adapter
# After using this target, issue "cd boop.orig && make resident"
# That will flash Betty with a boop image for flashing the scart adapter
betty_flasher: scart_image_boot.h
	cp scart_image_boot.h boop.orig/tools/scart_image.h
	cd boop.orig && make clean && make


# This file is only needed when we want to create a flashing firmware for Betty
# We then want to have a scart_image.h which includes the boot loader
# Usually only needed once to get the boot loader into the scart adapter.	
scart_image_boot.h: bin2c betty_scart_boot.bin
	./bin2c scart_image 8192 <betty_scart_boot.bin >scart_image_boot.h

# Generates a betty_scart_boot.bin which includes the boot loader
betty_scart_boot.bin: $(IMGMAKER) betty_scart_boot.ihx
	$(IMGMAKER) -s 8192  <betty_scart_boot.ihx >betty_scart_boot.bin

betty_scart_boot.ihx: $(OBJ_BOOT)
	$(CC) $(FLAGS) $(OBJ_BOOT) -o betty_scart_boot.ihx


# -------------------------------------------------------------------------
# This file is needed if we want to flash with the flashtool program 
# (and so we know a boot loader is already present on scart adapter)
scart_image.h: bin2c betty_scart.bin
	./bin2c scart_image 7680 <betty_scart.bin >scart_image.h
	
betty_scart.bin: $(IMGMAKER) betty_scart.ihx
	$(IMGMAKER) -s 7680 <betty_scart.ihx >betty_scart.bin

betty_scart.ihx: $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o betty_scart.ihx



# ------------------------------------------------------------------------
# Generating object files
main.rel: main.c
	$(CC) $(FLAGS) -c main.c

serial.rel: serial.c serial.h
	$(CC) $(FLAGS) -c serial.c

cc1100.rel: cc1100.c cc1100.h smartrf_CC1100.h
	$(CC) $(FLAGS) -c cc1100.c

isp.rel: isp.asm
	asx8051 -losg isp.asm
 

# -----------------------------------------------------------------------
# necessary binary tools

bin2c: bin2c.c
	gcc bin2c.c -o bin2c 

makebin: makebin.c
	gcc makebin.c -o makebin 

flashtool: flashtool.c scart_image.h
	gcc flashtool.c -o flashtool



# -------------------------------------------------------------------------
# Targets to flash the images via USB or serial

resident: flashtool
	./flashtool /dev/ttyUSB0 
	
resident_S0: flashtool
	./flashtool /dev/ttyS0 


# -------------------------------------------------------------------------

# Change this to the network name of your remote host where the scart adapter is connected
REMOTE_HOST = s100

# This target is used when we have the scart adapter connected to a remote host.
# Copies this complete directory over to REMOTE_HOST://root and flashes the adapter
remote: 
	scp -r ../scart_image root@${REMOTE_HOST}:/root/
	ssh  root@${REMOTE_HOST} "cd scart_image && make clean && make resident_S0"

# Copy this complete directory over to REMOTE_HOST://root and make the necessary files, but do not flash
remote_copy: 
	scp -r ../scart_image root@${REMOTE_HOST}:/root/
	ssh  root@${REMOTE_HOST} "cd scart_image && make clean && make scart"



clean:
	$(RM) -v ${OBJ} ${OBJ_BOOT} ${BIN} *.rel *.rst *.sym *.lst *.hex  *.mem *.map *.lnk *.hx *.ihx *.bin *~
