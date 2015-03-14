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

#define LINE_WIDTH_MIULTIMPLE_255 1200 // line width in eyes * 255
#define NUMBER_OF_EYES 8
class EyesLesa{
  public:
    int numberOfEyes = NUMBER_OF_EYES;
    int eyes[ NUMBER_OF_EYES ]= {A6,A7,A8,A9,A11,A10,A1,A0}; // Describes pins which is connected to eyes in right sequence
    int values[NUMBER_OF_EYES ]; // value related to the eye
    int wite_value[8] = { 200 , 200 , 200 , 200 , 200 , 200 , 200 , 250 };
    int black_value[8] = { 600 , 600 , 650 , 650 , 650 , 650 , 650 , 650 };
    int left_right[8] = { -1 , -1, -1, -1, 1, 1, 1, 1}; //!!!!!!!!!!!need to chose
    int white_black[8];
    int last_black_line_zenter_under_eye_numder_multiple_255 = 0;
    EyesLesa(){
    }
    int makeDecision(){
      int decision;
      //if (decision = see()){
      //  return decision;
      //}
      see();
      int maximum = 1, i, summ=0, calculated_line_width=0;
      for( i = 0;  i < numberOfEyes; i++){
        if ( values[i] > wite_value[i]) 
          {
            if ( values[i] < black_value[i]) 
              {
                long long temp = values[i] - wite_value[i] ;
                temp = temp * 255;
                temp = temp / ( black_value[i] - wite_value[i]);
                white_black[i] = (int) temp;
              }else{
                white_black[i] = 255; // black = 255
              }
            
          }
          else{
            white_black[i] = 0; //white = 0
          }
        summ += white_black[i]* left_right[i];  // summ < 0 -> line is in the left
        calculated_line_width += white_black[i];
        //Serial.print( white_black[i] );Serial.print("\t");
      } //Serial.println();
      
      if ( calculated_line_width == 0 ){ // all = white color!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
        return last_black_line_zenter_under_eye_numder_multiple_255;
      }
      
      
      int black_line_zenter_under_eye_numder_multiple_255 = 0; 
      
      if (summ > 0){ //right
        black_line_zenter_under_eye_numder_multiple_255 = -NUMBER_OF_EYES* 127;
        if ( calculated_line_width > LINE_WIDTH_MIULTIMPLE_255)
        {
          black_line_zenter_under_eye_numder_multiple_255 = black_line_zenter_under_eye_numder_multiple_255 + calculated_line_width / 2;
        }else
        {
          black_line_zenter_under_eye_numder_multiple_255 = black_line_zenter_under_eye_numder_multiple_255 + LINE_WIDTH_MIULTIMPLE_255 / 2;
        }
        for ( i = 0;i < numberOfEyes; i++){
          if( white_black[i] !=255 ){
            black_line_zenter_under_eye_numder_multiple_255 = black_line_zenter_under_eye_numder_multiple_255 + 255 - white_black[i];
          }else{
            break;
          }
        }
      } else{
        black_line_zenter_under_eye_numder_multiple_255 = NUMBER_OF_EYES* 127;
        if ( calculated_line_width > LINE_WIDTH_MIULTIMPLE_255)
        {
          black_line_zenter_under_eye_numder_multiple_255 = black_line_zenter_under_eye_numder_multiple_255 - calculated_line_width / 2;
        }else
        {
          black_line_zenter_under_eye_numder_multiple_255 = black_line_zenter_under_eye_numder_multiple_255 - LINE_WIDTH_MIULTIMPLE_255 / 2;
        }
        for ( i = numberOfEyes - 1 ;i >= 0; i--){
          if( white_black[i] !=255 ){
            black_line_zenter_under_eye_numder_multiple_255 = black_line_zenter_under_eye_numder_multiple_255 - 255 + white_black[i];
          }else{
            break;
          }
        }
      }
      
      last_black_line_zenter_under_eye_numder_multiple_255 = black_line_zenter_under_eye_numder_multiple_255;
      return black_line_zenter_under_eye_numder_multiple_255;     
    }
    int see(){ // int means mechanism for fast cnocking back to caller, when something goes wrong
      int i;
      for( i = 0; i < numberOfEyes; i++)
      {
        values[i] = analogRead( eyes[i] );
        //Serial.print(values[i]);
        //Serial.print(" ");
      }
      //Serial.print("\n");
      //delay(100);
      return 0;
    }
    
};

#define SENSIBILITY 0.150
#define AVERAGE_SPEED 85
#define DIFERENTIAL_STOP 7
#define DIFERENTIAL_ZETRER_TIME_EFFECT 30

class RiderLesa{
  public:
    EyesLesa *eyes;
    int last_decision = 0;
    int left = 3;
    int right = 5;
    int increase;
    int decrease;
    long long int approximate_speed = 0;
    int last_black_line_zenter_under_eye_numder_multiple_255 = 0;
    float diferential_zenter_time_last = 0;
    unsigned long last_loop_time = 0;
    
    RiderLesa( EyesLesa *my_eyes )
    {
      eyes = my_eyes;
    }
    int drive(){
      //delay(200);
      int black_line_zenter_under_eye_numder_multiple_255 = eyes->makeDecision();
      int boost = 0;
      unsigned long this_loop_time = millis();
      float diferential_zenter_time = 0;
      if  ( last_loop_time != 0){
          diferential_zenter_time = ((float)(black_line_zenter_under_eye_numder_multiple_255 - last_black_line_zenter_under_eye_numder_multiple_255))   ;
          if( last_loop_time==this_loop_time ){
            //Serial.println( "FATAL ERROR!");
            diferential_zenter_time_last = diferential_zenter_time_last*15 + diferential_zenter_time;
            diferential_zenter_time_last = diferential_zenter_time_last / 16;
          }else{
            diferential_zenter_time_last = diferential_zenter_time_last*15 + diferential_zenter_time/(this_loop_time - last_loop_time);
            diferential_zenter_time_last = diferential_zenter_time_last / 16;
          }
      }else{diferential_zenter_time = 0;}
      //Serial.print( "Time between w measures in milliseconds = ");
      //Serial.println( this_loop_time - last_loop_time );
      Serial.print( "Diferential_zenter_time = ");Serial.println( diferential_zenter_time );
      float mod_diferential_zenter_time = diferential_zenter_time > 0 ? diferential_zenter_time : - diferential_zenter_time;
      last_loop_time = this_loop_time;
      last_black_line_zenter_under_eye_numder_multiple_255 = black_line_zenter_under_eye_numder_multiple_255;
      
      
      int speed_increaseng = 0;
      // PID
      /*speed_increaseng =  black_line_zenter_under_eye_numder_multiple_255 * SENSIBILITY;
      speed_increaseng = speed_increaseng > 0 ? speed_increaseng : -speed_increaseng;
      if ( mod_diferential_zenter_time > CRITICAL_DIFERENTIAL_ZETRER_TIME)
      {
        speed_increaseng += DIFERENTIAL_INCREASE_BOOST * ( mod_diferential_zenter_time==diferential_zenter_time ?   ) ;
        boost = DIFERENTIAL_BOOST;
      }
      

      if (black_line_zenter_under_eye_numder_multiple_255 > 0 ) 
      {
         increase = right;
         decrease = left;
      } else{
         increase = left;
         decrease = right;
      }*/
      

        boost = DIFERENTIAL_STOP*(mod_diferential_zenter_time +diferential_zenter_time_last);
      
      speed_increaseng = (2*diferential_zenter_time+ diferential_zenter_time_last)*DIFERENTIAL_ZETRER_TIME_EFFECT + black_line_zenter_under_eye_numder_multiple_255*SENSIBILITY;

      
      
      int increase_speed = AVERAGE_SPEED + speed_increaseng + boost;
      int decrease_speed = AVERAGE_SPEED -speed_increaseng + boost;
      
      
      //Serial.print( "increase = ");Serial.print( increase );Serial.print( " = "); Serial.println( increase_speed );
      //Serial.print( "decrease = ");Serial.print( decrease );Serial.print( " = "); Serial.println( decrease_speed );
           
      
      
      if (decrease_speed < 0) {decrease_speed = 0;}
      if (increase_speed < 0) {increase_speed = 0;}
      if (decrease_speed > 255) {decrease_speed = 255;}
      if (increase_speed > 255) {increase_speed = 255;}
         increase = right;
         decrease = left;
      

        analogWrite(increase,increase_speed ); // 255
        analogWrite(decrease,decrease_speed ); // decision
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
  //Serial.begin(57600);
}

// the loop function runs over and over again forever
void loop() {
  lamp->toBlink();
  rider->drive();
  
}


