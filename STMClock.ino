


/*
   Contains code from HobbyComponents.com

   You may copy, alter and reuse this code in any way you like, but please leave
  reference to HobbyComponents.com in your comments if you redistribute this code.
  This software may not be used directly for the purpose of promoting products that
  directly compete with Hobby Components Ltd's own range of products.

  THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES,
  WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR
  LACK OF NEGLIGENCE. HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE
  FOR ANY DAMAGES INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR
  CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.



*/

/*
    To setup BluePill:

    https://circuitdigest.com/microcontroller-projects/getting-started-with-stm32-development-board-stm32f103c8-using-arduino-ide

*/


#include "HardwareTimer.h"
#include "Setup.h"

#include "Display.h"
#include "LFO.h"

//Low level routines to handle the timers.
//See https://www.st.com/content/ccc/resource/training/technical/product_training/c4/1b/56/83/3a/a1/47/64/STM32L4_WDG_TIMERS_GPTIM.pdf/files/STM32L4_WDG_TIMERS_GPTIM.pdf/jcr:content/translations/en.STM32L4_WDG_TIMERS_GPTIM.pdf
//Page 31 and 32

double Div = 1 / 72.0;

//This one used for Beat Timer
void CalcTCounters( long Time, PWMStruct *PWMS, long Pm1) {
  PWMS->PWM1 = Pm1 / 500;
  PWMS->PWM2 = (Time - Pm1) / 500;
  /*
    DisplayTime(Time, 3);
    DisplayTime(Pm1, 4);
    DisplayTime(PWMS->PWM1, 5);
    DisplayTime(PWMS->PWM2, 6);
  */
}

void SetUpTimer(ClockStruct *Clock, HardwareTimer myTimer, voidFuncPtr handler, timer_mode tm) {
  long Time = 500;  //Set to 2Khz master clock
  myTimer.pause();
  myTimer.setMode(TIMER_CH1, tm);
  myTimer.attachInterrupt(TIMER_CH1, handler);
  Clock->Psc = 1;
  Clock->Arr = 0;
  Clock->Arr = Time / (Div * Clock->Psc);
  while (Clock->Arr >= 65535) {
    Clock->Psc = Clock->Psc + 1;
    Clock->Arr = Time / (Div * Clock->Psc);

  }
  DisplayTime(Clock->Psc, 3);
  DisplayTime(Clock->Arr, 4);
  myTimer.setPrescaleFactor(Clock->Psc);
  myTimer.setOverflow(Clock->Arr - 1);
  myTimer.setCompare(TIMER_CH1, Clock->Arr - 1);
  myTimer.refresh();

}


void SetupSwitches() {
  pinMode(BUT1, INPUT);
  pinMode(BUT2, INPUT);
  pinMode(STARTBUT, INPUT);
  pinMode(BUTLED3, OUTPUT);
  pinMode(START, INPUT);
  pinMode(RESET, INPUT);

  attachInterrupt(BUT2, iMode, FALLING);
  attachInterrupt(BUT1, iTempo, FALLING);
attachInterrupt(START, IntReset, RISING);
 
  attachInterrupt(RESET, IntReset, RISING);
}

void SetupTimers() {
  //These set the timer modes to be correct for accurate timing
  // See http://docs.leaflabs.com/static.leaflabs.com/pub/leaflabs/maple-docs/latest/lang/api/hardwaretimer.html

  //SetUpTimer(SequenceTimer, SequenceGateOn, TIMER_OUTPUT_COMPARE);
  SetUpTimer(&MainClock, MainTimer, MasterClockInterrupt, TIMER_OUTPUT_COMPARE);
  setTempo(Tempo);  //Will also calculate timers.
  BeatPWMS.PWM1Counter = 0;
  BeatPWMS.PWM2Counter = 0;
  BeatPWMS.Phase = 0;
  RachetPWMS.PWM1Counter = 0;
  RachetPWMS.PWM2Counter = 0;
  RachetPWMS.Phase = 0;
  MainTimer.resume();

}

boolean rb=false;
void IntReset() {
  oscReset();
  digitalWrite(BUTLED3, rb);
  rb=!rb;
  
}



void setup() {
  //flash(5);
  SetupOLED();
  ReadEEPROM();
  SetupTimers();
  setTempo(Tempo);
  SetupEncoders();
  SetupDacs();
  SetupSwitches();
  SetupGates();
  setShape(0);
  //setShape(1);
  digitalWrite(BUTLED3, false);
  DisplayBackground();
  SetFrequency(Tempo, CurrentNotePos[0], 0);
  SetFrequency(Tempo, CurrentNotePos[1], 1);
  SetVolume(Volume[0],0);
SetVolume(Volume[1],1);
DisplayWaveShape(0,oscParams[0].WaveShape);
DisplayWaveShape(1,oscParams[0].WaveShape);
}


void loop() {
  handleEnc1();
  handleEnc2();
}

boolean TState = false;
short TCount = 0;
void MasterClockInterrupt() {

  if (Debug == true)
    digitalWrite(Gate1, TState);
  BeatGate();
  RachetGate();
  TState = !TState;


  Osc(0);
  Osc(1);
}

void BeatGate(void) {
  if (BeatPWMS.Phase == 0) { //first phase;
    digitalWrite(BUTLED3, HIGH);
    if (Debug == false)
      digitalWrite(Gate1, HIGH);
    BeatPWMS.PWM1Counter++;
    if (BeatPWMS.PWM1Counter > BeatPWMS.PWM1) {
      BeatPWMS.Phase = 1;
      BeatPWMS.PWM2Counter = 1;
      if (Debug == false)
        digitalWrite(Gate1, LOW);
      digitalWrite(BUTLED3, LOW);
    }
  } else {

    if (Debug == false)
      digitalWrite(Gate1, LOW);
    digitalWrite(BUTLED3, LOW);
    BeatPWMS.PWM2Counter++;
    if (BeatPWMS.PWM2Counter > BeatPWMS.PWM2) {
      BeatPWMS.Phase = 0;
      if (Debug == false)
        digitalWrite(Gate1, HIGH);
      digitalWrite(BUTLED3, HIGH);
      BeatPWMS.PWM1Counter = 1;

    }
  }


}

void RachetGate(void) {
  if (RachetPWMS.Phase == 0) { //first phase;

    digitalWrite(Gate2, HIGH);
    RachetPWMS.PWM1Counter++;
    if (RachetPWMS.PWM1Counter > RachetPWMS.PWM1) {
      RachetPWMS.Phase = 1;
      RachetPWMS.PWM2Counter = 1;
      digitalWrite(Gate2, LOW);

    }
  } else {


    digitalWrite(Gate2, LOW);
    RachetPWMS.PWM2Counter++;
    if (RachetPWMS.PWM2Counter > RachetPWMS.PWM2) {
      RachetPWMS.Phase = 0;
      digitalWrite(Gate2, HIGH);
      RachetPWMS.PWM1Counter = 1;

    }
  }


}




void setTempo(int newTempo) {
  Tempo = newTempo;
  RachetTime = 1000000L * 60 / (Ratio * Tempo);
  CalcTCounters(RachetTime, &RachetPWMS, (long)SeqGateTime);
  BeatTime = 1000000L * 60 / Tempo;
  //DisplayTime (BeatTime );
  CalcTCounters(BeatTime, &BeatPWMS, (long)ms15);
  //CalcTCounters(BaseTime, MainTimer, &SequencePWMS, SeqGateTime);
  Tempo2 = Ratio * Tempo;
  DisplayTempo() ;
  SetFrequency(Tempo, CurrentNotePos[0], 0);
  SetFrequency(Tempo, CurrentNotePos[1], 1);
}

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;
const long interval = 500;


void iTempo() {
  noInterrupts();
  currentMillis = millis();
  //DisplayTime(currentMillis,previousMillis);
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Tempo = Tempo * 2;
    if (Tempo > MaxTempo) {
      Tempo = MinTempo;
    }
    setTempo(Tempo);

  }
  interrupts();
}


void iMode() {
  noInterrupts();
  currentMillis = millis();
  //DisplayTime(currentMillis,previousMillis);
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Mode++;
    if (Mode > MaxMode) {
      Mode = 0;
    }

    DisplayMode();
  }
  interrupts();
}


int encodeVal(int val,int Diff) {
  encAVal = digitalRead(encA);
  encBVal = digitalRead(encB);
  if (encAVal != encALast[Mode]) {
    if (encAVal == encBVal) {
      val=val-Diff;
    } else {
      val=val+Diff;
    }
    encALast[Mode] = encAVal;

  }
  return val;
}



int encodeVal2(int val,int Diff) {
  encAVal2 = digitalRead(enc2A);
  encBVal2 = digitalRead(enc2B);
  if (encAVal2 != encALast2[Mode]) {
    if (encAVal2 == encBVal2) {
      val=val-Diff;
    } else {
      val=val+Diff;
    }
    encALast2[Mode] = encAVal2;

  }
  return val;
}
void handleEnc1() {
  switch (Mode) {

    case 0:  //Tempo
      NewEncPos = encodeVal(Tempo,1);
      if (NewEncPos != Tempo) {
        if (NewEncPos < MinTempo) {
          NewEncPos = MinTempo;
        }
        if (NewEncPos > MaxTempo ) {
          NewEncPos = MaxTempo;
        }
        setTempo(NewEncPos);
      }
      break;
    case 1: //Ratio of gate pulses for ratching
      NewEncPos = encodeVal(currentRatioPos,1);
      if (NewEncPos != currentRatioPos) {
        if (NewEncPos < 0) {
          NewEncPos = 0;
        }
        if (NewEncPos >= maxRatio ) {
          NewEncPos = maxRatio;
        }
        currentRatioPos = NewEncPos;
        Ratio = Ratios[NewEncPos];
        DisplayRatio(Ratio);
        Tempo2 = Ratio * Tempo;
        setTempo(Tempo);
      }
      break;
    case 2: //Frequency of F1
      NewEncPos = encodeVal(CurrentNotePos[0],1);
      if (NewEncPos != CurrentNotePos[0]) {
        if (NewEncPos < 0) {
          NewEncPos = 0;
        }
        if (NewEncPos >= maxRatioCount ) { //See lfo.h
          NewEncPos = maxRatioCount - 1;
        }
        CurrentNotePos[0] = NewEncPos;
        SetFrequency(Tempo, CurrentNotePos[0], 0);
      }
      break;
    case 3: //Frequency of F2
      NewEncPos = encodeVal(CurrentNotePos[1],1);
      if (NewEncPos != CurrentNotePos[1]) {
        if (NewEncPos < 0) {
          NewEncPos = 0;
        }
        if (NewEncPos >= maxRatioCount ) { //See lfo.h
          NewEncPos = maxRatioCount - 1;
        }
        CurrentNotePos[1] = NewEncPos;
        SetFrequency(Tempo, CurrentNotePos[1], 1);
      }
      break;
    case 4:  //Waveshape F1
       NewEncPos = encodeVal(oscParams[0].WaveShape,1);
       if (NewEncPos != oscParams[0].WaveShape) {
        if (NewEncPos < 0) {
          NewEncPos = 0;
        }
        if (NewEncPos >= 3) { //See lfo.h
          NewEncPos = 3;
        }
        setWaveShape(0, NewEncPos);

      }
      break;
    case 5:  //Waveshape F1
       NewEncPos = encodeVal(oscParams[0].Shape,20);
       if (NewEncPos != oscParams[0].Shape) {
        if (NewEncPos < 0) {
          NewEncPos = 0;
        }
        if (NewEncPos >= 4094) { //See lfo.h
          NewEncPos = 4094;
        }
        oscParams[0].Shape = NewEncPos;
        setShape(0);
        
      }
      break;  
    default: break;
  }
}



void handleEnc2() {
  long Time = 1000000L * 60 / Tempo;
  switch (Mode) {

    case 0:  //Tempo

      NewEncPos2 = encodeVal2(GateLength,1);
      if (NewEncPos2 != encPos2) {
        encPos2 = NewEncPos2;
        if (encPos2 < 1) {
          encPos2 = 1;
        }
        if (encPos2 > 16 ) {
          encPos2 = 16;
        }
        GateLength = encPos2;

        SeqGateTime = (long)((double)Time * ((double)GateLength / 16.0));
        setTempo(Tempo);

        DisplayGateLength() ;
      }
      break;

    case 2: //Volume  of F1
      NewEncPos2 = encodeVal2(Volume[0],10);
      if (NewEncPos2 != Volume[0]) {
        if (NewEncPos2 < 0) {
          NewEncPos2 = 0;
        }
        if (NewEncPos2 >= 4094) { //See lfo.h
          NewEncPos2 = 4094;
        }
        Volume[0] = NewEncPos2;
        SetVolume(Volume[0], 0);
      }
      break;
    case 3: //Volume  of F2
      NewEncPos2 = encodeVal2(Volume[1],10);
      if (NewEncPos2 != Volume[1]) {
        if (NewEncPos2 < 0) {
          NewEncPos2 = 0;
        }
        if (NewEncPos2 >= 4094) { //See lfo.h
          NewEncPos2 = 4094;
        }
        Volume[1] = NewEncPos2;
        SetVolume(Volume[1], 1);
      }
      break;
case 4:  //Waveshape F2
       NewEncPos = encodeVal2(oscParams[1].WaveShape,1);
       if (NewEncPos != oscParams[1].WaveShape) {
        if (NewEncPos < 0) {
          NewEncPos = 0;
        }
        if (NewEncPos >= 3) { //See lfo.h
          NewEncPos = 3;
        }
         setWaveShape(1, NewEncPos);
      }
      break;
      case 5:  //Waveshape F2
       NewEncPos = encodeVal2(oscParams[1].Shape,20);
       if (NewEncPos != oscParams[1].Shape) {
        if (NewEncPos < 0) {
          NewEncPos = 0;
        }
        if (NewEncPos >= 4094) { //See lfo.h
          NewEncPos = 4094;
        }
        oscParams[1].Shape = NewEncPos;
        setShape(1);
        
      }
      break;  
    default: break;
  }

}




void ReadEEPROM() {
  int addr = 0;
  Tempo = EEPROM.read(addr);
  if ((Tempo < MinTempo) || (Tempo > MaxTempo))
    Tempo = DefaultTempo;
  setTempo(Tempo);
  addr = addr + 1;
  GateLength = EEPROM.read(addr);
  if (GateLength < 1)
    GateLength = 4;
  if (GateLength > 16)
    GateLength = 4;

  encPos = Tempo;
  NewEncPos = Tempo;
  encPos2 = GateLength;
  NewEncPos2 = GateLength;
}
