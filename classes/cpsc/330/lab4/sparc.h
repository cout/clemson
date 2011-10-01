/*================================================
sparc.h
   This header file contains defines that
 abstract the instruction 'labels' from 
 the line number that the instruction occurs
 on in the sparc.ins file from the program
================================================*/

/* Branch condition fields */
#define INSTRC_ba   0x8
#define INSTRC_bne  0x9
#define INSTRC_be   0x1
#define INSTRC_ble  0x2
#define INSTRC_bcc  0xd
#define INSTRC_bcs  0x5
#define INSTRC_bneg 0x6
#define INSTRC_bvc  0xf
#define INSTRC_bvs  0x7
#define INSTRC_bl   0x3

/* Instruction formats */
#define FORMAT1        '1'
#define FORMAT2        '2'
#define FORMAT3        '3'

/* Instruction labels */
#define INSTR_ldsb     'a'
#define INSTR_ldsh     'b'
#define INSTR_ld       'c'
#define INSTR_ldub     'd'
#define INSTR_lduh     'e'
#define INSTR_ldd      'f'
#define INSTR_stb      'g'
#define INSTR_sth      'h'
#define INSTR_st       'i'
#define INSTR_std      'j'
#define INSTR_swap     'k'
#define INSTR_or       'l'
#define INSTR_sethi    'm'
#define INSTR_add      'n'
#define INSTR_addcc    'o'
#define INSTR_addx     'p'
#define INSTR_addxcc   'q'
#define INSTR_sub      'r'
#define INSTR_subx     's'
#define INSTR_mulscc   't'
#define INSTR_and      'u'
#define INSTR_andn     'v'
#define INSTR_orcc     'w'
#define INSTR_orn      'x'
#define INSTR_xor      'y'
#define INSTR_xnor     'z'
#define INSTR_sll      '0'
#define INSTR_srl      '1'
#define INSTR_sra      '2'
#define INSTR_bcc      '4'
#define INSTR_call     '5'
#define INSTR_jmpl     '6'
#define INSTR_save     '7'
#define INSTR_restore  '8'

