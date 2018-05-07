#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <LoRa.h>

#define SS 18
#define RST 14
#define DI0 26

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

void setup() {

  Serial.begin(9600);
  u8g2.begin();
    
  u8g2.setFontMode(0);
  u8g2.setFontDirection(0);
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_12b_tr);

  u8g2.drawStr(0, 8, "LoRA RX");
  u8g2.sendBuffer();

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(433E6)) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 8, "LoRa failed!");
    u8g2.sendBuffer();
    while (1);
  }
  
  u8g2.drawStr(0, 8, "LoRa RX OK!");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();

}

void loop() {

  //clear stats display
  u8g2.setDrawColor(0);
  u8g2.drawBox(0,0,64,64);
  u8g2.setDrawColor(1);
  
  u8g2.drawStr(0, 8, "LoRa XBM RX");
  
  // try to parse packet
  int packetsize = LoRa.parsePacket();
  if (packetsize) {

    // read packet
    unsigned char rx_bytes[512];
    int offset = 0;
    
    while (LoRa.available()) {
      
      char rx_buf;
      rx_buf = (char)LoRa.read();
      rx_bytes[offset] = rx_buf;
      offset++;
    }

    int gfxrow = rx_bytes[0];
    
    char row[3];
    itoa(gfxrow, row, 10);
    u8g2.drawStr(0, 17, "RX: ");
    u8g2.drawStr(38, 17, row);

    char gfxref[3];
    itoa(gfxrow * 8, gfxref, 10);
    u8g2.drawStr(0, 26, "RID:        ");
    u8g2.drawStr(38, 26, gfxref);
    
    // print RSSI of packet
    u8g2.drawStr(0, 35, "RSSI:       ");
    char rssi[12];
    itoa(LoRa.packetRssi(), rssi, 10);
    u8g2.drawStr(38, 35, rssi);

    Serial.println("RX");
    // loop over remaing bytes, convert to array of bits, loop
    for (int byteno = 0; byteno < 8; byteno++)
    {
      char current_byte = rx_bytes[byteno+1];
      Serial.print("Pushing index: ");
      Serial.print((gfxrow*8) + byteno);
      Serial.print(": ");
      Serial.println(current_byte, BIN);
      for (int bitno = 0; bitno < 8; bitno++)
      {
        int colour = current_byte & 0x01;
        u8g2.setDrawColor(colour);
        u8g2.drawPixel(65 + ((byteno*8) + bitno), gfxrow);
        current_byte = current_byte >> 1;
      }
      u8g2.setDrawColor(1);
    }
      
    u8g2.sendBuffer();
  }
}
