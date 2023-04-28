# TP_Final_MPU9255
Trabajo Final para la materia PCSE y PdM Autor Alan Andres Ayala

Entrega proyecto final utilizando placa nucleo f429zi y mpu 9255

El proyecto implementa un driver para el obetener la aceleracion del acelerometro dentro del MPU9255.

Detalle de los módulos:

API_doubonce.c API_doubonce.h -> Implementa una FSM para la MEF
API_delay.c API_delay.h -> Implementa delay no bloqueante
API_uart.c API_uart.h -> Implementa funciones de UART
port-I2C.c port-I2C.h -> transmicion I2C

