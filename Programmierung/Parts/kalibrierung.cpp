//delete threshold variable
//initial
unsigned char white[8] = Analogue_value[];
unsigned char black[8] = Analogue_value[];
unsigned char Threshold[8];

//slowly driving to detect values
Forward((unsigned char) d_L_normal/4,(unsigned char) d_R_normal/4);

timer = 1;
while (timer > 0 && timer < 30 && US_Time_L > 24 && US_Time_R > 24){
  for (unsigned char int i = 0; i < 8; i++){
    if (white[i] > Analogue_value[i]) {
      white[i] = Analogue_value[i];
      timer = 1;
    }
    if (black[i] < Analogue_value[i]){
      black[i] = Analogue_value[i];
      timer = 1;
    }
    Threshold[i] = (black[i] - white[i]) /2;
  }
}
timer = 0;
Stop();

Line_all_digit = 0b00000000; //set all line-values to 0
linecounter = 8;
if(Line_L3 < Threshold[0]){
   Line_all_digit |= 0b10000000;
   linecounter--;
} //if left sensor sees white set left bit to 1
if(Line_L2 < Threshold[1]){
  Line_all_digit |= 0b01000000;
  linecounter--;
}
if(Line_L1 < Threshold[2]){
   Line_all_digit |= 0b00100000;
   linecounter--;
}
if(Line_L0 < Threshold[3]){
  Line_all_digit |= 0b00010000;
  linecounter--;
}
if(Line_R0 < Threshold[4]){
  Line_all_digit |= 0b00001000;
  linecounter--;
}
if(Line_R1 < Threshold[5]){
  Line_all_digit |= 0b00000100;
  linecounter--;
}
if(Line_R2 < Threshold[6]){
  Line_all_digit |= 0b00000010;
  linecounter--;
}
if(Line_R3 < Threshold[7]){
  Line_all_digit |= 0b00000001;
  linecounter--;
} //if right sensor sees white set right bit to 1
