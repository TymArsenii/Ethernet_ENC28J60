/*
If the module is alive, wired correctly, web page on IP that you see in Serial port with content text "It works!" will appear

  Arsenii'sTechnologies ~Proper way — The only way~   https://exch.com.ua/Bio.php
*/

#define ENC28J60_CS 10
#include <UIPEthernet.h>

byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server(80);

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing ENC28J60...");

  Ethernet.init(ENC28J60_CS);

  if(Ethernet.begin(mac)==0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    IPAddress ip(192, 168, 1, 177);
    Ethernet.begin(mac, ip);
  }

  delay(1000);
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  server.begin();
}

void loop()
{
  EthernetClient client=server.available();
  if(client)
  {
    Serial.println("Client connected");
    while (client.connected())
    {
      if(client.available())
      {
        char c=client.read();
        Serial.write(c);
        if(c=='\n')
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println("Connection: close");
          client.println();
          client.println("It works!");
          break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }
}
