 #include <Arduino.h>
 #include <WiFi.h>
 #include <WebServer.h>
 // Wifi connection:
 const char* ssid = "ballCounter";
 const char* password = "12345678";
 // === Optional configuration for static IP when using Wifi ====
 // const IPAddress local_ip(192,168,56,251);
 // const IPAddress subnet(255,255,255,0);
 // const IPAddress gateway(192,168,56,254);
 int sensorValue = 0; // Variable to store the value read from the sensor
 uint8_t sensors[] = {32, 33, 34, 35};
 bool detected[4];
 unsigned int ballCount[4]={0,0,0,0};
 const int ON_THRESHOLD = 50;
 const int OFF_THRESHOLD = ON_THRESHOLD - 20;
 WebServer server(80);
 unsigned long lastMills = 0;
 int TotalCount = 0;
 //match kind buttons:

 const unsigned long MATCH_DURATION = 160000; // This stays 160000
 unsigned long MatchStartTime = 0;
 unsigned long StartCount = 0;
 int SecToPlay = 0;
 int TimeGap = 0;
 String MatchPeriod = "none";
 String CurrentMode = "unlimeted";//Deafult mode.


void handleRoot(){
//  String html = "<!DOCTYPE html><html><head>\n";
//  html += "<meta name = 'viewport' content = 'width = device-width, initial-scale = 1'>\n";
//  html += "<style>\n";
//  html += "body {background-color:#1a1a1a ; color:#1da3a1 ; font-family:'Segoe UI',sans-serif ; display:flex ; flex-direction:column ; align-items:center ; justify-content:center ; height:100vh ; marging:0 ;}";
//  html += "h1 {font-size:5vw ; letter-spacing:0.5vw ; margin-bottom:2vh ; color:#ffffff ; }";
//  html += "</style>\n</head><body>\n"; // Style ends here
//  html += ".counter-box {background:#333 ; padding:5vw 8vw ; border-radius:3vw ; border: 0.8vw solid #1da3a1 ; box-shadow:0 0 4vw #1da3a1 ; }";
//  html += ".number {font-size: 20vw ; font-weight: bold ; font-family:'Courier New', monospace ; text-shadow:2px 2px #000 ; }";
//  html += ".btn {background:transparent; border: 0.4vw solid #ff4444; color: #ff4444; padding: 1.5vh 3 vw; \n";
//  html += "   font-size: 2vw; cursor: pointer; border-radius:1vw;transition:0.3s;text-transform:uppercase;\n";
//  html += "   font-weight:bold; outline:none; text-align:center}";
//  html += ".btn:hover {background: #ff4444; color:white; box-shadow: 0 0 2vw #ff4444;}\n";
//  html += "@media(max-width:600px){h1 {font-size:8vw;} .number{font-size:25vw;}}\n";
//  html += "</style>\n</head><body>\n";

//  html += "<h1>Thunderbolts Hub Counter</h1>\n";

//  html += "<div style='margin-bottom: 20px;'>\n";
//  html += " <button class='btn btn-mode' onclick=\"fetch('/setWon')\">Won Auto</button>\n";
//  html += " <button class='btn btn-mode' onclick=\"fetch('/setLost')\">Lost Auto</button>\n";
//  html += " <button class='btn btn-mode' onclick=\"fetch('/setUnlim')\">Unlimited</button>\n";
//  html += "</div>\n";

//  html += "<div class = 'counter-box'>";
//  html += "<div class = 'number' id='counterDisplay'>0000";
//  html += "</div>\n";
//  html += "<div class='btn' onclick='resetCounter()'>Reset Counter</div></div>\n\n";

//  html += "<script>\n";
//  html += "window.updateCounter = function updateCounter(){";
//  html += "console.log('Fetching Counter');\n";
//  html += " fetch('/getCounter')\n";
//  html += ".then(response=>response.text())\n";
//  html += ".then(data=>{";
//  html += " const display=document.getElementById('counterDisplay');";
//  html += " if(display) display.innerText = data;})\n";
//  html += ".catch(err =>console.error('Fetch error',err));";
//  html += "};\n";
//  html += "function resetCounter(){\n";
//  html += " fetch('/resetCounter').then(()=>window.updateCounter());\n}\n";
//  html += "setInterval(window.updateCounter,1000);\n";
//  html += "</script></body></html>";
 
//  server.send(200, "text/html", html);
//  Serial.println("respond");

  String html = "<!DOCTYPE html><html><head>\n";
  html += "<meta name = 'viewport' content = 'width = device-width, initial-scale = 1'>\n";
  html += "<style>\n";
  // --- CSS VARIABLES AND STYLING ---
  html += "body {background-color:#1a1a1a ; color:#1da3a1 ; font-family:'Segoe UI',sans-serif ; display:flex ; flex-direction:column ; align-items:center ; justify-content:center ; min-height:100vh ; margin:0 ;}";
  html += "h1 {font-size:5vw ; letter-spacing:0.5vw ; margin-bottom:2vh ; color:#ffffff ; }";
  html += "h3 {color:#ffffff; margin-top:20px; font-weight:lighter;}";
  html += ".counter-box {background:#333 ; padding:5vw 8vw ; border-radius:3vw ; border: 0.8vw solid #1da3a1 ; box-shadow:0 0 4vw #1da3a1 ; text-align:center; }";
  html += ".number {font-size: 20vw ; font-weight: bold ; font-family:'Courier New', monospace ; text-shadow:2px 2px #000 ; margin-bottom: 20px;}";
  
  // Button Styling
  html += ".btn {background:transparent; border: 0.4vw solid #ff4444; color: #ff4444; padding: 1.5vh 3vw; \n";
  html += "      font-size: 2vw; cursor: pointer; border-radius:1vw; transition:0.3s; text-transform:uppercase;\n";
  html += "      font-weight:bold; outline:none; text-align:center; margin: 5px;}";
  html += ".btn:hover {background: #ff4444; color:white; box-shadow: 0 0 2vw #ff4444;}\n";
  
  // Mode Button Specific (Teal color)
  html += ".btn-mode {border-color: #1da3a1; color: #1da3a1;}\n";
  html += ".btn-mode:hover {background: #1da3a1; color:white; box-shadow: 0 0 2vw #1da3a1;}\n";
  
  html += "@media(max-width:600px){h1 {font-size:8vw;} .number{font-size:25vw;} .btn{font-size:4vw;}}\n";
  html += "</style>\n</head><body>\n";

  // --- HTML CONTENT ---
  html += "<h1>Thunderbolts Hub Counter</h1>\n";

  html += "<h3>Select Match Mode:</h3>\n";
  html += "<div style='margin-bottom: 30px;'>\n";
  html += "  <button class='btn btn-mode' onclick=\"fetch('/setWon')\">Won Auto</button>\n";
  html += "  <button class='btn btn-mode' onclick=\"fetch('/setLost')\">Lost Auto</button>\n";
  html += "  <button class='btn btn-mode' onclick=\"fetch('/setUnlim')\">Unlimited</button>\n";
  html += "</div>\n";

  html += "<div class = 'counter-box'>";
  html += "  <div class = 'number' id='counterDisplay'>0000</div>\n";
  html += "  <button class='btn' onclick='resetCounter()'>Reset Counter</button>\n";
  html += "</div>\n";

  // --- JAVASCRIPT ---
  html += "<script>\n";
  html += "function updateCounter(){";
  html += "  fetch('/getCounter')\n";
  html += "  .then(response=>response.text())\n";
  html += "  .then(data=>{";
  html += "    const display=document.getElementById('counterDisplay');";
  html += "    if(display) display.innerText = data;});\n";
  html += "};\n";
  html += "function resetCounter(){\n";
  html += "  fetch('/resetCounter').then(()=>updateCounter());\n}\n";
  html += "setInterval(updateCounter,1000);\n";
  html += "</script></body></html>";

  server.send(200, "text/html", html);

}

void getCounter(){
 char buf[5];
 sprintf(buf,"%04d", TotalCount);
 server.send(200,"text/plain",buf);

}

void resetSystem(){
 for(int indx = 0; indx<4; indx++){
  detected[indx] = false;
  ballCount[indx] = 0;

 }
 TotalCount = 0;
 server.send(200,"text/plain", "OK");

}

void setup() {
// Initialize serial communication at 115200 bits per second:
 Serial.begin(115200);
 Serial.println("starting setup");
 startCountdown();
 
  // === Enable these lines to use static IP ===
 // if(!WiFi.config(local_ip, gateway, subnet)){
 //  Serial.println("faild to configet wifi");
 // }

 // === Configure the ESP as WiFi Accesss Point 192.168.4.1 ===
 WiFi.softAP(ssid, password);
 Serial.println(WiFi.softAPIP());
 
   // === Alternativly enable connection to availble Wifi network ===
 // Wifi.begin(ssid,password);
 // while (WiFi.status() != WL_CONNECTED) {
 //  delay(500);
 //  Serial.print(".");
 // }
 // Serial.println(Wifi.localIP());
 
 Serial.println("\nWiFi Connected!");

 server.on("/", handleRoot);
 server.on("/setWon", []() { CurrentMode = "wonAuto"; startCountdown(); server.send(200, "text/plain", "OK"); });
 server.on("/setLost", []() { CurrentMode = "lostAuto"; startCountdown(); server.send(200, "text/plain", "OK"); });
 server.on("/setUnlim", []() { CurrentMode = "unlimeted"; server.send(200, "text/plain", "OK"); });
 server.on("/getCounter", getCounter);
 server.on("/resetCounter", resetSystem);
 server.begin();

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

void startCountdown(){
 MatchStartTime = millis();

}

int getMatchTime() {
 unsigned long elapsed = millis() - MatchStartTime;
  if (elapsed >= MATCH_DURATION) {
    return 0; // countdown finished 

 }
 return (MATCH_DURATION - elapsed) / 1000;
 
}

void getPeriod(){
 if(getMatchTime() > 140){
  MatchPeriod = "Auto";

 } else if (getMatchTime() > 130){
  MatchPeriod = "TransitionShift";

 } else if (getMatchTime() > 30){
  MatchPeriod = "Transition";

 } else if(getMatchTime() > 0){
  MatchPeriod = "EndGame";

 }else{
  MatchPeriod = "none";
 }
}

void wonAutoButton(){
 unsigned int interim_count = 0;
 getPeriod();
 if(MatchPeriod == "Auto" || MatchPeriod == "Transition" || MatchPeriod =="EndGame"){
  if(millis() - lastMills > 25){
   // read the sensors every 25 mSec
   for(int indx=0; indx < 4;indx++){

    SensorBallCount(indx);
    interim_count += ballCount[indx];

   }

  TotalCount = interim_count; 
  lastMills = millis();
  Serial.println(TotalCount);

  }

 }else if(MatchPeriod == "TransionShift"){
  delay(25);
  if(millis() - lastMills > 25){
   // read the sensors every 25 mSec
   for(int indx=0; indx < 4;indx++){

    SensorBallCount(indx);
    interim_count += ballCount[indx];

   }

  TotalCount = interim_count; 
  lastMills = millis();
  Serial.println(TotalCount);

  }

  delay(25);
  if(millis() - lastMills > 25){
   // read the sensors every 25 mSec
   for(int indx=0; indx < 4;indx++){

    SensorBallCount(indx);
    interim_count += ballCount[indx];

   }

  TotalCount = interim_count; 
  lastMills = millis();
  Serial.println(TotalCount);

  }
 }else {
  if(millis() - lastMills > 25){
   // read the sensors every 25 mSec
   for(int indx=0; indx < 4;indx++){

    SensorBallCount(indx);
    interim_count += ballCount[indx];

   }

  TotalCount = interim_count; 
  lastMills = millis();
  // If there is a randome problome with this function/an unknown part in the code,
  // i would recomend changing the Serial.println to a 0 value(with an uknown integer variable) to cheack :). 
  Serial.println(TotalCount);

  }
 } 
} 

void lostAutoButton(){
 unsigned int interim_count = 0;
 getPeriod();
 if(MatchPeriod == "Auto" || MatchPeriod == "Transition" || MatchPeriod == "EndGame"){
  if(millis() - lastMills > 25){
   // read the sensors every 25 mSec
   for(int indx=0; indx < 4;indx++){

    SensorBallCount(indx);
    interim_count += ballCount[indx];

   }

  TotalCount = interim_count; 
  lastMills = millis();
  Serial.println(TotalCount);

  }

 }else if(MatchPeriod == "TransionShift"){
  if(millis() - lastMills > 25){
   // read the sensors every 25 mSec
   for(int indx=0; indx < 4;indx++){

    SensorBallCount(indx);
    interim_count += ballCount[indx];

   }

  TotalCount = interim_count; 
  lastMills = millis();
  Serial.println(TotalCount);

  }

  delay(25);
  if(millis() - lastMills > 25){
   // read the sensors every 25 mSec
   for(int indx=0; indx < 4;indx++){

    SensorBallCount(indx);
    interim_count += ballCount[indx];

   }

  TotalCount = interim_count; 
  lastMills = millis();
  Serial.println(TotalCount);

  }

  delay(25);

 }else {
  if(millis() - lastMills > 25){
   // read the sensors every 25 mSec
   for(int indx=0; indx < 4;indx++){

    SensorBallCount(indx);
    interim_count += ballCount[indx];

   }

  TotalCount = interim_count; 
  lastMills = millis();
  // If there is a randome problome with this function/an unknown part in the code,
  // i would recomend changing the Serial.println to a 0 value(with an uknown integer variable) to cheack :). 
  Serial.println(TotalCount);

  }
 } 
} 

void unlimetedCountButton(){
 unsigned int interim_count = 0;
 if(millis() - lastMills > 25){
  // read the sensors every 25 mSec
  for(int indx=0; indx < 4;indx++){
   SensorBallCount(indx);
   interim_count += ballCount[indx];
  }

  TotalCount = interim_count; 
  lastMills = millis();
  Serial.println(TotalCount);

 }
}

void loop() {
 server.handleClient();
 if(CurrentMode == "wonAuto"){
  wonAutoButton();

 }else if(CurrentMode == "lostAuto"){
  lostAutoButton();

 }else if(CurrentMode == "unlimeted"){
  unlimetedCountButton();

 }
}