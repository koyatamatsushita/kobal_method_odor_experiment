/*
2020.02.20 @ author : Koyata Matsushita
2020.07.07 edited
    プロトコルを変更，安静時ニオイパルスチェックを導入
2020.07.10 added
    プレ実験の提示順を追加
2020.07.31 edited
    アンケートを減らしたプロトコルのプログラム．

For fMRI experiment.

In this program, control AirPin, Odor1Pin, Odor2Pin and SwitchingPin of
Kobal method odor exposure system.

SvmePin is always opened through experiment by toggle switch.

*/

/******************************************************************************/
// When you want to check device action, set isCheck true.
// If isCheck is true, skip dummyScan, initialRest and questionnaire.
boolean isCheck = false;

// If fMRI experiment, then true. @Saijo, then false.
boolean isMRIexperiment = true;

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


/******************************** Set variable ********************************/
// Please set session and trial num.
const int SessionNum = 3;
const int RunNum = 3;
const int BlockNum = 3;

// Please set stimulation pattern.
const int OdorPattern[SessionNum][RunNum][BlockNum] = {

  /*
  // Sub.A(Akiyoshi)
  { {Air, Air, Air},
    {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2} },// session1(block A)
  { {Odor2, Odor2, Odor2},
    {Air, Air, Air},
    {Odor1, Odor1, Odor1} },//session2(block B)
  { {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2},
    {Air, Air, Air} }//session3(block C)
  */
  /*
  // Sub.B(Harada)
  { {Air, Air, Air},
    {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1} },// session1(block D)
  { {Odor1, Odor1, Odor1},
    {Air, Air, Air},
    {Odor2, Odor2, Odor2} },//session2(block E)
  { {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1},
    {Air, Air, Air} }//session3(block F)
  */
  /*
  // Sub.C(Tachihara)
  { {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2},
    {Air, Air, Air} },//session1(block C)
  { {Odor2, Odor2, Odor2},
    {Air, Air, Air},
    {Odor1, Odor1, Odor1} },//session2(block B)
  { {Air, Air, Air},
    {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2} }// session3(block A)
  */
  /*
  // Sub.D(Kurota)
  { {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1},
    {Air, Air, Air} },//session1(block F)
  { {Odor1, Odor1, Odor1},
    {Air, Air, Air},
    {Odor2, Odor2, Odor2} },//session2(block E)
  { {Air, Air, Air},
    {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1} }// session3(block D)
  */
  /*
  // Sub.E(Matsushita)
  { {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2},
    {Air, Air, Air} },//session1(block C)
  { {Air, Air, Air},
    {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2} },// session2(block A)
  { {Odor2, Odor2, Odor2},
    {Air, Air, Air},
    {Odor1, Odor1, Odor1} }//session3(block B)
  */
  /*   
  // Sub.F(Sakagawa)
  { {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1},
    {Air, Air, Air} },//session1(block F)
  { {Air, Air, Air},
    {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1} },// session2(block D)
  { {Odor1, Odor1, Odor1},
    {Air, Air, Air},
    {Odor2, Odor2, Odor2} }//session3(block E)
  */
  
  // Sub.G(Anai)
  { {Odor2, Odor2, Odor2},
    {Air, Air, Air},
    {Odor1, Odor1, Odor1} },//session1(block B)
  { {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2},
    {Air, Air, Air} },//session2(block C)
  { {Air, Air, Air},
    {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2} }// session3(block A)
  
  /*
  // Sub.H()
  { {Odor1, Odor1, Odor1},
    {Air, Air, Air},
    {Odor2, Odor2, Odor2} },//session1(block E)
  { {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1},
    {Air, Air, Air} },//session2(block F)
  { {Air, Air, Air},
    {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1} }// session3(block D)
  */
  /*
  // Sub.I()
  { {Odor2, Odor2, Odor2},
    {Air, Air, Air},
    {Odor1, Odor1, Odor1} },//session1(block B)
  { {Air, Air, Air},
    {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2} },// session2(block A)
  { {Odor1, Odor1, Odor1},
    {Odor2, Odor2, Odor2},
    {Air, Air, Air} }//session3(block C)
  */
  /*
  // Sub.J()
  { {Air, Air, Air},
    {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1} },// session1(block D)
  { {Odor2, Odor2, Odor2},
    {Odor1, Odor1, Odor1},
    {Air, Air, Air} },//session2(block F)
  { {Odor1, Odor1, Odor1},
    {Air, Air, Air},
    {Odor2, Odor2, Odor2} }//session3(block E)
  */
  
};

/******************************************************************************/

int i = 0;
int odorType;
int sessionCounter = 0;
int blockCounter = 0;
int runCounter = 0;
//int mriPulseCounter = 0;
boolean breathFlag; // exist breath pulse or not

// Function prototype
void dummyScan(boolean); // Dummy scan (10000 ms)
boolean initialRest(long); // Rest for the first time (long ms)
void questionnaire(); // Questionnaire (24000 ms)
void fillUpOdor(int, long); // rest and fill up odor (long ms)
void odorBlock(int, long, boolean); // Odor trial (long ms)
void rest(long); // Rest (long ms)
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

  /*
   * dial check
   */
  if(Serial.read()=='s'){
    Serial.print('s'); // starting dial checking display
    delay(5000);

    Serial.print('t'); // dial checking
    delay(50);
  }

  // ENTER key is pressed.
  if(Serial.read()=='a'){

    // /*
    //  * threshold control
    //  */
    //  Serial.print('c'); // threshold control display
    //  while(1)
    //   if(Serial.read()=='a') break; // When press ENTER key, go to next step.

    // /*
    //  * structural imaging
    //  */
    // Serial.print('d'); // structural imaging display
    // while(1)
    //   if(Serial.read()=='a') break;

    // /*
    //  *  noise canceling settings
    //  */
    // Serial.print('e'); // noise canceling display
    // while(1)
    //   if(Serial.read()=='a') break;

    /*
     * Session loop
     */
    for(sessionCounter=0; sessionCounter<SessionNum; sessionCounter++){
      dummyScan(isMRIexperiment); // 10000 ms

      for(runCounter=0; runCounter<RunNum; runCounter++){

        breathFlag = initialRest(15000); // Rest

        questionnaire(); // initial questionnaire task for 24000 ms

        // rest + odor trial + questionnaire
        for(blockCounter=0; blockCounter<BlockNum; blockCounter++){
          odorType = OdorPattern[sessionCounter][runCounter][blockCounter];

          odorBlock(odorType, 20000, breathFlag); // long ms + Air Block 20000 ms

        }
        questionnaire(); // questionnaire 24000 ms
      }

      initialRest(15000);

      Serial.print('j'); // session interval
      sincPulse();

      if(sessionCounter<2){
        while(1) {
          if(Serial.read()=='a'){
            break;
          }
        }
      }
    }

    sincPulse(); // 100 ms

    Serial.print('z'); // finish display

    digitalWrite(Odor1, LOW);
    digitalWrite(Odor2, LOW);
    digitalWrite(SwitchingPin, HIGH);
    delay(60000);

    while(1); // finish!
  }
}


/******************************* define function ******************************/

// Dummy scan (10000 ms)
void dummyScan(boolean isMRIexperiment) {
  Serial.print('f'); // dummy scan display
  delay(50);

  if(isMRIexperiment == true) {
    // @fMRI experiment
    int mriPulseCounter = 0;

    while(1){
      if(digitalRead(MRIpulse) == HIGH && mriPulseCounter < 10) {
        mriPulseCounter++;
        Serial.print('y'); // test
        delay(750);

      } else if(digitalRead(MRIpulse) == HIGH && mriPulseCounter == 10) {
        //Serial.print('y');
        //delay(50);
        break;

      }
    }

  }else{
    // @Saijo
    isCheck? delay(100): delay(10000);

  }
}


// Rest for the first time (long(restTime) ms)
boolean initialRest(long restTime) {
  Serial.print('a'); // initial rest display

  sincPulse(); // 100 ms

  boolean breathFlag = false;
  
  delay(restTime - 100);

  return breathFlag;
}


// Questionnaire (24000 ms)
void questionnaire() {
  Serial.print('g'); // starting questionnaire display
  sincPulse(); // 100 ms
  isCheck? delay(50): delay(3900);

  for(i=0;i<6;i++){
    Serial.print('h'); // question display
    isCheck? delay(100): delay(3900);

    Serial.print('i'); // record question result signal
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
    // delay(restTime-2200);
    digitalWrite(Air, LOW);
    delay(100);
    digitalWrite(odorType, HIGH); // fill up odor
    delay(restTime-200);

  }
}


// Odor presentation (2000 ms)
void stimulation(int odorType) {

  if(odorType == Odor1) {
    Serial.print('p'); // preasant odor display
  } else {
    Serial.print('u'); // unpreasant odor display
  }

  delay(20);
  digitalWrite(SwitchingPin, HIGH);
  delay(20);
  digitalWrite(SincPin, HIGH);
  delay(1940);
  digitalWrite(SincPin, LOW);
  delay(20);
  digitalWrite(SwitchingPin, LOW);

}


// Odor trial (long(trialTime) ms)
// Last of this func, OdorPin is closed and AirPin is opened.
void odorBlock(int odorType, long trialTime, boolean breathFlag) {
  if(odorType == Air){
    // don't stimulation.
    Serial.print('n'); // odorless air display
    digitalWrite(SincPin, HIGH);
    delay(trialTime - 50);
    digitalWrite(SincPin, LOW);
    delay(50);

  } else {
    fillUpOdor(odorType, 2000);
    for(int i = 0; i < 4; i++) {
      delay(2000); // rest 2000 ms
      stimulation(odorType); // stimulation 2000 ms
    }
    delay(1980);
    digitalWrite(odorType, LOW);
    delay(20);
    digitalWrite(Air, HIGH);
  }
  delay(20000); // Air Block
}

// Rest (long(restTime) ms)
void rest(long restTime) {
  Serial.print('b'); // rest display
  sincPulse(); // 100 ms
  delay(restTime - 100);
}

// Send Arduino pulse (100 ms)
void sincPulse() {
  digitalWrite(SincPin,HIGH);
  delay(50);
  digitalWrite(SincPin,LOW);
  delay(50);
}
