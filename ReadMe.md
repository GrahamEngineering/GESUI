# Graham Engineering Serial User Interface (gesui)

A simple user interface for serial input/output via commands with callback functions via function pointers
* Author: matthew.j.graham@gmail.com
* Initial Build: 3 Mar 2017
* Documentation Update: 3 Mar 2017

## Overview

This library is configured primarily for the NodeMCU v1.0-ish ESP8266 ESP-12E boards.  With minor variation it can work well for Arduino boards as well, I'm sure.

Then again, I'm not a programmer so what the hell do I know?

## SerialUI Library

### SerialUI::SerialUI()
Instantiate an object with the default banner and prompt
* _Default Banner: Graham Engineering Serial User Interface_
* _Default prompt: (gesui)_

__Example:__
```
SerialUI myUI = SerialUI();

void setup()
{
	myUI.init();
}

void loop()
{

}
```

Wow, that was difficult.

You can also set a custom prompt [SerialUI::SerialUI(String p)]

And even a custom prompt and banner [SerialUI::SerialUI(String p, String b)]

__Example:__
```
SerialUI myUI = SerialUI("C:\\", "Marcrosorft Worndorws Embedded");

void setup()
{
	myUI.init();
}

void loop()
{

}
```

### SerialUI::init()
Call to show initial banner and command prompt
* _Typically called once, in the setup()_

__Example:__
```
SerialUI myUI = SerialUI();

void setup()
{
	myUI.init();
}

void loop()
{

}
```

_You can override showing the banner by using the quickInit feature of the init(bool quickInit) overload_

__Example:__
```
SerialUI myUI = SerialUI();

void setup()
{
	myUI.init(true);
}

void loop()
{

}
```


### SerialUI::serialRead();
This command should be called on every loop iteration to check for user input via serial

* _Currently serialEvent() is not supported on ESP8266 (3 Mar 2017) so this is the way it is.  Deal with it._
* _Write your code in a non-blocking way -- avoid use of delay()_

Example:
```
SerialUI myUI = SerialUI();

void setup()
{
	myUI.init();
}

void loop()
{
	myUI.serialRead();
}
```

### SerialUI::addCmd(String keywd, uiFunctionPointer ptr)
Used to add a command to the user interface

* _keywd is the keyword the user will type (the command)_
* _ptr is the function name you want to call in your main sketch_

__Example:__
```
int myInt = 103;
SerialUI myUI = SerialUI();

void setup()
{
	Serial.begin(9600);
	while (!Serial){}
	myUI.addCmd("ShowValue", showVal);
}

void loop()
{
	ui.serialRead();
}

void showVal()
{
	Serial.println("The value of myInt is: " + String(myInt, DEC));
}
```

### SerialUI::rmCmd(String keywd)
Used to remove commands from the user interface.

* _Not sure why you would need it, but it exists_
* _keywd is the keyword the user will type (the command)_


__Example:__
```
int myInt = 103;
SerialUI myUI = SerialUI();

void setup()
{
	Serial.begin(9600);
	while (!Serial){}
	myUI.addCmd("ShowValue", showVal);
}

void loop()
{
	ui.serialRead();
}

void showVal()
{
	Serial.println("The value of myInt is: " + String(myInt, DEC));
}
```