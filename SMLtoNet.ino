//#include "TickCounter.h"
//#include "inverter.h"

//TickCounter _tickCounter;

//extern bool _allMessagesUpdated;
//extern QpiMessage _qpiMessage;
//extern QpigsMessage _qpigsMessage;
//extern QmodMessage _qmodMessage;
//extern QpiwsMessage _qpiwsMessage;
//extern QflagMessage _qflagMessage;
//extern QidMessage _qidMessage;


String _commandBuffer = "";

void updateChargeApi(void);
void updateBatteryApi(void);
void updateLoadApi(void);

unsigned short cal_crc_half(byte* pin, byte len) {
  unsigned short crc;
  byte da;
  byte *ptr;
  byte bCRCHign;
  byte bCRCLow;

  const unsigned short crc_ta[16] = { 
      0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
      0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef
  };
  
  ptr=pin;
  crc=0;
  
  while(len--!=0) {
    da=((byte)(crc>>8))>>4; 
    crc<<=4;
    crc^=crc_ta[da^(*ptr>>4)]; 
    da=((byte)(crc>>8))>>4; 
    crc<<=4;
    crc^=crc_ta[da^(*ptr&0x0f)]; 
    ptr++;
  }
  
  bCRCLow = crc;
  bCRCHign= (byte)(crc>>8);
  
  if(bCRCLow==0x28 || bCRCLow==0x0d || bCRCLow==0x0a) {
    bCRCLow++;
  }
  if(bCRCHign==0x28 || bCRCHign==0x0d || bCRCHign==0x0a) {
    bCRCHign++;
  }
  
  crc = ((unsigned short)bCRCHign)<<8;
  crc += bCRCLow;
  return(crc);
}

//Parses out the next number in the command string, starting at index
//updates index as it goes
float getNextFloat(String& command, int& index)
{
  String term = "";
  while (index < command.length())
  {
    char c = command[index];
    ++index;

    if ((c == '.') || ((c >= '0') && (c <= '9')))
    {
      term += c;
    }
    else
    {
      return term.toFloat();
    }
  }
  return 0;
}

//Parses out the next number in the command string, starting at index
//updates index as it goes
long getNextLong(String& command, int& index)
{
  String term = "";
  while (index < command.length())
  {
    char c = command[index];
    ++index;

    if ((c == '.') || ((c >= '0') && (c <= '9')))
    {
      term += c;
    }
    else
    {
      return term.toInt();
    }
  }
  return 0;
}

//Gets if the next character is '1'
bool getNextBit(String& command, int& index)
{
  String term = "";
  if (index < command.length())
  {
    char c = command[index];
    ++index;
    return c == '1';
  }
  return false;
}

//----------------------------------------------------------------------
void setup() {
  delay(100);

  Serial.begin(115200);
  delay(10);

  Serial2.begin(2400);
}

//----------------------------------------------------------------------
void loop() {
  String _nextCommandNeeded = "QPIGS";
  unsigned short crc = cal_crc_half((byte*)_nextCommandNeeded.c_str(), _nextCommandNeeded.length());
  
  Serial2.print(_nextCommandNeeded);
  Serial2.print((char)((crc >> 8) & 0xFF));
  Serial2.print((char)((crc >> 0) & 0xFF));
  Serial2.print("\r");

  delay(10);

  while (Serial2.available() > 0) {
    byte c = Serial2.read();
    //Serial.print(c);

    if ((c != 0) && (c != '\r') && (c != '\n')) {
      if (_commandBuffer.length() < 255)
        _commandBuffer += (char)c;
    }
    else if ((c == '\r') || (c == '\n')) {
    //if (_commandBuffer.length() < 109)
    //return false;

      int index = 1; //after the starting '('
      Serial.print("Grid V:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("Grid Hz:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("AC out V:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("AC out Hz:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("AC out Va:");
      Serial.println((short)getNextLong(_commandBuffer, index));

      Serial.print("AC out W:");
      Serial.println((short)getNextLong(_commandBuffer, index));

      Serial.print("AC out %:");
      Serial.println((byte)getNextLong(_commandBuffer, index));

      Serial.print("Bus V:");
      Serial.println((short)getNextLong(_commandBuffer, index));

      Serial.print("Batt V:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("Batt charge A:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("Batt %:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("Heatsink DegC:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("Solar A:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("Solar V:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("Scc Batt V:");
      Serial.println(getNextFloat(_commandBuffer, index));

      Serial.print("Batt discharge A:");
      Serial.println(getNextFloat(_commandBuffer, index));
 
      Serial.print("Add Sbu priority version:");
      Serial.println(getNextBit(_commandBuffer, index));
      
      Serial.print("Is config changed:");
      Serial.println(getNextBit(_commandBuffer, index));

      Serial.print("Is scc firmware updated:");
      Serial.println(getNextBit(_commandBuffer, index));

      Serial.print("Is load On:");
      Serial.println(getNextBit(_commandBuffer, index));

      Serial.print("Batt voltage to steady while charging:");
      Serial.println(getNextBit(_commandBuffer, index));

      Serial.print("Charging St:");
      Serial.println((byte)getNextLong(_commandBuffer, index));

      Serial.print("Reserved Y:");
      Serial.println((byte)getNextLong(_commandBuffer, index));

      Serial.print("Reserved Z:");
      Serial.println((byte)getNextLong(_commandBuffer, index));

      Serial.print("Reserved AA:");
      Serial.println(getNextLong(_commandBuffer, index));

      Serial.print("Reserved BB:");
      Serial.println((short)getNextLong(_commandBuffer, index));

      _commandBuffer = "";
    }
 }
  delay(2000);
  
  // Comms with inverter
  //serviceInverter();

  //updateChargeApi();
  //updateBatteryApi();
  //updateLoadApi();

  /*if (_allMessagesUpdated) {
    delay(15000);
    _allMessagesUpdated = false;
  }*/
}

/*void updateChargeApi(void) {
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
}*/
