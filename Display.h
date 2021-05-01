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


void DisplayPWMS(PWMStruct *PWMS, int Osc){
  int CX = 8*Width;
  int CY = (4 + 2*Osc) * Height;
  
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * Width, CY + 2*Height);
  char sTmp[7];
  sprintf(sTmp, "S%i", PWMS->PWM1);
  Print(sTmp);
  CY = (4 + 2*Osc+1) * Height;
  
  Cursor(CX, CY);
  sprintf(sTmp, "S%i", PWMS->PWM2);
  Print(sTmp);
  
}

void DisplayTime(unsigned long currentMillis, unsigned long previousMillis, int Row) {
  //return; // Remove for Debug
  int CX = 0 * Width;
  int CY = Row * Height;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * 15, CY + Height);
  char sTmp[6];
  sprintf(sTmp, "%i,%i", currentMillis, previousMillis);
  Print(sTmp);
  Refresh();

}

void DisplayTime(long Time, int Row) {
  //return; // Remove for Debug
  int CX = 5 * Width;
  int CY = Row * Height;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * 15, CY + Height);
  char sTmp[6];
  sprintf(sTmp, "%i", Time);
  Print(sTmp);
  Refresh();

}

void DisplayWaveShapes(int Osc){
  char sTmp[9];
  int  WaveShape= oscParams[Osc1].WaveShape;
  //DisplayWaveShape(Osc, WaveShape);
  CX = 8* 8+1;
  CY = 3 * 8+1;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 1 * Width * 1, CY + Height);
   sprintf(sTmp, "%i", Osc);
  Print(sTmp);
  
  CX = 13 * 8-1;
  CY = 3 * 8+1;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 3 * Width * 5, CY + Height*2);
  
  sprintf(sTmp, "%s", Shapes[WaveShape]);
  Print(sTmp);
  
   CY = 4 * 8;
  Cursor(CX, CY);
   WaveShape= oscParams[Osc2].WaveShape;
  sprintf(sTmp, "%s", Shapes[WaveShape]);
  Print(sTmp);
   
}


void DisplayCurrentRad(float Rad1, float Rad2) {
  int CX = 1;
  char sTmp[10];
  int CY = 6 * Height;
  Cursor(CX, CY);
  Erase(CX, CY, CX +  5* Width, CY + Height);
  sprintf(sTmp, "%.2f", Rad1);
  Print(sTmp);
  Cursor(CX, CY+Height);
  Erase(CX, CY+Height, CX +  5* Width, CY + 2*Height);
  sprintf(sTmp, "%.2f", Rad2);
  Print(sTmp);
  Refresh();

}



void DisplayFrequency(float DisplayNum, int Row) {
  //return; // Remove for Debug
  int CX = 0;
  int CY = Row * Height;
  char sTmp[7];
  Cursor(CX, CY);
  Erase(CX, CY, CX + 7 * Width, CY + Height);

  sprintf(sTmp, "F%i %.1f", Row+1, DisplayNum);
  Print(sTmp);
  Refresh();
}

void DisplayVolume(int DisplayNum, int Row) {
  //return; // Remove for Debug
  int CX = 0;
  int CY = (2 + Row) * Width;
  char sTmp[7];
  int Volume = (DisplayNum/dacRange)*100;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 7 * Width, CY + Height);

  sprintf(sTmp, "V%i %i", Row, Volume);
  Print(sTmp);
  Refresh();
}

void DisplayShape(int DisplayNum,int Row) {
  //return; // Remove for Debug
  int CX = 0;
  int CY = (4 + Row) * Width;
  //int CY = 4  * Width;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * Width, CY + Height);
  char sTmp[7];
  sprintf(sTmp, "S%i %i\%", Row,  DisplayNum);
  Print(sTmp);
  /*Cursor(CX, CY+Height);
  sprintf(sTmp, "S%i %i", Row,  DisplayNum2);
  Print(sTmp);
  */
  Refresh();
}



void DisplayRatio(double DisplayNum) {
  int CX = Width * 8;
  int CY = 2 * Height;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * 6, CY + Height);
  char sTmp[7];
  sprintf(sTmp, "Rat %.1f", DisplayNum);
  Print(sTmp);
  Refresh();
}



void DisplayTempo() {
  int CX = Width * 8;
  int CY = 0;
  Cursor(CX, CY);
  Erase(CX, CY, CX + Width * 3, CY + 2 * Height);
  Print(Tempo);
  Cursor(CX, CY + Height);
  Print(Tempo2);
  Refresh();
}

void DisplayGateLength() {
  CX = 14 * Width;
  CY = 0;
  Cursor(CX , CY);
  Erase(CX , CY, CX + 2 * Width , CY + Height);
  Print(GateLength);
  Refresh();
}


void DisplayMode() {
  int CX = 12 * Width;
  int CY = 0;
  Cursor(CX, CY);
  Erase(CX, CY, CX + 2 * Width, CY + Height);

  Print(Modes[Mode]);
  Refresh();

}
void DisplayBackground() {
  ClearBuffer();
  Refresh();
  CX = Width * 8;
  CY = 3 * Height;
  int CX2 = 15 * Width + 7;
  int CY2 = 7 * Height + 7;
  DrawMode(NORMAL);
  
    Line(CX , CY, CX2, CY );
    Line(CX , CY2, CX2, CY2 );
    Line(CX , CY, CX, CY2 );
    Line(CX2 , CY, CX2, CY2 );
  
  DisplayTempo();
  DisplayRatio(Ratios[currentRatioPos]);
  DisplayMode();
  Refresh();
}
