# Uart Port Pins Table

## Available Pins
Port/Pin | UART | Select Register | Function 
------|-----------|----------|----
P0.0  | UART-3 Tx | PINSEL 0 | 10
P0.1  | UART-3 Rx | PINSEL 0 | 10
P2.0  | UART-1 Tx | PINSEL 4 | 10
P2.1  | UART-1 Rx | PINSEL 4 | 10
P2.8  | UART-2 Tx | PINSEL 4 | 10
P2.9  | UART-2 Rx | PINSEL 4 | 10
P4.28 | UART-3 Tx | PINSEL 9 | 11
P4.29 | UART-3 Rx | PINSEL 9 | 11

## Non-available Pins
Port/Pin | UART | Select Register | Function | Why
------|-----------|----------|----|-----------
P0.2  | UART-1 Tx | PINSEL 0 | 01 | FT323 TXD0
P0.3  | UART-1 Rx | PINSEL 0 | 01 | FT232 RXD0
P0.10 | UART-2 Tx | PINSEL 0 | 01 | SDA2
P0.11 | UART-2 Rx | PINSEL 0 | 01 | SCL2
P0.15 | UART-1 Tx | PINSEL 0 | 01 | SCK0
P0.16 | UART-1 Rx | PINSEL 1 | 01 | NORDIC_CS
P0.25 | UART-3 Tx | PINSEL 1 | 11 | Light Sensor
P0.26 | UART-3 Rx | PINSEL 1 | 11 | ADC CON
