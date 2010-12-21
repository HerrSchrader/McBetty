/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define USE_SCART_ICP   1

#include "lpc2220.h"
#include "lcd.h"
#include "fonty.h"
#include "keyboard.h"

extern volatile unsigned int keys[2];

#if USE_SCART_ICP

#include "scart_icp.h"
#include "scart_image.h"



/* The portpins (P0.x) to use to connect to scart */
#define PIN_RST        (1<<14)   /* Portpin P0.14 to RST/P1.5 on Scart */
#define PIN_PDA         (1<<1)   /* Portpin P0.1  to PDA/P0.4 on Scart */
#define PIN_PCL         (1<<0)   /* Portpin P0.0  to PCL/P0.5 on Scart */

/* bit mask for port value and direction register */
#define PORT_MASK      (PIN_RST | PIN_PDA | PIN_PCL)
/* bit mask for pin function select register */
#define PINSEL_MASK    (((PIN_RST*PIN_RST) | (PIN_PDA*PIN_PDA) | (PIN_PCL*PIN_PCL)) * 3)


/* Definitions of the ICP commands/opcodes */
/* see http://www.zlgmcu.com/philips/yingrong/lpc900/LPC900_ICP_Spec_en.pdf */
#define OP_NOP           0x00   /* NOP */
#define OP_WR_FMADRL     0x08   /* Write address low command */
#define OP_RD_FMADRL     0x09   /* Read address low command */
#define OP_WR_FMADRH     0x0A   /* Write address high command */
#define OP_RD_FMADRH     0x0B   /* Read address high command */
#define OP_WR_FMCON      0x0E   /* Write a command to FMCON */
#define OP_RD_FMCON      0x0F   /* Read a command from FMCON */
#define OP_WR_FMDATA_PG  0x14   /* Write a command to FMDATA and increment address */
#define OP_RD_FMDATA_PG  0x15   /* Read a command from FMDATA and increment address */
#define OP_WR_FMDATA     0x0C   /* Write a command to FMDATA */
#define OP_RD_FMDATA     0x0D   /* Read a command from FMDATA */
#define OP_WR_FMDATA_I   0x04   /* Write a command to FMDATA and increment address */
#define OP_RD_FMDATA_I   0x05   /* Read a command from FMDATA and increment address */

#define CMD_LOAD         0x00   /* Clear and then load the page register */
#define CMD_PROG         0x48   /* Program page with page register command */
#define CMD_ERS_G        0x72   /* Erase global command */
#define CMD_ERS_S        0x71   /* Erase sector command */
#define CMD_ERS_P        0x70   /* Erase page command */
#define CMD_CONF         0x6C   /* Accesses user configuration information addressed by FMADRL */
#define CMD_CRC_G        0x1A   /* Calculate CRC on the entire user code command */
#define CMD_CRC_S        0x19   /* Calculate CRC on a sector command */
#define CMD_CCP          0x67   /* Clear configuration protection */

#define ADR_UCFG1        0x00   /* User Configuration Reg. #1 */
#define ADR_BootVector   0x02   /* Bootvector */
#define ADR_StatusByte   0x03   /* Status Byte */
#define ADR_SEC0         0x08   /* Security byte, sector 0 */
#define ADR_SEC1         0x09   /* Security byte, sector 1 */
#define ADR_SEC2         0x0A   /* Security byte, sector 2 */
#define ADR_SEC3         0x0B   /* Security byte, sector 3 */
#define ADR_SEC4         0x0C   /* Security byte, sector 4 */
#define ADR_SEC5         0x0D   /* Security byte, sector 5 */
#define ADR_SEC6         0x0E   /* Security byte, sector 6 */
#define ADR_SEC7         0x0F   /* Security byte, sector 7 */
#define ADR_MFGID        0x10   /* Manufacturer ID */
#define ADR_ID1          0x11   /* Device ID 1 */
#define ADR_ID2          0x12   /* Device ID 2 */


/* Scart's flash memory attributes */
#define PAGE_SIZE          64   /* flash page size of LPC931 */
#define SECTOR_SIZE      1024   /* flash sector size of LPC931 */


/* saved pin configuration for later restore */
static unsigned int savedPinsel0, savedPindir0, savedPin0;


/**************************************************************************/
/* icp_init()                                                             */
/*   Purpose: Initialize the portpins for use as ICP                      */
/*   Input:   -                                                           */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
static void icp_init()
{
    /* save the current pin configuration */
    savedPinsel0 = PINSEL0;
    savedPindir0 = FIODIR0;
    savedPin0    = FIOPIN0;
    
    /* setup all used pins as GPIO, output, low */
    PINSEL0 &= ~PINSEL_MASK;
    FIODIR0 |= (PIN_RST | PIN_PDA | PIN_PCL);
    FIOCLR0  = (PIN_RST | PIN_PDA | PIN_PCL);
}

/**************************************************************************/
/* icp_close()                                                            */
/*   Purpose: Configure the portpins as they was before used for ICP      */
/*   Input:   -                                                           */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
static void icp_close()
{
    /* restore the saved pin configuration */
    PINSEL0 = (PINSEL0 & ~PINSEL_MASK) | (savedPinsel0 & PINSEL_MASK);
    FIODIR0 = (FIODIR0 & ~PORT_MASK)   | (savedPindir0 & PORT_MASK);
    FIOPIN0 = (FIOPIN0 & ~PORT_MASK)   | (savedPin0    & PORT_MASK);
}

/**************************************************************************/
/* icp_wait_us()                                                          */
/*   Purpose: Wait some us                                                */
/*   Input:   time: the time to wait in us                                */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
static void icp_wait_us(int time)
{
    int start;
    
    /* use Timer0, which increments every 1us and rollover at T0MR0 */
    start = T0TC;
    if ((start + time) < T0MR0)
    {
        while (T0TC < (start + time));
    }
    else
    {
        while ((T0TC < (start + time - T0MR0)) || (T0TC >= start));
    }
}

/**************************************************************************/
/* icp_wait_ns()                                                          */
/*   Purpose: Wait some ns                                                */
/*   Input:   time: the time to wait in ns                                */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
static void icp_wait_ns(int time)
{
    int i;
    
    /* inaccurate, but okay */
    for (i=0; i<(time>>4); i++);
}

/**************************************************************************/
/* icp_mode_entry()                                                       */
/*   Purpose: Bring the scart's CPU into ICP mode                         */
/*   Input:   -                                                           */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
static void icp_mode_entry()
{
    int i;
    
    /* see ICP spec */
    for (i=0; i<8; i++)
    {
        FIOCLR0 = PIN_RST;
        icp_wait_us(10);
        FIOSET0 = PIN_RST;
        icp_wait_us(10);
    }
    icp_wait_us(50);
}

/**************************************************************************/
/* icp_mode_exit()                                                        */
/*   Purpose: Bring the scart's CPU out of ICP mode, reset CPU            */
/*   Input:   -                                                           */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
static void icp_mode_exit()
{
    FIOCLR0 = (PIN_RST | PIN_PDA | PIN_PCL);
}

/**************************************************************************/
/* icp_shift_out()                                                        */
/*   Purpose: Shift out one byte to scart                                 */
/*   Input:   val: byte to shift out                                      */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
static void icp_shift_out(unsigned char val)
{
    int i;
    
    for (i=0; i<8; i++)
    {
        FIOCLR0 = PIN_PCL;
        icp_wait_ns(50);
        if (val & 1)
        {
            FIOSET0 = PIN_PDA;
        }
        else
        {
            FIOCLR0 = PIN_PDA;
        }
        val >>= 1;
        icp_wait_ns(50);
        FIOSET0 = PIN_PCL;
        icp_wait_ns(100);
    }
    icp_wait_ns(100);
}

/**************************************************************************/
/* icp_shift_in()                                                         */
/*   Purpose: Shift in one byte from scart                                */
/*   Input:   -                                                           */
/*   Output:  -                                                           */
/*   Return:  byte shifted in                                             */
/**************************************************************************/
static unsigned char icp_shift_in()
{
    int i;
    unsigned char val;
    
    /* configure pin as input */
    FIODIR0 &= ~PIN_PDA;
    val = 0;
    for (i=0; i<8; i++)
    {
        FIOCLR0 = PIN_PCL;
        icp_wait_ns(100);
        val >>= 1;
        val |= ((FIOPIN0 & PIN_PDA) ? 0x80 : 0);
        FIOSET0 = PIN_PCL;
        icp_wait_ns(100);
    }
    /* configure pin as output */
    FIODIR0 |= PIN_PDA;
    icp_wait_ns(100);
    return val;
}

/**************************************************************************/
/* icp_read_config_byte()                                                 */
/*   Purpose: Read a configuration byte from scart                        */
/*   Input:   adr: address of the config byte                             */
/*   Output:  -                                                           */
/*   Return:  value of the config byte                                    */
/**************************************************************************/
static unsigned char icp_read_config_byte(unsigned char adr)
{
    icp_shift_out(OP_WR_FMCON);
    icp_shift_out(CMD_CONF);
    icp_shift_out(OP_WR_FMADRL);
    icp_shift_out(adr);
    icp_shift_out(OP_RD_FMDATA);
    return icp_shift_in();
}

/**************************************************************************/
/* icp_write_config_byte()                                                */
/*   Purpose: Write a configuration byte to scart                         */
/*   Input:   adr: address of the config byte                             */
/*            val: value of the config byte                               */
/*   Output:  -                                                           */
/*   Return:  0 on success, else error code                               */
/**************************************************************************/
static unsigned char icp_write_config_byte(unsigned char adr, unsigned char val)
{
    unsigned char status;
    
    icp_shift_out(OP_WR_FMCON);
    icp_shift_out(CMD_CONF);
    icp_shift_out(OP_WR_FMADRL);
    icp_shift_out(adr);
    icp_shift_out(OP_WR_FMDATA);
    icp_shift_out(val);
    
    do
    {
        icp_shift_out(OP_RD_FMCON);
        status = icp_shift_in();
    } while ((status & 0x8F) == 0x80);
    
    return (status & 0x8F);
}

/**************************************************************************/
/* icp_calc_sector_crc()                                                  */
/*   Purpose: Calculate the CRC of one sector                             */
/*   Input:   adr: start address of the sector                            */
/*   Output:  crc: value of the CRC                                       */
/*   Return:  0 on success, else error code                               */
/**************************************************************************/
static unsigned char icp_calc_sector_crc(unsigned short adr, unsigned int *pcrc)
{
    int i;
    unsigned char status;
    
    icp_shift_out(OP_WR_FMADRH);
    icp_shift_out(adr>>8);
    icp_shift_out(OP_WR_FMCON);
    icp_shift_out(CMD_CRC_S);
    
    do
    {
        icp_shift_out(OP_RD_FMCON);
        status = icp_shift_in();
    } while ((status & 0x8F) == 0x80);
    
    if (status & 0x8F) return (status & 0x8F);
    
    *pcrc = 0;
    for (i=0; i<4; i++)
    {
        icp_shift_out(OP_RD_FMDATA_I);
        *pcrc >>= 8;
        *pcrc |= (unsigned int)icp_shift_in() << 24;
    }
    return 0;
}

/**************************************************************************/
/* icp_load_page_register()                                               */
/*   Purpose: Load the page registers with bytes                          */
/*   Input:   adr:    start address of the bytes in the flash             */
/*            pval:   pointer to the bytes to load                        */
/*            nbytes: number of bytes to load                             */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
static void icp_load_page_register(unsigned short adr, const unsigned char *pval, unsigned char nbytes)
{
    int i;
    
    icp_shift_out(OP_WR_FMCON);
    icp_shift_out(CMD_LOAD);
    icp_shift_out(OP_WR_FMADRL);
    icp_shift_out(adr & 0xFF);
    icp_shift_out(OP_WR_FMDATA_PG);
    for (i=0; i<nbytes; i++)
    {
        icp_shift_out(pval[i]);
    }
}

/**************************************************************************/
/* icp_prog_page_register()                                               */
/*   Purpose: Program the page registers into the flash                   */
/*   Input:   adr:    start address of the bytes in the flash             */
/*   Output:  -                                                           */
/*   Return:  0 on success, else error code                               */
/**************************************************************************/
static unsigned char icp_prog_page_register(unsigned short adr)
{
    unsigned char status;
    
    icp_shift_out(OP_WR_FMADRL);
    icp_shift_out(adr & 0xFF);
    icp_shift_out(OP_WR_FMADRH);
    icp_shift_out(adr >> 8);
    icp_shift_out(OP_WR_FMCON);
    icp_shift_out(CMD_PROG);
    
    do
    {
        icp_shift_out(OP_RD_FMCON);
        status = icp_shift_in();
    } while ((status & 0x8F) == 0x80);
    
    return (status & 0x8F);
}

/**************************************************************************/
/* icp_erase_sector()                                                     */
/*   Purpose: Erase one flash sector                                      */
/*   Input:   adr:    start address of the sector in the flash            */
/*   Output:  -                                                           */
/*   Return:  0 on success, else error code                               */
/**************************************************************************/
static unsigned char icp_erase_sector(unsigned short adr)
{
    unsigned char status;
    
    icp_shift_out(OP_WR_FMADRH);
    icp_shift_out(adr >> 8);
    icp_shift_out(OP_WR_FMCON);
    icp_shift_out(CMD_ERS_S);
    
    do
    {
        icp_shift_out(OP_RD_FMCON);
        status = icp_shift_in();
    } while ((status & 0x8F) == 0x80);
    
    return (status & 0x8F);
}

void scart_icp_header(void) {
	set_font(BOLDFONT);
	draw_string(0, 0, "Scart ICP", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);
	set_font(SMALLFONT);
}

/**************************************************************************/
/* scart_icp()                                                            */
/*   Purpose: Use ICP to program an image into scart's flash              */
/*            and read some infos from scart                              */
/*   Input:   -                                                           */
/*   Output:  -                                                           */
/*   Return:  -                                                           */
/**************************************************************************/
void scart_icp(void)
{
    int i;
    unsigned short offs, addr, blklen;
    unsigned int err, crc_sc, crc_im, tap, poly, crcf;
    unsigned char data;
    
    
    set_font(SMALLFONT);
    
    lcd_fill(0);
	scart_icp_header();
    draw_string(0, 20, "Um Schaeden an der Betty", 3, DRAW_PUT);
    draw_string(0, 29, "und am Scart-Adapter zu", 3, DRAW_PUT);
    draw_string(0, 38, "vermeiden, folgen Sie", 3, DRAW_PUT);
    draw_string(0, 47, "bitte den Anweisungen!", 3, DRAW_PUT);
    draw_string(0, 56, "Entfernen Sie alle", 3, DRAW_PUT);
    draw_string(0, 65, "Kabel von der Betty!", 3, DRAW_PUT);
    draw_string(0, 74, "Druecken Sie dann OK.", 3, DRAW_PUT);
    draw_string(0, 83, "Oder druecken Sie EXIT", 3, DRAW_PUT);
    draw_string(0, 92, "um jetzt abzubrechen.", 3, DRAW_PUT);
    
    do
    {
        waitKeyUpDown();
    } while(!KEY_OK && !KEY_Exit);
    
    if (KEY_Exit) return;
    
    /* init port pins */
    icp_init();
    
    lcd_fill(0);
	scart_icp_header();
    draw_string(0, 20, "Trennen Sie zuerst die", 3, DRAW_PUT);
    draw_string(0, 29, "Stromversorgung des", 3, DRAW_PUT);
    draw_string(0, 38, "SCART-Adapters.", 3, DRAW_PUT);
    draw_string(0, 47, "Verbinden Sie danach die", 3, DRAW_PUT);
    draw_string(0, 56, "Betty mit dem SCART-", 3, DRAW_PUT);
    draw_string(0, 65, "Adapter. Schliessen Sie", 3, DRAW_PUT);
    draw_string(0, 74, "dann die Stromversorgung", 3, DRAW_PUT);
    draw_string(0, 83, "des SCART-Adapters", 3, DRAW_PUT);
    draw_string(0, 92, "wieder an.", 3, DRAW_PUT);
    draw_string(0, 101, "Druecken Sie dann OK.", 3, DRAW_PUT);
    
    do
    {
        waitKeyUpDown();
    } while(!KEY_OK);
    
    /* bring scart into ICP mode */
    icp_mode_entry();
    
    while (1)
    {
        lcd_fill(0);
		scart_icp_header();
        draw_string(0, 20, "Druecken Sie eine Taste", 3, DRAW_PUT);
        draw_string(0, 40, "1     Lese Config-Bytes", 3, DRAW_PUT);
        draw_string(0, 49, "2     Programmiere Image", 3, DRAW_PUT);
        draw_string(0, 58, "3     Vergleiche CRCs", 3, DRAW_PUT);
        draw_string(0, 75, "Exit Beenden", 3, DRAW_PUT);
        
        do
        {
            waitKeyUpDown();
        } while(!KEY_Exit && !KEY_1 && !KEY_2 && !KEY_3);
        
        lcd_fill(0);
		scart_icp_header();
        if (KEY_Exit) break;
        
        if (KEY_1)
        {   /* show all config bytes */
            draw_string(0, 14, "UCFG1", 3, DRAW_PUT);
            draw_hexC (60, 14, icp_read_config_byte(ADR_UCFG1), 3, DRAW_PUT);
            draw_string(0, 23, "BootVector", 3, DRAW_PUT);
            draw_hexC (60, 23, icp_read_config_byte(ADR_BootVector), 3, DRAW_PUT);
            draw_string(0, 32, "StatusByte", 3, DRAW_PUT);
            draw_hexC (60, 32, icp_read_config_byte(ADR_StatusByte), 3, DRAW_PUT);
            draw_string(0, 41, "SEC0", 3, DRAW_PUT);
            draw_hexC (60, 41, icp_read_config_byte(ADR_SEC0), 3, DRAW_PUT);
            draw_string(0, 50, "SEC1", 3, DRAW_PUT);
            draw_hexC (60, 50, icp_read_config_byte(ADR_SEC1), 3, DRAW_PUT);
            draw_string(0, 59, "SEC2", 3, DRAW_PUT);
            draw_hexC (60, 59, icp_read_config_byte(ADR_SEC2), 3, DRAW_PUT);
            draw_string(0, 68, "SEC3", 3, DRAW_PUT);
            draw_hexC (60, 68, icp_read_config_byte(ADR_SEC3), 3, DRAW_PUT);
            draw_string(0, 77, "SEC4", 3, DRAW_PUT);
            draw_hexC (60, 77, icp_read_config_byte(ADR_SEC4), 3, DRAW_PUT);
            draw_string(0, 86, "SEC5", 3, DRAW_PUT);
            draw_hexC (60, 86, icp_read_config_byte(ADR_SEC5), 3, DRAW_PUT);
            draw_string(0, 95, "SEC6", 3, DRAW_PUT);
            draw_hexC (60, 95, icp_read_config_byte(ADR_SEC6), 3, DRAW_PUT);
            draw_string(0, 104, "SEC7", 3, DRAW_PUT);
            draw_hexC (60, 104, icp_read_config_byte(ADR_SEC7), 3, DRAW_PUT);
            draw_string(0, 113, "MFGID", 3, DRAW_PUT);
            draw_hexC (60, 113, icp_read_config_byte(ADR_MFGID), 3, DRAW_PUT);
            draw_string(0, 122, "ID1", 3, DRAW_PUT);
            draw_hexC (60, 122, icp_read_config_byte(ADR_ID1), 3, DRAW_PUT);
            draw_string(0, 131, "ID2", 3, DRAW_PUT);
            draw_hexC (60, 131, icp_read_config_byte(ADR_ID2), 3, DRAW_PUT);
        }
        
        if (KEY_2)
        {   /* erase sectors and program image */
            draw_string(0, 20, "Loesche", 3, DRAW_PUT);
            
            /* erase all 8 sectors, including custom boot loader! */
            for (i=0; i<8; i++)
            {
                draw_block (0, 29, 128, 9, 3, DRAW_ERASE);
                draw_string(0, 29, "Sektor", 3, DRAW_PUT);
                draw_numU (40, 29, i, 0, 3, DRAW_PUT);
                err = icp_erase_sector(i*SECTOR_SIZE);
                if (err) break;
            }
            
            if (!err)
            {
                lcd_fill(0);
                draw_string(0, 20, "Programmiere", 3, DRAW_PUT);
            }
            
            offs = 0;
            addr = scart_image_startaddr;
            while (!err && (offs < sizeof(scart_image)))
            {
                blklen = (addr | (PAGE_SIZE-1)) + 1 - addr;
                if ((sizeof(scart_image) - offs) < blklen)
                {
                    blklen = sizeof(scart_image) - offs;
                }
                
                draw_block(0, 29, 128, 9, 3, DRAW_ERASE);
                draw_hexS (0, 29, addr, 3, DRAW_PUT);
                
                icp_load_page_register(addr, &scart_image[offs], blklen);
                err = icp_prog_page_register(addr);
                
                addr += blklen;
                offs += blklen;
            }
            
            if (!err)
            {
                draw_block(0, 29, 128, 9, 3, DRAW_ERASE);
                draw_string(0, 29, "BootVector", 3, DRAW_PUT);
                err = icp_write_config_byte(ADR_BootVector, scart_bootvec);
            }
            if (!err)
            {
                draw_block(0, 29, 128, 9, 3, DRAW_ERASE);
                draw_string(0, 29, "StatusByte", 3, DRAW_PUT);
                err = icp_write_config_byte(ADR_StatusByte, scart_bootstat);
            }
            if (err)
            {
                draw_string(0, 40, "Fehler", 3, DRAW_PUT);
                draw_hexC (40, 40, err, 3, DRAW_PUT);
            }
            else
            {
                draw_string(0, 40, "OK", 3, DRAW_PUT);
            }
        }
        
        if (KEY_3)
        {   /* compare CRC of all programmed sectors */
            draw_string(0, 20, "Vergleiche", 3, DRAW_PUT);
            err = 0;
            crc_sc = 0;
            offs = 0;
            addr = scart_image_startaddr;
            while (!err && (offs < sizeof(scart_image)))
            {
                blklen = (addr | (SECTOR_SIZE-1)) + 1 - addr;
                if ((sizeof(scart_image) - offs) < blklen)
                {
                    blklen = sizeof(scart_image) - offs;
                }
                
                draw_block (0, 29, 128, 9, 3, DRAW_ERASE);
                draw_string(0, 29, "Sektor @", 3, DRAW_PUT);
                draw_hexS (44, 29, addr, 3, DRAW_PUT);
                
                /* calculate CRC from scart */
                err = icp_calc_sector_crc(addr, &crc_sc);
                
                /* calculate CRC from image */
                poly = 0x00400007;
                crc_im = tap = 0;
                for (i=0; i<blklen; i++)
                {
                    crcf = crc_im & 0x80000000;
                    crc_im <<= 1;
                    data = scart_image[offs+i];
                    tap  = ((data & 0x80) << 11) | ((data & 0x40) << 10) | ((data & 0x20) << 8) | ((data & 0x10) << 6);
                    tap |= ((data & 0x08) << 5)  | ((data & 0x04) << 3)  | ((data & 0x02) << 2) | ((data & 0x01) << 0);
                    crc_im ^= tap;
                    if (crcf) crc_im ^= poly;
                }
                
                addr += blklen;
                offs += blklen;
                
                if (crc_sc != crc_im) break;
            }
            if (err || (crc_sc != crc_im))
            {
                draw_string(0, 40, "Fehler", 3, DRAW_PUT);
                draw_hexC (40, 40, err, 3, DRAW_PUT);
            }
            else
            {
                draw_string(0, 40, "OK", 3, DRAW_PUT);
            }
        }
        
        draw_string(0, 140, "Weiter mit OK.", 3, DRAW_PUT);
        
        do
        {
            waitKeyUpDown();
        } while(!KEY_OK);
    }
    
    icp_mode_exit();
    
    lcd_fill(0);
	scart_icp_header();
    draw_string(0, 20, "Trennen Sie nun zuerst die", 3, DRAW_PUT);
    draw_string(0, 29, "Stromversorgung des", 3, DRAW_PUT);
    draw_string(0, 38, "SCART-Adapters.", 3, DRAW_PUT);
    draw_string(0, 47, "Trennen Sie dann die", 3, DRAW_PUT);
    draw_string(0, 56, "Verbindung zwischen Betty", 3, DRAW_PUT);
    draw_string(0, 65, "und SCART-Adapter.", 3, DRAW_PUT);
    draw_string(0, 74, "Druecken Sie dann OK.", 3, DRAW_PUT);
    
    do
    {
        waitKeyUpDown();
    } while(!KEY_OK);
    
    icp_close();
}
#else
void scart_icp(void){
	lcd_fill(0);
	set_font(BOLDFONT);
	draw_string(0, 0, "Scart ICP", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);
	set_font(SMALLFONT);
	draw_string(0, 20, "Diese Funktion ist in dieser", 3, DRAW_PUT);
    draw_string(0, 29, "Boop-Version nicht", 3, DRAW_PUT);
	draw_string(0, 38, "enthalten.", 3, DRAW_PUT);
	do
    {
        waitKeyUpDown();
    } while(!KEY_OK && !KEY_Exit);
	
}
#endif
