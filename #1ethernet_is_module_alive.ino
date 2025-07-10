/*
If the ENC28J60 is wired correctly and alive, you'll get something like:

Raw SPI test...
ESTAT = 0x1


  Arsenii'sTechnologies ~Proper way — The only way~   https://exch.com.ua/Bio.php
*/

#include <SPI.h>

#define ENC28J60_CS 10

uint8_t read_enc28j60(uint8_t address)
{
  digitalWrite(ENC28J60_CS, LOW);
  SPI.transfer(0x00 | (address & 0x1F));  // Read Control Register op
  uint8_t result=SPI.transfer(0x00);
  digitalWrite(ENC28J60_CS, HIGH);
  return result;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Raw SPI test...");

  pinMode(ENC28J60_CS, OUTPUT);
  digitalWrite(ENC28J60_CS, HIGH);

  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  uint8_t estat=read_enc28j60(0x1D);  // ESTAT register
  Serial.print("ESTAT = 0x");
  Serial.println(estat, HEX);
}

void loop() {}
