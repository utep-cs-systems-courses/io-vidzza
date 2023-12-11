#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"
#include "notes.h"
//DISCLAIMER: the songs does not quite get to the original song
//temp variable (testing purposes)
int temp = 0;

//Holds the switches states 1 or 0
int* states;

//Holds if megalovania or shooting stars
char play_m = ' ';

//Notes for Megalovania and note length
int mMelody[] = {G,0,D,0,D,0,G,0,D,0,C,0,F,0,D,0,C,0,G,0,D,0,};
float mDuration[] = {.14,.14,.200,.14,.200,.14,.120,.14,.120,.14,.120,.14,.251,.14,.121,.14,.128,.14,.121,.14,.242,.14};

//Notes for Shy Away
int shMelody[]={F,0,E,0,F,0,E,0,F,0,E,0,};
float shDuration[] ={.150,.5,.150,.5,.150,.5,.150,.5,.100,.5,.150,.5,};

//Notes for Neongravestones by Twenty One Pilots
int sMelody[] = {B,0,D,0,F2,0,B,0,D,0,F2,0,C2,0,E,0,F2,0,C2,0,E,0,F2,0,};
float sDuration[] = {.35,.5,.25,.5,.25,.5,.35,.5,.25,.5,.25,.5};

void play_neonGravestones(int melody[], float duration[],char play_m){

  if (play_m == 'n') {
    play_note(melody[temp] * 3,duration[temp]);
    temp++;
    if(temp >  7){
      temp = 0;
      play_neonGravestones(melody,duration,' ');
      return;
    }else{
      play_neonGravestones(melody,duration,'n');
    }
  }
    
}
// Shy away by twenty one pilots
void play_shy_away(int melody[], float duration[],char play_m){

  if (play_m == 's') {
    play_note(melody[temp] * 3,duration[temp]);
    temp++;
    if(temp >  12){
      temp = 0;
      play_shy_away(melody,duration,' ');
      return;
    }else{
      play_shy_away(melody,duration,'s');
    }
  }
    
}

void play_megalovania(int melody[], float duration[],char play_m){
  
  if (play_m == 'm') {
    play_note(melody[temp] * 4,duration[temp]);
    temp++;
    if(temp > 18) {
      temp = 0;
      play_megalovania(melody,duration,' ');
      return;
    }else{
      play_megalovania(melody,duration,'m');
    }
  }
    
}


int main() {
  configureClocks();
  led_init();
  switch_init();
  buzzer_init();
  enableWDTInterrupts();
  or_sr(0x18); // CPU off, GIE on

}

// global state var to count time
int reset_switches = 0;
int secondCount = 0;
int case_int = 0;



void __interrupt_vec(WDT_VECTOR) WDT(){    /* 250 interrupts/sec */

  states = get_switches_states();

  //Blinks both lights 10 times every half second with the switch in the red board

  if(states[0]){
    secondCount++;
    if(secondCount >= 60){
      reset_switches++;
      toggle_red();
      toggle_green();
      secondCount = 0;
      if(reset_switches >= 20){
	play_note(1000,3);
	reset_switches = 0;
	set_switches_states();
      }
    }
  }
  else if(states[1]){
      if(secondCount >= 125){
	case_int++;
	secondCount = 0;
      }
      switch(case_int){
      case 0:
	if(!secondCount){
	  toggle_red();
	}
      break;
      case 1:
	if(!secondCount){
	  toggle_green();
	}
      break;
      case 2:
	if(!secondCount){
	  toggle_red();
	  toggle_green();
	}
      break;
      case 3:
	if(!secondCount){
	  toggle_red();
	  toggle_green();
	}
      break;
      case 4:
	if(!secondCount){
	  toggle_red();
	  toggle_green();
	}
      break;
      case 5:
	if(!secondCount){
	  set_switches_states();
	  case_int = 0;
	}
	break;
      }
      secondCount++;
    }
  

  else if(states[2]){
    play_megalovania(mMelody,mDuration,'m');
    secondCount++;
    if(secondCount >= 125){
      reset_switches++;
      toggle_green();
      secondCount = 0;
      if(reset_switches >= 20){
  	reset_switches = 0;
  	set_switches_states();
      }
    }
  }
  

  
  else if(states[3]){
    toggle_green();
    play_shy_away(shMelody,shDuration,'s');
    play_shy_away(shMelody,shDuration,'s');
    toggle_green();
    set_switches_states();
   }

  else if(states[4]){
    toggle_green();
    play_neonGravestones(sMelody,sDuration,'n');
    play_neonGravestones(sMelody,sDuration,'n');
    play_neonGravestones(sMelody,sDuration,'n');
    play_neonGravestones(sMelody,sDuration,'n');
    toggle_green();
    set_switches_states();
  }
}
