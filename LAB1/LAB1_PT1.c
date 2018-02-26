/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 10/5/2017 19:10:27 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/


typedef struct task {
   int state;
   unsigned long period;
   unsigned long elapsedTime;
   int (*TickFct)(int);
} task;

task tasks[4];

const unsigned char tasksNum = 4;
const unsigned long periodBouncing_LED = 1000;
const unsigned long periodPattern_Sequencer = 1000;
const unsigned long periodState_machine_4 = 1000;
const unsigned long periodMusic = 700;

const unsigned long tasksPeriodGCD = 100;

int TickFct_Bouncing_LED(int state);
int TickFct_Pattern_Sequencer(int state);
int TickFct_State_machine_4(int state);
int TickFct_Music(int state);

unsigned char processingRdyTasks = 0;
void TimerISR() {
   unsigned char i;
   if (processingRdyTasks) {
      printf("Period too short to complete tasks\n");
   }
   processingRdyTasks = 1;
   for (i = 0; i < tasksNum; ++i) { // Heart of scheduler code
      if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
         tasks[i].state = tasks[i].TickFct(tasks[i].state);
         tasks[i].elapsedTime = 0;
      }
      tasks[i].elapsedTime += tasksPeriodGCD;
   }
   processingRdyTasks = 0;
}
int main() {
   // Priority assigned to lower position tasks in array
   unsigned char i=0;
   tasks[i].state = -1;
   tasks[i].period = periodBouncing_LED;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFct_Bouncing_LED;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = periodPattern_Sequencer;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFct_Pattern_Sequencer;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = periodState_machine_4;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFct_State_machine_4;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = periodMusic;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFct_Music;

   ++i;
   TimerSet(tasksPeriodGCD);
   TimerOn();
   
   while(1) { Sleep(); }

   return 0;
}

enum SM1_States { SM1_s1, SM1_s2, SM1_s3, SM1_s4, SM1_s5, SM1_s6, SM1_s7, SM1_wait, SM1_s8, SM1_begin } SM1_State;
int TickFct_Bouncing_LED(int state) {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(state) { // Transitions
      case -1:
         state = SM1_begin;
         break;
      case SM1_s1:
         if ((A6 == 0 && A7 == 0) && A0 == 0) {
            state = SM1_s2;
         }
         else if ((A6 == 0 && A7 == 0) && A0 == 1) {
            state = SM1_s8;
         }
         else if (!(A6 == 0 && A7 == 0)) {
            state = SM1_wait;
         }
         break;
      case SM1_s2:
         if ((A6 == 0 && A7 == 0) && A0 == 0) {
            state = SM1_s3;
         }
         else if ((A6 == 0 && A7 == 0) && A0 == 1) {
            state = SM1_s1;
         }
         else if (!(A6 == 0 && A7 == 0)) {
            state = SM1_wait;
         }
         break;
      case SM1_s3:
         if ((A6 == 0 && A7 == 0) && A0 == 0) {
            state = SM1_s4;
         }
         else if ((A6 == 0 && A7 == 0) && A0 == 1) {
            state = SM1_s2;
         }
         else if (!(A6 == 0 && A7 == 0)) {
            state = SM1_wait;
         }
         break;
      case SM1_s4:
         if ((A6 == 0 && A7 == 0) && A0 == 0) {
            state = SM1_s5;
         }
         else if ((A6 == 0 && A7 == 0) && A0 == 1) {
            state = SM1_s3;
         }
         else if (!(A6 == 0 && A7 == 0)) {
            state = SM1_wait;
         }
         break;
      case SM1_s5:
         if ((A6 == 0 && A7 == 0) && A0 == 0) {
            state = SM1_s6;
         }
         else if ((A6 == 0 && A7 == 0) && A0 == 1) {
            state = SM1_s4;
         }
         else if (!(A6 == 0 && A7 == 0)) {
            state = SM1_wait;
         }
         break;
      case SM1_s6:
         if ((A6 == 0 && A7 == 0) && A0 == 0) {
            state = SM1_s7;
         }
         else if ((A6 == 0 && A7 == 0) && A0 == 1) {
            state = SM1_s5;
         }
         else if (!(A6 == 0 && A7 == 0)) {
            state = SM1_wait;
         }
         break;
      case SM1_s7:
         if ((A6 == 0 && A7 == 0) && A0 == 1) {
            state = SM1_s6;
         }
         else if ((A6 == 0 && A7 == 0) && A0 == 0) {
            state = SM1_s8;
         }
         else if (!(A6 == 0 && A7 == 0)) {
            state = SM1_wait;
         }
         break;
      case SM1_wait:
         if ((A6 == 0 && A7 == 0)) {
            state = SM1_s1;
         }
         break;
      case SM1_s8:
         if ((A6 == 0 && A7 == 0) && A0 == 1) {
            state = SM1_s7;
         }
         else if ((A6 == 0 && A7 == 0) && A0 == 0) {
            state = SM1_s1;
         }
         else if (!(A6 == 0 && A7 == 0)) {
            state = SM1_wait;
         }
         break;
      case SM1_begin:
         if (A0 == 0) {
            state = SM1_s1;
         }
         break;
      default:
         state = -1;
      } // Transitions

   switch(state) { // State actions
      case SM1_s1:
         B0 = 1;
         B7 = 0;
         B1 = 0;
         
         break;
      case SM1_s2:
         B0 = 0;
         B1 = 1;
         B2 = 0;
         break;
      case SM1_s3:
         B1 = 0;
         B2 = 1;
         B3 = 0;
         break;
      case SM1_s4:
         B2 = 0;
         B3 = 1;
         B4 = 0;
         break;
      case SM1_s5:
         B3 = 0;
         B4 = 1;
         B5 = 0;
         break;
      case SM1_s6:
         B4 = 0;
         B5 = 1;
         B6 = 0;
         break;
      case SM1_s7:
         B5 = 0;
         B6 = 1;
         B7 = 0;
         break;
      case SM1_wait:
         break;
      case SM1_s8:
         B6 = 0;
         B7 = 1;
         B0 = 0;
         break;
      case SM1_begin:
         B0 = 0;
         B1 = 0;
         B2 = 0;
         B3 = 0;
         B4 = 0; 
         B5 = 0;
         B6 = 0;
         B7 = 0;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
   SM1_State = state;
   return state;
}


enum SM2_States { SM2_wait, SM2_s2, SM2_s3, SM2_s4, SM2_s5, SM2_s8, SM2_s7, SM2_s6, SM2_s9, SM2_s10, SM2_s11 } SM2_State;
int TickFct_Pattern_Sequencer(int state) {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
int arr[10];
arr[0] = 0xAB;
arr[1] = 0xFF;
arr[2] = 0x78;
arr[3] = 0xCF;
arr[4] = 0xFA;
arr[5] = 0xDB;
arr[6] = 0x12;
arr[7] = 0xEE;
arr[8] = 0xBC;
arr[9] = 0xAD;
   switch(state) { // Transitions
      case -1:
         state = SM2_wait;
         break;
      case SM2_wait:
         if ((A6 == 0 && A7 == 1)) {
            state = SM2_s2;
         }
         break;
      case SM2_s2:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s3;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s11;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s3:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s4;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s2;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s4:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s5;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s3;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s5:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s6;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s4;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s8:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s9;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s7;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s7:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s8;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s6;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s6:
         if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s5;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s7;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s9:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s10;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s8;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s10:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s11;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s9;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      case SM2_s11:
         if ((A6 == 0 && A7 == 1) && A0 == 0) {
            state = SM2_s2;
         }
         else if ((A6 == 0 && A7 == 1) && A0 == 1) {
            state = SM2_s10;
         }
         else if (!(A6 == 0 && A7 == 1)) {
            state = SM2_wait;
         }
         break;
      default:
         state = -1;
      } // Transitions

   switch(state) { // State actions
      case SM2_wait:
         break;
      case SM2_s2:
         B = arr[0];
         break;
      case SM2_s3:
         B = arr[1];
         break;
      case SM2_s4:
         B = arr[2];
         break;
      case SM2_s5:
         B = arr[3];
         break;
      case SM2_s8:
         B = arr[6];
         break;
      case SM2_s7:
         B = arr[5];
         break;
      case SM2_s6:
         B = arr[4];
         break;
      case SM2_s9:
         B = arr[7];
         break;
      case SM2_s10:
         B = arr[8];
         break;
      case SM2_s11:
         B = arr[9];
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
   SM2_State = state;
   return state;
}


enum SM4_States { SM4_Wait, SM4_S1 } SM4_State;
int TickFct_State_machine_4(int state) {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(state) { // Transitions
      case -1:
         state = SM4_Wait;
         break;
      case SM4_Wait:
         if (!(A6 == 1 && A7 == 1)) {
            state = SM4_Wait;
         }
         else if ((A6 == 1 && A7 == 1)) {
            state = SM4_S1;
         }
         break;
      case SM4_S1:
         if (!(A6 == 1 && A7 == 1)) {
            state = SM4_Wait;
         }
         else if ((A6 == 1 && A7 == 1)) {
            state = SM4_S1;
         }
         break;
      default:
         state = -1;
      } // Transitions

   switch(state) { // State actions
      case SM4_Wait:
         break;
      case SM4_S1:
         B = 0xFF;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
   SM4_State = state;
   return state;
}


enum SM5_States { SM5_Wait, SM5_PulseON, SM5_Beat, SM5_PulseOFF } SM5_State;
int TickFct_Music(int state) {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/

static int cnt = 0;
static int i = 0;
static int p = 24;
int arr[] = {0x01, 0x05, 0xA8, 0x01, 0x05, 0xA8, 0xFF, 0x01, 0x03, 0x9, 0xF8};

   switch(state) { // Transitions
      case -1:
         state = SM5_Wait;
         break;
      case SM5_Wait:
         if ((A6 == 1 && A7 == 0)) {
            state = SM5_PulseON;
         }
         break;
      case SM5_PulseON:
         if (!(A6 == 1 && A7 == 0)) {
            state = SM5_Wait;
         }
         else if (cnt < 7) {
            state = SM5_PulseOFF;
         }
         else if (1) {
            state = SM5_PulseOFF;
         }
         else if (cnt >= 7) {
            state = SM5_Beat;
         }
         break;
      case SM5_Beat:
         if (p == cnt || !(A6 == 1 && A7 == 0)) {
            state = SM5_Wait;
         }
         else if (p != cnt) {
            state = SM5_Beat;
         }
         break;
      case SM5_PulseOFF:
         if (cnt < 7) {
            state = SM5_PulseON;
         }
         else if (!(A6 == 1 && A7 == 0)) {
            state = SM5_Wait;
         }
         else if (cnt >= 7) {
            state = SM5_Beat;
         }
         break;
      default:
         state = -1;
      } // Transitions

   switch(state) { // State actions
      case SM5_Wait:
         cnt = 0;
         
         break;
      case SM5_PulseON:
         B = 0xA5;
         cnt++;
         break;
      case SM5_Beat:
         B = arr[i];
         
         if(i == 11)
         {
              i = 0;
         }
         else
         {
              i++;
         }
         cnt++;
         
         break;
      case SM5_PulseOFF:
         B = 0x00;
         cnt++;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
   SM5_State = state;
   return state;
}

