# 8085_emu
An emulator based on the intel 8085 chip.
Currently it only supports Linux systems. (possibly should work on macOS)

### Installation

**Compile**
```
make gen build
```

**Install**
```
make install
```

**Uninstall**
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

The `EI` instruction is used to set a breakpoint. It will start a debug prompt.
`BRK` can be used as a alias for `EI` in the assembler
