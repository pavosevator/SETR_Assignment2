# SETR_Assignment2
The primary objective of this project was to implement a C module capable of processing commands received via UART for a smart sensor node. The sensor node measures temperature, relative humidity, and CO₂ levels. The module was developed using Test-Driven Development principles, ensuring robustness and reliability through comprehensive unit testing.

## Implementation overview - part 1
Code is organised as following:   
📁 CmdProc - module for library that contains all necessary functionalities for smart sensor node  + includes Doxyfile  
📁 test - contains test_1.c where all Unity Tests are written   
📁 src - contains main.c where first tests were performed   
📁 Unity - files that allow using of Unity testing in test_1.c file  
📁 build - files generated with make command (includes unity tests executable and main.c executable)  
🖹 CMakeLists.txt - instructions for make command

## Implementation overview - part 2  
The module processes commands structured as # CMD DATA CS !  

• # is the Start of Frame symbol.  
• CMD is a one-byte command indication.  
• DATA is variable-sized command arguments.  
• CS is a one-byte checksum.  
• ! is the End of Frame symbol.  

Supported commands include:  
• A: Reads real-time values of the sensor variables.  
• P: Reads the real-time value of one of the sensors.  
• L: Returns the last 20 samples of each variable.  
• R: Resets the history.  

Sensor data is emulated using a Linear Congruential Generator for pseudo-random number generation within specified ranges.
The core of the command processing logic resides in the cmdProcessor function. This function is responsible for parsing incoming UART data, validating command structures, and executing the appropriate actions based on the command received. It checks for the presence of start and end frame symbols, verifies the checksum, and processes valid commands by invoking the corresponding handler functions. 
