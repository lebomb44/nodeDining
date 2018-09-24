#include <Shutter.h>
#include <Relay.h>
#include <CnC.h>
#include <OneWire.h>
#include <DallasTemperature.h>

Shutter diningShutterButton(12, 11);
Relay up("u", 6);
Relay down("d", 5);
Shutter tvShutterButton(A3, A2);
Relay up("u", 4);
Relay down("d", 3);

Relay light(13);

OneWire oneWire(2);
DallasTemperature tempSensors(&oneWire);

void setup() {
  Serial.begin(115200);
  tempSensors.begin();
  cncInit();
  cncAdd("dining_diningShutterButton_get", diningShutterButton_cmdGet);
  //cncAdd("dining_tvShutter_get", diningShutterDown);
  cncAdd("dining_light_cmdGet", light_cmdGet);
  cncAdd("dining_light_cmdSet", light_cmdSet);
  cncAdd("light.off", lightOff);
  cncAdd("heater.temp", heaterTemp);
  cncAdd("terrace.temp", terraceTemp);
  cncAdd("powerTerrace.temp", powerTerraceTemp);
  cncAdd("dining.temp", diningTemp);
  cncAdd("tv.temp", "tvTemp);
}

void loop() {
  static uint32_t loopNb = 0;
  delay(1);
  diningShutterButton.run();
  //tvShutterButton.run();
  tempSensors.requestTemperatures();
  cmdPoll();
}

