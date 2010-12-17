/*
    bfs.c - Betty File System
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

#include "bfs.h"
#include "flash.h"
#include "lcd.h"
#include "fonty.h"
#include "global.h"

extern const unsigned long secaddr[19];

//Public functions
unsigned char BFS_Mount()
{
	//Return true=ok
	unsigned char BfsSector;
	unsigned long Offset;
	unsigned long FileHeaderAddress;
	unsigned char RecordInfo;
	unsigned long Id;
	unsigned short Len;
	unsigned long FileBodyAddress;
	unsigned char IsFree;
	unsigned char IsActive;
	const unsigned long BfsVersion = BFS_VERSION;

	//Format
	for(BfsSector=0; BfsSector<BFS_SECTORS; BfsSector++)
	{
		//lcd_fill(0);//Clr screen
		//draw_string(0, 70, "init file system", 3, DRAW_PUT);
		//draw_string(0, 80,"BfsSektor:", 3, DRAW_PUT);
		//draw_hexC(60,80,BfsSector, 3, DRAW_PUT);

		if(!BFS_IsSectorFormated(BfsSector))
		{
			//Unformated sector
			//Erase sector
			if(eraseSector(BFS_CHIP, BFS_FIRST_SECTOR + BfsSector) != 0)
			{
				return false;//Error
			}
			//Write sector header
			Offset = BFS_GetPhyByteAddrFromBfsSector(BfsSector);
			writeBuffer(Offset, (unsigned char*)BFS_MAGIC, 4);
			writeBuffer(Offset + 4, (unsigned char*)&BfsVersion, 4);
		}
	}

	for(BfsSector=0; BfsSector<BFS_SECTORS; BfsSector++)
	{
		glBfsDeletedBytes[BfsSector] = 0;
		glBfsOffsetToFreeRecord[BfsSector] = 0;

		FileHeaderAddress = BFS_SECTOR_HEADER_LEN + BFS_GetPhyByteAddrFromBfsSector(BfsSector);
	
		do
		{
			BFS_GetFileHeaderInfo(FileHeaderAddress, &RecordInfo, &Id, &Len, &FileBodyAddress, &IsFree, &IsActive);
			if(IsFree)
			{
				glBfsOffsetToFreeRecord[BfsSector] = FileHeaderAddress - BFS_GetPhyByteAddrFromBfsSector(BfsSector);
			}
			else
			{
				if(!IsActive)
				{
					//Deleted
					glBfsDeletedBytes[BfsSector] += (FileBodyAddress - FileHeaderAddress) + Len;
				}
				FileHeaderAddress = FileBodyAddress + Len;
			}
		}while(!IsFree);
	}

	//Init fill order
	for(BfsSector=0; BfsSector<BFS_SECTORS; BfsSector++)
	{
		glBfsFillOrder[BfsSector] = BfsSector;
	}
	//Sort fill order
	BFS_SortArray();

	//Free sector number
	if(glBfsOffsetToFreeRecord[glBfsFillOrder[BFS_SECTORS-1]] > BFS_SECTOR_HEADER_LEN)
	{
		//No free sector
		//ASSERT(false);
		return false;//Error
	}

	return true;//OK
}

unsigned short BFS_LoadFile(unsigned long Id, unsigned short MaxLen, unsigned char *Buffer)
{
	//Return: ReadLen
	unsigned long FileHeaderAddress;
	unsigned char RecordInfo;
	unsigned long TempId;
	unsigned short FileBodyLen;
	unsigned long FileBodyAddress;
	unsigned char IsFree;
	unsigned char IsActive;

	if(!BFS_GetFileHeaderAddress(Id, &FileHeaderAddress))
	{
		return 0;//File not found
	}
	BFS_GetFileHeaderInfo(FileHeaderAddress, &RecordInfo, &TempId, &FileBodyLen, &FileBodyAddress, &IsFree, &IsActive);
	memcpy(Buffer, (unsigned char*)FileBodyAddress, min(FileBodyLen, MaxLen));

	return min(FileBodyLen, MaxLen);//Error
}

unsigned char BFS_CmpFile(unsigned long Id, unsigned short MaxLen, unsigned char *Buffer)
{
	//Return: ReadLen
	unsigned long FileHeaderAddress;
	unsigned char RecordInfo;
	unsigned long TempId;
	unsigned short FileBodyLen;
	unsigned long FileBodyAddress;
	unsigned char IsFree;
	unsigned char IsActive;
	int x;

	if(!BFS_GetFileHeaderAddress(Id, &FileHeaderAddress))
	{
		return 0;//File not found
	}
	BFS_GetFileHeaderInfo(FileHeaderAddress, &RecordInfo, &TempId, &FileBodyLen, &FileBodyAddress, &IsFree, &IsActive);
	x=memcmp(Buffer, (unsigned char*)FileBodyAddress, min(FileBodyLen, MaxLen));
	
	if (x)
		return 0;//not equal
	return 1;
}

void* BFS_LoadFileAddr(unsigned long Id)
{
	//Return: ReadLen
	unsigned long FileHeaderAddress;
	unsigned char RecordInfo;
	unsigned long TempId;
	unsigned short FileBodyLen;
	unsigned long FileBodyAddress;
	unsigned char IsFree;
	unsigned char IsActive;

	if(!BFS_GetFileHeaderAddress(Id, &FileHeaderAddress))
	{
		return 0;//File not found
	}
	BFS_GetFileHeaderInfo(FileHeaderAddress, &RecordInfo, &TempId, &FileBodyLen, &FileBodyAddress, &IsFree, &IsActive);
	
	return (void*)FileBodyAddress;

}

unsigned char BFS_SaveFile(unsigned long Id, unsigned short Len, unsigned char *Buffer)
{
	//Return true=OK
	unsigned char BfsSector;
	unsigned char RecordInfo;
	int i;
	unsigned long TempId;
	unsigned short FileBodyLen;
	unsigned long FileBodyAddress;
	unsigned char IsFree;
	unsigned char IsActive;
	unsigned char OverridePossible;
	unsigned char CreateNewFile;
	unsigned char Byte;
	unsigned long FileHeaderAddress;
	unsigned char HeaderSize;

	CreateNewFile = false;
	if(BFS_GetFileHeaderAddress(Id, &FileHeaderAddress))
	{
		//File is already present
		BFS_GetFileHeaderInfo(FileHeaderAddress, &RecordInfo, &TempId, &FileBodyLen, &FileBodyAddress, &IsFree, &IsActive);
		//Same len?
		OverridePossible = false;
		if(Len == FileBodyLen)
		{
			//Is override possible (no change from 0 to 1)?
			OverridePossible = true;
			for(i=0; i<Len; i++)
			{
				if(((unsigned char*)Buffer)[i] & (0xFF ^ readByte(FileBodyAddress+i)))
				{
					//0->1 error
					OverridePossible = false;
				}			
			}
		}
		if(OverridePossible)
		{
			if(writeBuffer(FileBodyAddress, Buffer, Len) != 0)
			{
				return false;//Error
			}
		}
		else
		{
			//Delete existing file
			Byte = (RecordInfo & 0x3F) | (BFS_TYPE_DELETED << 6);
			writeBuffer(FileHeaderAddress, &Byte, 1);
			glBfsDeletedBytes[BFS_ByteAddressToBfsSector(FileHeaderAddress)] += (FileBodyAddress - FileHeaderAddress) + FileBodyLen;

			CreateNewFile = true;
		}
	}
	else
	{
		//File not present
		CreateNewFile = true;
	}

	while(CreateNewFile)
	{
		//Check free space
		//Start with the fullest, ignore the last empty one
		for(i=0; (i<(BFS_SECTORS-1)) && CreateNewFile; i++)
		{
			BfsSector = glBfsFillOrder[i];
			FileHeaderAddress = glBfsOffsetToFreeRecord[BfsSector];
			if((FileHeaderAddress + BFS_MAX_RECORD_HEADER_LEN + Len) < BFS_SECTOR_SIZE)
			{
				//Enough space free
				BFS_WriteAtAddress(BFS_GetPhyByteAddrFromBfsSector(BfsSector) + FileHeaderAddress, Id, Len, Buffer, &HeaderSize);
				
				glBfsOffsetToFreeRecord[BfsSector] += HeaderSize + Len;
				BFS_SortArray();
				
				CreateNewFile = false;
			}
		}	

		if(CreateNewFile)
		{
			//No space free
			//copy only active files (not the files markes as deleted) to the empty sector and erase the source sector
			if(!BFS_MakeSpace(Len))
			{
				//Still not enough space free
				return false;//Error
			}
		}
	}

	return true;//OK
}

unsigned char BFS_DeleteFile(unsigned long Id)
{
	//Return true=OK
	//Return false=File not found/File was already deleted

	unsigned char RecordInfo;
	unsigned long TempId;
	unsigned short FileBodyLen;
	unsigned long FileBodyAddress;
	unsigned char IsFree;
	unsigned char IsActive;
	unsigned char Byte;
	unsigned long FileHeaderAddress;

	if(BFS_GetFileHeaderAddress(Id, &FileHeaderAddress))
	{
		//File is present
		BFS_GetFileHeaderInfo(FileHeaderAddress, &RecordInfo, &TempId, &FileBodyLen, &FileBodyAddress, &IsFree, &IsActive);

		//Delete existing file
		Byte = (RecordInfo & 0x3F) | (BFS_TYPE_DELETED << 6);
		writeBuffer(FileHeaderAddress, &Byte, 1);

		glBfsDeletedBytes[BFS_ByteAddressToBfsSector(FileHeaderAddress)] += (FileBodyAddress - FileHeaderAddress) + FileBodyLen;
		return true;//OK
	}

	return false;//File not found/File was already deleted
}

unsigned char BFS_DeleteAllFiles()
{
	//Return true=ok
	unsigned char BfsSector;
	unsigned long Offset;
	const unsigned long BfsVersion = BFS_VERSION;

	//Format
	for(BfsSector=0; BfsSector<BFS_SECTORS; BfsSector++)
	{
		//Erase sector
		if(eraseSector(BFS_CHIP, BFS_FIRST_SECTOR + BfsSector) != 0)
		{
			return false;//Error
		}
		//Write sector header
		Offset = BFS_GetPhyByteAddrFromBfsSector(BfsSector);
		writeBuffer(Offset, (unsigned char*)BFS_MAGIC, 4);
		writeBuffer(Offset + 4, (unsigned char*)&BfsVersion, 4);

		glBfsDeletedBytes[BfsSector] = 0;
		glBfsOffsetToFreeRecord[BfsSector] = BFS_SECTOR_HEADER_LEN;
	}

	//Sort fill order
	BFS_SortArray();

	return true;//OK
}


//Internal functions
unsigned char BFS_FlashToFlashCopy(unsigned long DstByteAddr, unsigned long SrcByteAddr, unsigned short ByteLen)
{
	//Return true=OK
	//TODO: optimize with Bulk prog

	writeBuffer(DstByteAddr, (unsigned char*)SrcByteAddr, ByteLen);

	return true;//OK
}

unsigned char BFS_MakeSpace(unsigned short RequiredFileBodySize)
{
	//Return true=OK (now it is space free for the required size)
	unsigned char BfsSector;
	unsigned long Offset;
	unsigned long DstAddress;
	unsigned long FileHeaderAddress;
	unsigned long FileBodyAddress;
	unsigned short FileBodyLen;
	unsigned char IsFree;
	unsigned char IsActive;
	unsigned long Id;
	unsigned char RecordInfo;
	unsigned char BfsSectorWithMostDelFiles;
	const unsigned long BfsVersion = BFS_VERSION;

	do
	{
		BfsSector = glBfsFillOrder[BFS_SECTORS - 2];
		FileHeaderAddress = glBfsOffsetToFreeRecord[BfsSector];
		if((FileHeaderAddress + BFS_MAX_RECORD_HEADER_LEN + RequiredFileBodySize) < BFS_SECTOR_SIZE)
		{
			//Enough space free
			return true;//OK
		}

		//copy only active files (not the files markes as deleted)
		//to the empty sector and erase the source sector

		//Calc sector with most bytes marked as deleted
		BfsSectorWithMostDelFiles = 0;
		for(BfsSector=0; BfsSector<(BFS_SECTORS-1); BfsSector++)
		{
			if(glBfsDeletedBytes[BfsSector] > glBfsDeletedBytes[BfsSectorWithMostDelFiles])
			{
				BfsSectorWithMostDelFiles = BfsSector;
			}
		}
		if(glBfsDeletedBytes[BfsSectorWithMostDelFiles] == 0)
		{
			//No sector has files marked as deleted -> nothing to free
			return false;//Not enough space free
		}

		//Copy active files to the empty bfs sector
		BfsSector = glBfsFillOrder[BFS_SECTORS - 1];//The empty destination sector
		DstAddress = BFS_GetPhyByteAddrFromBfsSector(BfsSector) + BFS_SECTOR_HEADER_LEN;

		FileHeaderAddress = BFS_GetPhyByteAddrFromBfsSector(BfsSectorWithMostDelFiles) + BFS_SECTOR_HEADER_LEN;//Source
		do
		{
			BFS_GetFileHeaderInfo(FileHeaderAddress, &RecordInfo, &Id, &FileBodyLen, &FileBodyAddress, &IsFree, &IsActive);
			if(IsActive)
			{
				//Copy the active file to the destination sector
				BFS_FlashToFlashCopy(DstAddress, FileHeaderAddress, (FileBodyAddress - FileHeaderAddress) + FileBodyLen);
				DstAddress += (FileBodyAddress - FileHeaderAddress) + FileBodyLen;

				glBfsOffsetToFreeRecord[BfsSector] += (FileBodyAddress - FileHeaderAddress) + FileBodyLen;
			}
			FileHeaderAddress = FileBodyAddress + FileBodyLen;
		}while(!IsFree);

		//Erase the source sector
		if(eraseSector(BFS_CHIP, BFS_FIRST_SECTOR + BfsSectorWithMostDelFiles) != 0)
		{
			return false;//Error
		}
		//Write sector header
		Offset = BFS_GetPhyByteAddrFromBfsSector(BfsSectorWithMostDelFiles);
		writeBuffer(Offset, (unsigned char*)BFS_MAGIC, 4);
		writeBuffer(Offset + 4, (unsigned char*)&BfsVersion, 4);
		
		glBfsDeletedBytes[BfsSectorWithMostDelFiles] = 0;
		glBfsOffsetToFreeRecord[BfsSectorWithMostDelFiles] = BFS_SECTOR_HEADER_LEN;

		BFS_SortArray();
	}while(1);

	return false;//Not enough space free
}

void BFS_SortArray()
{
	//Sort the glBfsFillOrder array
	unsigned char RepeatSort;
	unsigned char BfsSector;
	unsigned char Byte;

	do
	{
		RepeatSort = false;
		for(BfsSector=0; BfsSector<(BFS_SECTORS-1); BfsSector++)
		{
			if(glBfsOffsetToFreeRecord[glBfsFillOrder[BfsSector]] < glBfsOffsetToFreeRecord[glBfsFillOrder[BfsSector+1]])
			{
				//Swap
				Byte = glBfsFillOrder[BfsSector];
				glBfsFillOrder[BfsSector] = glBfsFillOrder[BfsSector+1];
				glBfsFillOrder[BfsSector+1] = Byte;
				RepeatSort = true;
			}
		}
	}while(RepeatSort);
}

unsigned long BFS_GetPhyByteAddrFromBfsSector(unsigned char BfsSector)
{
	//BFS_FIRST_SECTOR + BfsSector = PhyFlashSector

	//ASSERT((BFS_FIRST_SECTOR + BfsSector) < FLASH_SECTORS);

	return BFS_FLASH_BASE + (secaddr[BfsSector + BFS_FIRST_SECTOR] * 2);
}

unsigned char BFS_IsSectorFormated(unsigned char BfsSector)
{
	//Is the sector formated and is the version valid

	unsigned long Offset;
	unsigned long Version;

	Offset = BFS_GetPhyByteAddrFromBfsSector(BfsSector);

	if(memcmp((unsigned char*)Offset, BFS_MAGIC, 4) != 0)
	{
		return false;//Magic not found -> not formated
	}

	Version = *((unsigned long*) (Offset + 4));
	if(Version != BFS_VERSION)
	{
		return false;//Magic found but wrong version
	}

	return true;//Formated
}

void BFS_GetFileHeaderInfo(unsigned long FileHeaderAddress, unsigned char *pRecordInfo, unsigned long *pId, unsigned short *pLen, unsigned long *pFileBodyAddress, unsigned char *pIsFree, unsigned char *pIsActive)
{
	unsigned char State;
	unsigned char IdByteCount;
	unsigned char LenByteCount;
	unsigned long Offset;

	Offset = FileHeaderAddress;
	*pRecordInfo = readByte(Offset++);
	State = ((*pRecordInfo) >> 6) & 0x03;
	IdByteCount = 1 + (((*pRecordInfo) >> 1) & 0x03);
	LenByteCount = 1 + ((*pRecordInfo) & 0x01);

	if(State == BFS_TYPE_FREE)
	{
		//Free
		*pIsFree = true;
		*pIsActive = false;
		*pId = 0;
		*pLen = 0;
		*pFileBodyAddress = 0;
	}
	else
	{
		//Active or deleted
		*pIsFree = false;
		*pIsActive = (State == BFS_TYPE_ACTIVE) ? true : false;

		*pId = readByte(Offset++);
		if(IdByteCount > 1)
		{
			*pId |= readByte(Offset++) << 8;
		}
		if(IdByteCount > 2)
		{
			*pId |= readByte(Offset++) << 16;
		}
		if(IdByteCount > 3)
		{
			*pId |= readByte(Offset++) << 24;
		}

		*pLen = readByte(Offset++);
		if(LenByteCount > 1)
		{
			*pLen |= readByte(Offset++) << 8;
		}

		*pFileBodyAddress = Offset;
	}
}

unsigned char BFS_GetFileHeaderAddress(unsigned long Id, unsigned long *pFileHeaderAddress)
{
	//Return true = File is present, pFileHeaderAddress is valid
	//Return false = File not found (e.g. deleted), pFileHeaderAddress is invalid
	unsigned char BfsSector;
	unsigned long PhyAddr;
	unsigned char RecordInfo;
	unsigned long TempId;
	unsigned short FileBodyLen;
	unsigned long FileBodyAddress;
	unsigned char IsFree;
	unsigned char IsActive;

	*pFileHeaderAddress = 0;
	
	for(BfsSector=0; BfsSector<BFS_SECTORS; BfsSector++)
	{
		PhyAddr = BFS_GetPhyByteAddrFromBfsSector(BfsSector) + BFS_SECTOR_HEADER_LEN;

		do
		{
			BFS_GetFileHeaderInfo(PhyAddr, &RecordInfo, &TempId, &FileBodyLen, &FileBodyAddress, &IsFree, &IsActive);
			if(IsActive && (TempId == Id))
			{
				//Found the correct file
				*pFileHeaderAddress = PhyAddr;
				return true;//OK
			}
			if(!IsFree)
			{
				//Skip this file
				PhyAddr = FileBodyAddress + FileBodyLen;
			}
		}while(!IsFree);
	}

	return false;//File not found
}

unsigned char BFS_ByteAddressToBfsSector(unsigned long FileHeaderAddress)
{
	unsigned char BfsSector;
	unsigned long ByteAddress;

	for(BfsSector=0; BfsSector<BFS_SECTORS; BfsSector++)
	{
		ByteAddress = BFS_GetPhyByteAddrFromBfsSector(BfsSector);
		if((FileHeaderAddress >= ByteAddress) && (FileHeaderAddress < (ByteAddress + BFS_SECTOR_SIZE)))
		{
			return BfsSector;
		}
	}

	//ASSERT(false);
	BfsSector = 0xFF;//Error
	return BfsSector;
}

unsigned char BFS_WriteAtAddress(unsigned long FileHeaderAddress, unsigned long Id, unsigned short Len, unsigned char *Buffer, unsigned char *pHeaderSize)
{
	//Return true=OK
	unsigned char RecordHeaderPos;
	unsigned char RecordInfo;
	unsigned char RecordHeader[BFS_MAX_RECORD_HEADER_LEN];
	unsigned char IdByteCount;
	unsigned char LenByteCount;

	//IdByteCount
	if(Id & 0xFF000000)
	{
		IdByteCount = BFS_ID_4BYTES;
	}
	else
	{
		if(Id & 0xFF0000)
		{
			IdByteCount = BFS_ID_3BYTES;
		}
		else
		{
			if(Id & 0xFF00)
			{
				IdByteCount = BFS_ID_2BYTES;
			}
			else
			{
				IdByteCount = BFS_ID_1BYTE;
			}
		}
	}

	//LenByteCount
	LenByteCount = (Len < 0x100) ? BFS_ONE_LEN_BYTE : BFS_TWO_LEN_BYTES;

	//RecordInfo
	RecordInfo = (BFS_TYPE_ACTIVE << 6) | (IdByteCount << 1) | LenByteCount;
	RecordHeaderPos = 0;
	RecordHeader[RecordHeaderPos++] = RecordInfo;

	//Id
	RecordHeader[RecordHeaderPos++] = Id & 0xFF;
	if(Id >= 0x100)
	{
		RecordHeader[RecordHeaderPos++] = (Id >> 8) & 0xFF;
		if(Id >= 0x10000)
		{
			RecordHeader[RecordHeaderPos++] = (Id >> 16) & 0xFF;
			if(Id >= 0x1000000)
			{
				RecordHeader[RecordHeaderPos++] = (Id >> 24) & 0xFF;
			}
		}
	}

	//Len
	RecordHeader[RecordHeaderPos++] = Len & 0xFF;
	if(Len >= 0x100)
	{
		RecordHeader[RecordHeaderPos++] = (Len >> 8) & 0xFF;
	}

	//Write record header
	writeBuffer(FileHeaderAddress, RecordHeader, RecordHeaderPos);
	//Write record data
	writeBuffer(FileHeaderAddress + RecordHeaderPos, Buffer, Len);

	*pHeaderSize = RecordHeaderPos;

	return true;//OK
}

unsigned char readByte(unsigned long ByteAddr)
{
	unsigned char Byte;
	Byte = ((unsigned char*) ByteAddr)[0];

	return Byte;
}

unsigned short readWord(unsigned long WordAddr)
{
	//addr = unsigned short address
	int Offset;
	unsigned short Word;

	//ASSERT((WordAddr * 2) >= FLASH1_BASE);
	//ASSERT((WordAddr * 2) < (FLASH1_BASE + FLASH_SIZE));

	Offset = (WordAddr * 2) - FLASH1_BASE;

	Word = *((unsigned short *)(WordAddr<<1));

	return Word;
}


int writeBuffer(unsigned long ByteAddr, unsigned char *buffer, unsigned short len)
{
	//Return 0=OK
	int Offset;
	unsigned short Word;

	Offset = 0;

	if(ByteAddr & 1)
	{
		//Not word boundary
		Word = readWord(ByteAddr / 2);
		Word &= (buffer[Offset] << 8) | 0xFF;
		writeWord(ByteAddr & 0xFFFFFFFE, Word);
		Offset++;
		ByteAddr++;
		len--;
	}

	while(len >= 2)
	{
		Word = readWord(ByteAddr / 2);
		Word &= buffer[Offset] | (buffer[Offset + 1] << 8);
		writeWord(ByteAddr, Word);
		Offset += 2;
		ByteAddr += 2;
		len -= 2;
	}

	if(len == 1)
	{
		Word = readWord(ByteAddr / 2);
		Word &= 0xFF00 | buffer[Offset];
		writeWord(ByteAddr, Word);
		Offset++;
		ByteAddr++;
		len--;
	}

	return 0;//OK
}

