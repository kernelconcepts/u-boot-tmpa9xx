/*
 * (C) Copyright 2007
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Author: Igor Lisitsin <igor@emcraft.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

/* Cache test
 *
 * This test verifies the CPU data and instruction cache using
 * several test scenarios.
 */

#ifdef CONFIG_POST

#include <post.h>

#if CONFIG_POST & CFG_POST_CACHE

#include <asm/mmu.h>
#include <watchdog.h>

#define CACHE_POST_SIZE	1024

void program_tlb(u32 phys_addr, u32 virt_addr, u32 size, u32 tlb_word2_i_value);

int cache_post_test1 (int tlb, void *p, int size);
int cache_post_test2 (int tlb, void *p, int size);
int cache_post_test3 (int tlb, void *p, int size);
int cache_post_test4 (int tlb, void *p, int size);
int cache_post_test5 (int tlb, void *p, int size);
int cache_post_test6 (int tlb, void *p, int size);

static int tlb = -1;		/* index to the victim TLB entry */

static unsigned char testarea[CACHE_POST_SIZE]
__attribute__((__aligned__(CACHE_POST_SIZE)));

int cache_post_test (int flags)
{
	void* virt = (void*)CFG_POST_CACHE_ADDR;
	int ints, i, res = 0;
	u32 word0;

	if (tlb < 0) {
		/*
		 * Allocate a new TLB entry, since we are going to modify
		 * the write-through and caching inhibited storage attributes.
		 */
		program_tlb((u32)testarea, (u32)virt,
			    CACHE_POST_SIZE, TLB_WORD2_I_ENABLE);

		/* Find the TLB entry */
		for (i = 0;; i++) {
			if (i >= PPC4XX_TLB_SIZE) {
				printf ("Failed to program tlb entry\n");
				return -1;
			}
			word0 = mftlb1(i);
			if (TLB_WORD0_EPN_DECODE(word0) == (u32)virt) {
				tlb = i;
				break;
			}
		}
	}
	ints = disable_interrupts ();

	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test1 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test2 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test3 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test4 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test5 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test6 (tlb, virt, CACHE_POST_SIZE);

	if (ints)
		enable_interrupts ();

	return res;
}

#endif /* CONFIG_POST & CFG_POST_CACHE */
#endif /* CONFIG_POST */