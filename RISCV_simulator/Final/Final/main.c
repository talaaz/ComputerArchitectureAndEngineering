/*
============================================================================
 Name        : sim.c
 Author      : Tala & Kristian
 Copyright   : Your copyright notice
 Description : Instruction Set Simulator
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int PC;
int instr;
int NewPC = 0;
int showhelp = 0;
int dump = 1;
unsigned int R[33];//32
int Instructioncount = 0;
int Instructioncount2 = 0;
unsigned char memory[70000];
uint32_t ResRegCombined[33];
unsigned int opcode, rd, funct3, rs1, rs2, funct7, imm, funct6;

int finalInstructionNo =0;
unsigned char ResReg[4];


void LoadProgram(){
  FILE *fp;
  int line = 0;
  fp = fopen("C:/Users/Martin/Desktop/Elektro/4. semester/Computerarkitektur E19 02155/cae-lab-master/finasgmt/tests/task1/fact.bin", "rb");//read mode
  char file = fgetc(fp);

while(file != EOF){
		for(int i=0;i<=3;i++){
			memory[line] = file;
			file = fgetc(fp);
			line++;
		}
	}
	fclose(fp);
}

void InstructionFetch(){
	instr = memory[PC+3] << 24 | memory[PC+2] << 16 | memory[PC+1] << 8 | memory[PC];
	if(showhelp)printf("\n\nPC: %2d\tInstruction: %8X\t", PC, instr);
}

void DecodeInstruction(){
  opcode = instr & 0x7F;;
  if(showhelp)printf("Opcode: %2X\n", opcode);

  switch(opcode){
  //Format: R-type
    case 0x33:
    if(showhelp)printf("Format:  R-type\t");
    rd = (instr >> 7) & 0x1F;
    funct3 = (instr >> 12) & 0x7;
    rs1 = (instr >> 15) & 0x1F;
    rs2 = (instr >> 20) & 0x1F;
    funct7 = (instr>> 25) & 0x7F;
    break;

  //Format: I-type
    case 0x3:
    case 0x13:
	case 0x67:
    if(showhelp)printf("Format:  I-type\t");
    rd = (instr >> 7) & 0x1F;
    funct3 = (instr >> 12) & 0x7 ;
    rs1 = (instr >> 15) & 0x1F;
    imm = (instr >> 20) & 0xFFF;
	imm |= (signed int)(instr & 0x80000000) >> 20;
    funct6 = (instr >> 26) & 0x3F;
    break;

  //Format: S-type
    case 0x23:
    if(showhelp)printf("Format:  S-type\t");
    imm =  (instr  >> 7) & 0x1F;
    funct3 = (instr >> 12) & 0x7 ;
    rs1 = (instr >> 15) & 0x1F;
    rs2 = (instr >> 20) & 0x1F;
    imm |= (instr >> 20) & 0x7E0;
	imm |= (signed int)(instr & 0x80000000) >> 20;
    break;

  //Format: SB-type
    case 0x63:
    if(showhelp)printf("Format: SB-type\t");
    imm = (instr & 0xF00) >> 7;  //imm[4:1]
    imm |= (instr & 0x80) << 4 ; //imm[11]
    funct3 = (instr >> 12) & 0x7 ;
    rs1 = (instr >> 15) & 0x1F;
    rs2 = (instr >> 20) & 0x1F;
    imm |= (instr & 0x7E000080) >> 20; //imm[10:5]
    imm |= (signed int)(instr & 0x80000000) >> 19; //imm[12]
	if(showhelp)printf("ImmD: %12d\tImmX: %3X\t",imm,imm);

    break;

  //Format: U-type
    case 0x37:
	case 0x17:
    if(showhelp)printf("Format:  U-type\t");
    rd = (instr >> 7) & 0x1F;
    imm = instr & 0x7FFFF000;
	imm |= (signed int)(instr & 0x80000000);
    break;

  //Format: UJ-type
    case 0x6F:  //UJ type
    if(showhelp)printf("Format: UJ-type\t");
    rd = (instr >> 7) & 0x1F;
	imm = instr & 0xFF000;        //imm[19:12]
	imm |= (instr >> 9) & 0x800;     //imm[11]
	imm |= (instr >> 20) & 0x7FE;   //imm[10:1]
    imm |= (signed int)(instr & 0x80000000) >> 11; //imm[20]
	if(showhelp)printf("ImmD: %12d\tImmX: %3X\t",imm,imm);
    break;
  }
}

void ExecuteInstruction(){
  switch(opcode){
  //Format: R-type
    case 0x33:
    // add instruction
    if((funct3 == 0x0) && (funct7 == 0x00)){
      if(showhelp)printf("Function:  add\t");
      R[rd] = R[rs1] + R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    // sub instruction
    if((funct3 == 0x0) && (funct7 == 0x20)){
      if(showhelp)printf("Function:  sub\t");
      R[rd] = R[rs1] - R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    // sll instruction
    if((funct3 == 0x1) && (funct7 == 0x00)){
      if(showhelp)printf("Function:  sll\t");
      R[rd] = R[rs1] << R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    // xor instruction
    if((funct3 == 0x4) && (funct7 == 0x0)){
      if(showhelp)printf("Function:  xor\t");
      R[rd] = R[rs1] ^ R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      //PC++;
      break;
    }
    // srl instruction
    if((funct3 == 0x5) && (funct7 == 0x00)){
      if(showhelp)printf("Function:  srl\t");
      R[rd] = R[rs1] >> R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    // sra instruction
    if((funct3 == 0x5) && (funct7 == 0x20)){
      if(showhelp)printf("Function:  sra\t");
      R[rd] = (signed)R[rs1] >> (signed)R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    //  or instruction
    if((funct3 == 0x6) && (funct7 == 0x00)){
      if(showhelp)printf("Function:   or\t");
      R[rd] = R[rs1] | R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    // and instruction
    if((funct3 == 0x7) && (funct7 == 0x00)){
      if(showhelp)printf("Function:  and\t");
      R[rd] = R[rs1] & R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    //lr.d instruction               //ikke færdig
    if((funct3 == 0b011) && (funct7 == 0b0001000)){
      if(showhelp)printf("Function: lr.d\t");
      R[rd] = R[rs1] << R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    //sc.d instruction               //ikke færdig
    if((funct3 == 0b011) && (funct7 == 0b0001100)){
      if(showhelp)printf("Function: sc.d\t");
      R[rd] = R[rs1] << R[rs2];
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
	//slt instruction
    if((funct3 == 0x2)){
      if(showhelp)printf("Function: slt\t");
	  R[rd] = ((signed)R[rs1] < (signed)imm ? 1:0);
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    //sltu instruction
    if((funct3 == 0x3)){
      if(showhelp)printf("Function: sltu\t");
      R[rd] = (R[rs1] < imm) ? 1:0;
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    if(showhelp)printf("---------------------------Error: no matching instruction--------------------------------\n");

  //Format: I-type
    case 0x3:
    //  lb instruction
    if((funct3 == 0x0)){
      if(showhelp)printf("Function:   lb\t");
	  if((memory[R[rs1] + imm]) >> 7 == 1)
		R[rd] = (memory[R[rs1] + imm]) | 0xFFFFFF00;
	  else
		R[rd] = (memory[R[rs1] + imm]);
	  if(showhelp)printf("imm %d\tMemR[%d]: %d\tR[%d]: %d\t", imm, rs1, R[rs1],rd,R[rd]);
      break;
    }
    //  lh instruction
    if((funct3 == 0x1)){
      if(showhelp)printf("Function:   lh\t");
	  if((memory[R[rs1]+imm+1]) >> 7 == 1)
		R[rd] = (memory[R[rs1] + imm]) | ((memory[R[rs1]+imm+1]) << 8) | 0xFFFF0000;
	  else
		R[rd] = (memory[R[rs1] + imm]) | ((memory[R[rs1]+imm+1]) << 8);
	  if(showhelp)printf("imm %d\tMemR[%d]: %d\tR[%d]: %d\t", imm, rs1, R[rs1],rd,R[rd]);
      break;
    }
    //  lw instruction
    if((funct3 == 0x2)){
		if(showhelp)printf("Function: lw\t");
		//if((memory[R[rs1]+imm+3]) >> 7 == 1)
			R[rd] = (memory[R[rs1] + imm]) | ((memory[R[rs1]+imm+1]) << 8) | ((memory[R[rs1]+imm+2]) << 16) | ((memory[R[rs1]+imm+3]) << 24);
		//else
			//R[rd] = (memory[R[rs1] + imm]) | ((memory[R[rs1]+imm+1]) << 8) | ((memory[R[rs1]+imm+2]) << 16) | ((memory[R[rs1]+imm+3]) << 24);
	  if(showhelp)printf("imm %d\tMemR[%d]: %d\tR[%d]: %d\t", imm, rs1, R[rs1],rd,R[rd]);
      break;
    }
    //  ld instruction
    if((funct3 == 0x3)){
      if(showhelp)printf("Function:   ld\t");
      R[rd] = memory[R[rs1]+ imm];
      break;
    }
    // lbu instruction
    if((funct3 == 0x4)){
      if(showhelp)printf("Function:  lbu\t");
      R[rd] = memory[R[rs1] + imm];
      break;
    }
    // lhu instruction
    if((funct3 == 0x5)){
      if(showhelp)printf("Function:  lhu\t");
      R[rd] = (memory[R[rs1] + imm]) | ((memory[R[rs1]+imm+1]) << 8);
      break;
    }
    // lwu instruction
    if((funct3 == 0x6)){
      if(showhelp)printf("Function:  lwu\t");
	  R[rd] = (memory[R[rs1] + imm]) | ((memory[R[rs1]+imm+1]) << 8) | ((memory[R[rs1]+imm+2]) << 16) | ((memory[R[rs1]+imm+3]) << 24);
      break;
    }
    if(showhelp)printf("---------------------------Error: no matching instruction--------------------------------\n");

    case 0x13:
    //addi instruction
    if((funct3 == 0x0)){
      if(showhelp)printf("Function: addi\t");
      if((instr >> 31) == 1)
        R[rd] = R[rs1] + (imm | 0xFFFFF000);
      else
        R[rd] = R[rs1] + imm;
      if(showhelp)printf("R[%d]: %d\tImm: %d\tR[%d]: %d\t",rs1,R[rs1],imm,rd,R[rd]);
      break;
    }
    //slli instruction
    if((funct3 == 0x1) && (funct6 == 0x00)){
      if(showhelp)printf("Function: slli\t");
	  R[rd] = R[rs1] << (imm & 0x1F);
      if(showhelp)printf("R[%d] << imm = %d << %d = R[%d]: %d\t",rs1,R[rs1],imm,rd,R[rd]);
      break;
    }
    //slti instruction
    if((funct3 == 0x2)){
      if(showhelp)printf("Function: slti\t");
      R[rd] = ((signed)R[rs1] < (signed)imm) ? 1:0;
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    //sltiu instruction
    if((funct3 == 0x3)){
      if(showhelp)printf("Function: sltiu\t");
      R[rd] = (R[rs1] < imm) ? 1:0;
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    //xori instruction
    if((funct3 == 0x4)){
      if(showhelp)printf("Function: xori\t");
      R[rd] = R[rs1] ^ imm;
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    //srli instruction
    if((funct3 == 0x5) && (funct6==0x00)){
      if(showhelp)printf("Function: srli\t");
      R[rd] =  R[rs1] >> imm;
	  if(showhelp)printf("R[%d] >> imm = %d >> %d = R[%d]: %d\t",rs1,R[rs1],imm,rd,R[rd]);
      break;
    }
    //srai instruction
    if((funct3 == 0x5) && (funct6==0x10)){
      if(showhelp)printf("Function: srai\t");
      if(showhelp)printf("imm: %d\t",imm);
      R[rd] = (signed int)R[rs1] >> imm;
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    // ori instruction
    if((funct3 == 0x6)){
      if(showhelp)printf("Function:  ori\t");
      R[rd] = R[rs1] | imm;
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    //andi instruction
    if((funct3 == 0x7)){
      if(showhelp)printf("Function: andi\t");
      R[rd] = R[rs1] & imm;
      if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
      break;
    }
    if(showhelp)printf("---------------------------Error: no matching instruction--------------------------------\n");

    case 0x67:
    //Jalr instruction
    if ((funct3 == 0x0)){
      if(showhelp)printf("Function: Jalr\t");
	  if(rd != 0)
		R[rd] = PC + 4;
	  NewPC = R[rs1] + imm;
      if(showhelp)printf("LinkedregisterR[%d]: %d\tNextPC: %d",rd,R[rd],NewPC);
      break;
    }
    if(showhelp)printf("---------------------------Error: no matching instruction--------------------------------\n");

  //Format: S-type
    case 0x23:
    //  sb instruction
    if((funct3 == 0x0)){
	  if(showhelp)printf("Function:   sb \timm %d\t", imm);
	  if((instr >> 31) == 1)
		imm |= 0xFFFFF000;
      memory[R[rs1] + imm] = R[rs2];
	  if(showhelp)printf("imm %d\tMemR[%d]: %d\tValR[%d]: %d\t", imm, rs1, R[rs1],rs2,R[rs2]);
	  if(showhelp)printf("Memory[%d]: %d\t",R[rs1] + imm,(memory[R[rs1] + imm]));
      break;
    }
    //  sh instruction
    if((funct3 == 0x1)){
	  if(showhelp)printf("Function:   sh \timm %d\t", imm);
	  if((instr >> 31) == 1)
		imm |= 0xFFFFF000;
      memory[R[rs1] + imm]  = R[rs2];
      memory[R[rs1] + imm+1]  = R[rs2] >> 8 ;
	  if(showhelp)printf("imm %d\tMemR[%d]: %d\tValR[%d]: %d\t", imm, rs1, R[rs1],rs2,R[rs2]);
	  if(showhelp)printf("Memory[%d]: %d\t",R[rs1] + imm,(memory[R[rs1] + imm+1] << 8) | memory[R[rs1] + imm]);
      break;
    }
    //  sw instruction
    if((funct3 == 0x2)){
	  if(showhelp)printf("Function:   sw\t");
	    if((instr >> 31) == 1)//imm >> 11 == 1)
	       imm |= 0xFFFFF000;
	    memory[R[rs1] + imm]    = R[rs2];
	    memory[R[rs1] + imm+1]  = R[rs2] >> 8;
	    memory[R[rs1] + imm+2]  = R[rs2] >> 16;
	    memory[R[rs1] + imm+3]  = R[rs2] >> 24;
	  if(showhelp)printf("imm %d\tMemR[%d]: %d\tValR[%d]: %d\t", imm, rs1, R[rs1],rs2,R[rs2]);
	  if(showhelp)printf("Memory[%d]: %d\t",R[rs1] + imm,(memory[R[rs1] + imm+3] << 24) | (memory[R[rs1] + imm+2] << 16) | (memory[R[rs1] + imm+1] << 8) | memory[R[rs1] + imm]);
      break;
    }

    if(showhelp)printf("---------------------------Error: no matching instruction--------------------------------\n");


  //Format: SB-type
    case 0x63:
    // beq instruction
    if((funct3 == 0x0)){
      if(showhelp)printf("Function:  beq\tFunct3: %d\t",funct3);
      if((signed int)R[rs1] == (signed int)R[rs2]){
		NewPC = PC + imm;
		if(showhelp)printf("NewPC: %d\t",NewPC);
      };
      break;
    }
    // bne instruction
    if((funct3 == 0x1)){
      if(showhelp)printf("Function:  bne\tFunct3: %d\t",funct3);
      if((signed int)R[rs1] != (signed int)R[rs2]){
		NewPC = PC + imm;
		if(showhelp)printf("NewPC: %d\t",NewPC);
      };
      break;
    }
    // blt instruction
    if((funct3 == 0x4)){
      if(showhelp)printf("Function:  blt\tFunct3: %d\t", funct3);
      if((signed int)R[rs1] < (signed int)R[rs2]){
		NewPC = PC + imm;
      };
	  if(showhelp)printf("NewPC: %d\t",NewPC);
      break;
    }
    // bge instruction
    if((funct3 == 0x5)){
      if(showhelp)printf("Function:  bge\t");
	  if((signed int)R[rs1] >= (signed int)R[rs2])
		NewPC = PC + imm;
	  if(showhelp)printf("NewPC: %d\t",NewPC);
      break;
    }
    //bltu instruction
    if((funct3 == 0x6)){
      if(showhelp)printf("Function: bltu\t");
      if(R[rs1] < R[rs2]){
		NewPC = PC + imm;
		if(showhelp)printf("NewPC: %d\t",NewPC);
      };
      break;
    }
    //bgeu instruction
    if((funct3 == 0x7)){
      if(showhelp)printf("Function: bgeu\t");
      if(R[rs1] >= R[rs2]){
		NewPC = PC + imm;
		if(showhelp)printf("NewPC: %d\t",NewPC);
      };
      break;
    }
    if(showhelp)printf("---------------------------Error: no matching instruction--------------------------------\n");

  //Format: U-type
    case 0x37:
    // lui instruction
    if(showhelp)printf("Function:  lui\t");
	R[rd] = imm;
    if(showhelp)printf("Imm: %d\tR[%d]: %d\t",imm,rd,R[rd]);
    break;

	case 0x17:
	// auipc instruction
    if(showhelp)printf("Function:  auipc\t");
	R[rd] = PC + imm;
    if(showhelp)printf("Imm: %d\tR[%d]: %d\t",imm,rd,R[rd]);
    break;

  //Format: UJ-type
    case 0x6F:  //UJ type
    // Jal instruction
    if(showhelp)printf("Function:  Jal\t");
	if(rd != 0)
		R[rd] = PC + 4;
	if((instr >> 31) == 1)
		imm |= 0xFFE00000;
    NewPC = PC + imm;
	if(showhelp)printf("Imm: %d\t",imm);
	if(showhelp)printf("R[%d]: %d\t",rd,R[rd]);
    break;
  }
  //if(showhelp)printf("---------------------------Error: no matching instruction--------------------------------\n");
}

void LoadRes(){
  FILE *fp;
  fp = fopen("C:/Users/Martin/Desktop/Elektro/4. semester/Computerarkitektur E19 02155/cae-lab-master/finasgmt/tests/task1/dump.res", "rb");//read mode
  char fileContent = fgetc(fp);
  int lineNo = 0;

  //fread(ResRegCombined, sizeof(ResRegCombined), 1, rp);
  while( fileContent != EOF){
  		for(int i = 0 ; i <= 3 ; i++){
  			ResReg[i] = fileContent;
			fileContent = fgetc(fp);
  		}
		ResRegCombined[lineNo] = (ResReg[3] << 24) | (ResReg[2] << 16) | (ResReg[1] << 8) | ResReg[0];
		lineNo++;
  }
  finalInstructionNo = lineNo;
}

void PrintResults(){
	if(showhelp)printf("\n");
	for(int i = 0 ; i <= 32 ; i++){
		printf("Register[%2d] %10d",i, R[i]);
		if(showhelp){
			printf("\t%10d",ResRegCombined[i]);
			if(R[i] != ResRegCombined[i])
				printf("\tfalse");
		}
		printf("\n");
  }
}

void CreateDump(){
  FILE *wp;
  wp = fopen("C:/Users/Martin/Desktop/Elektro/4. semester/Computerarkitektur E19 02155/cae-lab-master/finasgmt/tests/task1/dump.res", "wb");//read mode
  fwrite(R,sizeof(R),1,wp);
	fclose(wp);
}

int main(){
  if(showhelp)printf("Hello World\n");
  LoadProgram();
  for(int i=0;(instr != 0x00000073)/* && (i <= 80)*/;i++){
	PC = NewPC;
	NewPC += 4;
	InstructionFetch();
    DecodeInstruction();
    ExecuteInstruction();
	if(showhelp)printf("instrukser kørt: %d\t",i);
	Instructioncount++;
  }
  if(dump)
	  CreateDump();
  if(showhelp){
	  LoadRes();
  }
  PrintResults();
  if(showhelp)printf("END\n");
  return 0;
}
