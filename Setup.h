
#include "SPI.h"
#include "fonts.h"
#include <EEPROM.h>

const boolean Debug =false;

//Encoder
#define encA PB0
#define encB PB1

#define enc2A PB10 //11
#define enc2B PB11 //10
#define BUT1 PA0
#define BUT2 PB15
#define STARTBUT PB7
#define BUTLED3 PA12
#define START PA2
#define RESET PA1

int DAC = PB6; //????
int Gate1 = PB5;
int Gate2 = PC13;
byte CV1 = 1;
byte CV2 = 0;

const char *Modes[] = {"T", "R", "F1", "F2", "W","S"};

byte Mode = 0;
byte MaxMode = 5;

int encAVal,  encBVal;
int encAVal2,  encBVal2;

int encALast[]={0,0,0,0,0,0};
int encALast2[]={0,0,0,0,0,0};

HardwareTimer MainTimer(1);

long ClockF = 72000000;
struct ClockStruct {
  char Name[15];
  long Psc = 0;
  long Arr = 0;
};

struct PWMStruct {
  long PWM1 = 1;
  long PWM2 = 1;
  long PWM1Counter = 0;
  long PWM2Counter = 0;
  boolean Phase = 0;
};

ClockStruct MainClock;
PWMStruct BeatPWMS;

PWMStruct RachetPWMS;
ClockStruct SequenceClock;








const int DefaultTempo = 120;
int Tempo = DefaultTempo;
int Tempo2 = 2.0 * DefaultTempo;
long BaseTime = 1000000L * 60 / Tempo;
long BeatTime = 1000000L * 60 / Tempo;
long RachetTime = 1000000L * 60 / Tempo;
short GateLength = 4;
long SeqGateTime = BaseTime * (GateLength / 8);
long ms15 = 015000L;
const short MinTempo = 30;
const short MaxTempo = 600;
const double Ratios[] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 5, 6, 7, 8};
const short RatiosLength = 11;
short currentRatioPos = 2;
float Ratio = Ratios[currentRatioPos];
short maxRatio = 10;



int PlaceCount = 0;
int ErasePos = -1;
int bErasePos;
int Beat = 1;
int MaxBeat = 10;

float NumberOfBeats = 1.0;


short encPos = Tempo;
short NewEncPos = Tempo;
short encPos2 = GateLength;
short NewEncPos2 = GateLength;




//float Range = 819.2; // (2^12/5)
float Range = 1365.33333; // (2^12/3)

volatile int hKey ;
volatile int hOctave;
volatile int hNote;
volatile int houtValue;

int CX = 0;
int CY = 0;
int Width = 8;
int Height = 8;


void SetupEncoders() {
  pinMode(encA, INPUT);
  pinMode(encB, INPUT);
  pinMode(enc2A, INPUT);
  pinMode(enc2B, INPUT);

  //digitalWrite(encA, HIGH);
  //digitalWrite(encB, HIGH);
  //digitalWrite(enc2A, HIGH);
  //digitalWrite(enc2B, HIGH);
}


void SetupOLED() {
  _Res_Max_X = SH1106_RES_X;
  _GRAM_Col_Start = SH1106_GRAM_COL_START;
  _GRAM_Col_End = SH1106_GRAM_COL_END;
  _GRAM_Page_Start = SH1106_GRAM_PAGE_START;
  _GRAM_Page_End = SH1106_GRAM_PAGE_END;
  _RAM_Pages = SH1106_GRAM_PAGE_END - SH1106_GRAM_PAGE_START + 1;

  SPI.begin();
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setBitOrder(MSBFIRST);
  pinMode(CS_DI, OUTPUT);
  pinMode(DC_DI, OUTPUT);
  pinMode(RST_DI, OUTPUT);

  Reset();
  Cursor(0, 0);
  DrawMode(NORMAL);
  _FontType = Terminal_8pt;
  _FontHight = Terminal_8ptFontInfo.CharacterHeight;
  _FontDescriptor = Terminal_8ptFontInfo.Descriptors;
  for (int x = 0; x < 128; x++) {
    for (int y = 0; y < 64; y++) {
      Plot(x, y);
    }
    Refresh();
  }

  DrawMode(CLEAR);
  for (int x = 0; x < 128; x++) {
    for (int y = 0; y < 64; y++) {
      Plot(x, y);
    }
    Refresh();
  }

  Erase(0, 0, 128, 64);
  Refresh();

}

void SetupDacs() {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  pinMode(DAC, OUTPUT);

  digitalWrite(DAC, HIGH);

}



void SetupGates() {
  pinMode(Gate2, OUTPUT);
  pinMode(Gate1, OUTPUT);
}


void mcpWrite(int value, int Channel) {


  //set top 4 bits of value integer to data variable
  byte data = value >> 8;
  data = data & B00001111;
  if (Channel == 0)
    data = data | B00110000; //DACA Bit 15 Low
  else
    data = data | B10110000; //DACB Bit 15 High
  if (digitalRead(CS_DI) == HIGH) { //Don't send if the Display is updating.


    noInterrupts();
    digitalWrite(DAC, LOW); //CS
    SPI.transfer(data);
    data = value;
    SPI.transfer(data);
    // Set digital pin DACCS HIGH
    digitalWrite(DAC, HIGH);
    interrupts();
  }

}




void WriteNote(int Note, int Octave, int Channel) {
  hOctave = Octave;
  hNote = (byte)(Note % 12);
  houtValue = (int)(Range * (hOctave + (float)hNote / 12));
  mcpWrite(houtValue, Channel);

}


void WriteEEProm() {
  int addr = 0;
  EEPROM.write(addr, Tempo);
  addr = addr + 1;
  EEPROM.write(addr, GateLength);

}
