/*****************************************************************************
* Product: GNU toolset for ARM, BSP for AT91SAM7S-EK
* Date of the Last Update:  Jun 29, 2007
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2007 Quantum Leaps, LLC. All rights reserved.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef isr_h
#define isr_h

#include "global.h"
#include "arm_exc.h"         /* ARM exceptions, including interrupt locking */

enum EventFlags {
    PIT_FLAG,
    TIMER0_FLAG,
    TIMER1_FLAG,
    /* . . . */
};

void eventFlagSet(uint8 flag);
uint8 eventFlagCheck(uint8 flag);

void ISR_pit(void);
void ISR_timer0(void);
void ISR_spur(void);

#endif                                                             /* isr_h */

