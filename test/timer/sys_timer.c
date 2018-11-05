/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "sys_timer.h"
#include "cdl_timer.h"
#include "cdl_pll.h"
#include "memleak.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
/* will overflow after 0xFFFFFFFF/PCLK seconds * divisor */
/* 25M pclk about 171 seconds * 256 ~= 729 minutes */
/* 10M pclk about 429 seconds * 16 ~= 114 minutes */

#define GTIMER_PRE_SCALER   0  /*01 = 4 stages of prescale, clock is divided by 16*/

#define CTIMER_US_PER_SEC			1000000
#define CTIMER_MS_PER_SEC			1000

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
// counter elapsed for 1ms
static uint32_t timer_rate_hz = 0;

// for 32 bit timer, one high 32 bit to acculate when overflow
static uint32_t tick_high = 0;
static uint32_t tick_id = 0;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
int sys_timer_init(uint32_t port, uint32_t base, uint32_t pclk)
{
	tick_id = port;
	tick_high = 0;
	timer_rate_hz = pclk/(1 << GTIMER_PRE_SCALER*4)/1000;   // how many cycles per ms

	timer_init(port, base, pclk);
	timer_set_ctrl(port, TIMER_WRAPPING_MODE, TIMER_SIZE_BITS32, GTIMER_PRE_SCALER,
						 TIMER_INT_DIS, TIMER_FREE_MODE, TIMER_DIS);
	timer_set_load(port, 0xFFFFFFFF);
	timer_set_bgload(port, 0xFFFFFFFF);
	timer_enable_ctrl(port, TIMER_EN);

	return 0;
}

// get timer counter
u64 get_ticks(void)
{
	u64 tick = 0;
	uint32_t nowl = 0, nowu = 0;

	nowl = timer_get_value(tick_id);	
	if (timer_get_ris(tick_id)) 
	{
		tick_high +=1;

		do {
			timer_clr_int(tick_id);
			nowl = timer_get_value(tick_id);
		} while ((timer_get_ris(tick_id)) || (nowl == 0x0));

		//info("overflow: tick_high %u nowl %u\n", tick_high, nowl);
	}

	nowu = tick_high;
	tick = (((u64)nowu) << 32) | (0xFFFFFFFF - nowl);
	
	// info("tick %lld\n", tick);
	return tick;
}

u64 cb_lldiv(u64 n, uint32_t div) 
{
	lldiv_t r = lldiv(n, div);

	return r.quot;
}

// return interval by ms
u64 get_timer(u64 base)
{
#if 1
	u64 tick = get_ticks();
	u64 hz = timer_rate_hz;
	u64 cur = cb_lldiv(tick, hz);

	// info("tick %lld hz %lld cur %lld\n base %ld", tick, hz, cur, base);
	cur -= base;
	return  cur;
#else
	return 0;
#endif
}

// delay ticks
void ticksdelay(uint32_t ticks)
{
	u64 endticks  = get_ticks() + ticks;

	while (get_ticks() < endticks)
		;
}

// delay us
void udelay(uint32_t usec)
{
	u64 endtime  = (u64)usec * timer_rate_hz;

	endtime = cb_lldiv(endtime, 1000UL);

	endtime += get_ticks();

	while (get_ticks() < endtime)
		;
}

// delay ms
void mdelay(uint32_t msec)
{
	while (msec--) {
		udelay(1000);
	}
}

// delay seconds
void sdelay(uint32_t sec)
{
	u64 endtime = 0L; 

	endtime = TIMER_PCLK * (unsigned long long)sec/(1<<GTIMER_PRE_SCALER*4);   

	endtime += get_ticks();
	while (get_ticks() < endtime);
}

