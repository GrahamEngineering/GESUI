#include <SerialUI.h>
#include <Fire.h>

/*
	SerialInterfaceExample.ino
		Graham Engineering Serial User Interface (gesui) Example
		Author: matthew.j.graham@gmail.com
		Date: 3 Mar 2017

	Example sketch
		- Uses the Fire library - if you don't have it, please get it.
		- Shows the use of the instantiation method with a custom prompt and custom banner
		- Blinks the onboard LED
			-- Originally written for use with the ESP8266 on the ESP-12E board
			-- Can be re-written easily to make a bit more sense on an Arduino Uno
		- Shows adding custom commands
			-- enableBlink			Starts the blink sequence
			-- removeThisCommand	An example of a command to remove (normally this wouldn't be done via the command line I don't believe)
			-- disableBlink			Stops the blink sequence
			-- statBlink			Shows the status of the blink sequence
		
*/

SerialUI ui = SerialUI("gesui", "Welcome to the GESUI Example\n\tType help to get a list of commands"); // Set a customized banner to be shown

Fire blinkPeriod = Fire(1000);			// Set the led to blink every 1000ms
Fire blinkDuration = Fire(100);			// Set the duration of the blink to 100ms
int ledPIN = LED_BUILTIN;			 	// Use the built in led (Uses reverse logic on the ESP8266, which this is set up for)
bool BlinkStatusIndicator = false;

void setup()
{
	Serial.begin(115200);										// Start Serial
	while (!Serial){}											// Wait for Serial to start
	Serial.println("");											// Print a blank line to get rid of any garbage that may exist
	ui.init();		 											// Show the SerialUI banner
	ui.addCmd("enableBlink", enableBlink);						// Add serial command to enable blink
	ui.addCmd("removeThisCommand", removeThisCommand);			// An example of a command that can be removed by the user
	ui.addCmd("disableBlink", disableBlink);					// Add serial command to disbale blink
	ui.addCmd("statBlink", statusMessage);						// Add serial command to check status of blink
	ui.addCmd("statBlink", statusMessage);						// this one should fail the add, as it's a duplicate
	
	pinMode(ledPIN, OUTPUT);									// Set up the LED pin
	digitalWrite(ledPIN, HIGH);									// Set it to high so that it's off (ESP8266)
}

void loop()
{
	ui.serialRead(); 

	if (BlinkStatusIndicator)
	{
		if (BlinkStatusIndicator && blinkPeriod.fire())
		{
			startBlink();
			blinkDuration.fire(); // clears out the blink so that it will fire again in 100ms
		}
		
		if (BlinkStatusIndicator && blinkDuration.fire())
		{
			stopBlink();
		}
	}
}

void statusMessage()
{
	String msg;
	if (BlinkStatusIndicator)
	{
		msg = "\tStatus indicator is currently enabled.";
	}
	else
	{
		msg = "\tStatus indicator is currently disabled.";
	}
	Serial.println(msg);
}

void enableBlink()
{
	BlinkStatusIndicator = true;
	Serial.println("\tBlink Enabled.");
}

void disableBlink()
{
	stopBlink();
	BlinkStatusIndicator = false;
	Serial.println("\tBlink Disabled.");
}

void startBlink()
{
	digitalWrite(ledPIN, LOW);
}

void stopBlink()
{
	digitalWrite(ledPIN, HIGH);
}

void removeThisCommand()
{
	if (ui.rmCmd("removeThisCommand"))
	{
		Serial.println("\tcmd 'removeThisCommand' removed");
	}
	else
	{
		Serial.println("\tcmd 'removeThisCommand' not removed");
	}
}