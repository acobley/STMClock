const float Pi = 3.14159;
const float twoPi = 2.0 * Pi;

const float dacRange = 4095.0;
const int idacRange = 4095;

const short Osc1=0;
const short Osc2=1;
const char *Modes[] = {"T", "R", "F1", "F2", "W","S","P"};
const byte MaxMode = 6;
const short MinTempo = 30;
const short MaxTempo = 600;
const double Ratios[] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 5, 6, 7, 8};
const short RatiosArrayLength = 11;

//Display
const int Width = 8;
const int Height = 8;
const int PlotWidth= 8*8-2;
const int PlotHeight=5*8-2;
const int PlotMiddle =PlotHeight/2+3*8;
const int PlotLeft=PlotWidth+1;

//Debounce times.
const long interval = 500;
const long enc1Interval = 50;
const long enc2Interval = 50;
