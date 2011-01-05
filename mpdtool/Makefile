###############################################################
#####
##### Makefile for mpdtool 
#####
###############################################################

###############################################################
#####
##### PATHS (default installation)
#####
###############################################################


INSTALLDIR_BIN=/home/music/bin


###############################################################
#####
##### Compiler, Linker and Tools
#####
###############################################################



###############################################################
#####
##### Do the whole shabang
#####
###############################################################

all: mpdtool


mpdtool: mpdtool.c  
	gcc -O2 -Wall mpdtool.c -o mpdtool

install: mpdtool
	cp mpdtool $(INSTALLDIR_BIN)

uninstall:
	rm $(INSTALLDIR_BIN)/mpdtool
	
	
REMOTE_HOST=s101
PRODUCTION_HOST=s100
# Copy this complete directory over to REMOTE_HOST://root and make the necessary files
remote: 
	scp -r ../mpdtool root@${REMOTE_HOST}:/root/
	ssh  root@${REMOTE_HOST} "cd mpdtool && make clean && make"

production: 
	scp -r ../mpdtool root@${PRODUCTION_HOST}:/root/
	ssh  root@${PRODUCTION_HOST} "cd mpdtool && make clean && make"

clean:
	rm -f *.o  *~ mpdtool 

### EOF
