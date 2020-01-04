/*
 * Tutorial pages:
 * https://arduinogetstarted.com/tutorials/arduino-button-debounce
 * https://www.arduino.cc/en/Tutorial/WebClient
 * 
 * Documentations:
 * https://www.arduino.cc/en/Main/ArduinoEthernetShieldV1
 * https://www.arduino.cc/en/uploads/Main/arduino-ethernet-shield-06-schematic.pdf
 * https://www.sparkfun.com/datasheets/DevTools/Arduino/W5100_Datasheet_v1_1_6.pdf
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

EthernetClient client;

const byte WALK_BUTTON_PIN = 1;
const byte HUNGRY_BUTTON_PIN = 2;
const byte THIRSTY_BUTTON_PIN = 3;
const byte PLAY_BUTTON_PIN = 4;
const byte PEE_BUTTON_PIN = 5;
const byte POOP_BUTTON_PIN = 6;
const byte CARE_BUTTON_PIN = 7;
const byte SLEEP_BUTTON_PIN = 8;

// The previous states from the input pins
int lastStateOfWalk = LOW;
int lastStateOfHungry = LOW;
int lastStateOfThirsty = LOW;
int lastStateOfPlay = LOW;
int lastStateOfPee = LOW;
int lastStateOfPoop = LOW;
int lastStateOfCare = LOW;
int lastStateOfSleep = LOW;

void CheckEthernetHardwarePresence()
{
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found. Sorry, can't run without hardware.");
    while (true)
    {
      delay(1); // Do nothing, no point running without Ethernet hardware
    }
  }
}

void CheckLinkStatus()
{
  byte linkStatus = Ethernet.linkStatus();
  if (linkStatus == Unknown)
  {
    Serial.println("Link status unknown. Link status detection is only available with W5200 and W5500.");
  }
  else if (linkStatus == LinkON)
  {
    Serial.println("Link status: On");
  }
  else if (linkStatus == LinkOFF)
  {
    Serial.println("Link status: Off. Plug in Ethernet cable.");
  }
}

void PrintIpAddress()
{
  Serial.print("Using IP address: ");
  Serial.println(Ethernet.localIP());
}

void InitializeEthernet()
{
  Serial.println("Trying to initialize Ethernet with DHCP...");
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    CheckEthernetHardwarePresence();
    CheckLinkStatus();
    Ethernet.begin(mac, ip);
  }
  PrintIpAddress();
}

void setup()
{
  Serial.begin(9600);
  while(!Serial);

  InitializeEthernet();
  
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

void sendUrlRequest(const char* request)
{
  IPAddress server(192,168,1,105);
  if (client.connect(server, 44358))
  {
    Serial.print("Connected to: ");
    Serial.println(client.remoteIP());
    
    // Make a HTTP request:
    client.println(request);
    client.println("Host: https://192.168.1.105:44358/");
    client.println("Connection: close");
    client.println();
  }
  else
  {
    Serial.print("Connection failed to: ");
    Serial.println(server);
  }
}

void WriteToSerialButtonState(byte pinToRead, boolean pressed)
{
  Serial.print("The button (");
  Serial.print(pinToRead);
  Serial.println(pressed ? ") is pressed." : ") is released.");
}

void readPin(byte pinToRead, int* lastState, char* request)
{
  // Read the state of the switch/button:
  int currentState = digitalRead(pinToRead);
  if (*lastState == HIGH && currentState == LOW)
  {
    WriteToSerialButtonState(pinToRead, true);
    sendUrlRequest(request);
  }
  else if(*lastState == LOW && currentState == HIGH)
  {
    WriteToSerialButtonState(pinToRead, false);
  }
  *lastState = currentState;
}

void loop()
{
  readPin(WALK_BUTTON_PIN, &lastStateOfWalk, (char*)"GET /Home/LetsGoForAWalk HTTP/1.1");
  readPin(HUNGRY_BUTTON_PIN, &lastStateOfHungry, (char*)"GET /Home/ImHungry HTTP/1.1");
  readPin(THIRSTY_BUTTON_PIN, &lastStateOfThirsty, (char*)"GET /Home/ImThirsty HTTP/1.1");
  readPin(PLAY_BUTTON_PIN, &lastStateOfPlay, (char*)"GET /Home/IWantToPlay HTTP/1.1");
  readPin(PEE_BUTTON_PIN, &lastStateOfPee, (char*)"GET /Home/INeedToPee HTTP/1.1");
  readPin(POOP_BUTTON_PIN, &lastStateOfPoop, (char*)"GET /Home/INeedToPoop HTTP/1.1");
  readPin(CARE_BUTTON_PIN, &lastStateOfCare, (char*)"GET /Home/INeedCare HTTP/1.1");
  readPin(SLEEP_BUTTON_PIN, &lastStateOfSleep, (char*)"GET /Home/SleepyDog HTTP/1.1");

  delay(200);
}
