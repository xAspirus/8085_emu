import sys


if len(sys.argv) > 1:
	FILE = sys.argv[1]
	if len(sys.argv) == 3:
		OUT_FILE = sys.argv[2]
	else:
		OUT_FILE = FILE.replace('.8085', '.bin')
else:
	print("Usage: assembler file.8085 output.bin")
	exit(1)


INST_0 = []
INST_1 = []
INST_2 = []
OPCODES = []

with open('/etc/8085_asm_opcodes.txt', 'r') as f:
	for i in f:
		if i.strip('\n') != '':
			OPCODES.append((i.split()[0].replace('_d', '').replace('_m', ''), int(i.split()[1], 16)))
		
		i = i.split()
		if len(i) != 2: continue
		if not '_' in i[0]:
			INST_0.append(i[0])
		elif i[0].endswith('_d') or i[0].endswith('_m'):
			if i[0].count('_') == 2:
				INST_1.append(i[0].split('_')[0])
			else:
				INST_0.append(i[0][:-2])
		elif i[0].count('_') == 1:
			INST_1.append(i[0].split('_')[0])
		elif i[0].count('_') == 2:
			INST_2.append(i[0].split('_')[0])

INST_0 = set(INST_0)
INST_1 = set(INST_1)
INST_2 = set(INST_2)


def error(file, line_no, error_type, value = None):
	
	syntax_rules = (
		('operator', '[,]'),
		('function', '[A-Za-z0-9_]+'),
		('function:', '[A-Za-z0-9_]+:'),
		('number', '[0-9A-Fa-f]+"'),
		('constant:', '(?i)\\b(a|b|c|d|e|h|l|m|A|B|C|D|E|H|L|M)\\b'),
		('statement', '(?i)\\b(mov|mvi|ldax|stax|lda|sta|lxi|lhld|shld|xchg|in|out)\\b'),
		('statement', '(?i)\\b(add|adi|adc|aci|sub|sui|sbb|sbi|inr|dcr|inx|dcx|dad|daa)\\b'),
		('statement', '(?i)\\b(ana|ora|xra|cmp|ani|ori|xri|cpi|cma|cmc|stc|rlc|rrc|ral|rar)\\b'),
		('statement', '(?i)\\b(jmp|jc|jnc|jp|jm|jpe|jpo|jz|jnz)\\b'),
		('statement', '(?i)\\b(call|cc|cnc|cp|cm|cpe|cpo|cz|cnz)\\b'),
		('statement', '(?i)\\b(ret|rc|rnc|rp|rm|rpe|rpo|rz|rnz)\\b'),
		('statement', '(?i)\\b(rst|push|pop|xthl|sphl|sim|rim|ei|pchl|nop|hlt|brk)\\b')
	)
	
	print_highlighted(file.split('\n')[line_no - 1], syntax_rules, Themes.default, numbered = True, ln = line_no)
	
	if error_type == "hex_literal_err":
		print(f'{Colors.Fg.red}{error_type}:{Colors.reset} Hex literal {value} too big.')
		print(f'{Colors.Fg.Bright.blue}hint: Hex literals should be either 2 bytes or 1 byte.{Colors.reset}')
		
	elif error_type == "label_undefined_err":
		print(f'{Colors.Fg.red}{error_type}:{Colors.reset} Label "{value}" was not defined.')

	elif error_type == "operand_err":
		print(f'{Colors.Fg.red}{error_type}:{Colors.reset} Expected register/pair but got "{value}"')
	
	exit(1)


def lex(f):
	
	tk = []
	add_tk = lambda t, val: tk.append((t, val, ln))
	quoted = False
	key = ''
	ln = 1
	
	i = 0
	while i < len(f):
		
		if quoted:
			
			if f[i] == '"':
				quoted = False
			else:
				if f[i] == '\\':
					n = 0
					if   f[i + 1] == 'n': n = ord('\n')
					elif f[i + 1] == 't': n = ord('\t')
					elif f[i + 1] == '"': n = ord('"')
					elif f[i + 1] == '0': n = 0
					tk.append(('data', n))
					i += 1
				else:
					tk.append(('data', ord(f[i])))
			
		else:
			
			if f[i] == '"':
				quoted = True
				
			elif f[i] == "'":
				i += 1
				if f[i] == '\\':
					i += 1
					n = 0
					if   f[i] == 'n': n = ord('\n')
					elif f[i] == 't': n = ord('\t')
					elif f[i] == '\'': n = ord('\'')
					elif f[i] == '0': n = 0
					add_tk('data', n)
					
				else:
					add_tk('data', ord(f[i]))
				i += 1
				
			elif f[i] == ';':
				while f[i] != '\n' and i < len(f): i += 1
				ln += 1
				
			elif f[i] in ' \t\n,':
				
				if key != '':
					if key.startswith('0x'):
						# Data Literal
						if len(key) == (2 + 2):
							add_tk('data', int(key, 0))
						# Address Literal (little-endian)
						elif len(key) == (2 + 4):
							add_tk('data', int(key[4:6], 16))
							add_tk('data', int(key[2:4], 16))
						else:
							error(f, ln, "hex_literal_err", key)
						
					elif key.endswith(':'):
						add_tk('label_def', key[:-1])
						
					elif key in 'abcdehlmswABCDEHLMSW':
						add_tk('register', key.lower())
						
					else:
						add_tk('identifier', key)
				
				if f[i] == '\n': ln += 1
				
				key = ''
				
			else:
				
				key += f[i]
		i += 1
	return tk


def parse(tk):
	
	OPLIST = list(map(lambda x: x[0], OPCODES))
	mem = []
	
	i = 0
	while i < len(tk):
		if tk[i][0] == 'identifier':
			if tk[i][1].upper() in INST_0:
				n = OPLIST.index(tk[i][1].upper())
				mem.append(('data', OPCODES[n][1]))
				
			elif tk[i][1].upper() in INST_1:
				try:
					opc = tk[i][1].upper() + '_' + tk[i + 1][1].upper()
				except IndexError:
					error(file, tk[i][2], "operand_err", "none")
				
				if not tk[i + 1][1].upper() in 'ABCDEHLMSW':
					error(file, tk[i][2], "operand_err", tk[i + 1][1])
				
				n = OPLIST.index(opc)
				mem.append(('data', OPCODES[n][1]))
				i += 1
				
			elif tk[i][1].upper() in INST_2:
				try:
					opc = tk[i][1].upper() + '_' + tk[i + 1][1].upper() + '_' + tk[i + 2][1].upper()
				except IndexError:
					error(file, tk[i][2], "operand_err", "none")
			
				if not tk[i + 1][1].upper() in 'ABCDEHLMSW':
					error(file, tk[i][2], "operand_err", tk[i + 1][1])
			
				if not tk[i + 2][1].upper() in 'ABCDEHLMSW':
					error(file, tk[i][2], "operand_err", tk[i + 2][1])
			
				n = OPLIST.index(opc)
				mem.append(('data', OPCODES[n][1]))
				i += 2
				
			else:
				mem.append(tk[i])
				
		else:
			mem.append(tk[i])
			
		i += 1
	
	return mem


def resolve(mem, file):
	
	labels = {}
	
	i = 0
	while i < len(mem):
		if mem[i][0] == 'label_def':
			labels[mem[i][1]] = i
			mem.pop(i)
			i -= 1
			
		elif mem[i][0] == 'identifier':
			mem.insert(i + 1, None)
			i += 1
		
		i += 1
	
	i = 0
	while i < len(mem):
		if mem[i][0] == 'identifier':
			try:
				n = labels[mem[i][1]]
			except KeyError:
				error(file, mem[i][2], "label_undefined_err", mem[i][1])
			
			mem[i] = ('data', n&0xff)
			mem[i+1] = ('data', (n&0xff00)>>8)
			i += 1
		
		i += 1
	
	return mem


with open(FILE, 'r') as f:
	
	file = f.read()
	lexed = lex(file)
	parsed = parse(lexed)
	resolved = resolve(parsed, file)
	mem = list(map(lambda x: x[1], resolved))
	with open(OUT_FILE, 'wb') as ff:
		ff.write(bytes(mem))
