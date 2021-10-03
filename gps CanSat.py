# Información GPS: https://learn.adafruit.com/adafruit-ultimate-gps/circuitpython-parsing
# Instalar CircuitPython: https://learn.adafruit.com/welcome-to-circuitpython/installing-circuitpython
# Instalar librería Python: https://github.com/adafruit/Adafruit_CircuitPython_GPS
# Usar SSH: https://learn.adafruit.com/adafruits-raspberry-pi-lesson-6-using-ssh/enabling-ssh

import time
import board
import busio

import adafruit_gps

RX = board.RX
TX = board.TX

uart = busio.UART(TX, RX, baudrate=9600, timeout=30)

gps = adafruit_gps.GPS(uart, debug=False)

gps.send_command(b'PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0')

gps.send_command(b'PMTK220,1000')

last_print = time.monotonic()
while True:

    gps.update()

    current = time.monotonic()
    if current - last_print >= 1.0:
        last_print = current
        if not gps.has_fix:
            print('Waiting for fix...')
            continue
        print('=' * 40)  # Print a separator line.
        print('Latitude: {0:.6f} degrees'.format(gps.latitude))
        print('Longitude: {0:.6f} degrees'.format(gps.longitude))