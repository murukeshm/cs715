#include "target.h"
#include "ipa-inline.h"
int is_index_reg PARAMS((int REGN));
int  is_base_reg PARAMS((int REGN));
int is_arg_reg PARAMS((int REGN));
int is_return_val_reg PARAMS((int REGN));
int non_strict_index_reg PARAMS((int REGN));
int non_strict_base_reg PARAMS((int regn));
int is_caller_saved_reg PARAMS((int REGN));
int is_callee_saved_reg PARAMS((int REGN));
int is_general_reg PARAMS((int REGN));
void spim_asm_internal_label PARAMS((FILE *stream, const char *prefix, long unsigned int labelno));
void spim_asm_globalize_label PARAMS((FILE *stream, const char *name));
rtx spim_struct_value_rtx PARAMS((tree fndecl, int incoming));
int hard_regno_mode_ok  PARAMS((int REGN, enum machine_mode MODE));
int  modes_tieable_p PARAMS((enum machine_mode MODE1, enum machine_mode MODE2));
enum reg_class regno_reg_class PARAMS((int REGN));
enum reg_class reg_class_from_letter  PARAMS((char ch));
int IITB_regno_ok_for_base_p  PARAMS((int REGN));
int regno_ok_for_index_p  PARAMS((int REGN));
int const_ok_for_letter_p PARAMS((int VALUE,char CH));
int starting_frame_offset  PARAMS((void));
int initial_frame_pointer_offset  PARAMS((int DEPTH));
 int registers_to_be_saved PARAMS((void));
int initial_elimination_offset PARAMS((int from, int to));
rtx function_value  PARAMS((void));
int constant_address_p  PARAMS((rtx X));
int legitimate_address1 PARAMS((enum machine_mode MODE,rtx X));
int legitimate_address2 PARAMS((enum machine_mode MODE,rtx X));
int  reg_ok_for_base_p1 PARAMS((rtx x));
int reg_ok_for_base_p2 PARAMS((rtx x));
int  reg_ok_for_index_p1 PARAMS((rtx x));
int reg_ok_for_index_p2 PARAMS((rtx x));
rtx  legitimize_address PARAMS((rtx X,rtx OLDX, enum machine_mode MODE));
int legitimate_constant_p  PARAMS((rtx X));
void asm_output_align PARAMS((FILE *STREAM, int POWER));
void asm_output_skip PARAMS((FILE  *STREAM,int NBYTES));
void print_operand PARAMS((FILE *STREAM,rtx X,char CODE));
void print_operand_address PARAMS((FILE *STREAM,rtx X));
void asm_generate_internal_label PARAMS((char *STRING,const char *PREFIX,int NUM));
void asm_output_local PARAMS((FILE *STREAM,const char *NAME,int SIZE,int ROUNDED));
void asm_output_common PARAMS((FILE *STREAM,const char *NAME,int SIZE,int ROUNDED));
int asm_output_symbol_ref PARAMS((FILE *stream, rtx sym));
void function_profiler PARAMS((FILE*asm_file,int labelno));
void initialize_trampoline PARAMS((void));
int symbolic_operand  PARAMS((rtx op, enum machine_mode mode));
int constant_operand  PARAMS((rtx op, enum machine_mode mode));
int zero_register_operand  PARAMS((rtx op, enum machine_mode mode));
void spim_prologue PARAMS((void));
void spim_epilogue PARAMS((void));
char* emit_asm_call PARAMS((rtx operands[],int type));
extern rtx compare_op0;
extern rtx compare_op1;

#ifdef RTX_CODE
char* conditional_insn(enum rtx_code code,rtx operands[], int isRev);
#endif
