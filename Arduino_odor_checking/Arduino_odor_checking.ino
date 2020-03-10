/*
2020.02.20 @ author : Koyata Matsushita

For odor threshold controll.

In this program, control AirPin, Odor1Pin, Odor2Pin and SwitchingPin of
Kobal method odor exposure system.

SvmePin is always opened through experiment by toggle switch.
*/

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
const int SessionNum = 2;
const int TrialNum = 15;

// Please set stimulation pattern.
const int OdorPattern[SessionNum][TrialNum] = { 
  {Air, Odor1, Odor2, Odor1, Air, Odor2, Air, Odor2, Odor1, 
   Air, Odor1, Odor2, Air, Odor1, Odor2},  //Session1
  {Odor1, Air, Odor2, Odor2, Air, Odor1, Air, Odor1, Odor2, 
   Air, Odor2, Air, Odor1, Odor2, Odor1}   //Session2
};

/******************************************************************************/

int i = 0;
int odorType;
int sessionCounter = 0;
int trialCounter = 0;
int mriPulseCounter = 0;
boolean trialFlag = true; // ニオイ提示前はtrue，提示したらfalse
boolean checkFlag;

// Function prototype
void dummyScan(boolean); // Dummy scan (10000 msec)
void initialRest(long); // Rest for the first time (long msec)
void questionnaire(); // Questionnaire (24000 msec)
void fillUpOdor(int, long); // rest and fill up odor (long msec)
void odorTrial(int, long); // Odor trial (long msec)
void rest(long); // Rest (long msec)
void sincPulse(); // Send Arduino pulse (100 msec)


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
        Serial.print('p');
        delay(50);
        
        fillUpOdor(Odor1, 1000); // long msec
        odorTrial(Odor1, 5000); // long msec
        rest(1000); // long msec

        checkFlag = true;
        break;
        
      case 'u':
        Serial.print('u');
        delay(50);
        
        fillUpOdor(Odor2, 1000); // long msec
        odorTrial(Odor2, 5000); // long msec
        rest(1000); // long msec
        
        checkFlag = true;
        break;
        
      case 'n':
        Serial.print('n');
        delay(50);
        
        fillUpOdor(Air, 1000); // long msec
        odorTrial(Air, 5000); // long msec
        rest(1000); // long msec
        
        checkFlag = true;
        break;
    }
    
    if(checkFlag == true) break;
    
  }
}


/******************************* define function ******************************/

// Dummy scan (10000 msec)
void dummyScan(boolean isMRIexperiment) {
  Serial.print('f'); // dummy scan display
  
  if(isMRIexperiment == true) {
    // @fMRI experiment
    while(1){
      if(digitalRead(MRIpulse) == HIGH && mriPulseCounter < 10){
        mriPulseCounter++;
        delay(500);

      } else if(digitalRead(MRIpulse == HIGH && mriPulseCounter == 10)){
        break;
      
      }
    }
    
  }else{
    // @Saijo
    isCheck? delay(100): delay(10000);
    
  }
}


// Rest for the first time (long(restTime) msec)
void initialRest(long restTime) {
  Serial.print('a'); // initial rest display
  
  sincPulse(); // 100 msec
  isCheck? delay(100): delay(restTime - 100);

}


// Questionnaire (24000 msec)
void questionnaire() {
  Serial.print('g'); // starting questionnaire display
  sincPulse(); // 100 msec
  isCheck? delay(50): delay(3900);
  
  for(i=0;i<5;i++){
    Serial.print('h'); // question display
    isCheck? delay(100): delay(3900);
    
    Serial.print('i'); // record question result signal
    delay(100); // debounce
  }
}


// rest and fill up odor (long(restTime) msec)
// With odor presentation task, AirPin is closed and OdorPin is opened.
void fillUpOdor(int odorType, long restTime) {
  //Serial.print('b'); // odor screen
  sincPulse(); // 100 msec
  
  if(odorType == Air) {
    delay(restTime - 100);

  } else {
    digitalWrite(Air, LOW);
    delay(100);
    digitalWrite(odorType, HIGH); // fill up odor
    delay(restTime - 200);
  
  }
}


// Odor presentation (2000 msec)
void stimulation(int odorType) {

  /*
  if(odorType == Odor1) {
    Serial.print('p'); // preasant odor display
  } else {
    Serial.print('u'); // unpreasant odor display
  }
  */
  
  delay(20);
  digitalWrite(SwitchingPin, HIGH);
  delay(20);
  digitalWrite(SincPin, HIGH);
  delay(1900);
  digitalWrite(SincPin, LOW);
  delay(20);
  digitalWrite(SwitchingPin, LOW);
  delay(20);
  digitalWrite(odorType, LOW);
  delay(20);
  digitalWrite(Air, HIGH);
  
}


// Odor trial (long(trialTime) msec)
// Last of this func, OdorPin is closed and AirPin is opened.
void odorTrial(int odorType, long trialTime) {
  if(odorType == Air){
    // don't stimulation.
    //Serial.print('n'); // odorless air display
    digitalWrite(SincPin, HIGH);
    delay(trialTime - 50);
    digitalWrite(SincPin, LOW);
    delay(50);
    
  } else {
    
    i = 0;
    trialFlag = true;
    while(1){
      
      if(digitalRead(BreathPulse) == HIGH && i < ((trialTime-2000)/10) && trialFlag){
        // When catch the breath pulse
        trialFlag = false;
        stimulation(odorType); // 2000 msec
        
      } else if(i == ((trialTime-2000)/10) && trialFlag) {
        // If arduino don't catch the breath pulse
        trialFlag = false;
        stimulation(odorType); // 2000 msec
        
      }
      
      if(i == ((trialTime-2000)/10)){
        // Finish stimulate odor trial
        break;
      }
      
      i++;
      delay(10);
    }
  }
}


// Rest (long(restTime) msec)
void rest(long restTime) {
  Serial.print('b'); // rest display
  sincPulse(); // 100 msec
  delay(restTime - 100);
}

// Send Arduino pulse (100 msec)
void sincPulse() {
  digitalWrite(SincPin,HIGH);
  delay(50);
  digitalWrite(SincPin,LOW);
  delay(50);
}
