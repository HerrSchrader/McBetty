/*
    ir_selector.h - select ir encoder & codetable
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

#ifndef IR_SELECTOR_H
#define IR_SELECTOR_H

struct irDevice_ {
	unsigned char encoder;
	unsigned char set;
};

struct irDeviceTable_ {
	unsigned char count;
	unsigned char active;
	struct irDevice_ device[4];
};

struct irDeviceTable_ irDevTab;
const struct irDeviceTable_ default_irDevTab;

char* getSetName(unsigned char enc, unsigned char set);
unsigned char EncIsValid(unsigned char enc, unsigned char set);
void select_ir(unsigned char *encoder,unsigned char *set);
void RC_setting(void);

#endif
