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

// user button
#define PIN_BUTTON 12

// buzzer in ZumoShield (enable with the 'BZ' jumper on shield to '3' or '6')
#define PIN_BUZZER 6
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
#define MOTOR_L 2

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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// controls direction and speed of every motor
void setMotor(Motor motor, Direction dir, uint8_t speed)
{
  digitalWrite(PINS[motor++], dir);
  analogWrite(PINS[motor], speed);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// scan the user button
int scanButton(uint64_t millis)
{
  static uint64_t pressed = 0;

  if (digitalRead(PIN_BUTTON) == LOW)
  {
    // pressed
    if (pressed == 0)
    {
      pressed = millis;
    }
  }
  else if (pressed != 0)
  {
    // released
    uint8_t tenths = (millis - pressed) / 100;
    if (tenths > 2)
    {
      // debounce
      pressed = 0;

      // released after long or short pressed
      return (tenths > 20 ? -1 : 1);
    }
  }

  // no released
  return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// Arduino setup
void setup()
{
  // pin configuration
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_MOTOR_R_DIR, OUTPUT);
  pinMode(PIN_MOTOR_L_DIR, OUTPUT);
  pinMode(PIN_MOTOR_R_PWM, OUTPUT);
  pinMode(PIN_MOTOR_L_PWM, OUTPUT);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// Arduino loop
void loop()
{
  static uint8_t prog = 0;
  uint64_t now = millis();

  // change program when button is released
  bool new_program = (scanButton(now) != 0);
  if (new_program)
  {
    // stop motors
    setMotor(MOTOR_R, DIR_FORWARD, 0);
    setMotor(MOTOR_L, DIR_FORWARD, 0);

    // next program
    prog = (prog + 1) % 3;
  }

  // execute program
  switch (prog) {

      case 1:
        // do warming
        progWarming(now, new_program);
        break;

      case 2:
        // do warming
        progProvoke(now, new_program);
        break;

      default:
        // do nothing
        break;
  }

  // dont burn the CPU
  delay(50);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define WARMING_SPEED 0x80

void progWarming(uint64_t millis, bool reset)
{
  static uint8_t current;
  if (reset)
  {
    current = -1;
  }

  uint8_t next = (millis / 1000) % 4;
  if (current != next)
  {
    current = next;
    switch (current % 4) {

        case 0:
          // step right
          setMotor(MOTOR_R, DIR_FORWARD, WARMING_SPEED);
          break;

        case 1:
          // pause
          setMotor(MOTOR_R, DIR_FORWARD, 0);
          beep();
          break;

        case 2:
          // step right
          setMotor(MOTOR_L, DIR_FORWARD, WARMING_SPEED);
          break;

        case 3:
          // pause
          setMotor(MOTOR_L, DIR_FORWARD, 0);
          beep();
          break;
    }
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define FORWARD_SECONDS 2
#define BACKWARD_SECONDS 2

void progProvoke(uint64_t millis, bool reset)
{
  static uint64_t zero;
  static uint8_t current;
  if (reset)
  {
    zero = millis;
    current = -1;
  }

  uint8_t next = (millis - zero) / 1000;
  if (current != next)
  {
    current = next % (FORWARD_SECONDS + BACKWARD_SECONDS);

    if (current < FORWARD_SECONDS)
    {
      setMotor(MOTOR_R, DIR_FORWARD, 0xff);
      setMotor(MOTOR_L, DIR_FORWARD, 0xff);
    }
    else
    {
      setMotor(MOTOR_R, DIR_REVERSE, 0xff * BACKWARD_SECONDS / FORWARD_SECONDS);
      setMotor(MOTOR_L, DIR_REVERSE, 0xff * BACKWARD_SECONDS / FORWARD_SECONDS);
    }
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// EOF

