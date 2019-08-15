//
//
//JoystickButtonTimer.cpp : 
// Measure time in milliseconds between pressing and releasing a Joystick Button
//
//

#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <math.h>

LARGE_INTEGER frequency;        // ticks per second
LARGE_INTEGER t1, t2;           // ticks
DWORD elapsedTi, ti1, ti2;
double elapsedTime;
int pressButton = 0;
int pressFlag = 0;
int iButton;
char cButton[3];



int main( int argc, char *argv[])
{

	printf("\n");
	printf("Display duration how long a joystick iButton has been pressed and not pressed. \n");
	printf("Program is configured for only one joystick attached on the system. \n");
	printf("\n");
	printf("Usage:\n");
	printf(" -f<Number of Joystick Button>\n");
	printf("Joystick Button counting starts with 0\n");
	printf("Terminate with Ctrl-C\n");
	
	printf("Program name: %s\n", argv[0]);

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'f':
				printf("Joystick Button: %s\n",&argv[1][2]);
				sprintf(cButton,"%s", &argv[1][2]);
				break;

			case 'd':
				printf("%s\n",&argv[1][2]);
				printf("%s\n",&argv[1][2]);
				break;

			default:
				printf("Wrong Argument: %s\n", argv[1]);
				printf("Usage:\n");
				printf(" -f<Number of Joystick Button>\n");
				printf("Joystick Button counting starts with 0\n");
				//printf(" -d<name>\n");
				exit (8);
		}

		++argv;
		--argc;
	}

	printf("\n");
	printf("\n");


   // The captured data will be written to the following struct:
   //typedef struct joyinfo_tag {
   //   UINT wXpos;
   //   UINT wYpos;
   //   UINT wZpos;
   //   UINT wButtons;
   //} JOYINFO,*PJOYINFO,*LPJOYINFO;
   JOYINFO joystickInfo;

   // The ID of the joystick you are using. If there is only one joystick use JOYSTICKID1.
   UINT joystickId = JOYSTICKID1;

   // get ticks per second
   QueryPerformanceFrequency(&frequency);
   
   iButton = atoi(cButton);
   iButton = pow(2, (double)iButton);

   while (1)
   {

	   MMRESULT errorCode = joyGetPos(joystickId, &joystickInfo);
	   switch(errorCode)
	   { 

		   case JOYERR_NOERROR: // No error. joystickInfo now contains contains the captured data.
		   {
			   //printf("The X position (left/right tilt) is %u\n", joystickInfo.wXpos);
			   //printf("The Y position (up/down tilt) is %u\n", joystickInfo.wYpos);
			   //printf("The Z position (usually the throttle) is %u\n", joystickInfo.wZpos);

			   // These values range from 0 to 65536. 32768 is the centre position.

			   // Test Button 1. You can do the same for JOY_Button2, JOY_Button3 etc.
			   // wButtons is a UNINT that is the OR of all pressed iButton flags.
		
			   if(joystickInfo.wButtons & iButton)
			   {
				   pressButton = 1;
			   }
			   else
			   {
				   pressButton = 0;
			   }
			   break;
		   }

		   case JOYERR_PARMS: fprintf(stderr, "Invalid parameters to joyGetPos."); break;
		   case JOYERR_NOCANDO: fprintf(stderr, " Failed to capture joystick input."); break;
		   case JOYERR_UNPLUGGED: fprintf(stderr, "The joystick identified by joystickId isn't plugged in."); break;
		   case MMSYSERR_NODRIVER: fprintf(stderr, "No (active) joystick driver available."); break;
		
	   }

	   if (pressButton && (!pressFlag))												//rising (on) edge
	   {
			pressFlag = 1;
			QueryPerformanceCounter(&t1);
			//ti1 = timeGetTime();
			printf("Button was not pressed for %d milliseconds \n", (t1.QuadPart - t2.QuadPart) *1000 / (frequency.QuadPart));
	   }
	   if (pressFlag && (!pressButton))												//falling (off) edga
	   {
			QueryPerformanceCounter(&t2);
			//ti2 =  timeGetTime();
			printf("Button was pressed for %d milliseconds \n", (t2.QuadPart - t1.QuadPart) *1000 / (frequency.QuadPart));
			//printf("Button 1 was pressed. %d \n", (ti2- ti1));
			pressFlag = 0;
	   }
	  // Sleep (10);
   }

}