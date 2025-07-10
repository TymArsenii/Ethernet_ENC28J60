/*
TCP modbus code example to send an array of data with any size

  Arsenii'sTechnologies ~Proper way — The only way~   https://exch.com.ua/Bio.php
*/


#include <UIPEthernet.h>

#define ENC28J60_CS 10
EthernetServer server(502);
byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

const int send_arr_size=30;
float send_arr[send_arr_size+1]={1.23, 4.56, 7.89, 12.34, 56.78, 90.12, -3.14, -42.42, 0.01, 1234.567};

void setup() {
  Serial.begin(115200);
  Serial.println("start");
  Ethernet.init(ENC28J60_CS);
  if (Ethernet.begin(mac)==0) 
  {
    IPAddress ip(192, 168, 1, 177);
    Ethernet.begin(mac, ip);
  }
  server.begin();
  Serial.print("Modbus server on ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client=server.available();
  if (!client || !client.connected()) return;

  if (client.available()>=12) 
  {
    byte request[12];
    client.read(request, 12);

    uint8_t unit_id=request[6];
    uint8_t func=request[7];
    uint16_t addr=(request[8] << 8) | request[9];
    uint16_t count=(request[10] << 8) | request[11];

    Serial.print("Req FC: "); Serial.print(func);
    Serial.print(" Addr: "); Serial.print(addr);
    Serial.print(" Count: "); Serial.println(count);

    uint16_t total_regs=send_arr_size*2;

    if (func==3 && addr+count<=total_regs) 
    {
      byte response[260];
      uint16_t byte_count=count*2;
      uint16_t len=3+byte_count;

      response[0]=request[0];
      response[1]=request[1];
      response[2]=0;
      response[3]=0;
      response[4]=highByte(len);
      response[5]=lowByte(len);
      response[6]=unit_id;
      response[7]=func;
      response[8]=byte_count;

      for (uint16_t i=0; i<count; i++) 
      {
        uint16_t reg_index=addr+i;
        uint8_t float_index=reg_index/2;
        uint8_t part=reg_index%2;

        //float val=(float)random(5, 1000)/100; //test
        //uint32_t fbits=*((uint32_t*)&val); //test

        float val=send_arr[float_index];
        uint32_t fbits=*((uint32_t*)&val);

        if (part==0) 
        {
          response[9+i*2]=(fbits >> 24) & 0xFF;
          response[10+i*2]=(fbits >> 16) & 0xFF;
        } 
        else
        {
          response[9+i*2]=(fbits >> 8) & 0xFF;
          response[10+i*2]=fbits & 0xFF;
        }
      }

      client.write(response, 9+byte_count);
    } 
    else 
    {
      byte err[]=
      {
        request[0], request[1], 0, 0, 0, 3, unit_id, func | 0x80, 0x02
      };
      client.write(err, 9);
    }

    client.flush();
    //delay(10);
    //client.stop();
  }
}
