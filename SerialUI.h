/*
	Graham Engineering Serial User Interface
		(gesui)
		Author: matthew.j.graham@gmail.com
	Create a simple command interface for your Arduino with callback functions
	
	See example sketch for usage
*/
#ifndef SerialUI_h
#define SerialUI_h

#include "Arduino.h"

typedef void (* uiFunctionPointer) ();

typedef struct uiPointer
{
  uiFunctionPointer functionPointer;
  int value;
  String keyword;
};


class SerialUI
{
	public:
		SerialUI();
		SerialUI(String);
		SerialUI(String, String);
		void serialRead();
		bool addCmd(String, uiFunctionPointer);
		bool rmCmd(String);
		void init(bool quickInit = false);
		

	private:
		static const int _cmdMax = 10;
		String _banner = "Graham Engineering Serial UI - Version 0.2";
		String _prompt = "gesui> ";
		uiPointer _pointerList[_cmdMax];
		String _incomingString = "";
		void _analyzeInput(String);
		int _cmdUsed = 0;
		void _showHelpMenu();
		bool _checkCmd(String, String);
		

};

#endif