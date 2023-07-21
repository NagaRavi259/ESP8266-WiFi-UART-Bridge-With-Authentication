// CONFIGURATION BLOCK------------------------------------------------------------------------------------\START
// FOr Hotspot configuration
const char *AP_ssid = "ESPSerialWiFi";  // AP mode: SSID for the Access Point
const char *AP_pw = "asdfghjkl"; // AP mode: Password for the Access Point

// to connect cotspot
const char *STA_ssid = "AndroidAP_5321";  // STA mode: Your ROUTER SSID
const char *STA_pw = "asdfghjkl"; // STA mode: WiFi PASSWORD

// TCP port
const int STA_port = 23; // STA mode: TCP/UDP Communication port

#define bufferSize 20000 // Size of the buffer
#define packTimeout 5 // ms (if nothing more on UART, then send packet)

const char* TCP_username = "user1";
const char* TCP_password = "password1";

const String mode = "STA";  // set "AP" for Access Point mode, set "STA" for Station mode
const char* protocol = "TCP"; // TCP or UDP
// END OF CONFIGURATION BLOCK-----------------------------------------------------------------------------/END


// Imports
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

// WiFi server setup and port configuration
WiFiServer server(STA_port);
WiFiClient client;
// UDP Configuration
WiFiUDP udp;
IPAddress remoteIp;

// Define the valid username and password credentials
const char* valid_credentials[][2] = {
  {TCP_username, TCP_password}  // username and passowrd to login from TCP server
};

uint8_t buf1[bufferSize];
uint16_t i1=0;

uint8_t buf2[bufferSize];
uint16_t i2=0;

void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println("its in setup");

  if(mode == "AP") {
    IPAddress AP_ip(192, 168, 0, 1); // AP mode: IP to connect to
    IPAddress AP_netmask(255, 255, 255, 0); // AP mode: Network mask
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(AP_ip, AP_ip, AP_netmask);
    WiFi.softAP(AP_ssid, AP_pw);
  } else {
    WiFi.mode(WIFI_STA); 
    WiFi.begin(STA_ssid, STA_pw);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }
  }

  if (strcmp(protocol, "TCP") == 0) {
    Serial.println("Starting TCP Server");
    server.begin();
  } else if (strcmp(protocol, "UDP") == 0) {
    Serial.println("Starting UDP Server");
    udp.begin(STA_port);
  }
}

void loop() {
  if (strcmp(protocol, "TCP") == 0) {
    if(!client.connected()) {
      client = server.available();
      if (client.connected()) {
        authenticate_client();
      }
    }
    handleTCPData();
  } else if (strcmp(protocol, "UDP") == 0) {
    handleUDPData();
  }
}

// Verify Password Function Start-------------------------------------------------------------------------\START

// This function takes a password and a stored password and compares them for equality. 
// It returns true if they match, and false otherwise.
bool verify_password(const char* password, const char* password_stored) {
  return strcmp(password, password_stored) == 0;
}
// Verify Password Function End---------------------------------------------------------------------------/END


// Authenticate Client Function Start---------------------------------------------------------------------\START

// This function attempts to authenticate the client by comparing the provided username and password 
// with the expected username and password. If they match, it returns true, otherwise it returns false.
bool authenticate_client() {
  int tries = 0;
  bool authenticated = false;

  while (tries < 3 && !authenticated) {
    // Prompt the client for username
    client.print("Username: \n");
    while (!client.available()) {
      delay(1);
    }
    String username = client.readStringUntil('\n');
    username.trim();

    // Prompt the client for password
    client.print("Password: \n");
    while (!client.available()) {
      delay(1);
    }
    String password = client.readStringUntil('\n');
    password.trim();

    // Verify the credentials
    for (size_t i = 0; i < sizeof(valid_credentials) / sizeof(valid_credentials[0]); i++) {
      if (username.equals(valid_credentials[i][0]) && verify_password(password.c_str(), valid_credentials[i][1])) {
        authenticated = true;
        client.print("Authentication successful. You are logged in.\n");
        Serial.println("Authenticated");
        break;
      }
    }

    if (!authenticated) {
      tries++;
      int remaining_tries = 3 - tries;
      client.print("Authentication failed. Invalid username or password. ");
      client.print(remaining_tries);
      client.print(" tries remaining.\n");
      Serial.println("Failed authentication");
      delay(1000);
    }
  }

  return authenticated;
}
// Authenticate Client Function End-----------------------------------------------------------------------/END


// Handle TCP Data Function Start ------------------------------------------------------------------------\START

// This function handles the TCP data communication. It reads the data from the client 
// and writes it to the buffer. It also reads the data from the Serial and sends it to the client.
void handleTCPData() {
  if(client.available()) {
    while(client.available()) {
      buf1[i1] = (uint8_t)client.read();
      if(i1<bufferSize-1) {
        i1++;
      } else {
        Serial.println("Warning: Buffer 1 overflow. Buffer is being reset.");
        i1 = 0;  // Reset the buffer index
      }
    }
    Serial.write(buf1, i1);
    i1 = 0;
  }

  if(Serial.available()) {
    while(1) {
      if(Serial.available()) {
        buf2[i2] = (char)Serial.read();
        if(i2<bufferSize-1) {
          i2++;
        } else {
          Serial.println("Warning: Buffer 2 overflow. Buffer is being reset.");
          i2 = 0;  // Reset the buffer index
        }
      } else {
        delay(packTimeout);
        if(!Serial.available()) {
          break;
        }
      }
    }
    client.write((char*)buf2, i2);
    i2 = 0;
  }
}
// Handle TCP Data Function End --------------------------------------------------------------------------/END


// Handle UDP Data Function Start-------------------------------------------------------------------------\START

// This function handles the UDP data communication. It reads the data from the UDP client 
// and writes it to the buffer. It also reads the data from the Serial and sends it to the UDP client.
void handleUDPData() {
  int packetSize = udp.parsePacket();
  if(packetSize>0) {
    remoteIp = udp.remoteIP();
    udp.read(buf1, bufferSize);
    Serial.write(buf1, packetSize);
  }

  if(Serial.available()) {
    while(1) {
      if(Serial.available()) {
        buf2[i2] = (char)Serial.read();
        if(i2<bufferSize-1) {
          i2++;
        } else {
          Serial.println("Warning: Buffer 2 overflow. Buffer is being reset.");
          i2 = 0;  // Reset the buffer index
        }
      } else {
        delay(packTimeout);
        if(!Serial.available()) {
          break;
        }
      }
    }
    udp.beginPacket(remoteIp, STA_port);
    udp.write(buf2, i2);
    udp.endPacket();
    i2 = 0;
  }
}
// Handle UDP Data Function End---------------------------------------------------------------------------/END

