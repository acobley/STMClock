
const float Pi = 3.14159;
float step = Pi / (256.0);
int Value = 0;
float dacRange = 4095.0;
float twoPi = 2.0 * Pi;


/*
 * float rad = 0;
  int Shape = 128.0;
  float Step1 = 2 * Pi / Shape;
  float Step2 = 2 * Pi / (512 - Shape);
  float Rate = 0;
 */


typedef struct {
  float rad;
  int Shape;
  float Step1;
  float Step2;
  float Rate;
  int WaveShape;
  int Volume;
}oscParam ;

oscParam oscParams[]={{ 0.0,2048,2 * Pi / 2048.0, 2 * Pi / (2048),40.0,0,4095},
                      { 0.0,2048,2 * Pi / 2048.0, 2 * Pi / (2048),160.0,3,4095}
                      };

int Volume[2]={4095,4095};

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
  int Out = (int)(Volume[Channel]* Value);
  mcpWrite(Out, Channel);

}

float divisor = 0.0;
void setShape(int Osc) {
  int an1 = 2*oscParams[Osc].Rate; //Rate

  if (an1 <= 0) {
    an1 = 1;
  }
  int an2 = oscParams[Osc].Shape; //Shape
  if (an2 <= 0) {
    an2 = 1;
  }
  float Percent = an2 / dacRange;
  float Percent2 = 1.0 - Percent;
  oscParams[Osc].Step1 = twoPi / (an1 * Percent);
  oscParams[Osc].Step2 = twoPi / (an1 * Percent2);
  //DisplayFloat(oscParams[Osc].Step1,3);
  //DisplayFloat(oscParams[Osc].Step2,4);
  
}

float ratio[]= {0.25,0.5,1,2,3,4,8};
int maxRatioCount=7;
int CurrentNotePos[2]={2,2};

void SetFrequency(int Tempo, int NotePos,int Osc){
  if (Ratio<7){
    float L=60.0/Tempo*ratio[NotePos];
    float f=1/L;
    DisplayFloat(f,3+Osc);
    DisplayVolume(Volume[Osc],3+Osc);
    int Samp=(int)round(2000.0/f);
    oscParams[Osc].Rate=Samp;
    setShape(Osc);
  }
  
}

void setWaveShape(int Osc, int Shape){
  oscParams[Osc].WaveShape = Shape;
  DisplayWaveShape(Osc,Shape);
}

void SetVolume(int sVolume,int Osc){
  Volume[Osc]=sVolume;
  DisplayVolume(sVolume,3+Osc);
}

void oscReset(){
  oscParams[0].rad=0.0;
  oscParams[1].rad=0.0;
}

float Wave = 0;
int Square = 0;
float Triangle = 0;
float Cos = 0;
float lastRad;

void Osc(int Osc) {

  float rad = oscParams[Osc].rad;
  
  //setShape(Osc);
  float Step1 = oscParams[Osc].Step1;
  float Step2 = oscParams[Osc].Step2;
  rad = rad + Square * Step1 + (1 - Square) * Step2;
  if (rad > twoPi)
    rad = 0.0;

  oscParams[Osc].rad=rad;

  Square = (int)(rad / Pi);
  Triangle = (-1 * (Pi / 2.0) + Square * Pi + (0 - Square) * (rad - (Square * Pi)) + (1 - Square) * (rad - Square * Pi));
  int WaveShape=oscParams[Osc].WaveShape;
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
