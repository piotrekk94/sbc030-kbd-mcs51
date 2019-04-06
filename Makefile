CC = sdcc
CFLAGS = -mmcs51 --acall-ajmp --iram-size 128
OBJCOPY = objcopy
SIZE = size

hex:
	$(CC) $(CFLAGS) kbd.c
	$(OBJCOPY) -I ihex kbd.ihx -O binary kbd.bin
	$(SIZE) kbd.ihx

clean:
	rm -f *.ihx *.bin *.lk *.lst *.map *.mem *.rel *.rst *.sym *.asm

.PHONY : hex clean
