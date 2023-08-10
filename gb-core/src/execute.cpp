#include "../include/cpu.h"
#include "../include/opcode.h"
#include "../include/tools.h"

uint8 CPU::_execute(uint16 opCode, InstructionConfig* config) {
	uint8 cycles = config->cycles[0];
	uint16 opStartAd = _regs.PC + 1;
	_regs.PC += config->bytes;
	switch (opCode)
	{
	case NOP:
		break;
	case LD_B_n:
		_regs.s.B = _bus->ReadByte(opStartAd);
		break;
	case LD_C_n:
		_regs.s.C = _bus->ReadByte(opStartAd);
		break;
	case LD_D_n:
		_regs.s.D = _bus->ReadByte(opStartAd);
		break;
	case LD_E_n:
		_regs.s.E = _bus->ReadByte(opStartAd);
		break;
	case LD_H_n:
		_regs.s.H = _bus->ReadByte(opStartAd);
		break;
	case LD_L_n:
		_regs.s.L = _bus->ReadByte(opStartAd);
		break;
	case LD_A_A:
		break;
	case LD_A_B:
		_regs.s.A = _regs.s.B;
		break;
	case LD_A_C:
		_regs.s.A = _regs.s.C;
		break;
	case LD_A_D:
		_regs.s.A = _regs.s.D;
		break;
	case LD_A_E:
		_regs.s.A = _regs.s.E;
		break;
	case LD_A_H:
		_regs.s.A = _regs.s.H;
		break;
	case LD_A_L:
		_regs.s.A = _regs.s.L;
		break;
	case LD_A_aHL:
		_regs.s.A = _bus->ReadByte(_regs.d.HL);
		break;
	case LD_B_B:
		break;
	case LD_B_C:
		_regs.s.B = _regs.s.C;
		break;
	case LD_B_D:
		_regs.s.B = _regs.s.D;
		break;
	case LD_B_E:
		_regs.s.B = _regs.s.E;
		break;
	case LD_B_H:
		_regs.s.B = _regs.s.H;
		break;
	case LD_B_L:
		_regs.s.B = _regs.s.L;
		break;
	case LD_B_aHL:
		_regs.s.B = _bus->ReadByte(_regs.d.HL);
		break;
	case LD_C_B:
		_regs.s.C = _regs.s.B;
		break;
	case LD_C_C:
		break;
	case LD_C_D:
		_regs.s.C = _regs.s.D;
		break;
	case LD_C_E:
		_regs.s.C = _regs.s.E;
		break;
	case LD_C_H:
		_regs.s.C = _regs.s.H;
		break;
	case LD_C_L:
		_regs.s.C = _regs.s.L;
		break;
	case LD_C_aHL:
		_regs.s.C = _bus->ReadByte(_regs.d.HL);
		break;
	case LD_D_B:
		_regs.s.D = _regs.s.B;
		break;
	case LD_D_C:
		_regs.s.D = _regs.s.C;
		break;
	case LD_D_D:
		break;
	case LD_D_E:
		_regs.s.D = _regs.s.E;
		break;
	case LD_D_H:
		_regs.s.D = _regs.s.H;
		break;
	case LD_D_L:
		_regs.s.D = _regs.s.L;
		break;
	case LD_D_aHL:
		_regs.s.D = _bus->ReadByte(_regs.d.HL);
		break;
	case LD_E_B:
		_regs.s.E = _regs.s.B;
		break;
	case LD_E_C:
		_regs.s.E = _regs.s.C;
		break;
	case LD_E_D:
		_regs.s.E = _regs.s.D;
		break;
	case LD_E_E:
		break;
	case LD_E_H:
		_regs.s.E = _regs.s.H;
		break;
	case LD_E_L:
		_regs.s.E = _regs.s.L;
		break;
	case LD_E_aHL:
		_regs.s.E = _bus->ReadByte(_regs.d.HL);
		break;
	case LD_H_B:
		_regs.s.H = _regs.s.B;
		break;
	case LD_H_C:
		_regs.s.H = _regs.s.C;
		break;
	case LD_H_D:
		_regs.s.H = _regs.s.D;
		break;
	case LD_H_E:
		_regs.s.H = _regs.s.E;
		break;
	case LD_H_H:
		break;
	case LD_H_L:
		_regs.s.H = _regs.s.L;
		break;
	case LD_H_aHL:
		_regs.s.H = _bus->ReadByte(_regs.d.HL);
		break;
	case LD_L_B:
		_regs.s.L = _regs.s.B;
		break;
	case LD_L_C:
		_regs.s.L = _regs.s.C;
		break;
	case LD_L_D:
		_regs.s.L = _regs.s.D;
		break;
	case LD_L_E:
		_regs.s.L = _regs.s.E;
		break;
	case LD_L_H:
		_regs.s.L = _regs.s.H;
		break;
	case LD_L_L:
		break;
	case LD_L_aHL:
		_regs.s.L = _bus->ReadByte(_regs.d.HL);
		break;
	case LD_aHL_B:
		_bus->WriteByte(_regs.d.HL, _regs.s.B);
		break;
	case LD_aHL_C:
		_bus->WriteByte(_regs.d.HL, _regs.s.C);
		break;
	case LD_aHL_D:
		_bus->WriteByte(_regs.d.HL, _regs.s.D);
		break;
	case LD_aHL_E:
		_bus->WriteByte(_regs.d.HL, _regs.s.E);
		break;
	case LD_aHL_H:
		_bus->WriteByte(_regs.d.HL, _regs.s.H);
		break;
	case LD_aHL_L:
		_bus->WriteByte(_regs.d.HL, _regs.s.L);
		break;
	case LD_aHL_n:
		_bus->WriteByte(_regs.d.HL, _bus->ReadByte(opStartAd));
		break;
	case LD_A_aBC:
		_regs.s.A = _bus->ReadByte(_regs.d.BC);
		break;
	case LD_A_aDE:
		_regs.s.A = _bus->ReadByte(_regs.d.DE);
		break;
	case LD_A_ann:
		_regs.s.A = _bus->ReadByte(_bus->Read2Byte(opStartAd));
		break;
	case LD_A_n:
		_regs.s.A = _bus->ReadByte(opStartAd);
		break;
	case LD_B_A:
		_regs.s.B = _regs.s.A;
		break;
	case LD_C_A:
		_regs.s.C = _regs.s.A;
		break;
	case LD_D_A:
		_regs.s.D = _regs.s.A;
		break;
	case LD_E_A:
		_regs.s.E = _regs.s.A;
		break;
	case LD_H_A:
		_regs.s.H = _regs.s.A;
		break;
	case LD_L_A:
		_regs.s.L = _regs.s.A;
		break;
	case LD_aBC_A:
		_bus->WriteByte(_regs.d.BC, _regs.s.A);
		break;
	case LD_aDE_A:
		_bus->WriteByte(_regs.d.DE, _regs.s.A);
		break;
	case LD_aHL_A:
		_bus->WriteByte(_regs.d.HL, _regs.s.A);
		break;
	case LD_ann_A:
		_bus->WriteByte(_bus->Read2Byte(opStartAd), _regs.s.A);
		break;
	case LD_A_aC:
		_regs.s.A = _bus->ReadByte(0xFF00 | _regs.s.C);
		break;
	case LD_aC_A:
		_bus->WriteByte(0xFF00 | _regs.s.C, _regs.s.A);
		break;
	case LDD_A_aHL:
		_regs.s.A = _bus->ReadByte(_regs.d.HL--);
		break;
	case LDD_aHL_A:
		_bus->WriteByte(_regs.d.HL--, _regs.s.A);
		break;
	case LDI_A_aHL:
		_regs.s.A = _bus->ReadByte(_regs.d.HL++);
		break;
	case LDI_aHL_A:
		_bus->WriteByte(_regs.d.HL++, _regs.s.A);
		break;
	case LDH_an_A:
		_bus->WriteByte(0xFF00 | _bus->ReadByte(opStartAd), _regs.s.A);
		break;
	case LDH_A_an:
		_regs.s.A = _bus->ReadByte(0xFF00 | _bus->ReadByte(opStartAd));
		break;
	case LD_BC_nn:
		_regs.d.BC = _bus->Read2Byte(opStartAd);
		break;
	case LD_DE_nn:
		_regs.d.DE = _bus->Read2Byte(opStartAd);
		break;
	case LD_HL_nn:
		_regs.d.HL = _bus->Read2Byte(opStartAd);
		break;
	case LD_SP_nn:
		_regs.SP = _bus->Read2Byte(opStartAd);
		break;
	case LD_SP_HL:
		_regs.SP = _regs.d.HL;
		break;
	case LDHL_SP_n:
		// ��ȻSP��16λ�Ĵ������������Ӧ�ð���8λ�Ĺ���
		// https://stackoverflow.com/questions/5159603/gbz80-how-does-ld-hl-spe-affect-h-and-c-flags
		_regs.d.HL = _regs.SP + (int8)_bus->ReadByte(opStartAd);
		_add((uint8)(_regs.SP & 0xFF), _bus->ReadByte(opStartAd));
		_setFlag(RegFlags::Z, 0);
		_setFlag(RegFlags::N, 0);
		break;
	case LD_ann_SP:
		_bus->Write2Byte(_bus->Read2Byte(opStartAd), _regs.SP);
		break;
	case PUSH_AF:
		_push16(_regs.d.AF);
		break;
	case PUSH_BC:
		_push16(_regs.d.BC);
		break;
	case PUSH_DE:
		_push16(_regs.d.DE);
		break;
	case PUSH_HL:
		_push16(_regs.d.HL);
		break;
	case POP_AF:
		_regs.d.AF = _pop16();
		_regs.s.F &= 0xF0;
		break;
	case POP_BC:
		_regs.d.BC = _pop16();
		break;
	case POP_DE:
		_regs.d.DE = _pop16();
		break;
	case POP_HL:
		_regs.d.HL = _pop16();
		break;
	case ADD_A_A:
		_regs.s.A = _add(_regs.s.A, _regs.s.A);
		break;
	case ADD_A_B:
		_regs.s.A = _add(_regs.s.A, _regs.s.B);
		break;
	case ADD_A_C:
		_regs.s.A = _add(_regs.s.A, _regs.s.C);
		break;
	case ADD_A_D:
		_regs.s.A = _add(_regs.s.A, _regs.s.D);
		break;
	case ADD_A_E:
		_regs.s.A = _add(_regs.s.A, _regs.s.E);
		break;
	case ADD_A_H:
		_regs.s.A = _add(_regs.s.A, _regs.s.H);
		break;
	case ADD_A_L:
		_regs.s.A = _add(_regs.s.A, _regs.s.L);
		break;
	case ADD_A_aHL:
		_regs.s.A = _add(_regs.s.A, _bus->ReadByte(_regs.d.HL));
		break;
	case ADD_A_n:
		_regs.s.A = _add(_regs.s.A, _bus->ReadByte(opStartAd));
		break;
	case ADC_A_A:
		_regs.s.A = _adc(_regs.s.A, _regs.s.A);
		break;
	case ADC_A_B:
		_regs.s.A = _adc(_regs.s.A, _regs.s.B);
		break;
	case ADC_A_C:
		_regs.s.A = _adc(_regs.s.A, _regs.s.C);
		break;
	case ADC_A_D:
		_regs.s.A = _adc(_regs.s.A, _regs.s.D);
		break;
	case ADC_A_E:
		_regs.s.A = _adc(_regs.s.A, _regs.s.E);
		break;
	case ADC_A_H:
		_regs.s.A = _adc(_regs.s.A, _regs.s.H);
		break;
	case ADC_A_L:
		_regs.s.A = _adc(_regs.s.A, _regs.s.L);
		break;
	case ADC_A_aHL:
		_regs.s.A = _adc(_regs.s.A, _bus->ReadByte(_regs.d.HL));
		break;
	case ADC_A_n:
		_regs.s.A = _adc(_regs.s.A, _bus->ReadByte(opStartAd));
		break;
	case SUB_A_A:
		_regs.s.A = _sub(_regs.s.A, _regs.s.A);
		break;
	case SUB_A_B:
		_regs.s.A = _sub(_regs.s.A, _regs.s.B);
		break;
	case SUB_A_C:
		_regs.s.A = _sub(_regs.s.A, _regs.s.C);
		break;
	case SUB_A_D:
		_regs.s.A = _sub(_regs.s.A, _regs.s.D);
		break;
	case SUB_A_E:
		_regs.s.A = _sub(_regs.s.A, _regs.s.E);
		break;
	case SUB_A_H:
		_regs.s.A = _sub(_regs.s.A, _regs.s.H);
		break;
	case SUB_A_L:
		_regs.s.A = _sub(_regs.s.A, _regs.s.L);
		break;
	case SUB_A_aHL:
		_regs.s.A = _sub(_regs.s.A, _bus->ReadByte(_regs.d.HL));
		break;
	case SUB_A_n:
		_regs.s.A = _sub(_regs.s.A, _bus->ReadByte(opStartAd));
		break;
	case SBC_A_A:
		_regs.s.A = _sbc(_regs.s.A, _regs.s.A);
		break;
	case SBC_A_B:
		_regs.s.A = _sbc(_regs.s.A, _regs.s.B);
		break;
	case SBC_A_C:
		_regs.s.A = _sbc(_regs.s.A, _regs.s.C);
		break;
	case SBC_A_D:
		_regs.s.A = _sbc(_regs.s.A, _regs.s.D);
		break;
	case SBC_A_E:
		_regs.s.A = _sbc(_regs.s.A, _regs.s.E);
		break;
	case SBC_A_H:
		_regs.s.A = _sbc(_regs.s.A, _regs.s.H);
		break;
	case SBC_A_L:
		_regs.s.A = _sbc(_regs.s.A, _regs.s.L);
		break;
	case SBC_A_aHL:
		_regs.s.A = _sbc(_regs.s.A, _bus->ReadByte(_regs.d.HL));
		break;
	case SBC_A_n:
		_regs.s.A = _sbc(_regs.s.A, _bus->ReadByte(opStartAd));
		break;
	case AND_A_A:
		_regs.s.A = _and(_regs.s.A, _regs.s.A);
		break;
	case AND_A_B:
		_regs.s.A = _and(_regs.s.A, _regs.s.B);
		break;
	case AND_A_C:
		_regs.s.A = _and(_regs.s.A, _regs.s.C);
		break;
	case AND_A_D:
		_regs.s.A = _and(_regs.s.A, _regs.s.D);
		break;
	case AND_A_E:
		_regs.s.A = _and(_regs.s.A, _regs.s.E);
		break;
	case AND_A_H:
		_regs.s.A = _and(_regs.s.A, _regs.s.H);
		break;
	case AND_A_L:
		_regs.s.A = _and(_regs.s.A, _regs.s.L);
		break;
	case AND_A_aHL:
		_regs.s.A = _and(_regs.s.A, _bus->ReadByte(_regs.d.HL));
		break;
	case AND_A_n:
		_regs.s.A = _and(_regs.s.A, _bus->ReadByte(opStartAd));
		break;
	case OR_A_A:
		_regs.s.A = _or(_regs.s.A, _regs.s.A);
		break;
	case OR_A_B:
		_regs.s.A = _or(_regs.s.A, _regs.s.B);
		break;
	case OR_A_C:
		_regs.s.A = _or(_regs.s.A, _regs.s.C);
		break;
	case OR_A_D:
		_regs.s.A = _or(_regs.s.A, _regs.s.D);
		break;
	case OR_A_E:
		_regs.s.A = _or(_regs.s.A, _regs.s.E);
		break;
	case OR_A_H:
		_regs.s.A = _or(_regs.s.A, _regs.s.H);
		break;
	case OR_A_L:
		_regs.s.A = _or(_regs.s.A, _regs.s.L);
		break;
	case OR_A_aHL:
		_regs.s.A = _or(_regs.s.A, _bus->ReadByte(_regs.d.HL));
		break;
	case OR_A_n:
		_regs.s.A = _or(_regs.s.A, _bus->ReadByte(opStartAd));
		break;
	case XOR_A_A:
		_regs.s.A = _xor(_regs.s.A, _regs.s.A);
		break;
	case XOR_A_B:
		_regs.s.A = _xor(_regs.s.A, _regs.s.B);
		break;
	case XOR_A_C:
		_regs.s.A = _xor(_regs.s.A, _regs.s.C);
		break;
	case XOR_A_D:
		_regs.s.A = _xor(_regs.s.A, _regs.s.D);
		break;
	case XOR_A_E:
		_regs.s.A = _xor(_regs.s.A, _regs.s.E);
		break;
	case XOR_A_H:
		_regs.s.A = _xor(_regs.s.A, _regs.s.H);
		break;
	case XOR_A_L:
		_regs.s.A = _xor(_regs.s.A, _regs.s.L);
		break;
	case XOR_A_aHL:
		_regs.s.A = _xor(_regs.s.A, _bus->ReadByte(_regs.d.HL));
		break;
	case XOR_A_n:
		_regs.s.A = _xor(_regs.s.A, _bus->ReadByte(opStartAd));
		break;
	case CP_A_A:
		_sub(_regs.s.A, _regs.s.A);
		break;
	case CP_A_B:
		_sub(_regs.s.A, _regs.s.B);
		break;
	case CP_A_C:
		_sub(_regs.s.A, _regs.s.C);
		break;
	case CP_A_D:
		_sub(_regs.s.A, _regs.s.D);
		break;
	case CP_A_E:
		_sub(_regs.s.A, _regs.s.E);
		break;
	case CP_A_H:
		_sub(_regs.s.A, _regs.s.H);
		break;
	case CP_A_L:
		_sub(_regs.s.A, _regs.s.L);
		break;
	case CP_A_aHL:
		_sub(_regs.s.A, _bus->ReadByte(_regs.d.HL));
		break;
	case CP_A_n:
		_sub(_regs.s.A, _bus->ReadByte(opStartAd));
		break;
	case INC_A:
		_regs.s.A = (uint8)_increase(_regs.s.A);
		break;
	case INC_B:
		_regs.s.B = (uint8)_increase(_regs.s.B);
		break;
	case INC_C:
		_regs.s.C = (uint8)_increase(_regs.s.C);
		break;
	case INC_D:
		_regs.s.D = (uint8)_increase(_regs.s.D);
		break;
	case INC_E:
		_regs.s.E = (uint8)_increase(_regs.s.E);
		break;
	case INC_H:
		_regs.s.H = (uint8)_increase(_regs.s.H);
		break;
	case INC_L:
		_regs.s.L = (uint8)_increase(_regs.s.L);
		break;
	case INC_aHL:
		_bus->WriteByte(_regs.d.HL, (uint8)_increase(_bus->ReadByte(_regs.d.HL)));
		break;
	case DEC_A:
		_regs.s.A = (uint8)_decrease(_regs.s.A);
		break;
	case DEC_B:
		_regs.s.B = (uint8)_decrease(_regs.s.B);
		break;
	case DEC_C:
		_regs.s.C = (uint8)_decrease(_regs.s.C);
		break;
	case DEC_D:
		_regs.s.D = (uint8)_decrease(_regs.s.D);
		break;
	case DEC_E:
		_regs.s.E = (uint8)_decrease(_regs.s.E);
		break;
	case DEC_H:
		_regs.s.H = (uint8)_decrease(_regs.s.H);
		break;
	case DEC_L:
		_regs.s.L = (uint8)_decrease(_regs.s.L);
		break;
	case DEC_aHL:
		_bus->WriteByte(_regs.d.HL, (uint8)_decrease(_bus->ReadByte(_regs.d.HL)));
		break;
	case ADD_HL_BC:
		_regs.d.HL = _add16(_regs.d.HL, _regs.d.BC);
		break;
	case ADD_HL_DE:
		_regs.d.HL = _add16(_regs.d.HL, _regs.d.DE);
		break;
	case ADD_HL_HL:
		_regs.d.HL = _add16(_regs.d.HL, _regs.d.HL);
		break;
	case ADD_HL_SP:
		_regs.d.HL = _add16(_regs.d.HL, _regs.SP);
		break;
	case ADD_SP_n:
		_add_sp_n((int8)_bus->ReadByte(opStartAd));
		break;
	case INC_BC:
		_regs.d.BC++;
		break;
	case INC_DE:
		_regs.d.DE++;
		break;
	case INC_HL:
		_regs.d.HL++;
		break;
	case INC_SP:
		_regs.SP++;
		break;
	case DEC_BC:
		_regs.d.BC--;
		break;
	case DEC_DE:
		_regs.d.DE--;
		break;
	case DEC_HL:
		_regs.d.HL--;
		break;
	case DEC_SP:
		_regs.SP--;
		break;
	case SWAP_A:
		_regs.s.A = _swap(_regs.s.A);
		break;
	case SWAP_B:
		_regs.s.B = _swap(_regs.s.B);
		break;
	case SWAP_C:
		_regs.s.C = _swap(_regs.s.C);
		break;
	case SWAP_D:
		_regs.s.D = _swap(_regs.s.D);
		break;
	case SWAP_E:
		_regs.s.E = _swap(_regs.s.E);
		break;
	case SWAP_H:
		_regs.s.H = _swap(_regs.s.H);
		break;
	case SWAP_L:
		_regs.s.L = _swap(_regs.s.L);
		break;
	case SWAP_aHL:
		_bus->WriteByte(_regs.d.HL, _swap(_bus->ReadByte(_regs.d.HL)));
		break;
	case DAA_A:
		_regs.s.A = _daa(_regs.s.A);
		break;
	case CPL_A:
		_regs.s.A = _cpl(_regs.s.A);
		break;
	case CCF:
		_ccf();
		break;
	case SCF:
		_scf();
		break;
	case HALT:
		//_ime = 1;
		_halted = true;
		break;
	case STOP:
		_bus->WriteByte(MEM_DIV, 0);
		break;
	case RLCA_A:
		_regs.s.A = _rlca(_regs.s.A);
		break;
	case RLA_A:
		_regs.s.A = _rla(_regs.s.A);
		break;
	case RRCA_A:
		_regs.s.A = _rrca(_regs.s.A);
		break;
	case RRA_A:
		_regs.s.A = _rra(_regs.s.A);
		break;
	case RLC_A:
		_regs.s.A = _rlc(_regs.s.A);
		break;
	case RLC_B:
		_regs.s.B = _rlc(_regs.s.B);
		break;
	case RLC_C:
		_regs.s.C = _rlc(_regs.s.C);
		break;
	case RLC_D:
		_regs.s.D = _rlc(_regs.s.D);
		break;
	case RLC_E:
		_regs.s.E = _rlc(_regs.s.E);
		break;
	case RLC_H:
		_regs.s.H = _rlc(_regs.s.H);
		break;
	case RLC_L:
		_regs.s.L = _rlc(_regs.s.L);
		break;
	case RLC_aHL:
		_bus->WriteByte(_regs.d.HL, _rlc(_bus->ReadByte(_regs.d.HL)));
		break;
	case RL_A:
		_regs.s.A = _rl(_regs.s.A);
		break;
	case RL_B:
		_regs.s.B = _rl(_regs.s.B);
		break;
	case RL_C:
		_regs.s.C = _rl(_regs.s.C);
		break;
	case RL_D:
		_regs.s.D = _rl(_regs.s.D);
		break;
	case RL_E:
		_regs.s.E = _rl(_regs.s.E);
		break;
	case RL_H:
		_regs.s.H = _rl(_regs.s.H);
		break;
	case RL_L:
		_regs.s.L = _rl(_regs.s.L);
		break;
	case RL_aHL:
		_bus->WriteByte(_regs.d.HL, _rl(_bus->ReadByte(_regs.d.HL)));
		break;
	case RRC_A:
		_regs.s.A = _rrc(_regs.s.A);
		break;
	case RRC_B:
		_regs.s.B = _rrc(_regs.s.B);
		break;
	case RRC_C:
		_regs.s.C = _rrc(_regs.s.C);
		break;
	case RRC_D:
		_regs.s.D = _rrc(_regs.s.D);
		break;
	case RRC_E:
		_regs.s.E = _rrc(_regs.s.E);
		break;
	case RRC_H:
		_regs.s.H = _rrc(_regs.s.H);
		break;
	case RRC_L:
		_regs.s.L = _rrc(_regs.s.L);
		break;
	case RRC_aHL:
		_bus->WriteByte(_regs.d.HL, _rrc(_bus->ReadByte(_regs.d.HL)));
		break;
	case RR_A:
		_regs.s.A = _rr(_regs.s.A);
		break;
	case RR_B:
		_regs.s.B = _rr(_regs.s.B);
		break;
	case RR_C:
		_regs.s.C = _rr(_regs.s.C);
		break;
	case RR_D:
		_regs.s.D = _rr(_regs.s.D);
		break;
	case RR_E:
		_regs.s.E = _rr(_regs.s.E);
		break;
	case RR_H:
		_regs.s.H = _rr(_regs.s.H);
		break;
	case RR_L:
		_regs.s.L = _rr(_regs.s.L);
		break;
	case RR_aHL:
		_bus->WriteByte(_regs.d.HL, _rr(_bus->ReadByte(_regs.d.HL)));
		break;
	case SLA_A:
		_regs.s.A = _sla(_regs.s.A);
		break;
	case SLA_B:
		_regs.s.B = _sla(_regs.s.B);
		break;
	case SLA_C:
		_regs.s.C = _sla(_regs.s.C);
		break;
	case SLA_D:
		_regs.s.D = _sla(_regs.s.D);
		break;
	case SLA_E:
		_regs.s.E = _sla(_regs.s.E);
		break;
	case SLA_H:
		_regs.s.H = _sla(_regs.s.H);
		break;
	case SLA_L:
		_regs.s.L = _sla(_regs.s.L);
		break;
	case SLA_aHL:
		_bus->WriteByte(_regs.d.HL, _sla(_bus->ReadByte(_regs.d.HL)));
		break;
	case SRA_A:
		_regs.s.A = _sra(_regs.s.A);
		break;
	case SRA_B:
		_regs.s.B = _sra(_regs.s.B);
		break;
	case SRA_C:
		_regs.s.C = _sra(_regs.s.C);
		break;
	case SRA_D:
		_regs.s.D = _sra(_regs.s.D);
		break;
	case SRA_E:
		_regs.s.E = _sra(_regs.s.E);
		break;
	case SRA_H:
		_regs.s.H = _sra(_regs.s.H);
		break;
	case SRA_L:
		_regs.s.L = _sra(_regs.s.L);
		break;
	case SRA_aHL:
		_bus->WriteByte(_regs.d.HL, _sra(_bus->ReadByte(_regs.d.HL)));
		break;
	case SRL_A:
		_regs.s.A = _srl(_regs.s.A);
		break;
	case SRL_B:
		_regs.s.B = _srl(_regs.s.B);
		break;
	case SRL_C:
		_regs.s.C = _srl(_regs.s.C);
		break;
	case SRL_D:
		_regs.s.D = _srl(_regs.s.D);
		break;
	case SRL_E:
		_regs.s.E = _srl(_regs.s.E);
		break;
	case SRL_H:
		_regs.s.H = _srl(_regs.s.H);
		break;
	case SRL_L:
		_regs.s.L = _srl(_regs.s.L);
		break;
	case SRL_aHL:
		_bus->WriteByte(_regs.d.HL, _srl(_bus->ReadByte(_regs.d.HL)));
		break;
	case BIT_A:
		_bit(_regs.s.A, (opCode >> 3) & 7);
		break;
	case BIT_B:
		_bit(_regs.s.B, (opCode >> 3) & 7);
		break;
	case BIT_C:
		_bit(_regs.s.C, (opCode >> 3) & 7);
		break;
	case BIT_D:
		_bit(_regs.s.D, (opCode >> 3) & 7);
		break;
	case BIT_E:
		_bit(_regs.s.E, (opCode >> 3) & 7);
		break;
	case BIT_H:
		_bit(_regs.s.H, (opCode >> 3) & 7);
		break;
	case BIT_L:
		_bit(_regs.s.L, (opCode >> 3) & 7);
		break;
	case BIT_aHL:
		_bit(_bus->ReadByte(_regs.d.HL), (opCode >> 3) & 7);
		break;
	case BIT_1_aHL:
		_bit(_bus->ReadByte(_regs.d.HL), 1);
		break;
	case BIT_2_aHL:
		_bit(_bus->ReadByte(_regs.d.HL), 2);
		break;
	case BIT_3_aHL:
		_bit(_bus->ReadByte(_regs.d.HL), 3);
		break;
	case BIT_4_aHL:
		_bit(_bus->ReadByte(_regs.d.HL), 4);
		break;
	case BIT_5_aHL:
		_bit(_bus->ReadByte(_regs.d.HL), 5);
		break;
	case BIT_6_aHL:
		_bit(_bus->ReadByte(_regs.d.HL), 6);
		break;
	case BIT_7_aHL:
		_bit(_bus->ReadByte(_regs.d.HL), 7);
		break;
	case BIT_1_B:
		_bit(_regs.s.B, 1);
		break;
	case BIT_1_C:
		_bit(_regs.s.C, 1);
		break;
	case BIT_1_D:
		_bit(_regs.s.D, 1);
		break;
	case BIT_1_E:
		_bit(_regs.s.E, 1);
		break;
	case BIT_1_H:
		_bit(_regs.s.H, 1);
		break;
	case BIT_1_L:
		_bit(_regs.s.L, 1);
		break;
	case BIT_1_A:
		_bit(_regs.s.A, 1);
		break;
	case BIT_2_B:
		_bit(_regs.s.B, 2);
		break;
	case BIT_2_C:
		_bit(_regs.s.C, 2);
		break;
	case BIT_2_D:
		_bit(_regs.s.D, 2);
		break;
	case BIT_2_E:
		_bit(_regs.s.E, 2);
		break;
	case BIT_2_H:
		_bit(_regs.s.H, 2);
		break;
	case BIT_2_L:
		_bit(_regs.s.L, 2);
		break;
	case BIT_2_A:
		_bit(_regs.s.A, 2);
		break;
	case BIT_3_B:
		_bit(_regs.s.B, 3);
		break;
	case BIT_3_C:
		_bit(_regs.s.C, 3);
		break;
	case BIT_3_D:
		_bit(_regs.s.D, 3);
		break;
	case BIT_3_E:
		_bit(_regs.s.E, 3);
		break;
	case BIT_3_H:
		_bit(_regs.s.H, 3);
		break;
	case BIT_3_L:
		_bit(_regs.s.L, 3);
		break;
	case BIT_3_A:
		_bit(_regs.s.A, 3);
		break;
	case BIT_4_B:
		_bit(_regs.s.B, 4);
		break;
	case BIT_4_C:
		_bit(_regs.s.C, 4);
		break;
	case BIT_4_D:
		_bit(_regs.s.D, 4);
		break;
	case BIT_4_E:
		_bit(_regs.s.E, 4);
		break;
	case BIT_4_H:
		_bit(_regs.s.H, 4);
		break;
	case BIT_4_L:
		_bit(_regs.s.L, 4);
		break;
	case BIT_4_A:
		_bit(_regs.s.A, 4);
		break;
	case BIT_5_B:
		_bit(_regs.s.B, 5);
		break;
	case BIT_5_C:
		_bit(_regs.s.C, 5);
		break;
	case BIT_5_D:
		_bit(_regs.s.D, 5);
		break;
	case BIT_5_E:
		_bit(_regs.s.E, 5);
		break;
	case BIT_5_H:
		_bit(_regs.s.H, 5);
		break;
	case BIT_5_L:
		_bit(_regs.s.L, 5);
		break;
	case BIT_5_A:
		_bit(_regs.s.A, 5);
		break;
	case BIT_6_B:
		_bit(_regs.s.B, 6);
		break;
	case BIT_6_C:
		_bit(_regs.s.C, 6);
		break;
	case BIT_6_D:
		_bit(_regs.s.D, 6);
		break;
	case BIT_6_E:
		_bit(_regs.s.E, 6);
		break;
	case BIT_6_H:
		_bit(_regs.s.H, 6);
		break;
	case BIT_6_L:
		_bit(_regs.s.L, 6);
		break;
	case BIT_6_A:
		_bit(_regs.s.A, 6);
		break;
	case BIT_7_B:
		_bit(_regs.s.B, 7);
		break;
	case BIT_7_C:
		_bit(_regs.s.C, 7);
		break;
	case BIT_7_D:
		_bit(_regs.s.D, 7);
		break;
	case BIT_7_E:
		_bit(_regs.s.E, 7);
		break;
	case BIT_7_H:
		_bit(_regs.s.H, 7);
		break;
	case BIT_7_L:
		_bit(_regs.s.L, 7);
		break;
	case BIT_7_A:
		_bit(_regs.s.A, 7);
		break;
	case SET_A:
		_regs.s.A = _set(_regs.s.A, (opCode >> 3) & 7);
		break;
	case SET_B:
		_regs.s.B = _set(_regs.s.B, (opCode >> 3) & 7);
		break;
	case SET_C:
		_regs.s.C = _set(_regs.s.C, (opCode >> 3) & 7);
		break;
	case SET_D:
		_regs.s.D = _set(_regs.s.D, (opCode >> 3) & 7);
		break;
	case SET_E:
		_regs.s.E = _set(_regs.s.E, (opCode >> 3) & 7);
		break;
	case SET_H:
		_regs.s.H = _set(_regs.s.H, (opCode >> 3) & 7);
		break;
	case SET_L:
		_regs.s.L = _set(_regs.s.L, (opCode >> 3) & 7);
		break;
	case SET_aHL:
		_bus->WriteByte(_regs.d.HL, _set(_bus->ReadByte(_regs.d.HL), (opCode >> 3) & 7));
		break;
	case SET_1_aHL:
		_bus->WriteByte(_regs.d.HL, _set(_bus->ReadByte(_regs.d.HL), 1));
		break;
	case SET_2_aHL:
		_bus->WriteByte(_regs.d.HL, _set(_bus->ReadByte(_regs.d.HL), 2));
		break;
	case SET_3_aHL:
		_bus->WriteByte(_regs.d.HL, _set(_bus->ReadByte(_regs.d.HL), 3));
		break;
	case SET_4_aHL:
		_bus->WriteByte(_regs.d.HL, _set(_bus->ReadByte(_regs.d.HL), 4));
		break;
	case SET_5_aHL:
		_bus->WriteByte(_regs.d.HL, _set(_bus->ReadByte(_regs.d.HL), 5));
		break;
	case SET_6_aHL:
		_bus->WriteByte(_regs.d.HL, _set(_bus->ReadByte(_regs.d.HL), 6));
		break;
	case SET_7_aHL:
		_bus->WriteByte(_regs.d.HL, _set(_bus->ReadByte(_regs.d.HL), 7));
		break;
	case SET_1_B:
		_regs.s.B = _set(_regs.s.B, 1);
		break;
	case SET_1_C:
		_regs.s.C = _set(_regs.s.C, 1);
		break;
	case SET_1_D:
		_regs.s.D = _set(_regs.s.D, 1);
		break;
	case SET_1_E:
		_regs.s.E = _set(_regs.s.E, 1);
		break;
	case SET_1_H:
		_regs.s.H = _set(_regs.s.H, 1);
		break;
	case SET_1_L:
		_regs.s.L = _set(_regs.s.L, 1);
		break;
	case SET_1_A:
		_regs.s.A = _set(_regs.s.A, 1);
		break;
	case SET_2_B:
		_regs.s.B = _set(_regs.s.B, 2);
		break;
	case SET_2_C:
		_regs.s.C = _set(_regs.s.C, 2);
		break;
	case SET_2_D:
		_regs.s.D = _set(_regs.s.D, 2);
		break;
	case SET_2_E:
		_regs.s.E = _set(_regs.s.E, 2);
		break;
	case SET_2_H:
		_regs.s.H = _set(_regs.s.H, 2);
		break;
	case SET_2_L:
		_regs.s.L = _set(_regs.s.L, 2);
		break;
	case SET_2_A:
		_regs.s.A = _set(_regs.s.A, 2);
		break;
	case SET_3_B:
		_regs.s.B = _set(_regs.s.B, 3);
		break;
	case SET_3_C:
		_regs.s.C = _set(_regs.s.C, 3);
		break;
	case SET_3_D:
		_regs.s.D = _set(_regs.s.D, 3);
		break;
	case SET_3_E:
		_regs.s.E = _set(_regs.s.E, 3);
		break;
	case SET_3_H:
		_regs.s.H = _set(_regs.s.H, 3);
		break;
	case SET_3_L:
		_regs.s.L = _set(_regs.s.L, 3);
		break;
	case SET_3_A:
		_regs.s.A = _set(_regs.s.A, 3);
		break;
	case SET_4_B:
		_regs.s.B = _set(_regs.s.B, 4);
		break;
	case SET_4_C:
		_regs.s.C = _set(_regs.s.C, 4);
		break;
	case SET_4_D:
		_regs.s.D = _set(_regs.s.D, 4);
		break;
	case SET_4_E:
		_regs.s.E = _set(_regs.s.E, 4);
		break;
	case SET_4_H:
		_regs.s.H = _set(_regs.s.H, 4);
		break;
	case SET_4_L:
		_regs.s.L = _set(_regs.s.L, 4);
		break;
	case SET_4_A:
		_regs.s.A = _set(_regs.s.A, 4);
		break;
	case SET_5_B:
		_regs.s.B = _set(_regs.s.B, 5);
		break;
	case SET_5_C:
		_regs.s.C = _set(_regs.s.C, 5);
		break;
	case SET_5_D:
		_regs.s.D = _set(_regs.s.D, 5);
		break;
	case SET_5_E:
		_regs.s.E = _set(_regs.s.E, 5);
		break;
	case SET_5_H:
		_regs.s.H = _set(_regs.s.H, 5);
		break;
	case SET_5_L:
		_regs.s.L = _set(_regs.s.L, 5);
		break;
	case SET_5_A:
		_regs.s.A = _set(_regs.s.A, 5);
		break;
	case SET_6_B:
		_regs.s.B = _set(_regs.s.B, 6);
		break;
	case SET_6_C:
		_regs.s.C = _set(_regs.s.C, 6);
		break;
	case SET_6_D:
		_regs.s.D = _set(_regs.s.D, 6);
		break;
	case SET_6_E:
		_regs.s.E = _set(_regs.s.E, 6);
		break;
	case SET_6_H:
		_regs.s.H = _set(_regs.s.H, 6);
		break;
	case SET_6_L:
		_regs.s.L = _set(_regs.s.L, 6);
		break;
	case SET_6_A:
		_regs.s.A = _set(_regs.s.A, 6);
		break;
	case SET_7_B:
		_regs.s.B = _set(_regs.s.B, 7);
		break;
	case SET_7_C:
		_regs.s.C = _set(_regs.s.C, 7);
		break;
	case SET_7_D:
		_regs.s.D = _set(_regs.s.D, 7);
		break;
	case SET_7_E:
		_regs.s.E = _set(_regs.s.E, 7);
		break;
	case SET_7_H:
		_regs.s.H = _set(_regs.s.H, 7);
		break;
	case SET_7_L:
		_regs.s.L = _set(_regs.s.L, 7);
		break;
	case SET_7_A:
		_regs.s.A = _set(_regs.s.A, 7);
		break;
	case RES_A:
		_regs.s.A = _res(_regs.s.A, (opCode >> 3) & 7);
		break;
	case RES_B:
		_regs.s.B = _res(_regs.s.B, (opCode >> 3) & 7);
		break;
	case RES_C:
		_regs.s.C = _res(_regs.s.C, (opCode >> 3) & 7);
		break;
	case RES_D:
		_regs.s.D = _res(_regs.s.D, (opCode >> 3) & 7);
		break;
	case RES_E:
		_regs.s.E = _res(_regs.s.E, (opCode >> 3) & 7);
		break;
	case RES_H:
		_regs.s.H = _res(_regs.s.H, (opCode >> 3) & 7);
		break;
	case RES_L:
		_regs.s.L = _res(_regs.s.L, (opCode >> 3) & 7);
		break;
	case RES_aHL:
		_bus->WriteByte(_regs.d.HL, _res(_bus->ReadByte(_regs.d.HL), (opCode >> 3) & 7));
		break;
	case RES_1_aHL:
		_bus->WriteByte(_regs.d.HL, _res(_bus->ReadByte(_regs.d.HL), 1));
		break;
	case RES_2_aHL:
		_bus->WriteByte(_regs.d.HL, _res(_bus->ReadByte(_regs.d.HL), 2));
		break;
	case RES_3_aHL:
		_bus->WriteByte(_regs.d.HL, _res(_bus->ReadByte(_regs.d.HL), 3));
		break;
	case RES_4_aHL:
		_bus->WriteByte(_regs.d.HL, _res(_bus->ReadByte(_regs.d.HL), 4));
		break;
	case RES_5_aHL:
		_bus->WriteByte(_regs.d.HL, _res(_bus->ReadByte(_regs.d.HL), 5));
		break;
	case RES_6_aHL:
		_bus->WriteByte(_regs.d.HL, _res(_bus->ReadByte(_regs.d.HL), 6));
		break;
	case RES_7_aHL:
		_bus->WriteByte(_regs.d.HL, _res(_bus->ReadByte(_regs.d.HL), 7));
		break;
	case RES_1_B:
		_regs.s.B = _res(_regs.s.B, 1);
		break;
	case RES_1_C:
		_regs.s.C = _res(_regs.s.C, 1);
		break;
	case RES_1_D:
		_regs.s.D = _res(_regs.s.D, 1);
		break;
	case RES_1_E:
		_regs.s.E = _res(_regs.s.E, 1);
		break;
	case RES_1_H:
		_regs.s.H = _res(_regs.s.H, 1);
		break;
	case RES_1_L:
		_regs.s.L = _res(_regs.s.L, 1);
		break;
	case RES_1_A:
		_regs.s.A = _res(_regs.s.A, 1);
		break;
	case RES_2_B:
		_regs.s.B = _res(_regs.s.B, 2);
		break;
	case RES_2_C:
		_regs.s.C = _res(_regs.s.C, 2);
		break;
	case RES_2_D:
		_regs.s.D = _res(_regs.s.D, 2);
		break;
	case RES_2_E:
		_regs.s.E = _res(_regs.s.E, 2);
		break;
	case RES_2_H:
		_regs.s.H = _res(_regs.s.H, 2);
		break;
	case RES_2_L:
		_regs.s.L = _res(_regs.s.L, 2);
		break;
	case RES_2_A:
		_regs.s.A = _res(_regs.s.A, 2);
		break;
	case RES_3_B:
		_regs.s.B = _res(_regs.s.B, 3);
		break;
	case RES_3_C:
		_regs.s.C = _res(_regs.s.C, 3);
		break;
	case RES_3_D:
		_regs.s.D = _res(_regs.s.D, 3);
		break;
	case RES_3_E:
		_regs.s.E = _res(_regs.s.E, 3);
		break;
	case RES_3_H:
		_regs.s.H = _res(_regs.s.H, 3);
		break;
	case RES_3_L:
		_regs.s.L = _res(_regs.s.L, 3);
		break;
	case RES_3_A:
		_regs.s.A = _res(_regs.s.A, 3);
		break;
	case RES_4_B:
		_regs.s.B = _res(_regs.s.B, 4);
		break;
	case RES_4_C:
		_regs.s.C = _res(_regs.s.C, 4);
		break;
	case RES_4_D:
		_regs.s.D = _res(_regs.s.D, 4);
		break;
	case RES_4_E:
		_regs.s.E = _res(_regs.s.E, 4);
		break;
	case RES_4_H:
		_regs.s.H = _res(_regs.s.H, 4);
		break;
	case RES_4_L:
		_regs.s.L = _res(_regs.s.L, 4);
		break;
	case RES_4_A:
		_regs.s.A = _res(_regs.s.A, 4);
		break;
	case RES_5_B:
		_regs.s.B = _res(_regs.s.B, 5);
		break;
	case RES_5_C:
		_regs.s.C = _res(_regs.s.C, 5);
		break;
	case RES_5_D:
		_regs.s.D = _res(_regs.s.D, 5);
		break;
	case RES_5_E:
		_regs.s.E = _res(_regs.s.E, 5);
		break;
	case RES_5_H:
		_regs.s.H = _res(_regs.s.H, 5);
		break;
	case RES_5_L:
		_regs.s.L = _res(_regs.s.L, 5);
		break;
	case RES_5_A:
		_regs.s.A = _res(_regs.s.A, 5);
		break;
	case RES_6_B:
		_regs.s.B = _res(_regs.s.B, 6);
		break;
	case RES_6_C:
		_regs.s.C = _res(_regs.s.C, 6);
		break;
	case RES_6_D:
		_regs.s.D = _res(_regs.s.D, 6);
		break;
	case RES_6_E:
		_regs.s.E = _res(_regs.s.E, 6);
		break;
	case RES_6_H:
		_regs.s.H = _res(_regs.s.H, 6);
		break;
	case RES_6_L:
		_regs.s.L = _res(_regs.s.L, 6);
		break;
	case RES_6_A:
		_regs.s.A = _res(_regs.s.A, 6);
		break;
	case RES_7_B:
		_regs.s.B = _res(_regs.s.B, 7);
		break;
	case RES_7_C:
		_regs.s.C = _res(_regs.s.C, 7);
		break;
	case RES_7_D:
		_regs.s.D = _res(_regs.s.D, 7);
		break;
	case RES_7_E:
		_regs.s.E = _res(_regs.s.E, 7);
		break;
	case RES_7_H:
		_regs.s.H = _res(_regs.s.H, 7);
		break;
	case RES_7_L:
		_regs.s.L = _res(_regs.s.L, 7);
		break;
	case RES_7_A:
		_regs.s.A = _res(_regs.s.A, 7);
		break;
	case JP_nn:
		_regs.PC = _bus->Read2Byte(opStartAd);
		break;
	case JP_NZ_nn:
		if (_jp(RegFlags::Z, 0, _bus->Read2Byte(opStartAd))) {
			cycles = 16;
		}
		else {
			cycles = 12;
		}
		break;
	case JP_Z_nn:
		if (_jp(RegFlags::Z, 1, _bus->Read2Byte(opStartAd))) {
			cycles = 16;
		}
		else {
			cycles = 12;
		}
		break;
	case JP_NC_nn:
		if (_jp(RegFlags::C, 0, _bus->Read2Byte(opStartAd))) {
			cycles = 16;
		}
		else {
			cycles = 12;
		}
		break;
	case JP_C_nn:
		if (_jp(RegFlags::C, 1, _bus->Read2Byte(opStartAd))) {
			cycles = 16;
		}
		else {
			cycles = 12;
		}
		break;
	case JP_HL:
		_regs.PC = _regs.d.HL;
		break;
	case JR_n:
		_regs.PC += (int8)_bus->ReadByte(opStartAd);
		break;
	case JR_NZ_n:
		if (_jr(RegFlags::Z, 0, _bus->ReadByte(opStartAd))) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	case JR_Z_n:
		if (_jr(RegFlags::Z, 1, _bus->ReadByte(opStartAd))) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	case JR_NC_n:
		if (_jr(RegFlags::C, 0, _bus->ReadByte(opStartAd))) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	case JR_C_n:
		if (_jr(RegFlags::C, 1, _bus->ReadByte(opStartAd))) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	case CALL_nn:
		_push16(_regs.PC);
		_regs.PC = _bus->Read2Byte(opStartAd);
		break;
	case CALL_NZ_nn:
		if (_call(RegFlags::Z, 0, _bus->Read2Byte(opStartAd))) {
			cycles = 24;
		}
		else {
			cycles = 12;
		}
		break;
	case CALL_Z_nn:
		if (_call(RegFlags::Z, 1, _bus->Read2Byte(opStartAd))) {
			cycles = 24;
		}
		else {
			cycles = 12;
		}
		break;
	case CALL_NC_nn:
		if (_call(RegFlags::C, 0, _bus->Read2Byte(opStartAd))) {
			cycles = 24;
		}
		else {
			cycles = 12;
		}
		break;
	case CALL_C_nn:
		if (_call(RegFlags::C, 1, _bus->Read2Byte(opStartAd))) {
			cycles = 24;
		}
		else {
			cycles = 12;
		}
		break;
	case RST_00H:
		_rst(_regs.PC, 0x00);
		break;
	case RST_08H:
		_rst(_regs.PC, 0x08);
		break;
	case RST_10H:
		_rst(_regs.PC, 0x10);
		break;
	case RST_18H:
		_rst(_regs.PC, 0x18);
		break;
	case RST_20H:
		_rst(_regs.PC, 0x20);
		break;
	case RST_28H:
		_rst(_regs.PC, 0x28);
		break;
	case RST_30H:
		_rst(_regs.PC, 0x30);
		break;
	case RST_38H:
		_rst(_regs.PC, 0x38);
		break;
	case RET:
		_regs.PC = _pop16();
		break;
	case RET_NZ:
		if (_ret(RegFlags::Z, 0)) {
			cycles = 20;
		}
		else {
			cycles = 8;
		}
		break;
	case RET_Z:
		if (_ret(RegFlags::Z, 1)) {
			cycles = 20;
		}
		else {
			cycles = 8;
		}
		break;
	case RET_NC:
		if (_ret(RegFlags::C, 0)) {
			cycles = 20;
		}
		else {
			cycles = 8;
		}
		break;
	case RET_C:
		if (_ret(RegFlags::C, 1)) {
			cycles = 20;
		}
		else {
			cycles = 8;
		}
		break;
	case DI:
		_enableIMEDelay = 0xFF;
		_ime = 0;
		break;
	case EI:
		_enableIMEDelay = 2;
		break;
	case RETI:
		_regs.PC = _pop16();
		_ime = 1;
		break;
	default:
		string str = format("opCode 0x%x %s is not implemented.\n", opStartAd - 1, config->mnemonic.c_str());
		log(str.c_str());
	}
	return cycles;
}
