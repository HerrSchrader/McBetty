/*
    encoders.c - structs for accessing the infrared-encoders
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
#include "ir_itt.h"
#include "ir_nrc17.h"
#include "ir_rc5.h"
#include "ir_rc6.h"
#include "ir_rca.h"
#include "ir_rcmm.h"
#include "ir_rec80.h"
#include "ir_recs80.h"
#include "ir_sirc.h"
#include "ir_spaceenc.h"
#include "ir_rf.h"
#include "ir_raw.h"
#include "encoders.h"
#include "codes.h"

const struct ENCODERS encoders =
{
	12,
	{
		{ ITT_Init,	(void*)&ITT,		1,	"ITT\0" },
		{ NRC17_Init,	(void*)&NRC17,		1,	"NRC17\0" },
		{ RC5_Init,	(void*)&RC5,		1,	"RC-5\0" },
		{ RC6_Init,	(void*)&RC6,		1,	"RC-6\0" },
		{ RCA_Init,	(void*)&RCA,		0,	"RCA\0" },
		{ RCMM_Init,	(void*)&RCMM,		0,	"RC-MM\0" },
		{ REC80_Init,	(void*)&REC80,		2,	"REC-80\0" },
		{ RECS80_Init,	(void*)&RECS80,		1,	"RECS-80\0" },
		{ SIRC_Init,	(void*)&SIRC,		1,	"SIRC\0" },
		{ SPACEENC_Init,(void*)&SPACEENC,	0,	"SPACEENC\0" },
		{ IRRF_Init,	(void*)&IRRF,		1,	"RF\0" },
		{ RAW_Init,	(void*)&RAW,		3,	"learn" }
	}
};
