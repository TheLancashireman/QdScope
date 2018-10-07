/* qdscope - a quick and dirty logic analyser using an Arduino Nano
 *
 * (c) 2016 David Haworth
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

#include "PinCapture.h"
#include "Freq.h"

/* Blinkenlight
*/
#define led1		13		// On-board LED connected to digital pin 13
char ledState;

/* Timed processing
*/
unsigned long then;

/* setup() - standard Arduino "Init Task"
*/
void setup(void)
{
	pinMode(led1, OUTPUT);				// Sets the digital pin as output
	digitalWrite(led1, LOW);			// Drive the pin low (LED off)
	ledState = 0;

	Serial.begin(9600);					// Start the serial port.
	Serial.println("Hello world!");		// ToDo : it'll need a "who are you?" response

	{
		char buffer[50];
		sprintf(buffer, "Sizes: %d %d %d %d", sizeof(short), sizeof(int), sizeof(long), sizeof(long long));
		Serial.println(buffer);
	}

	then = millis();					// Initialise the time reference.

	PinCapture_Setup();
	Freq_Setup();
}

/* loop() - standard Arduino "Background Task"
*/
void loop(void)
{
	unsigned long now = millis();
	unsigned long elapsed = now - then;

	if ( ledState )
	{
		/* LED stays on for 20 ms
		*/
		if ( elapsed > 20 )
		{
			then += 20;
			ledState = 0;
			digitalWrite(led1, LOW);
		}
	}
	else
	{
		/* LED stays off for 2 secs minus 20 ms
		*/
		if ( elapsed >= 1980 )
		{
			/* This branch gets executed every 2 seconds.
			*/
			then += 1980;
			ledState = 1;
			digitalWrite(led1, HIGH);
			PinCapture_Dump();
			Freq_Dump();
			Serial.println("woo");
		}
	}

	PinCapture_Loop();
	Freq_Loop();
}
