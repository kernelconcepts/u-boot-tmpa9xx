/*
 * (C) Copyright 2009,2010
 * Kernel Concepts <www.kernelconcepts.de>
 * Florian Boor (florian.boor@kernelconcepts.de)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/tmpa9xx.h>

/* TMPA9X0 just has 16 bit time. So we use 32K
 * Clock source here.  So one tick is about 31.25us.
 * if client want to delay < 32us, we let system delay 32us.
 * And the timer count overflow in 2s. Damn it.
 */
#define TIMER_LOAD_VAL 0xFFFF

/* macro to read the 32 bit timer */
#define READ_TIMER ((TIME4_DATA) & 0xFFFF)

static ulong timestamp;
static ulong lastdec;

/* nothing really to do with interrupts, just starts up a counter. */
int timer_init (void)
{
	/* Clock of timer 45 is 32kHz. */
	CLKCR5 &= ~(0x01 << 2);

	TIME4_MODE = 0x0;	/* disable TIME4 PWM mode */
	TIME4_CMPEN = 0x0;	/* disable TIME4 compare */
	TIME4_CAPEN = 0x0;	/* disable TIME4 catpure */

	TIME4_LD = 0xFFFF;	/* Always decrese from 0xFFFF */
	/* Free-running mode, Disable interrupt, 32kHz, 16bit count, Wrapping mode */
	TIME4_CONTROL = 0x82;

	return 0;
}

/*
 * timer without interrupts
 */
void reset_timer (void)
{
	reset_timer_masked ();
}

ulong get_timer (ulong base)
{
	return get_timer_masked () - base;
}

void set_timer (ulong t)
{
	timestamp = t;
}

/* delay x useconds AND perserve advance timstamp value */
void __udelay (unsigned long usec)
{
	ulong tmo, tmp;
	ulong sec_num, nsec_num;

	if(usec >= 1000){		/* if "big" number, spread normalization to seconds */
		tmo = usec / 1000;	/* start to normalize for usec to ticks per sec */
		tmo *= CONFIG_SYS_HZ;	/* find number of "ticks" to wait to achieve target */
		tmo /= 1000;		/* finish normalize. */
	} else {			/* else small number, don't kill it prior to HZ multiply */
		tmo = usec * CONFIG_SYS_HZ;
		tmo /= (1000*1000);
	}

	tmp = get_timer (0);		/* get current timestamp */
	if( (tmo + tmp + 1) < tmp)	/* if setting this fordward will roll time stamp */
		reset_timer_masked ();	/* reset "advancing" timestamp to 0, set lastdec value */
	else
		tmo += tmp;		/* else, set advancing stamp wake up time */

	while (get_timer_masked () < tmo)/* loop till event */
		/*NOP*/;
}

void reset_timer_masked (void)
{
	/* reset time */
	lastdec = READ_TIMER;  /* capure current decrementer value time */
	timestamp = 0;	       /* start "advancing" time stamp from 0 */
}

ulong get_timer_masked (void)
{
	ulong now = READ_TIMER;		/* current tick value */

	if (lastdec >= now) {		/* normal mode (non roll) */
		/* normal mode */
		timestamp += lastdec - now; /* move stamp fordward with absoulte diff ticks */
	} else {			/* we have overflow of the count down timer */
		/* nts = ts + ld + (TLV - now)
		 * ts=old stamp, ld=time that passed before passing through -1
		 * (TLV-now) amount of time after passing though -1
		 * nts = new "advancing time stamp"...it could also roll and cause problems.
		 */
		timestamp += lastdec + TIMER_LOAD_VAL - now;
	}
	lastdec = now;

	return timestamp;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk (void)
{
	ulong tbclk;

	tbclk = CONFIG_SYS_HZ;
	return tbclk;
}
