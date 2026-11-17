## Codes to check ENC28J60 module workability.

- #1 to test if the module is alive, wiring is correct;
- #2 creates an example webpage accesible on IP you'll see in a Serial port;
- #3 complete code for TCP MODBUS communication.

## How to use, wiring, suggestions
- Full wiring instruction is ENC28J60_wiring.PNG for arduino mega. All other AVR microcontrolers should work the same way.
- Module itself needs a ton of power. USB and dev board won't be able to power the module properly.
> [!WARNING]
> module is rated for 3.3v
- Using 6 pin port for a programmer is the best way to connect the module. Regular SPI pins may not work.
- For TCP MODBUS a great thing for developing is Open ModScan application.

<br>
<p align="right"><strong>Arsenii's Technologies</strong></p>
