#include <esp_now.h>
#include <WiFi.h>

// digital pins for distance sensor it can change for your board
#define ECHO_PIN 26
#define TRIG_PIN 25  

float distance;
int count = 0;

void distance_calc();
// Replace with !!RECEIVER!! MAC address
uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0xF4, 0xD7, 0x68};

// Example sturct to store data you can change how you want
// It must match with the receiver structure
typedef struct struct_message {
  char sensor_name[32];
  int id;
  float dist;
} struct_message;

// Create a struct variable that type of struct_message 
struct_message myData;

esp_now_peer_info_t peerInfo;

// Our callback function to check data delivery
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  
  Serial.begin(9600);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

    // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Checking peer         
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }  
}

void loop() {
  distance_calc();
  send_espnow();
  
  delay(2000);
}

void send_espnow(){
    // Values that we send
    strcpy(myData.sensor_name, "Distance Sensor");
    myData.id = count;
    myData.dist = distance;
    
    // Sending data using ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
    // Data status check 
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }
  
void distance_calc() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  distance = duration * 0.034 / 2;
  count += 1;
  Serial.printf("Distance = %.2f cm \n", distance);
}
