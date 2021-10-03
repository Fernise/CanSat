#include <Adafruit_GPS.h>
#include <SPI.h>
#include <LoRa.h>
#define GPSserial Serial2

Adafruit_GPS GPS(&GPSserial);

int tiempo = 0;
float latitudgps;
float longitudgps;
float latitudet;
float longitudet;
String cadena;
char lastbyte;

int packetSize;



void setup()
{
  Serial.begin(9600);
  GPS.begin(9600);

  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_GGAONLY);
  GPS.sendCommand(PGCMD_NOANTENNA);
}

void loop() {
  packetSize = LoRa.parsePacket();

  if (packetSize) {
    String cadena;
    while (LoRa.available()) {
      lastbyte = LoRa.read();
      cadena += lastbyte;

    }
    Serial.println(cadena);
  }


  if (millis() - tiempo > 1000) {
    tiempo = millis();
    GPS.read();
    if (GPS.newNMEAreceived()){
      Serial.println(GPS.lastNMEA());
      GPS.parse(GPS.lastNMEA());
    }
    Serial.print("\nHora: ");
    if (GPS.hour < 10) {        //todos es tos if(x<10) son para que quede 02:03:07.001 en vez de 2:3:7.1
      Serial.print('0');
    }
    Serial.print(GPS.hour, DEC);
    Serial.print(':');
    if (GPS.minute < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.minute, DEC);
    Serial.print(':');
    if (GPS.seconds < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.seconds, DEC);
    Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);

    Serial.print("Fecha: ");
    Serial.print(GPS.day, DEC);
    Serial.print('/');
    Serial.print(GPS.month, DEC);
    Serial.print("/20");
    Serial.println(GPS.year, DEC);

    Serial.print("Fix: ");
    Serial.print((int)GPS.fix);
    Serial.print(" calidad: ");
    Serial.println((int)GPS.fixquality);

    if (GPS.fix) {
      Serial.print("latitud y longitud: ");
      Serial.print(GPS.latitude, 4);
      Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4);
      Serial.println(GPS.lon);

      Serial.print("Altitud: ");
      Serial.println(GPS.altitude / 100);
      Serial.print(" m");
    }

    Serial.print(cadena);
    String lati = String(cadena.charAt(1)) + String(cadena.charAt(2)) + String(cadena.charAt(3)) + String(cadena.charAt(4)) + String(cadena.charAt(5)) + String(cadena.charAt(6)) + String(cadena.charAt(7)) + String(cadena.charAt(8)) + String(cadena.charAt(9)) + String(cadena.charAt(10)) + String(cadena.charAt(11));
    float latitudgpsnmea = lati.toFloat();
    String longi = String(cadena.charAt(13)) + String(cadena.charAt(14)) + String(cadena.charAt(15)) + String(cadena.charAt(16)) + String(cadena.charAt(17)) + String(cadena.charAt(18)) + String(cadena.charAt(19)) + String(cadena.charAt(20)) + String(cadena.charAt(21)) + String(cadena.charAt(22)) + String(cadena.charAt(23));
    float longitudgpsnmea = longi.toFloat();

    latitudgps = getmetros(latitudgpsnmea);
    longitudgps = getmetros(longitudgpsnmea);

    Serial.print("\nLatitud: ");
    Serial.println(latitudgps, 6);
    Serial.print("Longitud: ");
    Serial.println(longitudgps, 6);
  }
}

float getmetros(float nmea) {                  //Pasa la latitud del formato NMEA a metros

  String raw = String(nmea, 6); //Lo pasamos a String

  String gradosraw = String(raw.charAt(0)) + String(raw.charAt(1));  //Extraemos los grados del String
  int grados = gradosraw.toInt();

  String minutosraw = String(raw.charAt(2)) + String(raw.charAt(3));  //Extraemos los minutos del String
  int minutos = minutosraw.toInt();

  String mindec = String("0.") + String(raw.charAt(5)) + String(raw.charAt(6)) + String(raw.charAt(7)) + String(raw.charAt(8)) + String(raw.charAt(9)) + String(raw.charAt(10)) + String(raw.charAt(11));
  int segundos = mindec.toFloat() * 60;               //Extraemos los minutos decimales y los pasamos a segundos

  double metros = (grados * 3600 + minutos * 60 + segundos) * 30.5; //Pasamos los datos a metros

  Serial.print(metros, 6);

  return metros;

}
