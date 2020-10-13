/*
2020.02.20 @ author : Koyata Matsushita
2020.07.09 edited
    画像提示なし，ニオイ提示秒数を現行プロトコルのものに調整

For odor threshold controll.

In this program, control AirPin, Odor1Pin, Odor2Pin and SwitchingPin of
Kobal method odor exposure system.

SvmePin is always opened through experiment by toggle switch.
*/

/******************************************************************************/
// When you want to check device action, set isCheck true.
// If isCheck is true, skip dummyScan, initialRest and questionnaire.
boolean isCheck = false;

// If fMRI experiment, then true. @Saijo, then false.
boolean isMRIexperiment = false;

boolean isBreath = false;

// Select odor combination
// [Odor1Type] lactone: p, PEA: o, cycroten: i, heptanal: q, benzene: w
char Odor1Type = 'q';
// [Odor2Type] lactone: p, PEA: o, cycroten: i, heptanal: q, benzene: w
char Odor2Type = 'q';

/******************************* Set pin Number *******************************/
// !caution!
// Depending on pin number, some pins may conflict.
#define Air 2
#define Odor1 3 // pleasant odor
#define Odor2 4 // unpleasant odor
#define SwitchingPin 7 // changing air and odor
#define SincPin 9 // sinc pulse
#define BreathPulse 8 // receive breath pulse
#define MRIpulse 13 // receive fMRI pulse


/******************************************************************************/
int i = 0;
boolean checkFlag = false;

// Function prototype
void questionnaire(); // Questionnaire (24000 ms)
void fillUpOdor(int, long); // rest and fill up odor (long ms)
void odorTrial(int, long, boolean); // Odor trial (long ms)
void sincPulse(); // Send Arduino pulse (100 ms)


/*********************************** setup ************************************/
void setup() {
  Serial.begin(9600);

  pinMode(Air, OUTPUT);
  pinMode(Odor1, OUTPUT);
  pinMode(Odor2, OUTPUT);
  pinMode(SwitchingPin, OUTPUT);
  pinMode(SincPin, OUTPUT);
  pinMode(BreathPulse, INPUT); // breathing pulse receive
  pinMode(MRIpulse, INPUT); // MRI pulse receive

  // First time, Odor1(Pleasant odor) is closed.
  digitalWrite(Air, HIGH);
  digitalWrite(Odor1, LOW);
  digitalWrite(Odor2, LOW);
  digitalWrite(SwitchingPin, LOW);
}


/************************************ loop ************************************/
void loop() {
  
  Serial.print('a'); // White cross
  sincPulse();

  // clear received serial signals
  while(Serial.available()){
    Serial.read();
  }
  
  checkFlag = false;
  while(1){

    switch(Serial.read()){
      case 'p':
        
        fillUpOdor(Odor1, 6000); // long ms
        odorTrial(Odor1, 15000, isBreath); // long ms
        questionnaire(); // long ms

        checkFlag = true;
        break;
        
      case 'u':
        
        fillUpOdor(Odor2, 6000); // long ms
        odorTrial(Odor2, 15000, isBreath); // long ms
        questionnaire(); // long ms
        
        checkFlag = true;
        break;
        
      case 'n':
        
        fillUpOdor(Air, 1000); // long ms
        odorTrial(Air, 2000, isBreath); // long ms
        questionnaire(); // long ms
        
        checkFlag = true;
        break;
      
      case 'q':

        questionnaire();
        checkFlag = true;
        break;
        
    }
    
    if(checkFlag == true) break;
    
  }
}


/******************************* define function ******************************/

// Questionnaire (24000 ms)
void questionnaire() {
  Serial.print('x'); // starting questionnaire display
  sincPulse(); // 100 ms
  isCheck? delay(50): delay(3900);

  for(i=0;i<6;i++){
    Serial.print('y'); // question display
    isCheck? delay(100): delay(3900);

    Serial.print('z'); // record question result signal
    delay(100); // debounce
  }
}


// rest and fill up odor (long(restTime) ms, restTime>2000)
// With odor presentation task, AirPin is closed and OdorPin is opened.
void fillUpOdor(int odorType, long restTime) {
  Serial.print('b'); // odor screen
  sincPulse(); // 100 ms

  if(odorType == Air) {
    delay(restTime - 100);

  } else {
    delay(restTime-2200);
    digitalWrite(Air, LOW);
    delay(100);
    digitalWrite(odorType, HIGH); // fill up odor
    delay(2000);

  }
}


// Odor presentation (1000 ms)
void stimulation(int odorType) {

  delay(20);
  digitalWrite(SwitchingPin, HIGH);
  delay(20);
  digitalWrite(SincPin, HIGH);
  delay(940);
  digitalWrite(SincPin, LOW);
  delay(20);
  digitalWrite(SwitchingPin, LOW);

}


// Odor trial (long(trialTime) ms)
// Last of this func, OdorPin is closed and AirPin is opened.
void odorTrial(int odorType, long trialTime, boolean breathFlag) {
  if(odorType == Air){
    // don't stimulation.
    Serial.print('n'); // odorless air display
    digitalWrite(SincPin, HIGH);
    delay(trialTime - 50);
    digitalWrite(SincPin, LOW);
    delay(50);

  } else {
      if(odorType == Odor1) {
        switch (Odor1Type) {
          case 'p':
            Serial.print('p'); // lactone display
            break;
          
          case 'o':
            Serial.print('o'); // PEA display
            break;
          
          case 'i':
            Serial.print('i'); // cycroten display
            break;

          case 'w':
            Serial.print('w'); // benzene display
            break;

          case 'q':
            Serial.print('q'); // heptanal display
            break;
          
          default:
            break;
        }
      } else {
        switch (Odor2Type) {
          case 'p':
            Serial.print('p'); // lactone display
            break;
          
          case 'o':
            Serial.print('o'); // PEA display
            break;
          
          case 'i':
            Serial.print('i'); // cycroten display
            break;

          case 'w':
            Serial.print('w'); // benzene display
            break;

          case 'q':
            Serial.print('q'); // heptanal display
            break;
          
          default:
            break;
        }
      }

    if(breathFlag == true) {
      i = 0;
      int i_stimNum = 0;

      while(1){
        if(digitalRead(BreathPulse) == HIGH && i < ((trialTime-1000)/10)) {
          // When catch the breath pulse
          stimulation(odorType); // 1000 ms
          i_stimNum++;
        }

        if(i >= ((trialTime-i_stimNum*1000-40)/10)){
          // Finish stimulate odor trial
          delay(20);
          digitalWrite(odorType, LOW);
          delay(20);
          digitalWrite(Air, HIGH);
          break;
        }

        i++;
        delay(10);
      }
    } else {
      for(int i = 0; i < 4; i++) {
        stimulation(odorType); // stimulation 1000 ms
        delay(2000); // rest 2000 ms
      }
      stimulation(odorType); // stimulation 1000 ms
      delay(1980);
      digitalWrite(odorType, LOW);
      delay(20);
      digitalWrite(Air, HIGH);
    }
  }
}


// Send Arduino pulse (100 ms)
void sincPulse() {
  digitalWrite(SincPin,HIGH);
  delay(50);
  digitalWrite(SincPin,LOW);
  delay(50);
}
