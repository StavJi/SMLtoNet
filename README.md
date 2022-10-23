# SML inverter to internet
In uses sligly modified library for ESP32 originaly created by **scottwday** https://github.com/scottwday/InverterOfThings

# Used parts
- ESP-32S
- TTL UART to RS232 converter build on top of ST232CDR (datasheet recommended schematic)
- DB9 male connector

# Note
It useses UART2 of ESP-32S for communication with inverter and UART1 (USB) to send debug information
