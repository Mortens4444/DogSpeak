/*
 * Tutorial pages:
 * https://arduinogetstarted.com/tutorials/arduino-button-debounce
 * https://www.arduino.cc/en/Tutorial/WebClient
 * 
 * Documentations:
 * https://www.arduino.cc/en/Main/ArduinoEthernetShieldV1
 * https://www.arduino.cc/en/uploads/Main/arduino-ethernet-shield-06-schematic.pdf
 * https://www.sparkfun.com/datasheets/DevTools/Arduino/W5100_Datasheet_v1_1_6.pdf
 * https://subscription.packtpub.com/book/hardware_and_creative/9781785285486/1/ch01lvl1sec12/finding-the-mac-address-and-obtaining-a-valid-ip-address
 */

#include "Constants.h"
#include "EthernetUtils.h"

// The previous states from the input pins
int lastStateOfWalk = LOW;
int lastStateOfHungry = LOW;
int lastStateOfThirsty = LOW;
int lastStateOfPlay = LOW;
int lastStateOfPee = LOW;
int lastStateOfPoop = LOW;
int lastStateOfCare = LOW;
int lastStateOfSleep = LOW;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  EthernetUtils::InitializeEthernet();
  
  // Initialize the push-button pins as pull-up inputs.
  // The pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(WALK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(HUNGRY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(THIRSTY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PLAY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PEE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(POOP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CARE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SLEEP_BUTTON_PIN, INPUT_PULLUP);
}

void WriteToSerialButtonState(byte pinToRead, bool pressed)
{
  Serial.print("The button (");
  Serial.print(pinToRead);
  Serial.println(pressed ? ") is pressed." : ") is released.");
}

void readPin(byte pinToRead, int* lastState, char* action)
{
  // Read the state of the switch/button:
  int currentState = digitalRead(pinToRead);
  if (*lastState == HIGH && currentState == LOW)
  {
    WriteToSerialButtonState(pinToRead, true);
    EthernetUtils::SendUrlRequest(action);
  }
  else if(*lastState == LOW && currentState == HIGH)
  {
    WriteToSerialButtonState(pinToRead, false);
  }
  *lastState = currentState;
}

void loop()
{
  readPin(WALK_BUTTON_PIN, &lastStateOfWalk, (char*)"LetsGoForAWalk");
  readPin(HUNGRY_BUTTON_PIN, &lastStateOfHungry, (char*)"ImHungry");
  readPin(THIRSTY_BUTTON_PIN, &lastStateOfThirsty, (char*)"ImThirsty");
  readPin(PLAY_BUTTON_PIN, &lastStateOfPlay, (char*)"IWantToPlay");
  readPin(PEE_BUTTON_PIN, &lastStateOfPee, (char*)"INeedToPee");
  readPin(POOP_BUTTON_PIN, &lastStateOfPoop, (char*)"INeedToPoop");
  readPin(CARE_BUTTON_PIN, &lastStateOfCare, (char*)"INeedCare");
  readPin(SLEEP_BUTTON_PIN, &lastStateOfSleep, (char*)"SleepyDog");

  delay(150);
}
