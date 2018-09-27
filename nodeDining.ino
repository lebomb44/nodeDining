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

const char windowShutterButtonName[] PROGMEM = "windowShutterButton";
const char windowShutterUpRelayName[] PROGMEM = "windowShutterUpRelay";
const char windowShutterDownRelayName[] PROGMEM = "windowShutterDownRelay";
const char windowWindowContactName[] PROGMEM = "windowWindowContact";
const char windowShutterContactName[] PROGMEM = "windowShutterContact";
const char tvShutterButtonName[] PROGMEM = "tvShutterButton";
const char tvShutterUpRelayName[] PROGMEM = "tvShutterUpRelay";
const char tvShutterDownRelayName[] PROGMEM = "tvShutterDownRelay";
const char tvWindowContactName[] PROGMEM = "tvWindowContact";
const char tvShutterContactName[] PROGMEM = "tvShutterContact";
const char lightRelayName[] PROGMEM = "lightRelay";
const char tempSensorsName[] PROGMEM = "tempSensors";

ShutterButton windowShutterButton(windowShutterButtonName, 12, 11);
Relay windowShutterUpRelay(windowShutterUpRelayName, 6);
Relay windowShutterDownRelay(windowShutterDownRelayName, 5);
Contact windowWindowContact(windowWindowContactName, 10);
Contact windowShutterContact(windowShutterContactName, 9);

ShutterButton tvShutterButton(tvShutterButtonName, A3, A2);
Relay tvShutterUpRelay(tvShutterUpRelayName, 4);
Relay tvShutterDownRelay(tvShutterDownRelayName, 3);
Contact tvWindowContact(tvWindowContactName, A1);
Contact tvShutterContact(tvShutterContactName, A0);

Relay lightRelay(lightRelayName, 13);

OneWire oneWire(2);
DallasTemperature tempSensors(&oneWire);
uint8_t tempSensorsNb = 0;

void windowShutterButton_cmdGet(int arg_cnt, char **args) { windowShutterButton.cmdGet(arg_cnt, args); }
void windowWindowContact_cmdGet(int arg_cnt, char **args) { windowWindowContact.cmdGet(arg_cnt, args); }
void windowShutterContact_cmdGet(int arg_cnt, char **args) { windowShutterContact.cmdGet(arg_cnt, args); }
void lightRelay_cmdGet(int arg_cnt, char **args) { lightRelay.cmdGet(arg_cnt, args); }
void lightRelay_cmdSet(int arg_cnt, char **args) { lightRelay.cmdSet(arg_cnt, args); }

void setup() {
  Serial.begin(115200);
  tempSensors.begin();
  cncInit(nodeName);
  cnc_hkName_set(hkName);
  cnc_cmdGetName_set(cmdGetName);
  cnc_cmdSetName_set(cmdSetName);
  cnc_sepName_set(sepName);
  cnc_cmdGet_Add(windowShutterButtonName, windowShutterButton_cmdGet);
  cnc_cmdGet_Add(windowWindowContactName , windowWindowContact_cmdGet);
  cnc_cmdGet_Add(windowShutterContactName, windowShutterContact_cmdGet);
  cnc_cmdGet_Add(lightRelayName, lightRelay_cmdGet);
  cnc_cmdSet_Add(lightRelayName, lightRelay_cmdSet);
  windowShutterUpRelay.open();
  windowShutterDownRelay.open();
  tvShutterUpRelay.open();
  tvShutterDownRelay.open();
}

void loop() {
  static uint32_t loopNb = 0;
  delay(1);
  windowShutterButton.run(false);
  lightRelay.run(false);

  /* HK @ 1Hz */
  if(0 == loopNb%1000) {
    tempSensorsNb = tempSensors.getDeviceCount();
    tempSensors.requestTemperatures();
    for(uint8_t i=0; i<tempSensorsNb; i++)  {
      cnc_print_hk_index(tempSensorsName, i, tempSensors.getTempCByIndex(i));
    }
  }
  cncPoll();
  loopNb++;
}

