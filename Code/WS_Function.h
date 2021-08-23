void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Page Not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type)
  {
    case WStype_DISCONNECTED: Serial.printf("[%u] Disconnected!\n", num); break;
    case WStype_CONNECTED: {
        IPAddress ip = websocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        String message = "{\"LED\":\"OFF\",\"Mode\":";
        if (isDisplay) message = "{\"LED\":\"ON\",\"Mode\":";
        message = message + Mode + ",\"sliderValue\":" + BRIGHTNESS + '}';
        websocket.sendTXT(num, message);
      }
      break;
    case WStype_TEXT: {
        Serial.printf("[%u] get Text: %s\n", num, payload);
        String message = String((char*)( payload));
        DynamicJsonDocument doc(48);
        DeserializationError error = deserializeJson(doc, message);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        if (doc.containsKey("LED")) {
          if (doc["LED"] == "ON")isDisplay = true;
          else if (doc["LED"] == "OFF")isDisplay = false;
        }
        else if (doc.containsKey("sliderValue"))BRIGHTNESS = doc["sliderValue"];
        else if (doc.containsKey("Mode"))Mode = doc["Mode"];
        websocket.broadcastTXT(message);
      }
      break;
  }
}

void WiFi_Init() {
  WiFi.softAP(ssid, password);
  Serial.print("Access Point:");
  Serial.println(WiFi.softAPIP());

  server.on("/", [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();
  websocket.begin();
  websocket.onEvent(webSocketEvent);
}
