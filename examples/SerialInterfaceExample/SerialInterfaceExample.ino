#include <SerialUI.h>
#include <Fire.h>

Fire blinkPeriod = Fire(1000);  // Set the led to blink every 1000ms
Fire blinkDuration = Fire(100); // Set the duration of the blink to 100ms
int ledPIN = LED_BUILTIN;       // Use the built in led (Uses reverse logic on the ESP8266, which this is set up for)
SerialUI ui = SerialUI("gesui", "Graham Engineering SerialUI (gesui) Example`"); // Set a customized banner to be shown
bool BlinkStatusIndicator = false;

void setup()
{
  Serial.begin(115200);
  while (!Serial){}
  ui.init();     // Show the SerialUI banner
  ui.addCmd("enableBlink", enableBlink);                      // Add serial command to enable blink
  ui.addCmd("disableBlink", disableBlink);                    // Add serial command to disbale blink
  ui.addCmd("statBlink", statusMessage);                      // Add serial command to check status of blink
  
  pinMode(ledPIN, OUTPUT);                                    // Set up the LED pin
  digitalWrite(ledPIN, HIGH);                                 // Set it to high so that it's off (ESP8266)
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
    msg = "Status indicator is currently enabled.";
  }
  else
  {
    msg = "Status indicator is currently disabled.";
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

