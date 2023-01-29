//#include <avr/wdt.h>  /* Подключаем библиотеку для использования сторожевого таймера */
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_PRECISION 12

#define ONE_WIRE_BUS_BAK 5 /* термометры градиента температуры бака */
#define ONE_WIRE_BUS_KOTEL 6 /* термометры котла */
#define ONE_WIRE_BUS_VODOGREY 7 /* термометры водогрея */
#define ONE_WIRE_BUS_ROOM 8 /* термометры комнаты */
#define RS485_SEND 10

OneWire oneWire_Graduent_Bak(ONE_WIRE_BUS_BAK);
OneWire oneWire_Kotel(ONE_WIRE_BUS_KOTEL);
OneWire oneWire_Vodogrey(ONE_WIRE_BUS_VODOGREY);
OneWire oneWire_Room(ONE_WIRE_BUS_ROOM);

DallasTemperature tempSensor_Graduent_Bak(&oneWire_Graduent_Bak);
DallasTemperature tempSensor_Kotel(&oneWire_Kotel);
DallasTemperature tempSensor_Vodogrey(&oneWire_Vodogrey);
DallasTemperature tempSensor_Room(&oneWire_Room);

/* адреса устройств */
DeviceAddress adrSweatRoom = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
DeviceAddress adrGydroRoom = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
DeviceAddress adrKotelIn1 = { 0x28, 0xE4, 0x06, 0x16, 0xA8, 0x01, 0x3C, 0x4C };
DeviceAddress adrKotelIn2 = { 0x28, 0x18, 0xE4, 0x16, 0xA8, 0x01, 0x3C, 0x1A };
DeviceAddress adr3xCold = { 0x28, 0xCE, 0x0A, 0x16, 0xA8, 0x01, 0x3C, 0x14 };
DeviceAddress adr3xHot = { 0x28, 0x76, 0x35, 0x16, 0xA8, 0x01, 0x3C, 0x59 };
DeviceAddress adr3xMix = { 0x28, 0xB6, 0x03, 0x16, 0xA8, 0x01, 0x3C, 0xDE };
DeviceAddress adrPodacha = { 0x28, 0x75, 0xEC, 0x16, 0xA8, 0x01, 0x3C, 0xAD };
DeviceAddress adrObratka = { 0x28, 0x9C, 0x6C, 0x16, 0xA8, 0x01, 0x3C, 0xD9 };
DeviceAddress adrVodaUp = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
DeviceAddress adrVodaMidle = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
DeviceAddress adrVodaLow = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
DeviceAddress adrBakUp = { 0x28, 0x40, 0xE9, 0x16, 0xA8, 0x01, 0x3C, 0x79 };
DeviceAddress adrBakHigh = { 0x28, 0x5E, 0x15, 0x16, 0xA8, 0x01, 0x3C, 0xB6 };
DeviceAddress adrBakMiddle = { 0x28, 0x9A, 0x76, 0x16, 0xA8, 0x01, 0x3C, 0xAA };
DeviceAddress adrBakLow = { 0x28, 0xBA, 0xB0, 0x16, 0xA8, 0x01, 0x3C, 0xA2 };
DeviceAddress adrBakDown = { 0x28, 0x25, 0x13, 0x16, 0xA8, 0x01, 0x3C, 0x4A };


const char *termoNameArray[] = {
                                "tSweatRoom", "tGydroRoom",
                                "tKotelIn1", "tKotelIn2",
                                "t3xCold", "t3xHot", "t3xMix",
                                "tPodacha", "tObratka",
                                "tVodaUp", "tVodaMidle", "tVodaLow",
                                "tBakUp", "tBakHigh", "tBakMiddle", "tBakLow", "tBakDown"
                              };
                         
uint8_t headers_am = sizeof(termoNameArray) / 2;

float tSweatRoom, tGydroRoom;
float tKotelIn1, tKotelIn2;
float t3xCold, t3xHot, t3xMix;
float tPodacha, tObratka;
float tVodaUp, tVodaMidle, tVodaLow;
float tBakUp, tBakHigh, tBakMiddle, tBakLow, tBakDown;

float *termoValue[] = {
                        &tSweatRoom, &tGydroRoom,
                        &tKotelIn1, &tKotelIn2,
                        &t3xCold, &t3xHot, &t3xMix,
                        &tPodacha, &tObratka,
                        &tVodaUp, &tVodaMidle, &tVodaLow,
                        &tBakUp, &tBakHigh, &tBakMiddle, &tBakLow, &tBakDown
                      };

/*---------------------------------------------------------------------------------------------*/
void setup() {
//  wdt_enable(WDTO_2S); //WDTO_15MS,WDTO_30MS,WDTO_60MS,WDTO_120MS,WDTO_250MS,WDTO_500MS,WDTO_1S,WDTO_2S,WDTO_4S,WDTO_8S
  Serial.begin(9600); //19200

  pinMode(RS485_SEND, OUTPUT);
  digitalWrite(RS485_SEND, LOW);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  
  tempSensor_Graduent_Bak.begin();
  tempSensor_Kotel.begin();
  tempSensor_Vodogrey.begin();
  tempSensor_Room.begin();

  tempSensor_Kotel.setResolution(adrKotelIn1, TEMPERATURE_PRECISION);
  tempSensor_Kotel.setResolution(adrKotelIn2, TEMPERATURE_PRECISION);
  tempSensor_Kotel.setResolution(adr3xCold, TEMPERATURE_PRECISION);
  tempSensor_Kotel.setResolution(adr3xHot, TEMPERATURE_PRECISION);
  tempSensor_Kotel.setResolution(adr3xMix, TEMPERATURE_PRECISION);
  tempSensor_Kotel.setResolution(adrPodacha, TEMPERATURE_PRECISION);
  tempSensor_Kotel.setResolution(adrObratka, TEMPERATURE_PRECISION);

  tempSensor_Graduent_Bak.setResolution(adrBakUp, TEMPERATURE_PRECISION);
  tempSensor_Graduent_Bak.setResolution(adrBakHigh, TEMPERATURE_PRECISION);
  tempSensor_Graduent_Bak.setResolution(adrBakMiddle, TEMPERATURE_PRECISION);
  tempSensor_Graduent_Bak.setResolution(adrBakLow, TEMPERATURE_PRECISION);
  tempSensor_Graduent_Bak.setResolution(adrBakDown, TEMPERATURE_PRECISION);

  tempSensor_Graduent_Bak.setWaitForConversion(true);  // makes it async
  tempSensor_Kotel.setWaitForConversion(true);  // makes it async
  tempSensor_Vodogrey.setWaitForConversion(true);  // makes it async
  tempSensor_Room.setWaitForConversion(true);  // makes it async

  requestTermo();
}

/*---------------------------------------------------------------------------------------------*/
void loop() {

//  wdt_reset();
}

/* ------------------------------------------------------ */
/* Автоматически вызываетcя при поступлении новых данных. */
void serialEvent() {
    while (Serial.available()) {
        /* надо парсить на лету, читать чужие ответы и игнорировать */
        String postData = Serial.readStringUntil(';');
    
        if (postData.equals("IDgydro")) {
          digitalWrite(LED_BUILTIN, HIGH);
          requestTermo();
          digitalWrite(LED_BUILTIN, LOW);
          readTermoData();
          sendTermoData();
        }
    }
}

/* ------------------------------------------------------ */
void requestTermo() {

  tempSensor_Graduent_Bak.requestTemperatures();
  tempSensor_Kotel.requestTemperatures();
  tempSensor_Vodogrey.requestTemperatures();
  tempSensor_Room.requestTemperatures();

  delay(300);
}

/* ------------------------------------------------------ */
void readTermoData() {
  
  tSweatRoom = tempSensor_Room.getTempC(adrSweatRoom);
  tGydroRoom = tempSensor_Room.getTempC(adrGydroRoom);
  tKotelIn1 = tempSensor_Kotel.getTempC(adrKotelIn1);
  tKotelIn2 = tempSensor_Kotel.getTempC(adrKotelIn2);
  t3xCold = tempSensor_Kotel.getTempC(adr3xCold);
  t3xHot = tempSensor_Kotel.getTempC(adr3xHot);
  t3xMix = tempSensor_Kotel.getTempC(adr3xMix);
  tPodacha = tempSensor_Kotel.getTempC(adrPodacha);
  tObratka = tempSensor_Kotel.getTempC(adrObratka);
  tVodaUp = tempSensor_Vodogrey.getTempC(adrVodaUp);
  tVodaMidle = tempSensor_Vodogrey.getTempC(adrVodaMidle);
  tVodaLow = tempSensor_Vodogrey.getTempC(adrVodaLow);
  tBakUp = tempSensor_Graduent_Bak.getTempC(adrBakUp);
  tBakHigh = tempSensor_Graduent_Bak.getTempC(adrBakHigh);
  tBakMiddle = tempSensor_Graduent_Bak.getTempC(adrBakMiddle);
  tBakLow = tempSensor_Graduent_Bak.getTempC(adrBakLow);
  tBakDown = tempSensor_Graduent_Bak.getTempC(adrBakDown);
}

/* ------------------------------------------------------ */
void sendTermoData() {

  digitalWrite(RS485_SEND, HIGH);
  delay(100);
  for (int i = 0; i < headers_am; i++) {
    Serial.print(termoNameArray[i]);
    Serial.print("=");
    Serial.print(String(*termoValue[i]));
    Serial.print(";");
    Serial.flush();
    delay(5);
  }
  delay(100);
  digitalWrite(RS485_SEND, LOW);
}
