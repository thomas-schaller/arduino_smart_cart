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
* - Supprimer les println() qui ne sont plus nécessaires au débogage du code.
*/

#include <IRremote.h>
#include <Servo.h>

//*********************** Definition of motor pins ********************* ****
const int MotorLeft1 = 6;
const int MotorLeft2 = 5;
const int MotorRight1 = 11;
const int MotorRight2 = 10;
int counter = 0;
const int irReceiverPin = 2; //IR receiver connected to pin 2
char val;
// ******* Led d'information ajoutéé
const int LedInfraRouge = A1;
const int LedUltraSon = 8;
const int LedSuiveurLigne = A0;

//*********************** Set to detect the IRcodes ****************** *******
const long IRfront = 0x00FF629D; //Forward Fleche vers avant
const long IRback = 0x00FFA857; //Reverse Fleche arriere
const long IRturnright = 0x00FFC23D; //Right touche fleche droite
const long IRturnleft = 0x00FF22DD; //Left touche fleche gauche
const long IRstop = 0x00FF02FD; //Stop touche OK
const long IRcny70 = 0x00FF42BD; //Line following Touche *
const long IRAutorun = 0x00FF52AD; //Self-propelled mode ultrasound Touche #
const long IRturnsmallleft = 0x00FF22DD;

//************************* Defined CNY70 pins ******************* *****************
const int SensorLeft = 7; //Left sensor input pin
const int SensorMiddle = 4; //Middle sensor input pin
const int SensorRight = 3; //Right sensor input pin
int SL; //Left sensor status
int SM; //Middle sensor status
int SR; //Right sensor status
IRrecv irrecv(irReceiverPin); //Define an object to receive infrared signals IRrecv
decode_results results; //Decoding results will result in structural variables in decode_results
decode_results pendingResults; //Decoding results will result in structural variables in decode_results
//************************* Defined ultrasound pins ****************** ************
const int inputPin = 13 ; //Echo pin
const int outputPin = 12; //Trig pin
int Fspeedd = 0; //Distance in front
int Rspeedd = 0; //Distance right
int Lspeedd = 0; //Distance left
int directionn = 0; //Forward = 8, Rear = 2, Left = 4, Right = 6
Servo myservo; //Set myservo
const int delay_time = 250; //Settling time after steering servo motors
const int Fgo = 8; //Forward
const int Rgo = 6; //Turn right
const int Lgo = 4; //Turn left
const int Bgo = 2; //Reverse

const int near = 25;
const int veryNear = 15;
//************************ Servomotor **************************************************


const int servoLeftDirection = 177;
const int servoRightDirection = 5;
const int servoFrontDirection = 90;
const int servoPin = 9; // Define Pin for Ultrasound servomotor
//************************ Setup **************************************************

unsigned long endingMilli =0;

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

void advanceWD(int a) //Go Forward
{
  advance();
  endingMilli = millis() +a * 100;
}
void advance() //Go Forward
{
  digitalWrite(MotorLeft1,LOW);
  digitalWrite(MotorLeft2,HIGH);
  digitalWrite(MotorRight1,HIGH);
  digitalWrite(MotorRight2,LOW);
}

void right(int b) //Turn right
{
  right();
  delay(b * 100);
}

void rightWD(int b) //Turn right
{
  right();
  endingMilli = millis() +b * 100;
}
void right() //Turn right
{
  digitalWrite(MotorLeft1,LOW);
  digitalWrite(MotorLeft2,HIGH);
  digitalWrite(MotorRight1,LOW);
  digitalWrite(MotorRight2,LOW);

}
void left(int c) //Turn left
{
  left();
  delay(c*100);
}

void leftWD(int c) //Turn left
{
  left();
  endingMilli = millis() +c * 100;
}
void left() //Turn left
{
  digitalWrite(MotorLeft1,LOW);
  digitalWrite(MotorLeft2,LOW);
  digitalWrite(MotorRight1,HIGH);
  digitalWrite(MotorRight2,LOW);
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

void backWD(int g) //Reverse
{
  back();
endingMilli = millis() +g * 100;
}
void detection() //Measure three angles (front, left, right)
{
  ask_pin_F(); //Read from front

  if(Fspeedd < veryNear) //If the distance is less than 10 cm in front
  {
    stopp(1); //Stop
    back(2); //Reverse 0.2 seconds
  }
  if(Fspeedd < near) //If the distance is less than 25 cm in front
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

    if (Lspeedd < veryNear && Rspeedd < veryNear) //If the distance to the left and right are less than 15 cm
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
  myservo.write(servoFrontDirection);
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
  myservo.write( servoLeftDirection);
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
  myservo.write( servoRightDirection);
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

void followLine()
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

void autorun()
{
  Serial.println("Ultrasound mode");
      digitalWrite(LedInfraRouge,LOW);
      digitalWrite(LedSuiveurLigne,LOW);
      digitalWrite(LedUltraSon,HIGH);
      while(1)
      {
        myservo.write(servoFrontDirection); //Put servo in middle position (pointing to the front of the robot)
        detection(); //Measure the distance in each direction
        switch(directionn)
        {
          case Fgo:
            advance(1); //Go forward
            Serial.println("Forward");
          break;
          case Bgo:
            back(8); //Reverse
            turnL(3); //Move slightly to the left
            Serial.println("Reverse");
          break;
          case Rgo:
            back(1);
            turnR(6); //Turn right
            Serial.print("Right");
          break;
          case Lgo:
            back( 1);
            turnL(6); //Turn left
            Serial.print("Left");
          break;
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
        results.value=0;
      }

}
//**************************************** LOOP ******************************************
void loop()
{
  performCommand();
  if (endingMilli < millis())
  {
    stop();
  }
  //***************************************************************************** Normal remote mode
  if (irrecv.decode(&pendingResults))
  {
    results=pendingResults;
    irrecv.resume(); //Continue to accept a set of infrared signals
    digitalWrite(LedInfraRouge,LOW);
    //Decoding is successful, you receive a set of infrared signals
    digitalWrite(LedSuiveurLigne,LOW);
    digitalWrite(LedUltraSon,LOW);
    switch(results.value)
    {
      case IRfront://Forward
              ask_pin_F();
        if(Fspeedd < 25) //If the distance is less than 10 cm in front
        {
          stop();
        }
        else
        {
        advanceWD(6);
        }
      break;
      case IRback://Reverse
        backWD(6); //Reverse

      break;
      case IRturnright:
      rightWD(6); //Turn right
  
      break;
      case IRturnleft:
      leftWD(6); //Turn left
 
      break;
      case IRstop: //Stop
      stop();
      break;
      case IRcny70: //Line following
        followLine();
        break;
      case IRAutorun: //Self-propelled mode ultrasound
        autorun();
        break;
    }
  }

}


void performCommand()
{
  if (Serial.available())
  {
    val = Serial.read();
    digitalWrite(LedInfraRouge,HIGH);
    switch(val)
    {
      case 'f':
        advance(10);
        break;
      case 'b':
       back(10);
       break;
      case 'l':
        turnL(10);
        break;
      case 'r':
      turnR(10);
      break;
      case 's':
      stopp(10);
      
    }

  }
}
