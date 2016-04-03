/* qdscope - a quick and dirty logic analyser using an Arduino Nano
 *
 * (c) 2016 David Haworth
 *
 * lcdTerminal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lcdTerminal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DhG.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 *
 * qdscope is an Arduino sketch, written for an Arduino Nano
*/

/* Blinkenlight
*/
#define led1		13		// On-board LED connected to digital pin 13
char ledState;

/* Timed processing
*/
unsigned long then;

/* Capturing inputs
*/
#define NCAP 50
#define inputPin0	8		// PB0
#define inputPin1	9		// PB1
#define inputPin2	10		// PB2
#define inputPin3	11		// PB3

typedef unsigned char capture_t;
capture_t oldPin;

typedef struct pincapture_s pincapture_t;
struct pincapture_s
{
	unsigned long capTime;
	capture_t capValue;
};
pincapture_t pinCapture[NCAP];
int capIndex;

capture_t ReadInputs(void);
void DumpCapture(void);

/* setup() - standard Arduino "Init Task"
*/
void setup(void)
{
	int i;

	pinMode(led1, OUTPUT);				// Sets the digital pin as output
	digitalWrite(led1, LOW);			// Drive the pin low (LED off)
	ledState = 0;

	Serial.begin(9600);					// Start the serial port.
	Serial.println("Hello world!");		// ToDo : it'll need a "who are you?" response

	then = millis();					// Initialise the time reference.

	pinMode(inputPin0, INPUT_PULLUP); 			//	Set up the input pins
	pinMode(inputPin1, INPUT_PULLUP); 
	pinMode(inputPin2, INPUT_PULLUP); 
	pinMode(inputPin3, INPUT_PULLUP); 
	oldPin = ReadInputs();
	capIndex = 0;
}

/* loop() - standard Arduino "Background Task"
*/
void loop(void)
{
	unsigned long now = millis();
	unsigned long elapsed = now - then;
	capture_t newPin;

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
			then += 1980;
			ledState = 1;
			digitalWrite(led1, HIGH);
			DumpCapture();
		}
	}

	newPin = ReadInputs();
	if ( oldPin != newPin )
	{
		pinCapture[capIndex].capTime = micros();
		pinCapture[capIndex].capValue = newPin;
		oldPin = newPin;

		capIndex++;
		if ( capIndex >= NCAP )
		{
			DumpCapture();
		}
	}
}

capture_t ReadInputs(void)
{
	capture_t v = 0;
    if ( digitalRead(inputPin3) == HIGH )	v |= 0x08;
    if ( digitalRead(inputPin2) == HIGH )	v |= 0x04;
    if ( digitalRead(inputPin1) == HIGH )	v |= 0x02;
    if ( digitalRead(inputPin0) == HIGH )	v |= 0x01;

	return v;
}

void DumpCapture(void)
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
