avr-gcc -std=c99 -Os -mmcu=atxmega128a3  -Wall -foptimize-sibling-calls -fdata-sections -ffunction-sections -Wl,-gc-sections g.c -o g.elf
avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock g.elf g.hex