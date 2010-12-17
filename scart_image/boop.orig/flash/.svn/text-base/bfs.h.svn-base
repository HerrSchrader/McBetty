/*
    bfs.h - Betty File System
    Copyright (C) 2007  Colibri <colibri_dvb@lycos.com>

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

#ifndef BFS_H
#define BFS_H

//Define here your file ids (range 1 - 0xFFFFFFFF)
#define BFS_ID_remotes		1
#define BFS_ID_settings		2
#define BFS_ID_RFsettings	3
#define BFS_ID_RAWslot0		4
#define BFS_ID_RAWslot1		5
#define BFS_ID_RAWslot2		6
#define BFS_ID_RAWslot3		7


//Use last 3 64 KByte sectors (16..18) of the second chip for BFS
#define BFS_FIRST_SECTOR	16
#define BFS_SECTORS			3
#define BFS_SECTOR_SIZE		0x10000

#define BFS_SECTOR_HEADER_LEN	0x10
#define BFS_MAGIC	"BFS"
#define BFS_VERSION	1
#define BFS_CHIP	1

#define BFS_TYPE_DELETED	0
#define BFS_TYPE_ACTIVE		1
#define BFS_TYPE_FREE		3
#define BFS_ID_1BYTE	0
#define BFS_ID_2BYTES	1
#define BFS_ID_3BYTES	2
#define BFS_ID_4BYTES	3
#define BFS_ONE_LEN_BYTE	0
#define BFS_TWO_LEN_BYTES	1

//Max. 1 info byte, 4 ID bytes, 2 length bytes
#define BFS_MAX_RECORD_HEADER_LEN	(1+4+2)

#define BFS_FLASH_BASE	FLASH1_BASE

unsigned long glBfsOffsetToFreeRecord[BFS_SECTORS];//Based on sector start
unsigned short glBfsDeletedBytes[BFS_SECTORS];//FileHeader + FileBody
unsigned char glBfsFillOrder[BFS_SECTORS];//[0]=Fullest

//Public
unsigned char BFS_Mount();//Call this before calling other BFS functions
unsigned short BFS_LoadFile(unsigned long Id, unsigned short MaxLen, unsigned char *Buffer);
unsigned char BFS_CmpFile(unsigned long Id, unsigned short MaxLen, unsigned char *Buffer);
void* BFS_LoadFileAddr(unsigned long Id);
unsigned char BFS_SaveFile(unsigned long Id, unsigned short Len, unsigned char *Buffer);
unsigned char BFS_DeleteFile(unsigned long Id);
unsigned char BFS_DeleteAllFiles();

//Internal
unsigned char BFS_FlashToFlashCopy(unsigned long DstByteAddr, unsigned long SrcByteAddr, unsigned short ByteLen);	
unsigned char BFS_MakeSpace(unsigned short RequiredFileBodySize);
void BFS_SortArray();
unsigned long BFS_GetPhyByteAddrFromBfsSector(unsigned char BfsSector);
unsigned char BFS_IsSectorFormated(unsigned char BfsSector);
void BFS_GetFileHeaderInfo(unsigned long FileHeaderAddress, unsigned char *pRecordInfo, unsigned long *pId, unsigned short *pLen, unsigned long *pFileBodyAddress, unsigned char *pIsFree, unsigned char *pIsActive);
unsigned char BFS_GetFileHeaderAddress(unsigned long Id, unsigned long *pFileHeaderAddress);
unsigned char BFS_ByteAddressToBfsSector(unsigned long FileHeaderAddress);
unsigned char BFS_WriteAtAddress(unsigned long FileHeaderAddress, unsigned long Id, unsigned short Len, unsigned char *Buffer, unsigned char *pHeaderSize);

unsigned char readByte(unsigned long ByteAddr);
unsigned short readWord(unsigned long WordAddr);

int writeBuffer(unsigned long ByteAddr, unsigned char *buffer, unsigned short len);

#endif
