# ESP8266-WiFi-UART-Bridge-With-Authentication

This project leverages the power of the ESP8266 to establish a WiFi communication bridge for UART serial data. This setup allows for seamless transmission of data from a UART serial device to a WiFi device and vice versa. This code transforms the ESP8266 into a transparent bridge between WiFi and UART, effectively converting serial protocol to TCP/UDP protocol. It is also designed with flexibility in mind, allowing for operation in both AP and STATION modes.

Originally designed by [@roboremo](https://github.com/roboremo) as a Serial-to-WiFi bridge, this version has been further enhanced with additional features like buffer overflow handling, increased buffer size, and user authentication.

An enhanced version of the ESP8266 WiFi UART Bridge for seamless communication between UART and WiFi. Includes buffer overflow handling, increased buffer size, user authentication, and support for both TCP and UDP protocols. Ideal for remote interaction with UART devices even in network down situations.

## Features

- **Serial-to-WiFi Bridge**: This code reads data from a UART device, sends it to a TCP client, and vice versa. It allows for remote interaction with a UART device via WiFi.

- **Configurable Modes**: The code supports both AP (Access Point) and STATION mode. In AP mode, the ESP8266 creates its own wireless network that other devices can connect to. In STATION mode, the ESP8266 connects to an existing WiFi network.
  
- **TCP and UDP Protocols**: The code supports both TCP and UDP protocols, which you can select based on your use case. TCP offers guaranteed delivery but may be slower, while UDP is faster but does not guarantee delivery.
  
- **Buffer Overflow Handling**: The code includes handling for buffer overflow to prevent data loss and ensure smooth communication.

- **User Authentication**: For TCP protocol, the code implements a simple username/password authentication mechanism, ensuring secure communication.

## Configuration

To configure the code to your needs, set the variables at the beginning of the code. You can select the mode (AP/STATION), the protocol (TCP/UDP), and the username/password for TCP mode. See the code comments for more details.

## Usage

Flash the code to your ESP8266 device. Based on your configuration, connect your other device (like a Raspberry Pi) to the ESP8266's network (in AP mode) or to the same network the ESP8266 is connected to (in STATION mode). For TCP mode, provide the correct username and password when prompted. The ESP8266 will act as a bridge, allowing you to interact with your UART device through WiFi.

## Real-world Application

An example use-case of this project is enabling SSH access to a Raspberry Pi via UART, even in situations where the Raspberry Pi's network is down due to misconfigurations, driver issues, etc. By connecting the ESP8266 to the Raspberry Pi's UART pins (ESP8266's RX to Raspberry Pi's TX and vice versa), you can use this WiFi bridge to access the Raspberry Pi's command line interface.

## Contributing

Contributions are welcome! Feel free to create issues for bug reports or feature requests, and submit pull requests if you've implemented a cool feature or fix yourself.

## Acknowledgments

A big thanks to [@roboremo](https://github.com/roboremo) for the initial design of this Serial-to-WiFi bridge. The enhancements made in this project have significantly increased its functionality and usability.

---

You can find the original project here: [ESP8266 WiFi UART Bridge](https://github.com/roboremo/ESP8266-WiFi-UART-Bridge)
