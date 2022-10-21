#include "TickCounter.h"
#include "inverter.h"

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
  delay(100);

  Serial.begin(115200);
  delay(10);

  Serial2.begin(2400);
}

//----------------------------------------------------------------------
void loop() {
  delay(2000);
  
  // Comms with inverter
  serviceInverter();

  updateChargeApi();
  updateBatteryApi();
  updateLoadApi();

  /*if (_allMessagesUpdated) {
    delay(15000);
    _allMessagesUpdated = false;
  }*/
}

void updateChargeApi(void) {
  String params;

  params += String(_qpigsMessage.battV);
  params += "\t";
  params += String(_qpigsMessage.battChargeA);
  params += "\t";
  params += String(_qpigsMessage.solarV);
  params += "\t";
  params += String(_qpigsMessage.solarA);
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
