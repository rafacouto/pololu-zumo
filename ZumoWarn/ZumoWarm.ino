//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// 
// ZumoWarm.ino
// 
// Moves the Zumo like a fighter before the combat (warming the muscles).
// 
// License: GPLv3
// Further info: http://github.com/rafacouto/pololu-zumo/ZumoWarm
// 
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// buzzer in ZumoShield (enable with the 'BZ' jumper on shield to '3' or '6')
#define PIN_BUZZER 3
#define BUZZER_BEEP_FREQUENCY 4699
#define BUZZER_BEEP_MILLIS 100

// motor direction (DIR) and speed (PWM) control lines (Right and Left)
#define PIN_MOTOR_R_DIR 7
#define PIN_MOTOR_L_DIR 8
#define PIN_MOTOR_R_PWM 9
#define PIN_MOTOR_L_PWM 10

// motor enumeration
#define Motor uint8_t
#define MOTOR_R 0
#define MOTOR_L 1

// direction constants
#define Direction uint8_t
#define DIR_FORWARD LOW
#define DIR_REVERSE HIGH

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// generates a 'beep' sound (uses Arduino tone generation)
inline void beep()
{
  tone(PIN_BUZZER, BUZZER_BEEP_FREQUENCY, BUZZER_BEEP_MILLIS);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// mapping pins
const uint8_t PINS[] = {
  PIN_MOTOR_R_DIR, PIN_MOTOR_R_PWM,
  PIN_MOTOR_L_DIR, PIN_MOTOR_L_PWM,
};


// controls direction and speed of every motor
void setMotor(Motor motor, Direction dir, uint8_t speed)
{
  uint8_t pin = motor * 2;
  digitalWrite(PINS[pin++], dir);
  digitalWrite(PINS[pin], speed);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// Arduino setup
void setup()
{
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_MOTOR_R_DIR, OUTPUT);
  pinMode(PIN_MOTOR_L_DIR, OUTPUT);
  pinMode(PIN_MOTOR_R_PWM, OUTPUT);
  pinMode(PIN_MOTOR_L_PWM, OUTPUT);

}


// Arduino loop
void loop()
{
  delay(1000);  
  beep();
  delay(1000);  

  // step right
  setMotor(MOTOR_R, DIR_FORWARD, 255);
  delay(250);
  setMotor(MOTOR_R, DIR_FORWARD, 0);
  
  delay(1000);  

  // step left
  setMotor(MOTOR_L, DIR_FORWARD, 255);
  delay(250);
  setMotor(MOTOR_L, DIR_FORWARD, 0);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// EOF

