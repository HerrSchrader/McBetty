/* The 98LPCxx can store bytes in flash memory.
	We use this to store the address of this device
	See NXP appnote 10342 !
*/

#include <P89LPC932.h>               /* special function register declarations   */
                                    /* for the Philips P89C931 device           */

__sfr __at (0xE7) FMADRH;
__sfr __at (0xE6) FMADRL;
__sfr __at (0xE5) FMDATA;
__sfr __at (0xE4) FMCON;

#define LOAD 0x00; 					// define the load command
#define ERASEPROGRAM 0x68; 			// define the eraseprogram command
#define DATA_SIZE 1					// define size of the dataflash array
#define DATAFLASHSTART 0x0400 // define the startaddress of the array
#define LOWBYTE(address) ((unsigned char)(address))
#define HIGHBYTE(address) ((unsigned char)(((unsigned int)(address))>> 8))


/* This is the storage area 
	We initialize it with our default device address
*/
volatile __code unsigned char dataflash[DATA_SIZE]={0x01};

//***************************************************************************
//* flash_write_byte()
//* Input(s) : Memory address to write to.
//* Returns : bit, flash write succes or fail.
//* Description : write databyte to flash

//	NOTE disables interrupts while writing
//***************************************************************************

unsigned char 
flash_write(unsigned char index, char databyte)
{
	EA = 0;						// No interrupts please
	FMCON = LOAD;				// set up load command
	FMADRL = LOWBYTE(&(dataflash[index])); 			// set up low databyte
	FMADRH = HIGHBYTE(&(dataflash[index])); 		// set up high databyte
	FMDATA = databyte; 			// set up databyte
	FMCON = ERASEPROGRAM;		// erase program command
	EA = 1;						// interrupts enabled
	if (FMCON & 0x8F) 			// check for any error bits
		return 0;				// return fail on error

	return 1;					// return OK if operation was succesful

}

unsigned char
flash_read(unsigned char index){
	return dataflash[index];
}



