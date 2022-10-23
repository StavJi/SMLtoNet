#include <WiFi.h>
#include "TickCounter.h"
#include "inverter.h"
#include "settings.h"

TickCounter _tickCounter;

extern bool _allMessagesUpdated;
extern QpiMessage _qpiMessage;
extern QpigsMessage _qpigsMessage;
extern QmodMessage _qmodMessage;
extern QpiwsMessage _qpiwsMessage;
extern QflagMessage _qflagMessage;
extern QidMessage _qidMessage;

void updateChargeApi(void);
void updateBatteryApi(void);
void updateLoadApi(void);

//----------------------------------------------------------------------
void setup() {
  unsigned int numberOfConnections= 0;
  
  delay(100);

  Serial.begin(115200);
  delay(10);

  Serial2.begin(2400);
 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    numberOfConnections++;

    Serial.println("Number Of Connections:");
    Serial.println(numberOfConnections);

    // if ESP12E can't connect to WiFi -> enable deep.sleep
    if (numberOfConnections > 20) {
        delay(1000);
        return;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//----------------------------------------------------------------------
void loop() {
  delay(50);
  
  // Comms with inverter
  serviceInverter();

  if (_allMessagesUpdated) {
    delay(1000);
    updateChargeApi();
    updateBatteryApi();
    updateLoadApi();

    //*****************************************
    
    long rssi = WiFi.RSSI();
    
    Serial.print("RSSI:");
    Serial.println(rssi);
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) 
    {
      Serial.println("connection failed");
      return;
    }
    
    // This will send the request to the server
    client.print(String("GET /?") +
                 "battV=" + String(_qpigsMessage.battV) +
                 "&battChargeA=" + String(_qpigsMessage.battChargeA) +
                 "&solarA=" + String(_qpigsMessage.solarA) +
                 "&voltage=" + String(_qpigsMessage.solarV) + 
                 "&rssi=" + String(rssi) + 
                 " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    
    delay(10);
    
    // Read all the lines of the reply from server and print them to Serial
    Serial.println("Respond:");
    while(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    Serial.println();
    Serial.println("closing connection");

    //*****************************************
    
    delay(14000);
    _allMessagesUpdated = false;
  }
}

void updateChargeApi(void) {
  String params;

  params += String(_qpigsMessage.battV);//battV
  params += "\t";
  params += String(_qpigsMessage.battChargeA);//battChargeA
  params += "\t";
  params += String(_qpigsMessage.solarV);//voltage
  params += "\t";
  params += String(_qpigsMessage.solarA);//solarA
  params += "\t";
  params += String(_qpigsMessage.chargingStatus);
 
  Serial.println(params);
}

void updateBatteryApi(void) {
  String params;

  params += String(_qpigsMessage.battV);
  params += "\t";
  params += String(_qpigsMessage.battChargeA);
  params += "\t";
  params += String(_qpigsMessage.battDischargeA);
  params += "\t";
  params += String(_qpigsMessage.battDischargeA);

  Serial.println(params);
}

void updateLoadApi(void) {
  String params;

  params += String(_qpigsMessage.acOutV);
  params += "\t";
  params += String(_qpigsMessage.acOutHz);
  params += "\t";
  params += String(_qpigsMessage.acOutW);
  params += "\t";
  params += String(_qpigsMessage.acOutVa);
  params += "\t";
  params += String(_qpigsMessage.heatSinkDegC);
 
  Serial.println(params);
}
