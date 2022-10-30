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

eSource sourcePriority = NONE;

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

char incomingByte = 0; // for incoming serial data

//----------------------------------------------------------------------
void loop() {
  delay(50);
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    
    // Set output source priority to mains when battery voltage goes bellow 46.5V
    if(incomingByte == 'M'){
      if(sourcePriority != MAINS_SOLAR){
        Serial.println("MAINS_SOLAR");
        if(setOutputPrioritySource(MAINS_SOLAR)) {
          sourcePriority = MAINS_SOLAR;
        }
      }
    } else if(incomingByte == 'S') {
      if(sourcePriority != SOLAR_ONLY){
        Serial.println("SOLAR_ONLY");
        if(setOutputPrioritySource(SOLAR_ONLY)) {
          sourcePriority = SOLAR_ONLY;
        }
      }
    }
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
