#include <Shutter.h>
#include <Relay.h>
#include <Cmd.h>
#include <OneWire.h>
#include <DallasTemperature.h>

Shutter diningShutter(11, 12, 6, 5);
Shutter tvShutter(A2, A3, 4, 3);
Relay light(13);
OneWire oneWire(2);
DallasTemperature tempSensors(&oneWire);

void diningShutterUp(int arg_cnt, char **args) {
  diningShutter.up();
  Serial.println("diningShutter.up OK");
}

void diningShutterDown(int arg_cnt, char **args) {
  diningShutter.down();
  Serial.println("diningShutter.down OK");
}

void tvShutterUp(int arg_cnt, char **args) {
  tvShutter.up();
  Serial.println("tvShutter.up OK");
}

void tvShutterDown(int arg_cnt, char **args) {
  tvShutter.down();
  Serial.println("tvShutter.down OK");
}

void lightOn(int arg_cnt, char **args) {
  light.close();
  Serial.println("light.on OK");
}

void lightOff(int arg_cnt, char **args) {
  light.open();
  Serial.println("light.off OK");
}

void heaterTemp(int arg_cnt, char **args) {
  Serial.print("heater.temp=");
  Serial.println(tempSensors.getTempCByIndex(0));
}

void terraceTemp(int arg_cnt, char **args) {
  Serial.print("terrace.temp=");
  Serial.println(tempSensors.getTempCByIndex(1));
}

void powerTerraceTemp(int arg_cnt, char **args) {
  Serial.print("powerTerrace.temp=");
  Serial.println(tempSensors.getTempCByIndex(2));
}

void diningTemp(int arg_cnt, char **args) {
  Serial.print("dining.temp=");
  Serial.println(tempSensors.getTempCByIndex(3));
}

void tvTemp(int arg_cnt, char **args) {
  Serial.print("tv.temp=");
  Serial.println(tempSensors.getTempCByIndex(4));
}

void setup() {
  Serial.begin(115200);
  tempSensors.begin();
  cmdInit();
  cmdAdd("diningShutter.up", "Open dining shutter", diningShutterUp);
  cmdAdd("diningShutter.down", "Close dining shutter", diningShutterDown);
  cmdAdd("tvShutter.up", "Open tv shutter", tvShutterUp);
  cmdAdd("tvShutter.down", "Close tv shutter", tvShutterDown);
  cmdAdd("light.on", "Power light ON", lightOn);
  cmdAdd("light.off", "Power light OFF", lightOff);
  cmdAdd("heater.temp", "Heater temperature", heaterTemp);
  cmdAdd("terrace.temp", "Terrace temperature", terraceTemp);
  cmdAdd("powerTerrace.temp", "Power terrace temperature", powerTerraceTemp);
  cmdAdd("dining.temp", "Dining temperature", diningTemp);
  cmdAdd("tv.temp", "TV temperature", tvTemp);
}

void loop() {
  delay(1);
  diningShutter.run();
  tvShutter.run();
  tempSensors.requestTemperatures();
  cmdPoll();
}

