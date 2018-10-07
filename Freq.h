/* Freq.h - the frequency counter bits of qdscope.
 *
 * (c) 2018 David Haworth
 *
 * QdScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QdScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QdScope.  If not, see <http://www.gnu.org/licenses/>.
 *
 * qdscope is an Arduino sketch, written for an Arduino Nano
*/
#ifndef FREQ_H
#define FREQ_H

/* Count the number of pulses on T1 (timer 1 external clock). This is PD5 on the 'nano
*/

/* TCNT1 modes.
*/
#define FREQ_TCCR1B_EXT_RISING	0x07
#define FREQ_TCCR1B_EXT_FALLING	0x06

typedef struct
{
	unsigned long accumulator;
	unsigned lastTcnt1;
	unsigned long lastTime;
} Freq_t;

Freq_t Freq;

static inline void Freq_Loop(void)
{
	unsigned tcnt1 = TCNT1;

	/* We need the subtraction to overflow correctly, so the cast is done *afterwards*
	*/
	Freq.accumulator += (unsigned long)(tcnt1 - Freq.lastTcnt1);
	Freq.lastTcnt1 = tcnt1;
}

static inline void Freq_Dump(void)
{
	noInterrupts();
	unsigned long now = micros();
	/* Update the accumulator.
	*/
	Freq_Loop();
	interrupts();

	/* Calculate duration of the measurement interval and set the start time for the next interval.
	*/
	unsigned long duration = now - Freq.lastTime;
	Freq.lastTime = now;

	/* Calculate frequency as counts * 1000000 / seconds * 1000000
	*/
	double f = ((double)Freq.accumulator * 1000000.0)/((double)duration);

	/* Integer and fractional parts (looks like arduino's sprintf doesn't do %f)
	*/
	unsigned whole = (unsigned)f;
	unsigned frac = (unsigned)((f - (double)whole)*1000.0);

	char buffer[50];
	sprintf(buffer, "f = %u.%03u", whole, frac);
	Serial.println(buffer);

	/* Start the whole process again.
	*/
	Freq.accumulator = 0;
}

static inline void Freq_Setup(void)
{
	/* Clear all the settings of timer 1.
	*/
	TCCR1A = 0;
	TCCR1B = 0;

	/* Select external input as frequency source.
	 * 7 for rising edge, 6 for falling edge.
	 * All waveform generation functions are disabled (also in TCCR1A).
	*/
	TCCR1B = FREQ_TCCR1B_EXT_RISING;

	/* Clear the counter
	*/
	TCNT1 = 0;

	Freq.lastTcnt1 = TCNT1;
	Freq.accumulator = (unsigned long)Freq.lastTcnt1;
}

#endif
