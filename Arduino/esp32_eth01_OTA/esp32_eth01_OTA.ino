#include <Arduino.h>
#include <WiFi.h>
#include <ETH.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

AsyncWebServer server(80);

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

      // Start ElegantOTA server após obter IP
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "OTA via Ethernet esta ativa!");
      });

      ElegantOTA.begin(&server);
      server.begin();
      Serial.println("[OTA] Servidor HTTP iniciado (OTA via Ethernet)");
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
  ElegantOTA.loop();
  delay(10);
}