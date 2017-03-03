#include "Arduino.h"
#include "SerialUI.h"

SerialUI::SerialUI()
{
	// instantiate object
	// Serial output isn't called because the object should be instantiated before Serial.begin() is called.
	//	User can manually call SerialUI.init() to show initial command prompt and banner
}

SerialUI::SerialUI(String p)
{
	// Allow the capability to set a command prompt word (instead of default 'gesui')
	prompt = p + "> ";
}

SerialUI::SerialUI(String p, String b)
{
	// Set a custom command prompt and banner
	banner = b;
	prompt = p;
	
	// Format the prompt to end with "> " if it doesn't already.  
	prompt.trim();
	if (!prompt.endsWith(">"))
	{
		prompt += "> ";
	}
}

void SerialUI::init(bool QuickInit)
{
	// Initializes without showing the banner
	if (!QuickInit)
	{
		Serial.println(banner);
	}
	Serial.print(prompt);
}

void SerialUI::serialRead()
{
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
        incomingString += incomingByte;
      }
    }
  }
  if (stringComplete)
  {
    Serial.println(incomingString);
    analyzeInput(incomingString);
  }
}

bool SerialUI::checkCmd(String inputStr, String matchStr)
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

void SerialUI::analyzeInput(String str)
{
	incomingString = "";
	//str.toLowerCase();

	if (str == "help" || str == "?")
	{
		showHelpMenu();
	}
	else if (checkCmd(str, "addCmd"))
	{
		Serial.println("addCmd is an internal public member.\n\tPlease use SerialUI.addCmd(keyword, functionName) in code to add a new command.\n");
	}
	else if (checkCmd(str, "rmCmd"))
	{
		Serial.println("rmCmd is an internal public member.\n\tPlease use SerialUI.rmCmd(keyword) in code to remove a command.\n");
	}
	else
	{
		// Search through the array for used keywords
		for (int i = 0; i < cmdCount; i++)
		{
			if (checkCmd(str, pointerList[i].keyword))
			{
				//call the function
				pointerList[i].functionPointer();
				Serial.print(prompt);
				return;
			}
		}
		// pointer not found, show some error.
		Serial.println("ERROR - Command '" + str + "' not recognized.\n\tUse [ help ] to view list of commands\n");
		Serial.print(prompt);
		return;
		
	}
	str = "";
	Serial.print(prompt);
}

void SerialUI::showHelpMenu()
{
	const int itemsPerLine = 5;
	String keywordLine;

	Serial.println("\nHelp Menu");

	Serial.println("\tBuiltin Commands:");
	Serial.println("\t\thelp   addCmd   rmCmd\n");
	Serial.println("\tUser-Added Commands:");

	if (cmdUsed < 1)
	{
	Serial.println("\t\tnone");
	}
	else
	{
		for (int i = 0; i < cmdUsed; i++)
		{
			Serial.print("\t\t");
			Serial.println(pointerList[i].keyword);
		}
	}
	Serial.println("");
}

void SerialUI::addCmd(String keywd, uiFunctionPointer ptr)
{
	// look for the index of the keyword, make sure it doesn't exist.
	// add the keyword and funcptr to the arrays
	uiPointer addPointer;
	addPointer.value = cmdUsed;
	addPointer.keyword = keywd;
	addPointer.functionPointer = ptr;
	pointerList[cmdUsed] = addPointer;
	cmdUsed++;
}

void SerialUI::rmCmd(String keywd)
{
	// look for the index of the keyword
	//delete the keyword and the command
	// collapse the arrays?
}