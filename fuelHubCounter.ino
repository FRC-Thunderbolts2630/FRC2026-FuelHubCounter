  #include <Arduino.h>
  #include <WiFi.h>
  #include <WebServer.h>
  // Wifi connection:
  const char* ssid = "ballCounter";
  const char* pasword = "12345678";
  // const IPAddress local_ip(192,168,56,251);
  // const IPAddress subnet(255,255,255,0);
  // const IPAddress gateway(192,168,56,254);
  int sensorValue = 0; // Variable to store the value read from the sensor
  uint8_t sensors[] = {32, 33, 34, 35};
  bool detected[4];
  int ballCount[4];
  const int ON_THRESHOLD = 50;
  const int OFF_THRESHOLD = ON_THRESHOLD - 20;
  WebServer server(80);
  int lastMills = 0;
  

// bool detected = false;(Was a part of the code).

void handleRoot(){
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name = 'viewport' content = 'width = device-width, initial-scale = 1'>";
  html += "<style>";
  html += "body {background-color:#1a1a1a ; color:#00d4ff ; font-family:'Segoe UI',sans-serif ; display:flex ; flex-direction:column ; align-items:center ; justify-content:center ; height:100vh ; marging:0 ;}";
  html += "h1 {font-size:2rem ; letter-spacing:2px ; margin-bottom:20px ; color:#ffffff ; }";
  html += ".counter-box {background:#333 ; padding:40px 60px ; border-radius:20px ; border: 4px solid #00d4ff ; box-shadow:0 0 20px #00d4ff ; }";
  html += ".number {font-size: 120px ; font-weight: bold ; font-family:'Courier New', monospace ; text-shadow:2px 2px #000 ; }";
  html += "</style></head><body>";
  html += "<h1>Thunderbolts Hub Counter</h1>";
  html += "<div class = 'counter-box'>";
  html += "<div class = 'number'>0000";
  html += "</div></div></body></html>";
  server.send(200, "text/html", html);
  Serial.println("respond");
}

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  Serial.println("starting setup");

  
  // Connected to wifi:
  // if(!WiFi.config(local_ip, gateway, subnet)){
  //   Serial.println("faild to configet wifi");
  // }
  WiFi.softAP(ssid, pasword);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();

}

void resetSystem(){
  for(int indx = 0; indx<4; indx++){
    detected[indx] = false;
    ballCount[indx] = 0;
  }

}

void SensorBallCount(int indx){
  int sensorValue = analogRead(sensors[indx]);
  if(sensorValue > ON_THRESHOLD && !detected[indx]){
    ballCount[indx]++;
    detected[indx] = true;
  }
  else if(sensorValue < OFF_THRESHOLD && detected[indx]){
    detected[indx] = false;
  }
}

void loop() {
  // Add a small delay to avoid flooding the serial monitor:
  // delay(25);
  // int TotalCount = 0;
  // for(int indx=0; indx < 4;indx++){
  //   SensorBallCount(indx);
  //   TotalCount += ballCount[indx];
  // }  
  // Print the value to the Serial Monitor:
  // Serial.println(TotalCount);
  server.handleClient();
  if(millis() - lastMills > 5000){
    Serial.println("server is running");
    lastMills = millis();
  }

}
