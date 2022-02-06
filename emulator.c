#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "emulator.h"


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


void debugger_breakpoint() {
	
}


int execute() {
	
	switch (readmem()) {
		
		uint16_t TP;
		
		case MOV_A_A:A=A;break;
case MOV_A_B:A=B;break;
case MOV_A_C:A=C;break;
case MOV_A_D:A=D;break;
case MOV_A_E:A=E;break;
case MOV_A_H:A=H;break;
case MOV_A_L:A=L;break;
case MOV_B_A:B=A;break;
case MOV_B_B:B=B;break;
case MOV_B_C:B=C;break;
case MOV_B_D:B=D;break;
case MOV_B_E:B=E;break;
case MOV_B_H:B=H;break;
case MOV_B_L:B=L;break;
case MOV_C_A:C=A;break;
case MOV_C_B:C=B;break;
case MOV_C_C:C=C;break;
case MOV_C_D:C=D;break;
case MOV_C_E:C=E;break;
case MOV_C_H:C=H;break;
case MOV_C_L:C=L;break;
case MOV_D_A:D=A;break;
case MOV_D_B:D=B;break;
case MOV_D_C:D=C;break;
case MOV_D_D:D=D;break;
case MOV_D_E:D=E;break;
case MOV_D_H:D=H;break;
case MOV_D_L:D=L;break;
case MOV_E_A:E=A;break;
case MOV_E_B:E=B;break;
case MOV_E_C:E=C;break;
case MOV_E_D:E=D;break;
case MOV_E_E:E=E;break;
case MOV_E_H:E=H;break;
case MOV_E_L:E=L;break;
case MOV_H_A:H=A;break;
case MOV_H_B:H=B;break;
case MOV_H_C:H=C;break;
case MOV_H_D:H=D;break;
case MOV_H_E:H=E;break;
case MOV_H_H:H=H;break;
case MOV_H_L:H=L;break;
case MOV_L_A:L=A;break;
case MOV_L_B:L=B;break;
case MOV_L_C:L=C;break;
case MOV_L_D:L=D;break;
case MOV_L_E:L=E;break;
case MOV_L_H:L=H;break;
case MOV_L_L:L=L;break;
case MOV_M_A:MEM[XH()]=A;break;
case MOV_A_M:A=MEM[XH()];break;
case MOV_M_B:MEM[XH()]=B;break;
case MOV_B_M:B=MEM[XH()];break;
case MOV_M_C:MEM[XH()]=C;break;
case MOV_C_M:C=MEM[XH()];break;
case MOV_M_D:MEM[XH()]=D;break;
case MOV_D_M:D=MEM[XH()];break;
case MOV_M_E:MEM[XH()]=E;break;
case MOV_E_M:E=MEM[XH()];break;
case MOV_M_H:MEM[XH()]=H;break;
case MOV_H_M:H=MEM[XH()];break;
case MOV_M_L:MEM[XH()]=L;break;
case MOV_L_M:L=MEM[XH()];break;
case MVI_A_d:A=readmem();break;
case MVI_B_d:B=readmem();break;
case MVI_C_d:C=readmem();break;
case MVI_D_d:D=readmem();break;
case MVI_E_d:E=readmem();break;
case MVI_H_d:H=readmem();break;
case MVI_L_d:L=readmem();break;
case MVI_M_d:MEM[XH()]=readmem();break;
case ADD_A:A_add(A);break;
case ADC_A:A_adc(A);break;
case ADD_B:A_add(B);break;
case ADC_B:A_adc(B);break;
case ADD_C:A_add(C);break;
case ADC_C:A_adc(C);break;
case ADD_D:A_add(D);break;
case ADC_D:A_adc(D);break;
case ADD_E:A_add(E);break;
case ADC_E:A_adc(E);break;
case ADD_H:A_add(H);break;
case ADC_H:A_adc(H);break;
case ADD_L:A_add(L);break;
case ADC_L:A_adc(L);break;
case ADD_M:A_add(MEM[XH()]);break;
case ADC_M:A_adc(MEM[XH()]);break;
case ADI_d:A_add(readmem());break;
case ACI_d:A_adc(readmem());break;
case SUB_A:A_sub(A);break;
case SBB_A:A_sbb(A);break;
case SUB_B:A_sub(B);break;
case SBB_B:A_sbb(B);break;
case SUB_C:A_sub(C);break;
case SBB_C:A_sbb(C);break;
case SUB_D:A_sub(D);break;
case SBB_D:A_sbb(D);break;
case SUB_E:A_sub(E);break;
case SBB_E:A_sbb(E);break;
case SUB_H:A_sub(H);break;
case SBB_H:A_sbb(H);break;
case SUB_L:A_sub(L);break;
case SBB_L:A_sbb(L);break;
case SUB_M:A_sub(MEM[XH()]);break;
case SBB_M:A_sbb(MEM[XH()]);break;
case SUI_d:A_sub(readmem());break;
case SBI_d:A_sbb(readmem());break;
case INR_A: A++; flags(A); break;
case DCR_A: A--; flags(A); break;
case INR_B: B++; flags(B); break;
case DCR_B: B--; flags(B); break;
case INR_C: C++; flags(C); break;
case DCR_C: C--; flags(C); break;
case INR_D: D++; flags(D); break;
case DCR_D: D--; flags(D); break;
case INR_E: E++; flags(E); break;
case DCR_E: E--; flags(E); break;
case INR_H: H++; flags(H); break;
case DCR_H: H--; flags(H); break;
case INR_L: L++; flags(L); break;
case DCR_L: L--; flags(L); break;
case INR_M: MEM[XH()]++; flags(MEM[XH()]); break;
case DCR_M: MEM[XH()]--; flags(MEM[XH()]); break;
case ANA_A: A &= A; flags(A); CY = 0; AC = 1; break;
case ORA_A: A |= A; flags(A); CY = 0; AC = 0; break;
case XRA_A: A ^= A; flags(A); CY = 0; AC = 0; break;
case ANA_B: A &= B; flags(A); CY = 0; AC = 1; break;
case ORA_B: A |= B; flags(A); CY = 0; AC = 0; break;
case XRA_B: A ^= B; flags(A); CY = 0; AC = 0; break;
case ANA_C: A &= C; flags(A); CY = 0; AC = 1; break;
case ORA_C: A |= C; flags(A); CY = 0; AC = 0; break;
case XRA_C: A ^= C; flags(A); CY = 0; AC = 0; break;
case ANA_D: A &= D; flags(A); CY = 0; AC = 1; break;
case ORA_D: A |= D; flags(A); CY = 0; AC = 0; break;
case XRA_D: A ^= D; flags(A); CY = 0; AC = 0; break;
case ANA_E: A &= E; flags(A); CY = 0; AC = 1; break;
case ORA_E: A |= E; flags(A); CY = 0; AC = 0; break;
case XRA_E: A ^= E; flags(A); CY = 0; AC = 0; break;
case ANA_H: A &= H; flags(A); CY = 0; AC = 1; break;
case ORA_H: A |= H; flags(A); CY = 0; AC = 0; break;
case XRA_H: A ^= H; flags(A); CY = 0; AC = 0; break;
case ANA_L: A &= L; flags(A); CY = 0; AC = 1; break;
case ORA_L: A |= L; flags(A); CY = 0; AC = 0; break;
case XRA_L: A ^= L; flags(A); CY = 0; AC = 0; break;
case ANA_M: A &= MEM[XH()]; flags(A); CY = 0; AC = 1; break;
case ORA_M: A |= MEM[XH()]; flags(A); CY = 0; AC = 0; break;
case XRA_M: A ^= MEM[XH()]; flags(A); CY = 0; AC = 0; break;
case ANI_d: A &= readmem(); flags(A); CY = 0; AC = 1; break;
case ORI_d: A |= readmem(); flags(A); CY = 0; AC = 0; break;
case XRI_d: A ^= readmem(); flags(A); CY = 0; AC = 0; break;

		case CMP_A:
			if (A < A) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;
		case CMP_B:
			if (A < B) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;
		case CMP_C:
			if (A < C) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;
		case CMP_D:
			if (A < D) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;
		case CMP_E:
			if (A < E) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;
		case CMP_H:
			if (A < H) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;
		case CMP_L:
			if (A < L) {
				CY = 1;
			}
			else if (A == 0) {
				ZR = 1;
			} else {
				CY = 0;
				ZR = 0;
			}
			flags(A);
			break;
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
			break;
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
			break;
		
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
	
	while (execute());
	
}
 
