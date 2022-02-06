# 8085_emu
An emulator based on the intel 8085 chip. Tested on Arch Linux only.

### Installation

**Emulator**
```
make gen build
```

**Assembler**
The assembler requires python 3 and rich library.
```
pip3 install rich
```

Install both emulator and assembler
```
make install
```

Uninstall both emulator and assembler
```
make uninstall
```

### Output Ports

```
port   |   device
-------+------------------------------------------------------------------
0xF0   |   stdout (print Acc. as ASCII char)
0xF1   |   stdout (print Acc. as decimal)
0xF2   |   stdout (print Acc. as hexadecimal without any prefix or suffix)
```

### Unsupported Instructions

```
RST n
SIM
RIM
EI
```

### Debugging

The `EI` instruction is used to set a breakpoint. It will start a debug prompt which accepts the following commands.
`BRK` can be used as a alias for `EI` in the assembler.

```
all               print all registers and their values
mem addr          print memory at addr
mem addr1 addr2   print memory from addr1 to addr2
```

### Assembler

The assembler requires `opcodes.txt` to be present in current directory.
