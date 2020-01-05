#include <SPI.h>
#include <Ethernet.h>

#include "EthernetUtils.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
IPAddress dns(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress server(192, 168, 1, 105);
int serverServicePort = 44358;

EthernetClient client;

void EthernetUtils::CheckEthernetHardwarePresence()
{
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found. Sorry, can't run without hardware.");
    while (true)
    {
      delay(100); // Do nothing, no point running without Ethernet hardware
    }
  }
}

void EthernetUtils::CheckLinkStatus()
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

void EthernetUtils::PrintIpAddress()
{
  Serial.print("Using IP address: ");
  Serial.println(Ethernet.localIP());
}

void EthernetUtils::InitializeEthernet()
{
  Serial.println("Trying to initialize Ethernet with DHCP...");
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    CheckEthernetHardwarePresence();
    CheckLinkStatus();
    Ethernet.begin(mac, ip, dns, gateway, subnet);
  }
  PrintIpAddress();
}

void EthernetUtils::SendUrlRequest(const char* action)
{
  if (client.connect(server, serverServicePort))
  {
    Serial.print("Connected to: ");
    Serial.println(client.remoteIP());
    
    client.print("GET /Home/");
    client.print(action);
    client.print(" HTTP/1.1");
    
    client.print("Host: https://");
    client.print(server);
    client.print(":");
    client.print(serverServicePort);    
    client.println("/");
    
    client.println("Connection: close");
    client.println();
  }
  else
  {
    Serial.print("Connection failed to: ");
    Serial.println(server);
  }
}
