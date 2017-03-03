/*
	Graham Engineering Serial User Interface
		(gesui)
		Author: matthew.j.graham@gmail.com
	Create a simple command interface for your Arduino with callback functions
*/
#include "Arduino.h"
#include "GESUI.h"

GESUI::GESUI()
{
	// instantiate object
	// Serial output isn't called because the object should be instantiated before Serial.begin() is called.
	//	User can manually call GESUI.init() to show initial command _prompt and _banner
}

GESUI::GESUI(String p)
{
	// Allow the capability to set a command _prompt word (instead of default 'gesui')
	_prompt = p + "> ";
}

GESUI::GESUI(String p, String b)
{
	// Set a custom command _prompt and _banner
	_banner = b;
	_prompt = p;
	
	// Format the _prompt to end with "> " if it doesn't already.  
	_prompt.trim();
	if (!_prompt.endsWith(">"))
	{
		_prompt += "> ";
	}
}

void GESUI::init(bool quickInit)
{
	// Initializes without showing the _banner
	if (!quickInit)
	{
		Serial.println(_banner);
	}
	Serial.print(_prompt);
}

void GESUI::serialRead()
{
	/*
		This is the main part of the class.  Call this on a loop to read serial input and act accordingly.
		Try to write your code as non-blocking as possible.
	*/

	char incomingByte;
	boolean stringComplete = false;
	
	while (Serial.available() > 0)
	{
		if (!stringComplete)
		{
			incomingByte = (char)Serial.read();

			if (incomingByte == '\n' || incomingByte == '\r')
			{
				stringComplete = true;
			}
			else
			{
				_incomingString += incomingByte;
			}
		}
	}
	
	if (stringComplete)
	{
		Serial.println(_incomingString);
		_analyzeInput(_incomingString);			// This is the next step: Process the command that was received
	}
}

void GESUI::_analyzeInput(String str)
{
	//str.toLowerCase();

	if (str == "")
	{
		// User has entered a carriage return
		Serial.println(_prompt);
		return;
	}
	else if (str == "help" || str == "?")
	{
		_showHelpMenu();
	}
	else if (_checkCmd(str, "addCmd"))
	{
		Serial.println("addCmd is an internal public member.\n\tPlease use GESUI.addCmd(keyword, functionName) in code to add a new command.\n");
	}
	else if (_checkCmd(str, "rmCmd"))
	{
		Serial.println("rmCmd is an internal public member.\n\tPlease use GESUI.rmCmd(keyword) in code to remove a command.\n");
	}
	else
	{
		// Search through the array for used keywords
		for (int i = 0; i < _cmdMax; i++)
		{
			if (_checkCmd(str, _pointerList[i].keyword))
			{
				//call the function
				_pointerList[i].functionPointer();
				Serial.print(_prompt);
				return;
			}
		}
		// pointer not found, show some error.
		Serial.println("ERROR - Command '" + str + "' not recognized.\n\tUse [ help ] to view list of commands\n");
		Serial.print(_prompt);
		return;
		
	}
}

bool GESUI::_checkCmd(String inputStr, String matchStr)
{
	if (inputStr == matchStr || inputStr.startsWith(matchStr + " "))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GESUI::_showHelpMenu()
{
	const int itemsPerLine = 5;
	String keywordLine;

	Serial.println("\nHelp Menu");

	Serial.println("\tBuiltin Commands:");
	Serial.println("\t\thelp   addCmd   rmCmd\n");
	Serial.println("\tUser-Added Commands:");

	if (_cmdUsed < 1)
	{
	Serial.println("\t\tnone");
	}
	else
	{
		for (int i = 0; i < _cmdUsed; i++)
		{
			Serial.print("\t\t");
			Serial.println(_pointerList[i].keyword);
		}
	}
	Serial.println("");
}

bool GESUI::addCmd(String keywd, uiFunctionPointer ptr)
{
	// look for the index of the keyword, make sure it doesn't exist.
	// add the keyword and funcptr to the arrays
	bool cmdExists = false;
	
	// Look through existing list of commands to see if this keyword is already used
	for (int i = 0; i < _cmdUsed; i++)
	{
		if (_pointerList[i].keyword == keywd)
		{
			cmdExists = true;
		}
	}
	
	if (!cmdExists)
	{
		// The cmd does not yet exist, add it.
		uiPointer addPointer;
		addPointer.value = _cmdUsed;
		addPointer.keyword = keywd;
		addPointer.functionPointer = ptr;
		_pointerList[_cmdUsed] = addPointer;
		_cmdUsed++;
		return true;
	}
	else
	{
		// The cmd already exists.
		return false;
	}
}

bool GESUI::rmCmd(String keywd)
{
	/*
		Testing data from Example .ino
		_cmdUsed = 4
		_cmdMax = 10;
		
		_pointerList[0] = enableBlink (_cmdUsed = 1)
		_pointerList[1] = remthis (_cmdUsed = 2)
		_pointerList[2] = disableBlink (_cmdUsed = 3)
		_pointerList[3] = statBlink (_cmdUsed = 4)
		_pointerList[4] = statBlink (second) (_cmdUsed = 5) // this one wasn't added because it was a duplicate
		
	*/
	uiPointer emptyPointer = uiPointer();
	bool foundCmd = false;
	int cmdIndex;
	
	for (int i = _cmdUsed; i > 0; i--)
	{
		cmdIndex = (i - 1);
		
		/*
		Serial.println("cmdIndex: " + String(cmdIndex, DEC));
		
		Serial.println("Keyword: " + _pointerList[cmdIndex].keyword);
		*/
		
		if (_pointerList[cmdIndex].keyword == keywd && (!foundCmd))
		{
			// This is the one to remove.
			foundCmd = true;
			_pointerList[cmdIndex] = emptyPointer;
			break;
		}
	}
	
	if (foundCmd)	// the command is in the list, remove it.
	{
		// collapse array - find quicker solution
		for (int j = cmdIndex; j < (_cmdUsed - 1); j++)
		{
			// Bring the next cmd down 1 spot in the list
			_pointerList[j] = _pointerList[ (j + 1) ];
		}
		
		// Remove the last cmd in the list
		_pointerList[ (_cmdUsed ) ] = emptyPointer;
		
		// Decrement total number of cmds
		_cmdUsed--;
		
		return true;
	}
	else
	{
		// No matching keyword was found in the stuct.  Return false.
		return false;
	}
}