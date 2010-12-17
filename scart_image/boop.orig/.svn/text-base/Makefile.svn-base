###############################################################
#####
##### Makefile for boop - OpenSource firmware for Betty
##### Created at 30.8.2007 02:26 am 
#####
##### boop V0.1 by netguy - ck@mamalala.net
##### Makefile V0.1 by alterego - alteregon@gmx.net
##### Makefile v0.2 by Tobias - tobias-betty@23.gs
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

ARMBASE = /opt/armtool/4.1.1
INCLUDEPATH = $(ARMBASE)/include
LIBPATH = $(ARMBASE)/lib/gcc/arm-elf/4.1.1/interwork
ARMPATH = $(ARMBASE)/bin
TOOLPREFIX = arm-elf-
LPCTOOL = lpctool

###############################################################
#####
##### Compiler, Linker and Tools
#####
###############################################################

CC = $(ARMPATH)/$(TOOLPREFIX)gcc
AS = $(ARMPATH)/$(TOOLPREFIX)as
LD = $(ARMPATH)/$(TOOLPREFIX)ld
OC = $(ARMPATH)/$(TOOLPREFIX)objcopy
OD = $(ARMPATH)/$(TOOLPREFIX)objdump

CPUFLAGS = -mcpu=arm7tdmi-s
OPTFLAGS = -Os
CFLAGS = -Wall -mthumb-interwork -msoft-float
INC = -I$(INCLUDEPATH) -I. -Iinterrupt -Idisplay -Ikeyboard -Iaudio -Iinfrared -Iserial -Iflash -Icc1100 -Igui -Itimer -Igames -Iadc -Irtc  -Itools
ASFLAGS = -D --gstabs -mthumb-interwork -mfpu=softfpa
LDFLAGS = -Tlpc2220_rom.ld -Map boop.map
LIBS = -lgcc
THUMBFLAGS = -mthumb

COMPILE = $(CC) $(CPUFLAGS) $(CFLAGS) $(INC)

-include Makefile.local

###############################################################
#####
##### Do the boop
#####
###############################################################

# Recursive expansion of Makefile rules.
define expand_dir
 # Reset vars for subdir for the case that Make.conf does not exist
 SUBDIRS :=
 SRCS :=
 THUMBSRCS :=
 THUMBSRCSUNOPT :=
 -include $(1)Make.conf
 ALLSRCS += $$(SRCS:%=$(1)%)
 ALLTHUMBSRCS += $$(THUMBSRCS:%=$(1)%)
 ALLTHUMBSRCSUNOPT += $$(THUMBSRCSUNOPT:%=$(1)%)
 DEPS += $(1).deps
 $$(foreach adir,$$(SUBDIRS),$$(eval $$(call expand_dir,$(1)$$(adir)/)))
endef

ALLSRCS :=
ALLTHUMBSRCS :=
ALLTHUMBSRCSUNOPT :=

$(eval $(call expand_dir,))

OBJS := $(patsubst %.s,%.o,$(ALLSRCS:.c=.o)) $(ALLTHUMBSRCS:.c=.thumb.o) $(ALLTHUMBSRCSUNOPT:.c=.thumbunopt.o)

all: $(DEPS) boop_rom.bin boop_rom.hex

test: boop_rom.elf
	$(OD) -h $<

%.bin: %.elf
	$(OC) -O binary $< $@

%.hex: %.elf
	$(OC) -O ihex $< $@

boop_rom.elf: $(OBJS)
	$(LD) $(LDFLAGS) -L$(LIBPATH) -o $@ $^ $(LIBS)

%.o: %.s
	$(AS) $(CPUFLAGS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(COMPILE) $(OPTFLAGS) -c -MMD -MF $(dir $<).deps/$(notdir $@) -o $@ $<

%.thumb.o: %.c
	$(COMPILE) $(THUMBFLAGS) $(OPTFLAGS) -c -MMD -MF $(dir $<).deps/$(notdir $@) -o $@ $<
	
%.thumbunopt.o: %.c
	$(COMPILE) $(THUMBFLAGS) -c -MMD -MF $(dir $<).deps/$(notdir $@) -o $@ $<

$(DEPS):
	mkdir -p $@

uresident: resident
resident: boop_rom.bin
	$(LPCTOOL) -i -v -e -a $<

clean:
	-rm -Rf $(DEPS)
	-rm -f $(OBJS) *.elf *.bin *.hex *~

-include $(DEPS:=/*)
