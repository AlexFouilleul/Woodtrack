#include <ArduinoBLE.h>
#include <Adafruit_GPS.h>

#define GPSSerial Serial1
#define GPSECHO false

// GPIO
const int ILS = 4;                                                              // ILS
const int GPSEN = 2;                                                            // GPS Enable

// Variables globales
int flag = 0;                                                                   // Flag de présence de données à envoyer
int warning = 0;                                                                // Flag de detection d'ouverture
int envoie = 0;                                                                 // Pointeur du tableau de données
uint32_t timetime = 0;                                                          // Variable contenant le temps en secondes
uint32_t data[9];                                                               // Tableau de données à envoyer [temps1, latitude1, longitude1, temps2, latitude2, ...]

// Objets
Adafruit_GPS GPS(&GPSSerial);                                                   // GPS
BLEService customSensingService("0000180a-0000-1000-8000-00805f9b34fb");        // BLE

// Charatéristiques associés au service
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
  // Economie d'energie
  digitalWrite(PIN_ENABLE_SENSORS_3V3, LOW);        // Eteint les capteurs
  digitalWrite(PIN_ENABLE_I2C_PULLUP, LOW);         // Desactive résistances de tirages I2C
  //digitalWrite(LED_PWR, LOW);
  
  // LED - intégré à la carte, en sortie et éteinte
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // ILS - en entrée sur le GPIO 4
  pinMode(ILS, INPUT);

  // UART - communication entre PC/Arduino et Arduino/GPS
  //Serial.begin(9600);
  GPS.begin(9600);

  // Bluetooth
  if (!BLE.begin())
  {
    Serial.println("Erreur initialisation Bluetooth");
    while(1);
  }

  // Paramètrage GPS - se référer à la doc
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PGCMD_ANTENNA);
  digitalWrite(GPSEN, HIGH);

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
  GPS.read();                               // Réception des données GPS

  // Si systeme fermée
  if(digitalRead(ILS) == 0)
  {
    // Allume le GPS
    // digitalWrite(GPSEN, HIGH);                       

    if(warning == 1)                        // Fait l'acquisition si une ouverture à été faite sans avoir  récupéré les données
    {
      if(GPS.newNMEAreceived())             // Traitement des données GPS
      {
        if(!GPS.parse(GPS.lastNMEA()))
        {
          return;
        }
      }
      
      if(GPS.fix)                           // Mise à jour
      {
        Serial.print("\nHeure : ");
        if (GPS.hour < 10) { Serial.print('0'); }
        Serial.print(GPS.hour, DEC); Serial.print(':');
        if (GPS.minute < 10) { Serial.print('0'); }
        Serial.print(GPS.minute, DEC); Serial.print(':');
        if (GPS.seconds < 10) { Serial.print('0'); }
        Serial.println(GPS.seconds, DEC);
        Serial.print("Latitude : ");
        Serial.println(GPS.latitudeDegrees, 4);
        Serial.print("Longitude : ");
        Serial.println(GPS.longitudeDegrees, 4);

        // Conversion temps en secondes
        timetime = GPS.hour*3600 + GPS.minute*60 + GPS.seconds;

        // Stockage des données
        data[envoie] = timetime;
        data[envoie+1] = GPS.latitudeDegrees*10000;
        data[envoie+2] = GPS.longitudeDegrees*10000;

        // --- DEMO A COMMENTER ---
          if(envoie == 3)
          {
            data[4] = 458992; 
            data[5] = 61313;
          }
          if(envoie == 6)
          {
            data[7] = 457764; 
            data[8] = 48535;
          }
        // --- FIN DEMO ---
        
        // Mise à jour des flags
        flag = 1;
        warning = 0;
        envoie = envoie + 3;

        // Eteint le GPS
        //digitalWrite(GPSEN, LOW);
      } 
    }
  }
  else    // Si sangle ouverte
  { 
    if((central) && (flag == 1))             // Si a deja recupéré les données et que l'on se connecte au système
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("\nConnecté à : ");
      Serial.println(central.address());

      if((central.connected()) && (flag == 1)) 
      {
        temps1Characteristic.writeValue(data[0]);
        latitude1Characteristic.writeValue(data[1]);
        longitude1Characteristic.writeValue(data[2]);
        Serial.print("--> Envoie position 1\n");
 
        if(envoie >= 6)
        {
          delay(1000);
          temps2Characteristic.writeValue(data[3]);
          latitude2Characteristic.writeValue(data[4]);
          longitude2Characteristic.writeValue(data[5]);
          Serial.print("--> Envoie position 2\n");
        }
        if(envoie >= 9)
        {
          delay(1000);
          temps3Characteristic.writeValue(data[6]);
          latitude3Characteristic.writeValue(data[7]);
          longitude3Characteristic.writeValue(data[8]);
          Serial.print("--> Envoie position 3\n");
        }

        //Remise à 0 des flags de detection et avertissement et des data
        flag = 0;
        warning = 0;
        envoie = 3;
        memset(data, 0, sizeof(data)/sizeof(data[0]));
      }
      
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Connection terminée");
    }
    else                                // Si on a deja récupéré les données mais pas de connexion au système
    {
      warning = 1;
    }
  }
}
