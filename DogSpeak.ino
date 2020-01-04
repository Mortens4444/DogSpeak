/*
 * Tutorial pages:
 * https://arduinogetstarted.com/tutorials/arduino-button-debounce
 * https://www.arduino.cc/en/Tutorial/WebClient
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

void setup()
{
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while(!Serial);

  // Start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true)
      {
        delay(1); // Do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF)
    {
      Serial.println("Ethernet cable is not connected.");
    }
    // Try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
    Serial.print("Using IP address: ");
    Serial.println(Ethernet.localIP());
  }
  else
  {
    Serial.print("DHCP assigned IP: ");
    Serial.println(Ethernet.localIP());
  }
  
  // Initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
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
  // If you get a connection, report back via serial:
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
    // If you didn't get a connection to the server:
    Serial.print("Connection failed to: ");
    Serial.println(client.remoteIP());
  }
}

void readPin(byte pinToRead, int* lastState, char* request)
{
  // Read the state of the switch/button:
  int currentState = digitalRead(pinToRead);
  if (*lastState == HIGH && currentState == LOW)
  {
    Serial.println("The button is pressed");
    sendUrlRequest(request);
  }
  else if(*lastState == LOW && currentState == HIGH)
  {
    Serial.println("The button is released");
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
