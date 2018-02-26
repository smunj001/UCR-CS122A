/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 10/5/2017 19:11:56 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/

int acOn = 0;
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_ACOFF, SM1_Cool, SM1_Heat } SM1_State;

TickFct_AirConditioning() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_ACOFF;
         break;
      case SM1_ACOFF:
         if (A5 == 1) {
            SM1_State = SM1_Cool;
         }
         else if (A6 == 1) {
            SM1_State = SM1_Heat;
         }
         else if (A5 == 0) {
            SM1_State = SM1_ACOFF;
         }
         break;
      case SM1_Cool:
         if (A5 == 0) {
            SM1_State = SM1_ACOFF;
         }
         break;
      case SM1_Heat:
         if (A6 == 0) {
            SM1_State = SM1_ACOFF;
         }
         break;
      default:
         SM1_State = SM1_ACOFF;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_ACOFF:
         acOn = 0;
         B0 = 0;
         B1 = 0;
         B2 = 0;
         B3 = 0;
         break;
      case SM1_Cool:
         B0 = 1;
         B1 = 1;
         acOn = 1;
         break;
      case SM1_Heat:
         B0 = 1;
         B2 = 1;
         acOn = 1;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM3_States { SM3_fanOff, SM3_Low, SM3_Medium, SM3_High } SM3_State;

TickFct_State_machine_3() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM3_State) { // Transitions
      case -1:
         SM3_State = SM3_fanOff;
         break;
      case SM3_fanOff:
         if (A0 == 1 || acOn == 1) {
            SM3_State = SM3_Low;
         }
         else if (A0 == 0) {
            SM3_State = SM3_fanOff;
         }
         break;
      case SM3_Low:
         if (A0 == 1 && A1 == 1) {
            SM3_State = SM3_Medium;
         }
         else if (A0 == 0 && acOn == 0) {
            SM3_State = SM3_fanOff;
         }
         else if (A0 == 1 || acOn == 1) {
            SM3_State = SM3_Low;
         }
         break;
      case SM3_Medium:
         if (A0 == 1 && A1 == 1 && A2 == 1) {
            SM3_State = SM3_High;
         }
         else if (A0 == 0) {
            SM3_State = SM3_fanOff;
         }
         else if (A0 == 1 && A1 == 1) {
            SM3_State = SM3_Medium;
         }
         break;
      case SM3_High:
         if (A0 == 1 && A1 == 0 && A2 == 0) {
            SM3_State = SM3_Low;
         }
         else if (A0 == 0) {
            SM3_State = SM3_fanOff;
         }
         else if (A0 == 1 && A1 == 1 && A2 == 1) {
            SM3_State = SM3_High;
         }
         break;
      default:
         SM3_State = SM3_fanOff;
      } // Transitions

   switch(SM3_State) { // State actions
      case SM3_fanOff:
         acOn = 0;
         B0 = 0;
         B1 = 0;
         B2 = 0;
         B3 = 0;
         break;
      case SM3_Low:
         B0 = 1;
         
         
         break;
      case SM3_Medium:
         B0 = 1;
         
         
         break;
      case SM3_High:
         B0 = 1;
         
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(1000);
   TimerOn();
   SM1_State = -1;
   SM3_State = -1;
   while(1) {
      TickFct_AirConditioning();
      TickFct_State_machine_3();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}