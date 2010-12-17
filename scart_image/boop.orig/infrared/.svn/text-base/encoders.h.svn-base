/*
    encoders.h - structs for accessing the infrared-encoders
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

#ifndef ENCODERS_H
#define ENCODERS_H

#define ENC_ITT		0
#define ENC_NRC17	1
#define ENC_RC5		2
#define ENC_RCA		3
#define ENC_RCMM	4
#define ENC_REC80	5
#define ENC_RECS80	6
#define ENC_SIRC	7
#define ENC_SPACEENC	8
#define ENC_RF		9
#define ENC_RAW		10

typedef void(*cfunc)(unsigned char);

typedef struct IR_ENCODER
{
	const cfunc		init;
	const void*		data;
	const unsigned char	type;
	const char		*name;
} IR_ENCODER;

typedef struct ENCODERS
{
	const unsigned int	num_encoders;
	const struct IR_ENCODER encoder[];
} ENCODERS;

extern const struct TABLES_S ITT;
extern const struct TABLES_S NRC17;
extern const struct TABLES_S RC5;
extern const struct TABLES_S RC6;
extern const struct TABLES_C RCA;
extern const struct TABLES_C RCMM;
extern const struct TABLES_I REC80;
extern const struct TABLES_S RECS80;
extern const struct TABLES_S SIRC;
extern const struct TABLES_C SPACEENC;
extern const struct TABLES_S IRRF;
extern const struct TABLES_N RAW;

#endif
