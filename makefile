gen:
	python3 gen_emulator.py

build:
	cc emulator.c -o emulator
	echo -e "#!/usr/bin/env python3\n" | cat - terminal_py/terminal.py 8085_asm/8085_asm.py > assembler

install:
	chmod +x assembler
	chmod +x emulator
	cp emulator /bin/8085_emu
	cp assembler /bin/8085_asm
	cp 8085_asm/opcodes.txt /etc/8085_asm_opcodes.txt

uninstall:
	rm /bin/8085_emu
	rm /bin/8085_asm
	rm /etc/8085_asm_opcodes.txt
