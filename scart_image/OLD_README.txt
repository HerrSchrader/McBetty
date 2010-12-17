To run the programm the following µC settings has to be changend:

(with Betty-Heaven -> Scart -> Konfiguration)

UCFG1:       07
UCFG2:       FF
Boot Vector: 00
Status Byte: 00


If you use the SCART-Betty-Kombiinterface remove R2, otherwise the programm will not work!

Serial settings: 38400,n,8,1, no handshake


Commands:


Terminal Mode:
==============

t<data>                                 transmitt <data>
s                                       show cc1100 status bytes
c                                       show cc1100 configuration registers
b                                       switch to binary mode
w                                       send WOR packet

Binary mode:
============
 
0x00                                    NOP
0x01 <n> <data_1> ... <data_n>          transmitt n Bytes (n<=61)
0x02 <cmd>                              send command strobe to cc1100, get status byte
0x03 <addr>                             read cc1100 single byte
0x04 <addr> <value>                     write cc1100 single byte
0x05 <n> <addr>                         read n bytes cc1100 burst mode
0x06 <n> <addr> <data_1> ... <data_n>   write n bytes cc1100 burst mode
0x07 <addr>                             send WOR packet

Return value

0x00                                    syntax error
0x01                                    ready
0x02 <n> <data_1> ... <data_n>          n bytes data received	

