/* Definitions for bits and bytes of cc1100 */

#ifndef CC1100_DEFS_H
#define CC1100_DEFS_H

#define SCK1			(1<<17)
#define MISO1			(1<<18)
#define MOSI1			(1<<19)

// SPI Transmit fifo empty
#define TFE	(1<<0)

// SPI Transmit fifo not full
#define TNF	(1<<1)

// SPI Receive fifo not empty
#define RNE 	(1<<2)

// SPI Receive fifo full
#define RFF	(1<<3)

// is <> 0 if SPI Receive fifo not empty
#define SPI_RNE (SSPSR & RNE)

// is <> 0 if SPI Receive fifo not full
#define SPI_TNF (SSPSR & TNF)

// is <> 0 if SPI is busy
#define SPI_BSY (SSPSR & (1<<4))

#define TXIM	(1<<3)

#define CS1			(1<<23)

#define WRITE			0x00
#define BURST			0x40
#define READ			0x80
#define TX_fifo			0x7F
#define RX_fifo			0xff

#define MAX_PKTLEN		0x3D

// status register of the CC1100
#define MARCSTATE		0x35
#define PKTSTATUS		0x38 
#define RXBYTES			0x3B
#define TXBYTES			0x3A

// Configuration registers of the CC1100
#define CC1100_ADDR	0x09
#define MCSM2		0x16
#define MCSM1		0x17
#define MCSM0		0x18
#define IOCFG2		0x00        // GDO2 output pin configuration
#define IOCFG1		0x01        // GDO1 output pin configuration
#define IOCFG0		0x02        // GDO0 output pin configuration
#define FIFOTHR		0x03        // RX FIFO and TX FIFO thresholds

#define LQI_CRC_OK_BM	0x80

// status register contents
#define MARCSTATE_IDLE		0x01


#define RX_OK                0
#define RX_LENGTH_VIOLATION  1
#define RX_CRC_MISMATCH      2
#define RX_FIFO_OVERFLOW     3

// commands
#define SRES			0x30
#define SFSTXON			0x31
#define SXOFF			0x32
#define SCAL			0x33
#define SRX				0x34
#define STX 			0x35
#define SIDLE			0x36
#define SWOR			0x38
#define SPWD			0x39
#define SFRX			0x3A
#define SFTX			0x3B
#define SWORRST			0x3C
#define SNOP			0x3D

// PCONP bits
#define PCSPI1	10
#define PCSSP	21

#define EINT0 (1<<0)
#define EINT1 (1<<1)
#define EINT2 (1<<2)
#define EINT3 (1<<3)

#endif
