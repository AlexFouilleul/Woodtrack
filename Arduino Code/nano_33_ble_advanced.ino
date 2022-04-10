#include <ArduinoBLE.h>
#include <Adafruit_GPS.h>

#define GPSSerial Serial1
#define GPSECHO false

const int ILS = 4;                                                              // GPIO 4 dédié à l'ILS
uint32_t timer = millis();                                                      // Timer
int flag = 0;                                                                   // Variable de récupération de données
int warning = 0;
int envoie = 3;
uint32_t timetime = 0;                                                          // Variable contenant le temps en secondes
uint32_t data[9];                                                               // Tableau de données à envoyer [temps1, latitude1, longitude1, temps2, latitude2, ...]

Adafruit_GPS GPS(&GPSSerial);                                                   // Objet GPS
BLEService customSensingService("0000180a-0000-1000-8000-00805f9b34fb");        // Service Bluetooth Arduino- A changer en fonction de l'Arduino utilisé !!!
BLEUnsignedIntCharacteristic temps1Characteristic("0000180a-0001-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEUnsignedIntCharacteristic latitude1Characteristic("0000180a-0002-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEUnsignedIntCharacteristic longitude1Characteristic("0000180a-0003-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEUnsignedIntCharacteristic temps2Characteristic("0000180a-0004-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEUnsignedIntCharacteristic latitude2Characteristic("0000180a-0005-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEUnsignedIntCharacteristic longitude2Characteristic("0000180a-0006-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEUnsignedIntCharacteristic temps3Characteristic("0000180a-0007-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEUnsignedIntCharacteristic latitude3Characteristic("0000180a-0008-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEUnsignedIntCharacteristic longitude3Characteristic("0000180a-0009-1000-8000-00805f9b34fb", BLERead | BLENotify);

void setup() 
{
  // LED - intégré à la carte, en sortie et éteinte
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //delay(3000);

  // ILS - en entrée sur le GPIO 4
  pinMode(ILS, INPUT);

  // UART - communication entre PC/Arduino et Arduino/GPS
  Serial.begin(9600);
  GPS.begin(9600);

  // Bluetooth
  if (!BLE.begin())
  {
    Serial.println("Failed.");
    while(1);
  }

  // Paramètrage GPS - se référer à la doc
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PGCMD_ANTENNA);

  // Paramètrage Bluetooth
  BLE.setLocalName("Woodtrack 1 - Beta");
  BLE.setAdvertisedService(customSensingService);
  customSensingService.addCharacteristic(temps1Characteristic);
  customSensingService.addCharacteristic(latitude1Characteristic);
  customSensingService.addCharacteristic(longitude1Characteristic);
  customSensingService.addCharacteristic(temps2Characteristic);
  customSensingService.addCharacteristic(latitude2Characteristic);
  customSensingService.addCharacteristic(longitude2Characteristic);
  customSensingService.addCharacteristic(temps3Characteristic);
  customSensingService.addCharacteristic(latitude3Characteristic);
  customSensingService.addCharacteristic(longitude3Characteristic);
  BLE.addService(customSensingService);
  BLE.setConnectable(true);
  BLE.advertise();  
}

void loop()
{
  BLEDevice central = BLE.central();

  // Lecture GPS
  GPS.read();

  // Si systeme fermée
  if(digitalRead(ILS) == 0)
  {
    if(flag == 0)
    {
      if(GPS.newNMEAreceived())             // Réception des données GPS
      {
        GPS.lastNMEA();
        GPS.parse(GPS.lastNMEA());
      }
      
      if (GPS.fix)                          // Mise à jour
      {
        Serial.print("\nTemps : ");
        if (GPS.hour < 10) { Serial.print('0'); }
        Serial.print(GPS.hour, DEC); Serial.print(':');
        if (GPS.minute < 10) { Serial.print('0'); }
        Serial.print(GPS.minute, DEC); Serial.print(':');
        if (GPS.seconds < 10) { Serial.print('0'); }
        Serial.print(GPS.seconds, DEC);
        Serial.print("\nDate : ");
        Serial.print(GPS.day, DEC); Serial.print('/');
        Serial.print(GPS.month, DEC); Serial.print("/20");
        Serial.println(GPS.year, DEC);
        Serial.print("Latitude : ");
        Serial.println(GPS.latitudeDegrees, 4);
        Serial.print("Longitude : ");
        Serial.println(GPS.longitudeDegrees, 4);
        timetime = GPS.hour*3600 + GPS.minute*60 + GPS.seconds;

        data[0] = timetime;
        data[1] = GPS.latitudeDegrees*10000;
        data[2] = GPS.longitudeDegrees*10000;
        flag = 1;
      }
    }
    if(warning == 1 && flag == 1)            // Refait l'acquisition si une ouverture à été faite sans avoir 
    {
      if(GPS.newNMEAreceived())             // Réception des données GPS
      {
        GPS.lastNMEA();
        GPS.parse(GPS.lastNMEA());
      }
      
      if (GPS.fix)                          // Mise à jour
      {
        Serial.print("\nTemps : ");
        if (GPS.hour < 10) { Serial.print('0'); }
        Serial.print(GPS.hour, DEC); Serial.print(':');
        if (GPS.minute < 10) { Serial.print('0'); }
        Serial.print(GPS.minute, DEC); Serial.print(':');
        if (GPS.seconds < 10) { Serial.print('0'); }
        Serial.print(GPS.seconds, DEC);
        Serial.print("\nDate : ");
        Serial.print(GPS.day, DEC); Serial.print('/');
        Serial.print(GPS.month, DEC); Serial.print("/20");
        Serial.println(GPS.year, DEC);
        Serial.print("Latitude : ");
        Serial.println(GPS.latitudeDegrees, 4);
        Serial.print("Longitude : ");
        Serial.println(GPS.longitudeDegrees, 4);
        timetime = GPS.hour*3600 + GPS.minute*60 + GPS.seconds;

        data[envoie] = timetime;
        data[envoie+1] = GPS.latitudeDegrees*10000;
        data[envoie+2] = GPS.longitudeDegrees*10000;

        // DEMO - A COMMENTER
        data[4] = 458992; 
        data[5] = 61313;
        data[7] = 457764; 
        data[8] = 48535;
        
        flag = 1;
        warning = 0;
        envoie = envoie + 3;
      }
    }
  }
  else    // Si sangle ouverte
  {
    if((central) && (flag == 1))             // Si a deja recupéré les données et que l'on se connecte au système
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("Incoming connection from: ");
      Serial.println(central.address());

      if((central.connected()) && (flag == 1)) 
      {
       /*
        tempsCharacteristic.writeValue(timetime);
        latitudeCharacteristic.writeValue((uint32_t) (GPS.latitudeDegrees*10000));
        longitudeCharacteristic.writeValue((uint32_t) (GPS.longitudeDegrees*10000));
        */

        temps1Characteristic.writeValue(data[0]);
        latitude1Characteristic.writeValue(data[1]);
        longitude1Characteristic.writeValue(data[2]);
        Serial.print("envoie position 1\n");
 
        // Récupère la taille du tableau
        if(envoie >= 1)
        {
          delay(1000);
          temps2Characteristic.writeValue(data[3]);
          latitude2Characteristic.writeValue(data[4]);
          longitude2Characteristic.writeValue(data[5]);
          Serial.print("envoie position 2\n");
        }
        if(envoie >= 2)
        {
          delay(1000);
          temps3Characteristic.writeValue(data[6]);
          latitude3Characteristic.writeValue(data[7]);
          longitude3Characteristic.writeValue(data[8]);
          Serial.print("envoie position 3\n");
        }

        //Remise à 0 des flags de detection et avertissement et des data
        flag = 0;
        warning = 0;
        envoie = 3;
        memset(data, 0, sizeof(data)/sizeof(data[0]));
      }
      
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Connection terminated.");
    }
    else                                // Si on a deja récupéré les données mais pas de connexion au système
    {
        if(flag == 1)
        {
          warning = 1;
        }
    }
  }
}
