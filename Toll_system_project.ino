#include <SPI.h>              
#include <MFRC522.h>            
#include <Wire.h>               
#include <Adafruit_GFX.h>       
#include <Adafruit_SSD1306.h>  
#include <Servo.h>             
#include <WiFi.h>               
#include "secrets.h"            // Include the secrets file for sensitive data

// Pin Definitions
#define RST_PIN 9            
#define SS_PIN  10             
#define BlueLED  2          
#define GreenLED 3            
#define RedLED   4             
#define Buzzer   5             
#define SCREEN_WIDTH 128       // Width of OLED screen
#define SCREEN_HEIGHT 64       // Height of OLED screen

// Web server details
const char* server = "www.ysjcs.net"; // Server domain
const int port = 80;                 // HTTP port

String MasterTag = "23 17 1F 2D";    // Master RFID tag for access
String UIDCard = "";                 // Variable to store the detected RFID UID

// Initialize Objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // OLED display object
MFRC522 mfrc522(SS_PIN, RST_PIN);                                 // RFID module object
Servo servo;                                                      // Servo object for controlling the lock

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor

  // Initialize RFID
  SPI.begin();         // Start SPI communication
  mfrc522.PCD_Init();  // Initialize RFID module

  // Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (1); // Stop here if OLED initialization fails
  }
  display.clearDisplay();             // Clear OLED display
  display.setTextSize(1);             // Set text size
  display.setTextColor(SSD1306_WHITE);// Set text color
  display.setCursor(0, 0);
  display.println("Access Control");
  display.println("Scan Your Card>>");
  display.display();

  // Initialize Servo
  servo.attach(6); // Attach servo to pin 6
  servo.write(10); // Set servo to closed position

  // Initialize LEDs and Buzzer
  pinMode(GreenLED, OUTPUT); // Green LED for access granted
  pinMode(BlueLED, OUTPUT);  // Blue LED for default state
  pinMode(RedLED, OUTPUT);   // Red LED for access denied
  pinMode(Buzzer, OUTPUT);   // Buzzer for alerts

  // Default LED state
  digitalWrite(BlueLED, HIGH);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password); // Use credentials from secrets.h
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("."); // Print dots until connected
  }
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the assigned IP address
}

void loop() {
  // Default LED states
  digitalWrite(BlueLED, HIGH); // Default state
  digitalWrite(RedLED, LOW);
  digitalWrite(GreenLED, LOW);
  noTone(Buzzer);             // Turn off buzzer
  servo.write(10);            // Keep servo closed

  // Wait for an RFID card to be detected
  if (getUID()) {
    Serial.print("UID: ");
    Serial.println(UIDCard); // Print the detected UID

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Permission: ");

    String accessStatus = "Denied"; // Default access status
    if (UIDCard == MasterTag) {
      // Access Granted
      display.println("Granted");
      digitalWrite(GreenLED, HIGH);
      digitalWrite(BlueLED, LOW);
      digitalWrite(RedLED, LOW);
      servo.write(90); // Open servo for access
      delay(50);

      // Blink Buzzer for Granted Access
      for (int i = 0; i < 2; i++) {
        tone(Buzzer, 2000); // Generate sound at 2000 Hz
        delay(250);
        noTone(Buzzer);
        delay(250);
      }
      accessStatus = "Granted"; // Update access status
    } else {
      // Access Denied
      display.println("Denied");
      digitalWrite(BlueLED, LOW);
      digitalWrite(GreenLED, LOW);
      tone(Buzzer, 2000); // Generate sound for denied access
      servo.write(10);    // Keep servo closed
      for (int i = 0; i < 10; i++) {
        digitalWrite(RedLED, HIGH);
        delay(250);
        digitalWrite(RedLED, LOW);
        delay(250);
      }
      noTone(Buzzer);
    }

    // Send the data to the server
    sendDataToServer(UIDCard, accessStatus);

    display.display();
    delay(2000); // Delay before resetting display

    // Reset the display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Access Control");
    display.println("Scan Your Card>>");
    display.display();
  }
}

// Function to detect RFID card and retrieve its UID
boolean getUID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false; // No card detected
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return false; // Failed to read card
  }

  UIDCard = ""; // Reset UID string
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    UIDCard.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    UIDCard.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  UIDCard.toUpperCase();             // Convert to uppercase
  UIDCard = UIDCard.substring(1);    // Remove leading space

  mfrc522.PICC_HaltA();              // Halt communication with card
  return true; // UID successfully retrieved
}

// Function to send RFID data and access status to a web server
void sendDataToServer(String rfid_id, String access_status) {
  WiFiClient client; // Create a client object

  if (client.connect(server, port)) { // Connect to the server
    String postData = "rfid_id=" + rfid_id + "&access_status=" + access_status;

    // Send HTTP POST request
    client.println("POST /~emily.long/view_toll_data.php HTTP/1.1");
    client.println("Host: " + String(server)); // Specify server host
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);

    // Wait for a response
    delay(1000);

    // Print server response
    while (client.available()) {
      String response = client.readString();
      Serial.println("Server Response: ");
      Serial.println(response);
    }
    client.stop(); // Close the connection
  } else {
    Serial.println("Connection to server failed");
  }
}

