#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "emulator.h"
#include "ui.c"


uint8_t A, T, B, C, D, E, H, L;
uint16_t PC, SP;
uint8_t SN, ZR, AC, PR, CY;
uint8_t MEM[0xFFFF];


uint16_t XH() {
	return ((uint16_t)H << 8) | (uint16_t)L;
}

uint16_t XB() {
	return ((uint16_t)B << 8) | (uint16_t)C;
}

uint16_t XD() {
	return ((uint16_t)D << 8) | (uint16_t)E;
}

uint8_t readmem() {
	return MEM[PC++];
}

uint16_t readmemx() {
	uint8_t a = readmem();
	uint8_t b = readmem();
	return (uint16_t)a | ((uint16_t)b << 8);
}


void flags(uint8_t n) {
	ZR = n == 0;
	SN = n >> 7;
	PR = n;
	PR ^= PR >> 4;
	PR ^= PR >> 2;
	PR ^= PR >> 1;
	PR = (~PR) & 1;
}

void A_add(uint8_t n) {
	CY = (A + n) < A;
	A += n;
	flags(A);
}

void A_sub(uint8_t n) {
	A -= n;
	flags(A);
	CY = SN;
}

void A_adc(uint8_t n) {
	T = CY;
	CY = (A + n + T) < A;
	A += n + T;
	flags(A);
}

void A_sbb(uint8_t n) {
	A -= n + CY;
	flags(A);
	CY = SN;
}


void call(uint16_t a) {
	SP--;
	MEM[SP] = (uint8_t)PC;
	SP--;
	MEM[SP] = (uint8_t)(PC >> 8);
	PC = a;
}

void ret() {
	PC = (uint16_t)MEM[SP + 1] | ((uint16_t)MEM[SP] << 8);
	SP += 2;
}


int execute() {
	
	switch (readmem()) {
		
		uint16_t TP;
		
		/*<INST_CODE>*/
		
	  case OUT_d:
		T = readmem();
		if (T == 0xF0) {
			fputc(A, stdout);
		}
		else if (T == 0xF1) {
			printf("%d", A);
		}
		else if (T == 0xF2) {
			printf("%02X", A);
		}
		break;
		
	  case LDAX_B:
		A = MEM[XB()];
		break;
		
	  case LDAX_D:
		A = MEM[XD()];
		break;
		
	  case STAX_B:
		MEM[XB()] = A;
		break;
		
	  case STAX_D:
		MEM[XD()] = A;
		break;
		
	  case LDA_m:
		A = MEM[readmemx()];
		break;
		
	  case STA_m:
		MEM[readmemx()] = A;
		break;
		
	  case LXI_B_m:
		C = readmem();
		B = readmem();
		break;
		
	  case LXI_D_m:
		E = readmem();
		D = readmem();
		break;
		
	  case LXI_H_m:
		L = readmem();
		H = readmem();
		break;
	
	  case LXI_S_m:
		SP = readmemx();
		break;
		
	  case LHLD_m:
		TP = readmemx();
		L = MEM[TP];
		H = MEM[TP + 1];
		break;
		
	  case SHLD_m:
		TP = readmemx();
		MEM[TP] = L;
		MEM[TP + 1] = H;
		break;
		
	  case XCHG:
		T = H;
		H = D;
		D = T;
		T = L;
		L = E;
		E = T;
		break;
		
	  case IN_d:
		// TODO:
		break;
		
	  case HLT:
		return 0;
		
	  case INX_B:
		TP = XB() + 1;
		B = (uint8_t)(TP >> 8);
		C = (uint8_t)TP;
		break;
		
	  case INX_D:
		TP = XD() + 1;
		D = (uint8_t)(TP >> 8);
		E = (uint8_t)TP;
		break;
		
	  case INX_H:
		TP = XH() + 1;
		H = (uint8_t)(TP >> 8);
		L = (uint8_t)TP;
		break;
		
	  case INX_S:
		SP++;
		break;
		
	  case DCX_B:
		TP = XB() - 1;
		B = (uint8_t)(TP >> 8);
		C = (uint8_t)TP;
		break;
		
	  case DCX_D:
		TP = XD() - 1;
		D = (uint8_t)(TP >> 8);
		E = (uint8_t)TP;
		break;
		
	  case DCX_H:
		TP = XH() - 1;
		H = (uint8_t)(TP >> 8);
		L = (uint8_t)TP;
		break;
		
	  case DCX_S:
		SP--;
		break;
		
	  case DAD_B:
		TP = XH() + XB();
		CY = TP < XH();
		H = (uint8_t)(TP >> 8);
		L = (uint8_t)TP;
		break;
		
	  case DAD_D:
		TP = XH() + XD();
		CY = TP < XH();
		H = (uint8_t)(TP >> 8);
		L = (uint8_t)TP;
		break;
		
	  case DAD_H:
		TP = XH() + XH();
		CY = TP < XH();
		H = (uint8_t)(TP >> 8);
		L = (uint8_t)TP;
		break;
		
	  case DAD_S:
		TP = XH() + SP;
		CY = TP < XH();
		H = (uint8_t)(TP >> 8);
		L = (uint8_t)TP;
		break;
		
	  case DAA:
		// TODO:
		break;
		
	  case CMA:
		A = ~A;
		break;
		
	  case CMC:
		CY = ~CY;
		break;
		
	  case STC:
		CY = 1;
		break;
		
	  case RLC:
		CY = A >> 7;
		A = (A << 1) | (A >> 7);
		break;
		
	  case RRC:
		CY = A & 0b00000001;
		A = (A >> 1) | (A << 7);
		break;
	
	  case RAL:
		T = CY;
		CY = A >> 7;
		A = (A << 1) | T;
		break;
		
	  case RAR:
		T = CY;
		CY = A & 0b00000001;
		A = (A >> 1) | (T << 7);
		break;
		
	  case JMP_m:
		PC = readmemx();
		break;
		
	  case JC_m:
		if (CY) {
			PC = readmemx();
		} else { PC += 2; }
		break;
		
	  case JNC_m:
		if (!CY) {
			PC = readmemx();
		} else { PC += 2; }
		break;
		
	  case JP_m:
		if (!SN) {
			PC = readmemx();
		} else { PC += 2; }
		break;
		
	  case JM_m:
		if (SN) {
			PC = readmemx();
		} else { PC += 2; }
		break;
		
	  case JPE_m:
		if (PR) {
			PC = readmemx();
		} else { PC += 2; }
		break;
		
	  case JPO_m:
		if (!PR) {
			PC = readmemx();
		} else { PC += 2; }
		break;
		
	  case JZ_m:
		if (ZR) {
			PC = readmemx();
		} else { PC += 2; }
		break;
		
	  case JNZ_m:
		if (!ZR) {
			PC = readmemx();
		} else { PC += 2; }
		break;
		
	  case CALL_m:
		call(readmemx());
		break;
		
	  case CC_m:
		if (CY) {
			call(readmemx());
		} else { PC += 2; }
		break;
		
	  case CNC_m:
		if (!CY) {
			call(readmemx());
		} else { PC += 2; }
		break;
		
	  case CP_m:
		if (!SN) {
			call(readmemx());
		} else { PC += 2; }
		break;
		
	  case CM_m:
		if (SN) {
			call(readmemx());
		} else { PC += 2; }
		break;
		
	  case CPE_m:
		if (PR) {
			call(readmemx());
		} else { PC += 2; }
		break;
		
	  case CPO_m:
		if (!PR) {
			call(readmemx());
		} else { PC += 2; }
		break;
		
	  case CZ_m:
		if (ZR) {
			call(readmemx());
		} else { PC += 2; }
		break;
		
	  case CNZ_m:
		if (!ZR) {
			call(readmemx());
		} else { PC += 2; }
		break;
		
	  case RET:
		ret();
		break;
		
	  case RC:
		if (CY) {
			ret();
		}
		break;
		
	  case RNC:
		if (!CY) {
			ret();
		}
		break;
		
	  case RP:
		if (!SN) {
			ret();
		}
		break;
		
	  case RM:
		if (SN) {
			ret();
		}
		break;
		
	  case RPE:
		if (PR) {
			ret();
		}
		break;
		
	  case RPO:
		if (!PR) {
			ret();
		}
		break;
		
	  case RZ:
		if (ZR) {
			ret();
		}
		break;
		
	  case RNZ:
		if (!ZR) {
			ret();
		}
		break;
		
	  case PUSH_B:
		SP--;
		MEM[SP] = B;
		SP--;
		MEM[SP] = C;
		break;
		
	  case PUSH_D:
		SP--;
		MEM[SP] = D;
		SP--;
		MEM[SP] = E;
		break;
		
	  case PUSH_H:
		SP--;
		MEM[SP] = H;
		SP--;
		MEM[SP] = L;
		break;
		
	  case PUSH_W:
		SP--;
		MEM[SP] = A;
		SP--;
		MEM[SP] = (SN << 7) | (ZR << 6) | (AC << 4) | (PR << 2) | CY;
		break;
		
	  case POP_B:
		C = MEM[SP];
		SP++;
		B = MEM[SP];
		SP++;
		break;
		
	  case POP_D:
		E = MEM[SP];
		SP++;
		D = MEM[SP];
		SP++;
		break;
		
	  case POP_H:
		L = MEM[SP];
		SP++;
		H = MEM[SP];
		SP++;
		break;

	  case POP_W:
		T = MEM[SP];
		SN = T >> 7;
		ZR = (T & 0b01000000) >> 6;
		AC = (T & 0b00010000) >> 4;
		PR = (T & 0b00000100) >> 2;
		CY = T & 1;
		SP++;
		A = MEM[SP];
		SP++;
		break;
		
	  case XTHL:
		T = L;
		L = MEM[SP];
		MEM[SP] = T;
		T = H;
		H = MEM[SP + 1];
		MEM[SP + 1] = T;
		break;
		
	  case SPHL:
		MEM[SP] = L;
		MEM[SP + 1] = H;
		break;

	  case PCHL:
		PC = XH();
		break;
		
	  case EI:
		debugger_breakpoint();
		break;
		
	}
	
	return 1;
	
}

void load_bin_file(const char* filename) {
	
	FILE* bin = fopen(filename, "rb");
	
	if (!bin) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	struct stat sb;
	if (stat(filename, &sb) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}

	fread(MEM, sb.st_size, 1, bin);
	fclose(bin);
	
}

int main(int argc, char** argv) {
	
	if (argc <= 1) {
		puts("Usage: 8085_emu file.bin");
		exit(EXIT_FAILURE);
	}
	
	load_bin_file(argv[1]);
	
	SP = 0xFFFF;
	
	while (execute()) {
		ui_show();
	}
	
}
 
