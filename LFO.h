


void writeSin(float Value, int Channel) {
  int Out = (int)(Volume[Channel] * Value);
  mcpWrite(Out, Channel);

}

void writeSquare(float Value, int Channel) {
  int Out = (int)(Volume[Channel] * Value);
  mcpWrite(Out, Channel);

}

void writeTriangle(float Value, int Channel) {
  int Out = (int)(Volume[Channel] * Value);
  mcpWrite(Out, Channel);

}

void writeCos(float Value, int Channel) {
  int Out = (int)(Volume[Channel] * Value);
  mcpWrite(Out, Channel);

}


void plotWavePoint(long Sample, float Value, long Samples) {
  int X = (int)(PlotWidth * ((float)Sample / (float)Samples)) + PlotLeft;
  int Y = PlotHeight/2 * (0.95*Value) + PlotMiddle;
  Plot(X, Y);
}


void DisplayWave(int Osc) {
  float Wave = 0;
  int CX = 8 * 8 + 1;
  int CY = 3 * 8 + 1;
  int CX2 = 15 * 8 + 7 - 1;
  int CY2 = 7 * 8 + 7 - 1;
  int  WaveShape= oscParams[Osc].WaveShape;
  long Samples = oscParams[Osc].SamplesPhase1+oscParams[Osc].SamplesPhase2;
  Erase(CX , CY, CX2 , CY2);
  float rad = oscParams[Osc].phaseShift;
  DisplayWaveShapes(Osc);
  float Step1 = oscParams[Osc].Step1;
  float Step2 = oscParams[Osc].Step2;
  
  long Sample=0;
  while (rad < (twoPi+oscParams[Osc].phaseShift)) {
    Square = (int)(rad / Pi);
    rad = rad + Square * Step1 + (1 - Square) * Step2;
    Sample++;
    Triangle = (-1 * (Pi / 2.0) + Square * Pi + (0 - Square) * (rad - (Square * Pi)) + (1 - Square) * (rad - Square * Pi));

    switch (WaveShape) {
      case 0:
        Wave = sin(rad);
        plotWavePoint(Sample, Wave,Samples);
        break;
      case 1:
        plotWavePoint(Sample, Square,Samples);
        break;
      case 2:
        plotWavePoint(Sample, Triangle/1.6,Samples);

        break;
      case 3:
        Cos = sin(Triangle);
        plotWavePoint(Sample,  Cos,Samples);

      default:
        break;
    }
  }

  Refresh();

}




void setShape(int Osc) {
  int an1 = oscParams[Osc].Rate; //Rate

  if (an1 <= 0) {
    an1 = 1;
  }
  int an2 = oscParams[Osc].Shape; //Shape
  if (an2 <= 0) {
    an2 = 1;
  }
  float Percent = an2 / dacRange;
  float Percent2 = 1.0 - Percent;

  long SamplesPhase1 = an1 * Percent;
  long SamplesPhase2 = an1 * Percent2;
  oscParams[Osc].SamplesPhase1 = SamplesPhase1;
  oscParams[Osc].SamplesPhase2 = SamplesPhase2;
  oscParams[Osc].Step1 = Pi / SamplesPhase1;
  oscParams[Osc].Step2 = Pi / SamplesPhase2;

  DisplayShape((int)(Percent*100), Osc);
  DisplayWave(Osc);
}

float LfoRatio[] = {0.0625, 0.125, 0.25, 0.5, 1, 2, 3, 4, 8, 16, 32};
int maxLfoRatioCount = 11;
int CurrentNotePos[2] = {5, 5};

void SetFrequency(int Tempo, int newLfoRatio, int Osc) {
  if (newLfoRatio < maxLfoRatioCount) {
    float L = 60.0 / Tempo * LfoRatio[newLfoRatio];
    float f = 1 / L;
    DisplayFrequency(f, Osc);
    DisplayVolume(Volume[Osc], Osc);
    int Samp = (int)round(2000.0 / f);
    oscParams[Osc].Rate = Samp;
    oscParams[Osc].lfoRatio=newLfoRatio;
    setShape(Osc);
  }

}

void setWaveShape(int Osc, int Shape) {
  oscParams[Osc].WaveShape = Shape;
  
  DisplayWave(Osc);
}

void SetVolume(int sVolume, int Osc) {
  Volume[Osc] = sVolume;
  DisplayVolume(sVolume, Osc);
}

void oscReset() {
  oscParams[0].rad = 0.0;
  oscParams[1].rad = 0.0;
  BeatPWMS.PWM1Counter = 0;
  BeatPWMS.PWM2Counter = 0;
  RachetPWMS.PWM1Counter =0;
  RachetPWMS.PWM2Counter =0;
}



void Osc(int Osc) {

  float rad = oscParams[Osc].rad;

  //setShape(Osc);
  float Step1 = oscParams[Osc].Step1;
  float Step2 = oscParams[Osc].Step2;
  int Square = (int)(rad / Pi);
  rad = rad + Square * Step1 + (1 - Square) * Step2;
  if (rad > (twoPi)){
    rad = 0;
    oscParams[Osc].rad = rad;
    /*
    if (Osc==Osc1){
      DisplayCurrentRad(oscParams[Osc1].rad,oscParams[Osc2].rad);
    }
    */
  }

  oscParams[Osc].rad = rad;

  
  Triangle = (-1 * (Pi / 2.0) + Square * Pi + (0 - Square) * (rad - (Square * Pi)) + (1 - Square) * (rad - Square * Pi));
  int WaveShape = oscParams[Osc].WaveShape;
  switch (WaveShape) {
    case 0:
      Wave = 0.5 * sin(rad) + 0.5;
      writeSin(Wave, Osc);
      break;
    case 1:
      writeSquare(Square, Osc);
      break;
    case 2:
      writeTriangle(0.24 * Triangle + 0.5, Osc);
      break;
    case 3:
      Cos = sin(Triangle);
      writeCos(0.5 * Cos + 0.5, Osc);
    default:
      break;
  }

}
