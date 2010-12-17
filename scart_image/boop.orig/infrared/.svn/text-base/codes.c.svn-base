/*
    codes.c - structs for accessing the infrared-codetables
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

#include "codes.h"

const struct TABLES_S ITT =
{
	4,
	{
		{
			#include "ir_codes/itt/seleco_019"
			"Seleco #19\0"
		},
		{
			#include "ir_codes/itt/seleco_049"
			"Seleco #49\0"
		},
		{
			#include "ir_codes/itt/seleco_108"
			"Seleco #108\0"
		},
		{
			#include "ir_codes/itt/seleco_131"
			"Seleco #131\0"
		},
	}
};

const struct TABLES_S NRC17 =
{
	2,
	{
		{
			#include "ir_codes/nrc17/dbox2"
			"DBox 2\0"
		},
		{
			#include "ir_codes/nrc17/grundig_tp720"
			"Grundig TP720\0"
		}
	}
};


#define RC5_STD(cmd) 0x1000 | cmd
#define RC5X   (cmd) cmd

const struct TABLES_S RC5 =
{
	2,
	{
		{
			#include "ir_codes/rc5/tv"
			"Generic TV\0"
		},
		{
			#include "ir_codes/rc5/technisat_satreceiver_103TS103"
			"SAT(103TS103)\0"
		}
	}
};

const struct TABLES_S RC6 =
{
	2,
	{
		{
			#include "ir_codes/rc6/tv"
			"RC6 TV\0"
		},
		{
			#include "ir_codes/rc6/dvd"
			"RC6 DVD\0"
		}
	}
};

const struct TABLES_C RCA =
{
	1,
	{
		{
			#include "ir_codes/rca/xbox"
			"X-Box\0"
		}
	}
};

const struct TABLES_C RCMM =
{
	1,
	{
		{
			#include "ir_codes/rcmm/galaxis"
			"Galaxis Sat\0"
		}
	}
};

const struct TABLES_I REC80 =
{
	1,
	{
		{
			#include "ir_codes/rec80/rak_sc957wk"
			"RAK SC-957-WK\0"
		}
	}
};

const struct TABLES_S RECS80 =
{
	2,
	{
		{
			#include "ir_codes/recs80/seleco_020"
			"seleco#20\0"
		},
		{
			#include "ir_codes/recs80/seleco_157"
			"Seleco #157\0"
		}
	}
};

const struct TABLES_S SIRC =
{
	2,
	{
		{
			#include "ir_codes/sirc/rm-pp411"
			"RM-PP411\0"
		},
		{
			#include "ir_codes/sirc/tv"
			"Generic TV\0"
		}
	}
};

const struct TABLES_C SPACEENC =
{
	2,
	{
		{
			#include "ir_codes/spaceenc/schneider_dtv3"
			"Schneider DTV3\0"
		},
		{
			#include "ir_codes/spaceenc/seleco_027"
			"Seleco #27\0"
		}
	}
};

const struct TABLES_S IRRF =
{
	2,
	{
		{
			#include "ir_codes/rf/cc1100_a0"
			"RF 0\0"
		},
		{
			#include "ir_codes/rf/cc1100_a1"
			"RF 1\0"
		}
	}
};

const struct TABLES_N RAW =
{
	4,
	{
		{
			"RAW Set 1"
		},
		{
			"RAW Set 2"
		},
		{
			"RAW Set 3"
		},
		{
			"RAW Set 4"
		}
		
	}
};
