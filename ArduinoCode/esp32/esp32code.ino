#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
// #define CAMERA_MODEL_M5STACK_NO_PSRAM
// #define CAMERA_MODEL_AI_THINKER
//  #define CAMERA_MODEL_ESP32_CAM_BOARD
#define CAMERA_MODEL_M5STACK_WIDE
// #include <HardwareSerial.h>
//#include <IRremote.h>
#include "camera_pins.h"

// #define RXD2 2
// #define TXD2 1
// #define PWDN_GPIO_NUM -1 // Power down pin
// #define RESET_GPIO_NUM -1 // Reset pin
// #define XCLK_GPIO_NUM 21 // XCLK pin
// #define SIOD_GPIO_NUM 26 // SDA pin
// #define SIOC_GPIO_NUM 27 // SCL pin

// #define Y9_GPIO_NUM 35 // Y9 pin
// #define Y8_GPIO_NUM 34 // Y8 pin
// #define Y7_GPIO_NUM 39 // Y7 pin
// #define Y6_GPIO_NUM 36 // Y6 pin
// #define Y5_GPIO_NUM 19 // Y5 pin
// #define Y4_GPIO_NUM 18 // Y4 pin
// #define Y3_GPIO_NUM 5 // Y3 pin
// #define Y2_GPIO_NUM 4 // Y2 pin
// #define VSYNC_GPIO_NUM 25 // VSYNC pin
// #define HREF_GPIO_NUM 23 // HREF pin
// #define PCLK_GPIO_NUM 22 // PCLK pin


#define RXD2 33
#define TXD2 4

//#define RXD2 13
//#define TXD2 14

// HardwareSerial Serial2 (2);
const char* ssid = "";
const char* password = "";
const char* flaskServerIP = "192...";
const int flaskServerPort = 5000;
const char* endpoint = "/upload";

WebServer server(80);

void initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 10000000;
    config.pixel_format = PIXFORMAT_JPEG;

    // Use PSRAM found on WROVER modules
    if(psramFound()){
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif
    // Initialize the camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
}



void setup() {
  Serial.begin(115200); // Start the hardware serial port for communication with the Uno
  //Serial2.begin(9600,SERIAL_8N1,33,4);
  Serial2.begin(9600,SERIAL_8N1,RXD2,TXD2);
  // Serial.begin(9600);
  initCamera();
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    //Serial.print("Setup complete");
    //Serial2.println("Hello from ESP32");
   //Serial.println(WiFi.localIP());
    }
  server.on("/forward", []() {
    // Serial2.println('{"N":102,"D1":1,"D2":200}');
    Serial2.println("FORWARD"); // Send command to Arduino Uno

    server.send(200, "text/plain", "Moving Forward");
  });

  server.on("/halfforward", []() {
    // Serial2.println('{"N":102,"D1":1,"D2":200}');
    Serial2.println("HALFFORWARD"); // Send command to Arduino Uno

    server.send(200, "text/plain", "Moving Forward");
  });


  server.on("/left", []() {
    Serial2.println("LEFT"); // Send command to Arduino Uno

    server.send(200, "text/plain", "Moving Left");
  });

    server.on("/halfleft", []() {
    Serial2.println("HALFLEFT"); // Send command to Arduino Uno

    server.send(200, "text/plain", "Moving Left");
  });

server.on("/right", []() {
    Serial2.println("RIGHT"); // Send command to Arduino Uno
    server.send(200, "text/plain", "Moving Right");
  });

  server.on("/halfright", []() {
    Serial2.println("HALFRIGHT"); // Send command to Arduino Uno
    server.send(200, "text/plain", "Moving Right");
  });

    server.on("/backward", []() {
    Serial2.println("BACKWARD"); // Send command to Arduino Uno
    server.send(200, "text/plain", "Moving Backward");
  });

      server.on("/halfbackward", []() {
    Serial2.println("HALFBACKWARD"); // Send command to Arduino Uno
    server.send(200, "text/plain", "Moving Backward");
  });

  server.on("/stop", []() {
    Serial2.println("STOP"); // Send command to Arduino Uno
    server.send(200, "text/plain", "Stopping");
  });

  server.begin();
}



void captureAndSendPhoto() {
  camera_fb_t * fb = NULL;

  // Take a picture
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  Serial.println("Photo captured, size: " + String(fb->len));

  WiFiClient client;
  if (!client.connect(flaskServerIP, flaskServerPort)) {
    Serial.println("Connection to server failed");
    esp_camera_fb_return(fb);
    return;
  }
  Serial.println("Connected to server");
  String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
  String head = "--" + boundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"image\"; filename=\"esp32-cam.jpg\"\r\n";
  head += "Content-Type: image/jpeg\r\n\r\n";
  String tail = "\r\n--" + boundary + "--\r\n";

  uint32_t imageLen = fb->len;
  uint32_t extraLen = head.length() + tail.length();
  uint32_t totalLen = imageLen + extraLen;

  client.println("POST " + String(endpoint) + " HTTP/1.1");
  client.println("Host: " + String(flaskServerIP));
  client.println("Content-Length: " + String(totalLen));
  client.println("Content-Type: multipart/form-data; boundary=" + boundary);
  client.println();
  client.print(head);

  // Stream image bytes to the server
  uint8_t *fbBuf = fb->buf;
  size_t fbLen = fb->len;
  for (size_t n=0; n<fbLen; n=n+1024) {
    if (n+1024 < fbLen) {
      client.write(fbBuf, 1024);
      fbBuf += 1024;
    } else if (fbLen%1024>0) {
      size_t remainder = fbLen%1024;
      client.write(fbBuf, remainder);
    }
  }

  client.print(tail);
  esp_camera_fb_return(fb);

  int timeout = 10000; // 10 second timeout
  long start = millis();
  while (client.connected() && millis() - start < timeout) {
    String line = client.readStringUntil('\n');
    if (line.startsWith("HTTP/1.1")) {
      if (line.indexOf("200") > 0) {
        Serial.println("Image successfully sent");
      } else {
        Serial.println("Sending image failed");
        Serial.println(line);
      }
    }
    if (line == "\r") {
      break;
    }
  }
}
 //Serial.println(" CONNECTED");



void loop() {

    //   if (Serial2.available() > 0) {
    //      Serial.println("in Serial2");

    //   Serial.print((char) Serial2.read());
    // }
  server.handleClient();
 captureAndSendPhoto();
 //delay(1000);
  // Serial2.print("Hello ");
  //  Serial2.println(" message from esp32 to arduino");
}