/*
    codes.h - structs for accessing the infrared-codetables
    Copyright (C) 2007  Ch. Klippel <ck@mamalala.net>

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

#ifndef CODES_H
#define CODES_H

typedef struct CODE_TABLE_C
{
	const unsigned char codes[42];
	const char *name;
} CODE_TABLE_C;

typedef struct CODE_TABLE_S
{
	const unsigned short codes[42];
	const char *name;
} CODE_TABLE_S;

typedef struct CODE_TABLE_I
{
	const unsigned int codes[42];
	const char *name;
} CODE_TABLE_I;

typedef struct CODE_TABLE_N
{
	const char *name;
} CODE_TABLE_N;

typedef struct TABLES_C
{
	const unsigned int		num_tables;
	const struct CODE_TABLE_C	table[];
} TABLES_C;

typedef struct TABLES_S
{
	const unsigned int		num_tables;
	const struct CODE_TABLE_S	table[];
} TABLES_S;

typedef struct TABLES_I
{
	const unsigned int		num_tables;
	const struct CODE_TABLE_I	table[];
} TABLES_I;

typedef struct TABLES_N
{
	const unsigned int		num_tables;
	const struct CODE_TABLE_N	table[];
} TABLES_N;

extern const struct ENCODERS encoders;

#endif
