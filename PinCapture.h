/* PinCapture - the logic analyser bits of qdscope.
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

/* Capturing inputs.
 *
 * ToDo: make the number and selection of pins configurable.
 * Avoid PD5 - it's the ext. timer input for T1 (Freq,h)
*/
#define NCAP 50
#define inputPin0	8		// PB0
#define inputPin1	9		// PB1
#define inputPin2	10		// PB2
#define inputPin3	11		// PB3

/* These are the data types used in this module.
*/
typedef unsigned char capture_t;			/* Allows up to 8 pins */

typedef struct pincapture_s pincapture_t;	/* Structure hold time and (changed) state)\ */
struct pincapture_s
{
	unsigned long capTime;
	capture_t capValue;
};

/* Static/global variables
*/
capture_t oldPins;							/* Previous state of the pins. We only store changes. */
pincapture_t pinCapture[NCAP];				/* Array of captures */
int capIndex;								/* Position in the array */

static inline capture_t PinCapture_ReadInputs(void)
{
	capture_t v = 0;
    if ( digitalRead(inputPin3) == HIGH )	v |= 0x08;
    if ( digitalRead(inputPin2) == HIGH )	v |= 0x04;
    if ( digitalRead(inputPin1) == HIGH )	v |= 0x02;
    if ( digitalRead(inputPin0) == HIGH )	v |= 0x01;
	return v;
}

static inline void PinCapture_Dump(void)
{
	int i;
	capture_t v;

	for ( i = 0; i < capIndex; i++ )
	{
		Serial.print(pinCapture[i].capTime);
		Serial.print(' ');
		v = pinCapture[i].capValue;
		Serial.print((v&0x08)?" 1":" 0");
		Serial.print((v&0x04)?" 1":" 0");
		Serial.print((v&0x02)?" 1":" 0");
		Serial.print((v&0x01)?" 1":" 0");
		Serial.println("");
	}

	capIndex = 0;
}

static inline void PinCapture_Loop(void)
{
	capture_t newPins = PinCapture_ReadInputs();

	if ( oldPins != newPins )
	{
		pinCapture[capIndex].capTime = micros();
		pinCapture[capIndex].capValue = newPins;
		oldPins = newPins;

		capIndex++;
		if ( capIndex >= NCAP )
		{
			PinCapture_Dump();
		}
	}
}

static inline void PinCapture_Setup(void)
{
	pinMode(inputPin0, INPUT_PULLUP); 			//	Set up the input pins
	pinMode(inputPin1, INPUT_PULLUP); 
	pinMode(inputPin2, INPUT_PULLUP); 
	pinMode(inputPin3, INPUT_PULLUP); 
	oldPins = PinCapture_ReadInputs();
	capIndex = 0;
}
