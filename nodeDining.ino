#include <ShutterButton.h>
#include <Relay.h>
#include <CnC.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char nodeName[] PROGMEM = "dining";
const char sepName[] PROGMEM = " ";
const char hkName[] PROGMEM = "hk";
const char cmdGetName[] PROGMEM = "get";
const char cmdSetName[] PROGMEM = "set";

const char pingName[] PROGMEM = "ping";
const char windowShutterButtonName[] PROGMEM = "windowShutterButton";
const char windowWindowContactName[] PROGMEM = "windowWindowContact";
const char windowShutterContactName[] PROGMEM = "windowShutterContact";
const char doorShutterButtonName[] PROGMEM = "doorShutterButton";
const char doorShutterUpRelayName[] PROGMEM = "doorShutterUpRelay";
const char doorShutterDownRelayName[] PROGMEM = "doorShutterDownRelay";
const char doorWindowContactName[] PROGMEM = "doorWindowContact";
const char doorShutterContactName[] PROGMEM = "doorShutterContact";
const char tvShutterButtonName[] PROGMEM = "tvShutterButton";
const char tvShutterUpRelayName[] PROGMEM = "tvShutterUpRelay";
const char tvShutterDownRelayName[] PROGMEM = "tvShutterDownRelay";
const char tvWindowContactName[] PROGMEM = "tvWindowContact";
const char tvShutterContactName[] PROGMEM = "tvShutterContact";
const char lightRelayName[] PROGMEM = "lightRelay";
const char tempSensorsName[] PROGMEM = "tempSensors";

ShutterButton windowShutterButton(windowShutterButtonName, A2, A3);
Contact windowWindowContact(windowWindowContactName, A1);
Contact windowShutterContact(windowShutterContactName, A0);

ShutterButton doorShutterButton(doorShutterButtonName, 7, 8);
Relay doorShutterUpRelay(doorShutterUpRelayName, 6);
Relay doorShutterDownRelay(doorShutterDownRelayName, 5);
Contact doorWindowContact(doorWindowContactName, A5);
Contact doorShutterContact(doorShutterContactName, A4);

ShutterButton tvShutterButton(tvShutterButtonName, 12, 11);
Relay tvShutterUpRelay(tvShutterUpRelayName, 4);
Relay tvShutterDownRelay(tvShutterDownRelayName, 3);
Contact tvWindowContact(tvWindowContactName, 10);
Contact tvShutterContact(tvShutterContactName, 9);

Relay lightRelay(lightRelayName, 13);

OneWire oneWire(2);
DallasTemperature tempSensors(&oneWire);

static uint32_t loopNb = 0;

void ping_cmdGet(int arg_cnt, char **args) { cnc_print_cmdGet_u32(pingName, loopNb); }
void windowShutterButton_cmdGet(int arg_cnt, char **args) { windowShutterButton.cmdGet(arg_cnt, args); }
void windowWindowContact_cmdGet(int arg_cnt, char **args) { windowWindowContact.cmdGet(arg_cnt, args); }
void windowShutterContact_cmdGet(int arg_cnt, char **args) { windowShutterContact.cmdGet(arg_cnt, args); }
void doorWindowContact_cmdGet(int arg_cnt, char **args) { doorWindowContact.cmdGet(arg_cnt, args); }
void doorShutterContact_cmdGet(int arg_cnt, char **args) { doorShutterContact.cmdGet(arg_cnt, args); }
void tvShutterContact_cmdGet(int arg_cnt, char **args) { tvShutterContact.cmdGet(arg_cnt, args); }
void lightRelay_cmdGet(int arg_cnt, char **args) { lightRelay.cmdGet(arg_cnt, args); }
void lightRelay_cmdSet(int arg_cnt, char **args) { lightRelay.cmdSet(arg_cnt, args); }
uint8_t tempSensorsNb = 0;

void setup() {
  Serial.begin(115200);
  tempSensors.begin();
  cncInit(nodeName);
  cnc_hkName_set(hkName);
  cnc_cmdGetName_set(cmdGetName);
  cnc_cmdSetName_set(cmdSetName);
  cnc_sepName_set(sepName);
  cnc_cmdGet_Add(pingName, ping_cmdGet);
  cnc_cmdGet_Add(windowShutterButtonName , windowShutterButton_cmdGet);
  cnc_cmdGet_Add(windowWindowContactName , windowWindowContact_cmdGet);
  cnc_cmdGet_Add(windowShutterContactName, windowShutterContact_cmdGet);
  cnc_cmdGet_Add(doorWindowContactName   , doorWindowContact_cmdGet);
  cnc_cmdGet_Add(doorShutterContactName  , doorShutterContact_cmdGet);
  cnc_cmdGet_Add(tvShutterContactName    , tvShutterContact_cmdGet);
  cnc_cmdGet_Add(lightRelayName, lightRelay_cmdGet);
  cnc_cmdSet_Add(lightRelayName, lightRelay_cmdSet);
  doorShutterUpRelay.open();
  doorShutterDownRelay.open();
  tvShutterUpRelay.open();
  tvShutterDownRelay.open();
}

void loop() {
  delay(1);
  windowShutterButton.run(false);
  lightRelay.run(false);

  /* HK @ 1Hz */
  if(0 == loopNb%1000) {
    windowWindowContact.run(true);
    windowShutterContact.run(true);
    doorWindowContact.run(true);
    doorShutterContact.run(true);
    tvShutterContact.run(true);
    tempSensorsNb = tempSensors.getDeviceCount();
    tempSensors.requestTemperatures();
    for(uint8_t i=0; i<tempSensorsNb; i++)  {
      cnc_print_hk_index_float(tempSensorsName, i, tempSensors.getTempCByIndex(i));
    }
  }
  cncPoll();
  loopNb++;
  if(1000000000 <= loopNb) { loopNb = 0; }
}

