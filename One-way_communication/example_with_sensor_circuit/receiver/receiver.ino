#include <esp_now.h>
#include <WiFi.h>

// Example sturct to store data you can change how you want
// It must match with the sender structure
typedef struct struct_message {
  char sensor_name[32];
  int id;
  float dist;
} struct_message;

// Create a struct variable that type of struct_message
struct_message myData;

// Our callback function will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.sensor_name);
  Serial.print("Int: ");
  Serial.println(myData.id);
  Serial.print("Float: ");
  Serial.println(myData.dist);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop() {

}
