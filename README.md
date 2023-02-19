# SBG-Ellipse-2N-Sensor-Data-Extraction-Code
This code implements a firmware for extracting data from an SBG Ellipse-2N sensor. It configures three serial interfaces to communicate with the sensor and other devices, and reads data from the sensor to extract Euler angles, navigation data, and GPS data.
SBG Ellipse-2N Sensor Data Extraction Code
The code presented here extracts data from an SBG Ellipse-2N sensor connected to an STM32F103 board. The sensor provides Euler angles, EKF-Navigation data, and GPS data in the NMEA format. The code extracts the relevant data and sends it over a serial connection.

# How to use
The code can be compiled and uploaded to an STM32F103 microcontroller board, and the board can be connected to an SBG Ellipse-2N sensor and other devices as per the pin configurations. The code reads data from the sensor at a rate of 400 milliseconds, extracts Euler angles, navigation data, and GPS data, and sends the data to the other devices via Serial3.
The code requires the following libraries: string.h, ctype.h, and Arduino.h.

The setup() function initializes three serial connections: Serial1, Serial2, and Serial3. Serial1 is used to receive data from the SBG Ellipse-2N sensor, while Serial2 and Serial3 are used to send data over a serial connection. The function also sets some initial values for various flags and variables.

The loop() function continuously checks whether there is data available on Serial1. When there is data available, it reads the data and processes it according to the type of data received. If the data is Euler data, it extracts the Euler angles and sends them over the serial connection. If the data is EKF-Navigation data, it extracts the latitude, longitude, and altitude and sends them over the serial connection. If the data is GPS data, it extracts the speed and course over ground and sends them over the serial connection.

# Variables
The following variables are used in the code:

msg_c: A counter that keeps track of the number of messages received.
i: An index used to read incoming data.
byte_c[2]: An array used to store a counter value as two bytes.
y: A temporary variable used to store a character from the incoming data.
rxstream[1024]: A buffer used to store incoming data.
msg_tx[65]: A buffer used to store outgoing data.
f_euler: A flag used to indicate whether incoming data is Euler data.
f_nav: A flag used to indicate whether incoming data is EKF-Navigation data.
f_ekf_nav: A flag used to indicate whether incoming data is GPS data.
rmc_rd: A flag used to indicate whether incoming data is in the NMEA format.
indc0, indc1, indc2, indc3, indc4, indc5, indc6, indc7, indc8, indc9: Index variables used to extract GPS data from the NMEA format.
tr_angle: A variable used to store the course over ground.
ac_speed: A variable used to store the speed over ground.
stringGPS: A string used to store the NMEA data.
previousMillis: A variable used to keep track of the last time an action was taken.
interval: The interval at which to take an action.

# Implementation Details
The code has a setup function that initializes the serial interfaces, sets some variables, and waits for 90 seconds before starting the loop function.

The loop function reads data from Serial1 and checks for the sync bytes that indicate the start of a message. If the sync bytes for an Euler message are found, the code reads the Euler angles from the message and stores them in the msg_tx byte array. If the sync bytes for an EKF-navigation message are found, the code reads the navigation data from the message and stores it in the msg_tx byte array. If the sync bytes for a GPS message are found, the code reads the GPS data from the message and stores it in the msg_tx byte array.

The msg_tx byte array is updated with the extracted data, and its contents are sent to the other devices via Serial3.

The loop function also checks if the elapsed time since the last data reading is greater than the specified interval (400 milliseconds), and if so, it reads the data again.

# Future Improvements
The following improvements can be made to the code:

Add error handling and recovery mechanisms to handle communication failures and sensor malfunctions.
Implement better synchronization and parsing of the sensor messages to improve the reliability and accuracy of the extracted data.
Add support for more sensor data types and output formats to make the code more versatile and flexible.
Optimize the code for better performance and memory usage, and remove unnecessary delays and variables.
