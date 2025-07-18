#include <Arduino.h>
#include <WiFi.h>
#include <ETH.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include <ElegantOTA.h>

WebServer server(80);

unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("[ETH] Ethernet inicializado");
      ETH.setHostname("esp32-eth01");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("[ETH] Cabo conectado");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("[ETH] IP: ");
      Serial.println(ETH.localIP());

      server.on("/", []() {
        server.send(200, "text/plain", "Servidor OTA funcionando via Ethernet.");
      });

      ElegantOTA.begin(&server);    // Start ElegantOTA
      // ElegantOTA callbacks
      ElegantOTA.onStart(onOTAStart);
      ElegantOTA.onProgress(onOTAProgress);
      ElegantOTA.onEnd(onOTAEnd);

      server.begin();
      Serial.println("HTTP server started");
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("[ETH] Ethernet desconectado");
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("[ETH] Ethernet parado");
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.onEvent(WiFiEvent);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  // Para Arduino-ESP32 v3.0.0 ou superior
  if (!ETH.begin(ETH_PHY_LAN8720, 1, 23, 18, 16, ETH_CLOCK_GPIO0_IN)) {
    Serial.println("[ETH] Falha ao iniciar Ethernet");
  }
#else
  // Para versões anteriores
  if (!ETH.begin(1, 16, 23, 18, ETH_PHY_LAN8720, ETH_CLOCK_GPIO0_IN)) {
    Serial.println("[ETH] Falha ao iniciar Ethernet");
  }
#endif
}

void loop() {
  server.handleClient();
  ElegantOTA.loop();
  delay(10);
}