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
		void passFunction(uiFunctionPointer functionPointer) ;
		void serialRead();
		void addCmd(String, uiFunctionPointer);
		void rmCmd(String);
		void init();
		

	private:
		static const int cmdCount = 10;
		
		uiPointer pointerList[cmdCount];
		void analyzeInput(String);
		String incomingString = "";
		int cmdUsed = 0;
		void showHelpMenu();
		bool checkCmd(String, String);
		String prompt = "gesui> ";

};

#endif