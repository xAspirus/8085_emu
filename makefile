gen:
	python3 gen_emulator.py

build:
	cc emulator.c -o emulator

install:
	cp emulator /bin/8085_emu
	cp assembler /bin/8085_asm
	cp opcodes.txt /etc/8085_asm_opcodes.txt

uninstall:
	rm /bin/8085_emu
	rm /bin/8085_asm
	rm /etc/8085_asm_opcodes.txt
