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

void DisplayWaveShape(int Osc, int Shape) {
  int CX = 8 * 3;;
  int CY = (6 + Osc) * 8;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 3 * Width, CY + Height);
  Print(Shapes[Shape]);
  Refresh();

}



void DisplayFrequency(float DisplayNum, int Row) {
  //return; // Remove for Debug
  int CX = 0;
  int CY = Row * 8;
  char sTmp[7];
  Cursor(CX, CY);
  Erase(CX, CY, CX + 7 * Width, CY + Height);

  sprintf(sTmp, "F%i %.1f", 2, DisplayNum);
  Print(sTmp);
  Refresh();
}

void DisplayVolume(int DisplayNum, int Row) {
  //return; // Remove for Debug
  int CX = 0;
  int CY = (2 + Row) * 8;
  char sTmp[7];
  Cursor(CX, CY);
  Erase(CX, CY, CX + 7 * Width, CY + Height);

  sprintf(sTmp, "V%i %i", Row, DisplayNum);
  Print(sTmp);
  Refresh();
}

void DisplayShape(int DisplayNum, int DisplayNum2,int Row) {
  //return; // Remove for Debug
  int CX = 0;
  //int CY = (4 + Row) * 8;
  int CY = 4  * 8;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 8 * Width, CY + 2*Height);
  char sTmp[7];
  sprintf(sTmp, "S%i %i", Row,  DisplayNum);
  Print(sTmp);
  Cursor(CX, CY+Height);
  sprintf(sTmp, "S%i %i", Row,  DisplayNum2);
  Print(sTmp);
  Refresh();
}



void DisplayRatio(double DisplayNum) {
  int CX = 8 * 8;
  int CY = 2 * 8;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 4 * Width * 6, CY + Height);
  char sTmp[7];
  sprintf(sTmp, "Rat %.1f", DisplayNum);
  Print(sTmp);
  Refresh();
}



void DisplayTempo() {
  int CX = 8 * 8;
  int CY = 0;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * 3, CY + 2 * Height);
  Print(Tempo);
  Cursor(CX, CY + Height);
  Print(Tempo2);
  Refresh();
}

void DisplayGateLength() {
  CX = 14 * 8;
  CY = 0;
  Cursor(CX , CY);
  Erase(CX , CY, CX + 2 * Width , CY + Height);
  Print(GateLength);
  Refresh();
}


void DisplayMode() {
  int CX = 12 * 8;
  int CY = 0;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 2 * Width, CY + Height);

  Print(Modes[Mode]);
  Refresh();

}
void DisplayBackground() {
  ClearBuffer();
  Refresh();
  CX = 8 * 8;
  CY = 3 * 8;
  int CX2 = 15 * 8 + 7;
  int CY2 = 7 * 8 + 7;
  DrawMode(NORMAL);
  
    Line(CX , CY, CX2, CY );
    Line(CX , CY2, CX2, CY2 );
    Line(CX , CY, CX, CY2 );
    Line(CX2 , CY, CX2, CY2 );
  
  DisplayTempo();
  DisplayRatio(Ratios[currentRatioPos]);
  DisplayMode();
}
