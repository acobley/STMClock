const char *Shapes[] = {"Sin", "Squ", "Tri", "Cos"};

void DisplayChar(char C, byte x, byte y) {
  CX = Width * x;
  CY = Height * y;
  noInterrupts();
  Erase(CX, CY, CX + Width , CY + Height);
  Cursor(CX, CY);

  _WriteChar(C);
  interrupts();
}

void DisplayTime(unsigned long currentMillis, unsigned long previousMillis, int Row) {
  //return; // Remove for Debug
  int CX = 0 * 8;
  int CY = Row * 8;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * 15, CY + Height);
  char sTmp[6];
  sprintf(sTmp, "%i,%i", currentMillis, previousMillis);
  Print(sTmp);
  Refresh();

}

void DisplayTime(long Time, int Row) {
  //return; // Remove for Debug
  int CX = 5 * 8;
  int CY = Row * 8;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * 15, CY + Height);
  char sTmp[6];
  sprintf(sTmp, "%i", Time);
  Print(sTmp);
  Refresh();

}

void DisplayWaveShape(int Osc,int Shape) {
 int CX = (6*8) * Osc;
  int CY = 5 * 8;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 3*Width, CY + Height);
  Print(Shapes[Shape]);
  Refresh();

}

void DisplayMode() {
  Cursor(6, 0);
  Erase(6, 0, 6 + Width * 2, 0 + Height);
  Print(Modes[Mode]);
  Refresh();

}

void DisplayFloat(float DisplayNum, int Row) {
  //return; // Remove for Debug
  int CX = 0 * 8;
  int CY = Row * 8;
Cursor(CX, CY);
  Erase(CX, CY, CX + 7*Width, CY + Height);
  char sTmp[8];
  sprintf(sTmp, "F%i %.1f", Row-2,DisplayNum);
  Print(sTmp);
  Refresh();
}

void DisplayVolume(int DisplayNum, int Row) {
  //return; // Remove for Debug
  int CX = 8 * 8;
  int CY = Row * 8;
Cursor(CX, CY);
  Erase(CX, CY, CX + 6*Width, CY + Height);
  char sTmp[8];
  sprintf(sTmp, "V:%i", DisplayNum);
  Print(sTmp);
  Refresh();
}


void DisplayRatio(double DisplayNum) {
  int CX = 0 * 8;
  int CY = 1 * 8;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 4*Width * 6, CY + Height);
  char sTmp[6];
  sprintf(sTmp, "Rat %.1f", DisplayNum);
  Print(sTmp);
  Refresh();
}



void DisplayTempo() {
  int CX = 8 * 8;
  int CY = 0;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * 3, CY + 2*Height);
  Print(Tempo);
  Cursor(CX, CY+Height);
  Print(Tempo2);
  Refresh();
}

void DisplayGateLength() {
  CX = 12 * 8;
  CY=0;
  Cursor(CX , CY);
  Erase(CX , CY, CX + 2*Width , CY + Height);
  Print(GateLength);
  Refresh();
}

void DisplayBackground() {
  ClearBuffer();
  Refresh();
  CX = 54;
  CY = 0;
  Line(CX - 1, CY, CX - 1, CY + Height + 1);
  Line(CX - 1, CY + Height + 1, 78, CY + Height + 1);
  Line(1 + CX + Width * 3, 0, 1 + CX + Width * 3, CY + Height + 1);
  int i = 0;


  DisplayTempo();
  DisplayRatio(Ratios[currentRatioPos]);
  DisplayMode();
}
