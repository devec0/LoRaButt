LoRaButt
========

The IoTs have been plagued by hax since inception.
Newer protocols, smaller devices... but where is the assurance?
There is no S in IoT, afterall.

Introducing LoRaButt. A slower, ThreatButt secured LoRaWAN application which can be used to transfer _sensitive_ data at record-low
speeds, over great distances, with all of the security assosciated with ThreatButt branded IoT appliances.

LoRaButt comes in two parts -
* LoRaButt Sender -> used for sending _sensitive_ data.
* LoRaButt Reciever -> used for receiving _sensitive_ data.

Each contains an Arduino sketch, written for the Heltec WiFi LoRa 32 - they should be very easy to port to any ESP32 board with an OLED
supported by u8g2, and a LoRa chipset supported by Arduino-LoRa.
