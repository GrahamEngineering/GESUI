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
		void addCmd(String, uiFunctionPointer);
		void rmCmd(String);
		void init(bool QuickInit = false);
		

	private:
		static const int cmdCount = 10;
		String banner = "Graham Engineering Serial UI - Version 0.2";
		uiPointer pointerList[cmdCount];
		void analyzeInput(String);
		String incomingString = "";
		int cmdUsed = 0;
		void showHelpMenu();
		bool checkCmd(String, String);
		String prompt = "gesui> ";

};

#endif