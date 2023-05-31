#include "timer.h"
#include "main.h"

static uint8_t count_of_regFunc = 0;

static struct timerFunctions {
	timer_fp_t timerfp;
	uint16_t div;
	uint16_t ticks;
} Arr[TIMER_MAX_TIMERS];

/*
 * Process next tick.
 * Must be called periodically with a frequency of TIMER_TICKRATE_HZ
 */
void timer_tick() {
	for (int i = 0; i < count_of_regFunc; i++) {
		Arr[i].ticks++;
		if (Arr[i].ticks == Arr[i].div) {
			Arr[i].ticks = 0;
			(Arr[i].timerfp)();
		}
	}
}

/*
 * Register a new timer function.
 * The new timer function "timerfp" is called with a frequency of
 * "TIMER_TICKRATE_HZ" / "div".
 * @param timerfp Pointer to the timer function that is to be called
 * @param div Clock divider f_out = f_in / div
 * Return: 0 for success, others for failure
 */
int timer_register(timer_fp_t timerfp, uint32_t div) {
	if (count_of_regFunc < TIMER_MAX_TIMERS) {
		Arr[count_of_regFunc].timerfp = timerfp;
		Arr[count_of_regFunc].div = div;
		Arr[count_of_regFunc].ticks = 0;
		count_of_regFunc++;
		return 0;
	}

	return 1;
}

/**
 * For timer initilization
 */
int timer_init() {
	return 0;
}
