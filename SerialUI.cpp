#include "Arduino.h"
#include "SerialUI.h"

SerialUI::SerialUI()
{
	// instantiate object
}

SerialUI::SerialUI(String p)
{
	// instantiate object
	prompt = p + "> ";
}

void SerialUI::init()
{
	Serial.println("Graham Engineering Serial UI - Version 0.2");
	Serial.print(prompt);
}

void SerialUI::passFunction(uiFunctionPointer functionPointer) 
{
  // Call the passed function pointer
  functionPointer();
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

	if (str == "help")
	{
		showHelpMenu();
	}
	else if (checkCmd(str, "addCmd"))
	{
		Serial.println("addCmd is an internal public member.  Please use SerialUI.addCmd(keyword, functionName) in code to add a new command.\n");
	}
	else if (checkCmd(str, "rmCmd"))
	{
		Serial.println("rmCmd is an internal public member.  Please use SerialUI.rmCmd(keyword) in code to remove a command.\n");
	}
	else
	{
		// Search through the array for used keywords
		for (int i = 0; i < sizeof(pointerList); i++)
		{
			if (checkCmd(str, pointerList[i].keyword))
			{
				//call the function
				pointerList[i].functionPointer();
				Serial.println(prompt);
				return;
			}
		}
		// pointer not found, show some error.
		Serial.println("ERROR - Command [ " + str + " ] not recognized.  Use [ help ] to view list of commands\n");
		Serial.println(prompt);
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

	Serial.println("\t[ Builtin Commands ]");
	Serial.println("\t\thelp   addCmd   rmCmd\n");
	Serial.println("\t[ User-Added Commands ]");

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