/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
class BlinkLesa{
  public:
    int blinkStatus;
    BlinkLesa()
    {
      blinkStatus = 1;
      pinMode(13, OUTPUT);
      
    }
    void toBlink(){
      blinkStatus = ( blinkStatus + 1 ) % 2;
      if (blinkStatus) {
          digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      } else {
          digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      }
    }
};

#define NUMBER_OF_EYES 8
#define MAXIMUM_ANALOG_READ_VALUE (824) //fix
class EyesLesa{
  public:
    int numberOfEyes = NUMBER_OF_EYES;
    int eyes[ NUMBER_OF_EYES ]= {A6,A7,A8,A9,A11,A10,A1,A0}; // Describes pins which is connected to eyes in right sequence
    int values[NUMBER_OF_EYES ]; // value related to the eye
    int weights_without_colib[8] = { -25 , -27, -22, -20, 20, 22, 27, 25}; //!!!!!!!!!!!need to chose
    int weights_colib1[8] = { 0,0,0,0,0,0,0,-40}; //!!!!!!!!!!!need to chose
    float weights_colibration[8] = { 1 , 1, 1, 1, 1, 1, 1, 1}; //!!!!!!!!!!!need to chose // awerage have to be 1
    int weights[8] ; //!!!!!!!!!!!need to chose
    
    EyesLesa(){
      int i;
      for ( i = 0; i < NUMBER_OF_EYES; i++)
        weights[i] = (int) ( (float) weights_without_colib[i] * weights_colibration[i] );
    }
    int makeDecision(){
      long long int decision;
      //if (decision = see()){
      //  return decision;
      //}
      raw_see();
      long long int  summ=0;
      int i;
      for( i = 0;  i < numberOfEyes; i++){
        summ += weights[i]*values[i];
      }
      long long int summ2= 0;
      if ( summ < 0){
                for( i = 0;  i < numberOfEyes; i++){
                    summ2 += weights[i]*values[i];
                    if ((values[i]) < 200) break;
                  }
      } else{
                        for( i = numberOfEyes-1;  i >=0; i--){
                    summ2 += weights[i]*values[i];
                    if ((values[i]) < 200) break;
                  }
      }
      
      
      Serial.print( "summ2 = ");Serial.println( (int)summ2);
      // Fixing 
      for ( i =0; i< numberOfEyes; i++) { Serial.print( i);Serial.print("="); Serial.print( weights_colibration[i]*values[i]);Serial.print("\t"); }  Serial.println();
      decision = summ2 / MAXIMUM_ANALOG_READ_VALUE;
      Serial.print( "decision see = ");Serial.println( (int)decision);
      return decision;      
    }
    int raw_see(){ // int means mechanism for fast cnocking back to caller, when something goes wrong
      int i;
      for( i = 0; i < numberOfEyes; i++)
      {
        values[i] = analogRead( eyes[i] );
      }
      return 0;
    }
    
};

class RiderLesa{
  public:
    EyesLesa *eyes;
    int last_decision = 0;
    int left = 3;
    int right = 5;
    int increase;
    int decrease;
    
    RiderLesa( EyesLesa *my_eyes )
    {
      eyes = my_eyes;
    }
    int drive(){
      int see_decision = eyes->makeDecision();
      int acceleration = 1;
      //Serial.println( see_decision);
      if ( last_decision == 0) last_decision = 1;
      if ( see_decision/last_decision < 0 || see_decision/last_decision > 2 ) acceleration = 8; // !!!!!!!!!!!!!!need to choose
      int decision = (last_decision*2 + 3*see_decision* acceleration) / 5;
      //int decision = see_decision;
      last_decision = decision;
      Serial.print( "decision drive= ");Serial.println( decision);
      if (decision > 0 ) 
      {
         increase = right;
         decrease = left;
      } else{
         decision = - decision;
         increase = left;
         decrease = right;
      }
        decision = decision / 1; // !!!!!!!!!!!!!!need to choose
        if ( decision > 255) decision = 255;
        decision = 255 - decision;
        analogWrite(increase, 65); // 255
        analogWrite(decrease, 0); // decision
    }
};

BlinkLesa *lamp;
EyesLesa *eyes;
RiderLesa *rider;
// the setup function runs once when you press reset or power the board
void setup() {
  lamp = new BlinkLesa();
  eyes = new EyesLesa();
  rider = new RiderLesa(eyes);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  lamp->toBlink();
  rider->drive();
  
}


