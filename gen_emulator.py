code = ''
def puts(string):
	global code
	code += string


# OPCODE ENUMS
def header():
	
	global code
	code = ''
	puts('enum {\n')
	with open('8085_asm/opcodes.txt', 'r') as f:
		
		for opcode in f:
			opcode = opcode.split()
			#print(opcode)
			if len(opcode) != 2: continue
			puts(f'\t{opcode[0]} = 0x{opcode[1]},\n')
		
	puts('};\n')
	return code
	

def inst_code():
	
	global code
	code = ''
	
	for x in 'ABCDEHL':
		for y in 'ABCDEHL':
			puts(f'case MOV_{x}_{y}:{x}={y};break;\n')
	for i in 'ABCDEHL':
		puts(f'case MOV_M_{i}:MEM[XH()]={i};break;\n')
		puts(f'case MOV_{i}_M:{i}=MEM[XH()];break;\n')
	
	for x in 'ABCDEHL':
		puts(f'case MVI_{x}_d:{x}=readmem();break;\n')
	puts('case MVI_M_d:MEM[XH()]=readmem();break;\n')

	for x in 'ABCDEHL':
		puts(f'case ADD_{x}:A_add({x});break;\n')
		puts(f'case ADC_{x}:A_adc({x});break;\n')
	puts('case ADD_M:A_add(MEM[XH()]);break;\n')
	puts('case ADC_M:A_adc(MEM[XH()]);break;\n')
	puts('case ADI_d:A_add(readmem());break;\n')
	puts('case ACI_d:A_adc(readmem());break;\n')

	# Subtraction
	
	for x in 'ABCDEHL':
		puts(f'case SUB_{x}:A_sub({x});break;\n')
		puts(f'case SBB_{x}:A_sbb({x});break;\n')
	
	puts('case SUB_M:A_sub(MEM[XH()]);break;\n')
	puts('case SBB_M:A_sbb(MEM[XH()]);break;\n')
	puts('case SUI_d:A_sub(readmem());break;\n')
	puts('case SBI_d:A_sbb(readmem());break;\n')
	
	# Increment Register
	# Decrement Register

	for x in 'ABCDEHL':
		puts(f'case INR_{x}: {x}++; flags({x}); break;\n')
		puts(f'case DCR_{x}: {x}--; flags({x}); break;\n')
	puts('case INR_M: MEM[XH()]++; flags(MEM[XH()]); break;\n')
	puts('case DCR_M: MEM[XH()]--; flags(MEM[XH()]); break;\n')
	
	# Logical Operators

	for x in 'ABCDEHL':
		puts(f'case ANA_{x}: A &= {x}; flags(A); CY = 0; AC = 1; break;\n')
		puts(f'case ORA_{x}: A |= {x}; flags(A); CY = 0; AC = 0; break;\n')
		puts(f'case XRA_{x}: A ^= {x}; flags(A); CY = 0; AC = 0; break;\n')
	
	puts('case ANA_M: A &= MEM[XH()]; flags(A); CY = 0; AC = 1; break;\n')
	puts('case ORA_M: A |= MEM[XH()]; flags(A); CY = 0; AC = 0; break;\n')
	puts('case XRA_M: A ^= MEM[XH()]; flags(A); CY = 0; AC = 0; break;\n')
	
	puts('case ANI_d: A &= readmem(); flags(A); CY = 0; AC = 1; break;\n')
	puts('case ORI_d: A |= readmem(); flags(A); CY = 0; AC = 0; break;\n')
	puts('case XRI_d: A ^= readmem(); flags(A); CY = 0; AC = 0; break;\n')
	
	# CMP Compare with Acc.
	
	for x in 'ABCDEHL':
		puts(f'''
		case CMP_{x}:
			if (A < {x}) {{
				CY = 1;
			}}
			else if (A == 0) {{
				ZR = 1;
			}} else {{
				CY = 0;
				ZR = 0;
			}}
			flags(A);
			break;''')
	
	puts('''
		case CPI_d:
			if (A < readmem()) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;''')
	
	puts('''
		case CMP_M:
			if (A < MEM[XH()]) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;''')
	
	return code

with open('emulator.h', 'w') as f:
	f.write(header())

with open('__emulator.c', 'r') as f:
	
	f = f.read()
	f = f.replace('/*<INST_CODE>*/', inst_code())
	
	with open('emulator.c', 'w') as ff:
		ff.write(f)
