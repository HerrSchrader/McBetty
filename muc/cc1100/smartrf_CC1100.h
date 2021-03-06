/* Chipcon */
/* Product = CC1100 */
/* Chip version = F   (VERSION = 0x03) */
/* Crystal accuracy = 10 ppm */
/* X-tal frequency = 27 MHz */
/* RF output power = 7 dBm */
/* RX filterbandwidth = 105.468750 kHz */
/* Deviation = 20 kHz */
/* Datarate = 38.417816 kBaud */
/* Modulation = (0) 2-FSK */
/* Manchester enable = (0) Manchester disabled */
/* RF Frequency = 433.649185 MHz */
/* Channel spacing = 99.906921 kHz */
/* Channel number = 6 */
/* Optimization = - */
/* Sync mode = (3) 30/32 sync word bits detected */
/* Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX */
/* CRC operation = (1) CRC calculation in TX and CRC check in RX enabled */
/* Forward Error Correction = (0) FEC disabled */
/* Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word. */
/* Packetlength = 255 */
/* Preamble count = (3)  6 bytes */
/* Append status = 1 */
/* Address check = (2) Address check, 0 (0x00) broadcast */
/* FIFO autoflush = 1 */
/* Device address = 1 */
/* GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet */
/* GDO2 signal selection = (41) CHIP_RDY */
/***************************************************************
 *  SmartRF Studio(tm) Export
 *
 *  Radio register settings specifed with C-code
 *  compatible #define statements.
 *
 ***************************************************************/

#ifndef SMARTRF_CC1100_H
#define SMARTRF_CC1100_H

#define SMARTRF_RADIO_CC1100

#define SMARTRF_SETTING_FSCTRL1    0x08
#define SMARTRF_SETTING_FSCTRL0    0x00
#define SMARTRF_SETTING_FREQ2      0x10
#define SMARTRF_SETTING_FREQ1      0x09
#define SMARTRF_SETTING_FREQ0      0xF4
#define SMARTRF_SETTING_MDMCFG4    0xCA
#define SMARTRF_SETTING_MDMCFG3    0x75
#define SMARTRF_SETTING_MDMCFG2    0x83
#define SMARTRF_SETTING_MDMCFG1    0x31
#define SMARTRF_SETTING_MDMCFG0    0xE5
#define SMARTRF_SETTING_CHANNR     0x06
#define SMARTRF_SETTING_DEVIATN    0x34
#define SMARTRF_SETTING_FREND1     0x56
#define SMARTRF_SETTING_FREND0     0x10
#define SMARTRF_SETTING_MCSM0      0x18
#define SMARTRF_SETTING_FOCCFG     0x16
#define SMARTRF_SETTING_BSCFG      0x6C
#define SMARTRF_SETTING_AGCCTRL2   0x43
#define SMARTRF_SETTING_AGCCTRL1   0x40
#define SMARTRF_SETTING_AGCCTRL0   0x91
#define SMARTRF_SETTING_FSCAL3     0xE9
#define SMARTRF_SETTING_FSCAL2     0x2A
#define SMARTRF_SETTING_FSCAL1     0x00
#define SMARTRF_SETTING_FSCAL0     0x1F
#define SMARTRF_SETTING_FSTEST     0x59
#define SMARTRF_SETTING_TEST2      0x81
#define SMARTRF_SETTING_TEST1      0x35
#define SMARTRF_SETTING_TEST0      0x09
#define SMARTRF_SETTING_FIFOTHR    0x07
#define SMARTRF_SETTING_IOCFG2     0x29
#define SMARTRF_SETTING_IOCFG0D    0x06
#define SMARTRF_SETTING_PKTCTRL1   0x05
#define SMARTRF_SETTING_PKTCTRL0   0x05
#define SMARTRF_SETTING_ADDR       0x01
#define SMARTRF_SETTING_PKTLEN     0xFF

#endif

