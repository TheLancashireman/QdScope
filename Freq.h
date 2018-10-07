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

#define FREQ_TCCR1B_EXT_RISING	0x07
#define FREQ_TCCR1B_EXT_FALLING	0x06

typedef struct
{
	unsigned long accumulator;
	unsigned lastTcnt1;
} Freq_t;

Freq_t Freq;

static inline void Freq_Dump(unsigned millisecs)
{
	char buffer[50];
	unsigned long whole = (unsigned long)( ((unsigned long long)Freq.accumulator * 1000uLL) /
											(unsigned long long)millisecs );
	unsigned frac = (unsigned)( ((unsigned long long)Freq.accumulator * 1000uLL) %
											(unsigned long long)millisecs );
	sprintf(buffer, "f = %ld.%02d", whole, frac);
	Serial.println(buffer);
	Freq.accumulator = 0;
}

static void Freq_Loop(void)
{
	unsigned tcnt1 = TCNT1;

	/* We need the subtraction to overflow correctly, so the cast is done *afterwards*
	*/
	Freq.accumulator += (unsigned long)(tcnt1 - Freq.lastTcnt1);
	Freq.lastTcnt1 = tcnt1;
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
