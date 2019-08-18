/*
* Code originalement trouvé sur le site http://www.Banggood.com
* Adapté par https://www.TD72PRO.com
*
* Description des modifications apportées au code:
* - Seuls les commentaires en anglais ont été concervés. Ceux-ci ont aussi été mieux traduits et corrigés.
* - Certains éléments logiques ont été modifiés.
* - Les lignes doublées ont été supprimées.
* - Correction de la syntaxe.
* - La structure du code n'a pas changée (fonctions, conditions, déclarations...).
*
* Améliorations possibles:
* - Améliorer les conditions (switch au lieu de plusieurs if).
* - Optimiser les fonctions (certains éléments sont répétitifs).
* - Supprimer les println() qui ne sont plus nécessaires au débogage du code.
*/

#include <IRremote.h>
#include <Servo.h>

//*********************** Definition of motor pins ********************* ****
int MotorLeft1 = 6;
int MotorLeft2 = 5;
int MotorRight1 = 11;
int MotorRight2 = 10;
int counter = 0;
const int irReceiverPin = 2; //IR receiver connected to pin 2
char val;

// ******* Led d'information ajoutéé
int LedInfraRouge = A1;
int LedUltraSon = 8;
int LedSuiveurLigne = A0;


//*********************** Set to detect the IRcodes ****************** *******
long IRfront = 0x00FF629D; //Forward Fleche vers avant
long IRback = 0x00FFA857; //Reverse Fleche arriere
long IRturnright = 0x00FFC23D; //Right touche fleche droite
long IRturnleft = 0x00FF22DD; //Left touche fleche gauche
long IRstop = 0x00FF02FD; //Stop touche OK
long IRcny70 = 0x00FF42BD; //Line following Touche *
long IRAutorun = 0x00FF52AD; //Self-propelled mode ultrasound Touche #
long IRturnsmallleft = 0x00FF22DD;

//************************* Defined CNY70 pins ******************* *****************
const int SensorLeft = 7; //Left sensor input pin
const int SensorMiddle = 4; //Middle sensor input pin
const int SensorRight = 3; //Right sensor input pin
int SL; //Left sensor status
int SM; //Middle sensor status
int SR; //Right sensor status
IRrecv irrecv(irReceiverPin); //Define an object to receive infrared signals IRrecv
decode_results results; //Decoding results will result in structural variables in decode_results

//************************* Defined ultrasound pins ****************** ************
int inputPin = 13 ; //Echo pin
int outputPin = 12; //Trig pin
int Fspeedd = 0; //Distance in front
int Rspeedd = 0; //Distance right
int Lspeedd = 0; //Distance left
int directionn = 0; //Forward = 8, Rear = 2, Left = 4, Right = 6
Servo myservo; //Set myservo
int delay_time = 250; //Settling time after steering servo motors
int Fgo = 8; //Forward
int Rgo = 6; //Turn right
int Lgo = 4; //Turn left
int Bgo = 2; //Reverse
int servoPin = 9; // Define Pin for Ultrasound servomotor
//************************ Setup **************************************************
void setup()
{
  Serial.begin(9600);
  pinMode(MotorRight1, OUTPUT); //Pin 5 (PWM)
  pinMode(MotorRight2, OUTPUT); //Pin 6 (PWM)
  pinMode(MotorLeft1, OUTPUT);  //Pin 10 (PWM)
  pinMode(MotorLeft2, OUTPUT);  //Pin 11 (PWM)
  irrecv.enableIRIn(); //Start infrared decoding
  pinMode(SensorLeft, INPUT); //Define left Sensors
  pinMode(SensorMiddle, INPUT); //Define middle sensors
  pinMode(SensorRight, INPUT); //Define right sensor
  digitalWrite(2, HIGH);
  pinMode(inputPin, INPUT); //Define ultrasound echo pin
  pinMode(outputPin, OUTPUT); //Define ultrasonic trig pin
  myservo.attach(servoPin); //Define servo motor output pin (PWM)

  // LED d'information
  pinMode(LedInfraRouge, OUTPUT);
  pinMode(LedUltraSon, OUTPUT);
  pinMode(LedSuiveurLigne, OUTPUT);
}

//************************************ (Void)*************************************
void advance(int a) //Go Forward
{
  advance();
  delay(a * 100);
}

void advance() //Go Forward
{
  digitalWrite(MotorLeft1,LOW);
  digitalWrite(MotorLeft2,HIGH);
  digitalWrite(MotorRight1,HIGH);
  digitalWrite(MotorRight2,LOW);
  myservo.write(90);
}

void right(int b) //Turn right
{
  right();
  delay(b * 100);
}
void right() //Turn right
{
  digitalWrite(MotorLeft1,LOW);
  digitalWrite(MotorLeft2,HIGH);
  digitalWrite(MotorRight1,LOW);
  digitalWrite(MotorRight2,LOW);
  myservo.write( 5);

}
void left(int c) //Turn left
{
  left();
  delay(c*100);
}

void left() //Turn left
{
  digitalWrite(MotorLeft1,LOW);
  digitalWrite(MotorLeft2,LOW);
  digitalWrite(MotorRight1,HIGH);
  digitalWrite(MotorRight2,LOW);
  myservo.write( 177);
}

void turnR() //Pivot right
{
  digitalWrite(MotorLeft1,LOW);
  digitalWrite(MotorLeft2,HIGH);
  digitalWrite(MotorRight1,LOW);
  digitalWrite(MotorRight2,HIGH);
}

void turnR(int d) //Pivot right
{
  turnR();
  delay(d * 100);
}

void turnL(int e) //Pivot left
{
  turnL();
  delay(e * 100);
}

void turnL() //Pivot left
{
  digitalWrite(MotorLeft1,HIGH);
  digitalWrite(MotorLeft2,LOW);
  digitalWrite(MotorRight1,HIGH);
  digitalWrite(MotorRight2,LOW);
}

void stopp(int f) //Stop
{
  stop();
  delay(f * 100);
}

void stop() //Stop
{
  digitalWrite(MotorRight1,LOW);
  digitalWrite(MotorRight2,LOW);
  digitalWrite(MotorLeft1,LOW);
  digitalWrite(MotorLeft2,LOW);
}
void back() //Reverse
{
  digitalWrite(MotorLeft1,HIGH);
  digitalWrite(MotorLeft2,LOW);
  digitalWrite(MotorRight1,LOW);
  digitalWrite(MotorRight2,HIGH);
}

void back(int g) //Reverse
{
  back();
  delay(g * 100);
}

void detection() //Measure three angles (front, left, right)
{
  int delay_time = 250; //Settling time after steering servo motors
  ask_pin_F(); //Read from front

  if(Fspeedd < 10) //If the distance is less than 10 cm in front
  {
    stopp(1); //Stop
    back(2); //Reverse 0.2 seconds
  }

  if(Fspeedd < 25) //If the distance is less than 25 cm in front
  {
    stopp(1); //Stop
    ask_pin_L(); //Read from left
    delay(delay_time); //Wait for stable servo motor
    ask_pin_R(); //Read the right distance
    delay(delay_time); //Wait for stable servo motor

    if(Lspeedd > Rspeedd) //If the left distance is greater than the right
    {
      directionn = Lgo; //Go left
    }

    if(Lspeedd <= Rspeedd) //If the left distance is less than or equal to the right
    {
      directionn = Rgo; //Go right
    }

    if (Lspeedd < 15 && Rspeedd < 15) //If the distance to the left and right are less than 15 cm
    {
      directionn = Bgo; //Reverse
    }
  }
  else //If the distance is greater than 25 cm in front
  {
    directionn = Fgo; //Go Forward
  }
}

//*************************************************************************************
void ask_pin_F() //Measure the distance from the front
{
  myservo.write(90);
  digitalWrite(outputPin, LOW); //Make ultrasonic transmitter low voltage 2 μs
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH); //Make ultrasonic transmitting high voltage 10 μs
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW); //Maintain low voltage ultrasonic transmitter
  float Fdistance = pulseIn(inputPin, HIGH); //Read time difference
  Fdistance = Fdistance/5.8/10; //Will turn time to distance (Unit: cm)
  Serial.println("Front distance:"); //Output distance (unit: cm)
  Serial.println(Fdistance); //Display the distance
  Fspeedd = Fdistance;
}

//****************************************************************************************
void ask_pin_L() //Measure the distance from the left
{
  myservo.write( 177);
  delay( delay_time);
  digitalWrite(outputPin, LOW); //Make ultrasonic transmitter low voltage 2 μs
  delayMicroseconds( 2);
  digitalWrite(outputPin, HIGH); //Make ultrasonic transmitting high voltage 10 μs
  delayMicroseconds( 10);
  digitalWrite(outputPin, LOW); //Maintain low voltage ultrasonic transmitter
  float Ldistance = pulseIn(inputPin, HIGH); //Read time difference
  Ldistance= Ldistance/5.8/10; //Will turn time to distance (unit: cm)
  Serial.println("Left distance:"); //Output distance (Unit: cm)
  Serial.println(Ldistance); //Display the distance
  Lspeedd = Ldistance;
}

//****************************************************************************************
void ask_pin_R() //Measure the distance from the right
{
  myservo.write( 5);
  delay( delay_time);
  digitalWrite(outputPin, LOW); //Make ultrasonic transmitter low voltage 2 μs
  delayMicroseconds( 2);
  digitalWrite(outputPin, HIGH); //Make ultrasonic transmitting high voltage 10 μs
  delayMicroseconds( 10);
  digitalWrite(outputPin, LOW); //Maintain low voltage ultrasonic transmitter
  float Rdistance = pulseIn(inputPin, HIGH); //Read time difference
  Rdistance= Rdistance/5.8/10; //Will turn time to distance (Unit: cm)
  Serial.println("Right distance:"); //Output distance (Unit: cm)
  Serial.println(Rdistance); //Display the distance
  Rspeedd = Rdistance;
}

//**************************************** LOOP ******************************************
void loop()
{
  SL = digitalRead(SensorLeft);
  SM = digitalRead(SensorMiddle);
  SR = digitalRead(SensorRight);
  performCommand();

  //***************************************************************************** Normal remote mode
  if (irrecv.decode(&results))
  {
    //Decoding is successful, you receive a set of infrared signals
    irrecv.resume(); //Continue to accept a set of infrared signals
    Serial.println(results.value,HEX);
    digitalWrite(LedInfraRouge,HIGH);
    digitalWrite(LedSuiveurLigne,LOW);
    digitalWrite(LedUltraSon,LOW);
    if (results.value == IRfront) //Forward
    {
      advance(6); //Forward
    }

    if (results.value == IRback) //Reverse
    {
      back(6); //Reverse
    }

    if (results.value == IRturnright) //Turn right
    {
      right(6); //Turn right
    }

    if (results.value == IRturnleft) //Turn left
    {
      left(6); //Turn left
    }

    if (results.value == IRstop) //Stop
    {
      stop();
    }

    //**************************************************************************** Line following
    if (results.value == IRcny70)
    {
      Serial.println("Line tracking mode");
      digitalWrite(LedInfraRouge,LOW);
      digitalWrite(LedSuiveurLigne,HIGH);
      digitalWrite(LedUltraSon,LOW);
      while(1)
      {
        SL = digitalRead(SensorLeft);
        SM = digitalRead(SensorMiddle);
        SR = digitalRead(SensorRight);

        if(SM == HIGH) //If the middle sensor is in a black area
        {
          if (SL == HIGH && SR == LOW) //If left detects black and right detects white, turn left
          {
            Serial.println("Left");
            left();
          }
          else if (SR == HIGH && SL == LOW) //If right detects black and left detects white, turn right
          {
            Serial.println("Right");
            right();
          }
          else //If all sensors detect black, go forward
          {
            Serial.println("Forward");
            advance();
          }
        }
        else //If the middle sensor is in a white area
        {
          if (SL == HIGH && SR == LOW) //If left detects black and right detects white, turn left
          {
            Serial.println("Left");
            left();
          }
          else if (SR == HIGH && SL == LOW) //If right detects black and left detects white, turn right
          {
            Serial.println("Right");
            right();
          }
          else //If all sensors detect white, reverse
          {
            Serial.println("Reverse");
            back();
          }
        }

        if (irrecv.decode(&results))
        {
          irrecv.resume();
          Serial.println(results.value,HEX);
          if( results.value == IRstop)
          {
            stop();
            break;
          }
        }
      }
      results.value=0;
    }

    //********************************************************************************************* Self-propelled mode ultrasound
    if (results.value == IRAutorun)
    {
      Serial.println("Ultrasound mode");
      digitalWrite(LedInfraRouge,LOW);
      digitalWrite(LedSuiveurLigne,LOW);
      digitalWrite(LedUltraSon,HIGH);
      while(1)
      {
        myservo.write(90); //Put servo in middle position (pointing to the front of the robot)
        detection(); //Measure the distance in each direction
        if(directionn == 8) //Forward
        {
          if (irrecv.decode(&results))
          {
            irrecv.resume();
            Serial.println(results.value,HEX);
            if( results.value == IRstop)
            {
              stop();
              break;
            }
          }
          results.value=0;
          advance(1); //Go forward
          Serial.println("Forward");
        }
        if(directionn == 2) //Reverse
        {
          if (irrecv.decode(&results))
          {
            irrecv.resume();
            Serial.println(results.value,HEX);
            if( results.value == IRstop)
            {
              stop();
              break;
            }
          }
          results.value=0;
          back(8); //Reverse
          turnL(3); //Move slightly to the left
          Serial.println("Reverse");
        }
        if(directionn == 6) //Turn right
        {
          if (irrecv.decode(&results))
          {
            irrecv.resume();
            Serial.println(results.value,HEX);
            if(results.value == IRstop)
            {
              stop();
              break;
            }
          }
          results.value=0;
          back(1);
          turnR(6); //Turn right
          Serial.print("Right");
        }
        if(directionn == 4) //Turn left
        {
          if (irrecv.decode(&results))
          {
            irrecv.resume();
            Serial.println(results.value,HEX);
            if( results.value == IRstop)
            {
              stop();
              break;
            }
          }
          results.value=0;
          back( 1);
          turnL(6); //Turn left
          Serial.print("Left");
        }

        if (irrecv.decode(&results))
        {
          irrecv.resume();
          Serial.println(results.value,HEX);
          if( results.value == IRstop)
          {
            stop();
            break;
          }
        }
      }
      results.value=0;
    }
    else
    {
      stop();
    }
  }
}

void performCommand()
{
  if (Serial.available())
  {
    val = Serial.read();
  }
  if (val == 'f') //Forwards
  {
    advance(10);
  }
  else if (val == 'b') //back
  {
    back(10);
  }
  else if (val == 'l') //right
  {
    turnL(10);
  }
  else if (val == 'r') //left
  {
    turnR(10);
  }
  else if (val == 's' ) //stop
  {
    stopp(10);
  }
}