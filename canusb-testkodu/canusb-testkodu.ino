#include <LiquidCrystal.h>

byte veri_dizisi[14];
byte veri_dizisi2[14];
byte veri_dizisi3[14];
uint16_t crc_bytes;
uint8_t crc_byte_small;
uint8_t crc_byte_high;
uint16_t calcCRC16(uint8_t *pt, uint8_t msgLen);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
static int step;
int i = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  veri_dizisi[0] = 171; //0xab header
  veri_dizisi[1] = 15; //extended, dlc 8
  veri_dizisi[2] = 0; 
  veri_dizisi[3] = 0; 
  veri_dizisi[4] = 0;
  veri_dizisi[5] = 1; //id = 1
  veri_dizisi[6] = 2;
  veri_dizisi[7] = 2;
  veri_dizisi[8] = 2;
  veri_dizisi[9] = 2;
  veri_dizisi[10] = 2;
  veri_dizisi[11] = 2;
  veri_dizisi[12] = 2;
  veri_dizisi[13] = 2; //tüm mesaj 2

  veri_dizisi2[0] = 171; //0xab header
  veri_dizisi2[1] = 10; //extended, dlc 3
  veri_dizisi2[2] = 21; 
  veri_dizisi2[3] = 85; 
  veri_dizisi2[4] = 221;
  veri_dizisi2[5] = 221; //id = 0x1555dddd
  veri_dizisi2[6] = 4;
  veri_dizisi2[7] = 4;
  veri_dizisi2[8] = 4;
  
  veri_dizisi3[0] = 171; //0xab header
  veri_dizisi3[1] = 10; //extended, dlc 3
  veri_dizisi3[2] = 0; 
  veri_dizisi3[3] = 0; 
  veri_dizisi3[4] = 0;
  veri_dizisi3[5] = 0; //id = 0x1555dddd
  veri_dizisi3[6] = 4;
  veri_dizisi3[7] = 4;
  veri_dizisi3[8] = 4;
}

void loop()
{
  
  for(int j = 0; j < 14; j++)
  {
      Serial.write(veri_dizisi[j]);
  }
  delay(100);
  for(int j = 0; j < 9; j++)
  {
      Serial.write(veri_dizisi2[j]);
  }
  delay(100);
  for(int j = 0; j < 9; j++)
  {
      Serial.write(veri_dizisi3[j]);
  }
  delay(100);


}
