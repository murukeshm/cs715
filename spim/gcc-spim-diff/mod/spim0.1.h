/* 	     Some of these macros are deprecated. They are marked by D 		 */
/* ------------------------------------------------------------------------------*
 * 			Data Sizes, Layout and Memory                            *
 * ------------------------------------------------------------------------------*/
#define BITS_BIG_ENDIAN \
0

#define BYTES_BIG_ENDIAN \
0

#define WORDS_BIG_ENDIAN \
0

#define UNITS_PER_WORD \
4

#define PARM_BOUNDARY \
32

#define STACK_BOUNDARY \
64

#define FUNCTION_BOUNDARY \
32

#define BIGGEST_ALIGNMENT \
64

#define STRICT_ALIGNMENT \
0

#define MOVE_MAX       /* Essentially size of the data bus */ \
4

#define Pmode SImode

#define FUNCTION_MODE SImode

#define SLOW_BYTE_ACCESS 0

#define CASE_VECTOR_MODE SImode

/* ------------------------------------------------------------------------------*
 * 			Registers and their usage conventions 			 *
 * ------------------------------------------------------------------------------*/
/* We might need to add few internal registers like status register, HI,
 * LO registers,  FP registers. But  not at  this stage. Will  add those
 * only when  they are required.  Currently adding only  those registers
 * which are there  in SPIM. The registers for FPU  and co-proc0 will be
 * added, as per requirement.*/

#define FIRST_PSEUDO_REGISTER \
32

#define FIXED_REGISTERS /* Even the registers which are not available */ \
			/* are marked fixed so that they are not      */ \
			/* considered for register allocation.        */ \
{1,1,0,0, \
 0,0,0,0, \
 0,0,0,0, \
 0,0,0,0, \
 0,0,0,0, \
 0,0,0,0, \
 0,0,1,1, \
 1,1,1,1}

#define CALL_USED_REGISTERS /* Caller-saved registers. This is irrelevant */ \
                            /* here because this version does not support */ \
			    /* function calls. We will make this more     */ \
			    /* precise later.                             */ \
{1,1,1,1, \
 1,1,1,1, \
 1,1,1,1, \
 1,1,1,1, \
 1,1,1,1, \
 1,1,1,1, \
 1,1,1,1, \
 1,1,1,1}

/* Number of consecutive hard registers required to hold 
 * value of given mode, starting from register REGNO. */
#define HARD_REGNO_NREGS(REGNO, MODE) \
((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1)  \
               / UNITS_PER_WORD)

/* The following macro returns 1 if a  value of mode MODE can be held in
 * register REGNO. If the mode is  double, it checks for register number
 * and allows only if register has  even number, else returns 0. This is
 * because for double  values, register with even number  is paired with
 * the succeeding  odd numbered  register. For  single integer  mode, it
 * allows all registers.*/
#define HARD_REGNO_MODE_OK(REGN, MODE) \
hard_regno_mode_ok (REGN, MODE)

/* This  macro defines  if object  of mode2  can be  moved in  object of
 * mode1. If the modes are same or they belong to same class (eg. int or
 * float) and mode2 has size less than mode1, then we allow the move.*/
#define MODES_TIEABLE_P(MODE1, MODE2) \
modes_tieable_p (MODE1,MODE2)
	
/* The  additional  classes   defined  here  are  CALLER_SAVED_REGISTER,
 * CALLEE_SAVED_REGISTER  and BASE_REGISTER.  Others are  pre-defined by
 * GCC. */
enum reg_class \
{\
	NO_REGS,\
	CALLER_SAVED_REGS,\
	CALLEE_SAVED_REGS,\
	BASE_REGS,\
	GENERAL_REGS,\
	ALL_REGS,\
	LIM_REG_CLASSES \
   };

#define N_REG_CLASSES LIM_REG_CLASSES

#define REG_CLASS_NAMES \
{\
	"NO_REGS",\
	"CALLER_SAVED_REGS",\
	"CALLEE_SAVED_REGS",\
	"BASE_REGS",\
	"GEN_REGS",\
	"ALL_REGS"\
}

#define REG_CLASS_CONTENTS  /* We have included un-available registers also */ \
                            /* because in this level, it is irrelevant      */ \
{0x00000000,0x0200ff00,0x00ff0000,0xf2fffffc,0xffffffff,0xffffffff}


#define REGNO_REG_CLASS(REGNO) \
regno_reg_class(REGNO)

#define BASE_REG_CLASS \
BASE_REGS

#define INDEX_REG_CLASS \
NO_REGS

/* Some  instructions  may  contrain  the set  of  acceptable  registers
 * further through contraints like "r", "g" etc. in md files. Since this
 * is irrelevant  at this level,  currently no constraint  character has
 * been defined for  various register classes and for  any character, we
 * return matched class to be NO_REGS. */
#define REG_CLASS_FROM_LETTER(ch)\
NO_REGS

/* Currently we assume any register can be used as base register. But in
 * later levels,  we will define  the registers acording  to appropriate
 * register class.*/
#define REGNO_OK_FOR_BASE_P(REGNO)\
1

/* Spim does not support indexed addressing mode. */
#define REGNO_OK_FOR_INDEX_P(REGNO)\
0
	
#define PREFERRED_RELOAD_CLASS(X, CLASS) \
CLASS

/* This is closely  related to the macro  HARD_REGNO_NREGS. It specifies
 * the maximum number  of consecutive registers of  class CLASS required
 * for  holding  a  value of  mode  MODE.  In  fact,  the value  of  the
 * macro CLASS_MAX_NREGS  (class, mode) should  be the maximum  value of
 * HARD_REGNO_NREGS  (regno, mode)  for all  regno values  in the  class
 * CLASS. */
#define CLASS_MAX_NREGS(CLASS, MODE) \
((GET_MODE_SIZE(MODE)+UNITS_PER_WORD-1)/UNITS_PER_WORD)

/* ------------------------------------------------------------------------------*
 * 			Activation Record and Calling Conventions                *
 * ------------------------------------------------------------------------------*/

/* All activation record and  function calling convention related issues
 * are  assigned dummy  values.  In  later levels,  we  will assign  the
 * correct values as per stack design. */

#define STACK_GROWS_DOWNWARD 1

#define FRAME_GROWS_DOWNWARD 1

#define STARTING_FRAME_OFFSET \
0

#define STACK_POINTER_OFFSET \
0

#define FIRST_PARM_OFFSET(FUN)\
0

#define STACK_POINTER_REGNUM \
29

#define FRAME_POINTER_REGNUM \
30

#define ARG_POINTER_REGNUM \
FRAME_POINTER_REGNUM


#define INITIAL_FRAME_POINTER_OFFSET(DEPTH)\
DEPTH=0

#define ACCUMULATE_OUTGOING_ARGS \
0


/* Currently, arguments  are passed  on stack. Irrelevant  because there
 * are no function calls in this level. */
#ifdef _M_USE_POISONED
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
0
#endif

#define FUNCTION_ARG_REGNO_P(r) /* Irrelevant in this level */ \
0

/* Type  of data  structure to  record the  information about  arguments
 * passed in  registers. Irrelevant in this  level so a simple  int will
 * do. */
#define CUMULATIVE_ARGS \
int

#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, FNDECL, NAMED_ARGS)	\
{\
CUM = 0;\
}

#ifdef _M_USE_POISONED
#define FUNCTION_ARG_ADVANCE(cum, mode, type, named) \
cum++
#endif

#define FUNCTION_VALUE(valtype, func)\
function_value()

#define LIBCALL_VALUE(MODE) \
function_value()

#define FUNCTION_VALUE_REGNO_P(REGN) \
((REGN) == 2)

/* ------------------------------------------------------------------------------*
 * 			Addressing Mode information                              *
 * ------------------------------------------------------------------------------*/

/* To validate use of labels as symbolic references or numeric addresses */
#define CONSTANT_ADDRESS_P(X) \
constant_address_p(X)

/* Since we don't have base indexed mode, we do not need more than one 
 * register for any address. */
#define MAX_REGS_PER_ADDRESS \
1

/* This macro is  used to validate an  address used in an  insn by using
 * the constraints specified  in the template in the  md (describing the
 * addressing) mode and the constraints on the register classes. This is
 * implemented by transferring control  to appropriate code fragments in
 * source of generated compiler.*/
#ifdef REG_OK_STRICT
#define GO_IF_LEGITIMATE_ADDRESS(mode,x,label) \
{\
if (legitimate_address1(mode,x))\
	goto label;\
}
#else
#define GO_IF_LEGITIMATE_ADDRESS(mode,x,label) \
{\
	if (legitimate_address2(mode,x))\
		        goto label;\
}
#endif
#ifdef REG_OK_STRICT
#define REG_OK_FOR_BASE_P(x) \
reg_ok_for_base_p1(x)
#else
#define REG_OK_FOR_BASE_P(x)  \
reg_ok_for_base_p2(x) 
#endif
	
#ifdef REG_OK_STRICT
#define REG_OK_FOR_INDEX_P(x) \
reg_ok_for_index_p1(x)
#else
#define REG_OK_FOR_INDEX_P(x) \
reg_ok_for_index_p2(x) 
#endif


#ifdef _M_USE_POISONED
#define GO_IF_MODE_DEPENDENT_ADDRESS(addr,label) 
#endif

/* ------------------------------------------------------------------------------*
 * 			Assembly Output Format                                   *
 * ------------------------------------------------------------------------------*/

#define ASM_OUTPUT_ALIGN(STREAM, POWER)                                   \
         asm_output_align(STREAM, POWER)

#define ASM_OUTPUT_SKIP(STREAM, NBYTES)                                   \
         asm_output_skip(STREAM, NBYTES)

#define PRINT_OPERAND(STREAM, X, CODE)                                    \
         print_operand(STREAM, X, CODE)

#define PRINT_OPERAND_ADDRESS(STREAM, X)                                  \
         print_operand_address(STREAM, X)

#define ASM_GENERATE_INTERNAL_LABEL(STRING, PREFIX, NUM)                  \
        asm_generate_internal_label(STRING, PREFIX, NUM)

#define ASM_OUTPUT_LOCAL(STREAM, NAME, SIZE, ROUNDED)                     \
         asm_output_local(STREAM, NAME, SIZE, ROUNDED)

#define ASM_OUTPUT_COMMON(STREAM, NAME, SIZE, ROUNDED)                    \
         asm_output_common(STREAM, NAME, SIZE, ROUNDED)

#define FUNCTION_PROFILER(file,lab) \
function_profiler(file,lab)

#define ASM_APP_ON                                                        \
	"#APP"
#define ASM_APP_OFF                                                       \
	"#NO_APP"


/* Required for for producing assembly output. */
#define REGISTER_NAMES \
{"$zero","$at","$v0","$v1", \
 "$a0","$a1","$a2","$a3", \
 "$t0","$t1","$t2","$t3", \
 "$t4","$t5","$t6","$t7", \
 "$s0","$s1","$s2","$s3", \
 "$s4","$s5","$s6","$s7", \
 "$t8","$t9","$k0","$k1", \
 "$gp","$sp","$fp","$ra", \
}


#define TEXT_SECTION_ASM_OP                                               \
"\t.text"

#define DATA_SECTION_ASM_OP                                               \
"\t.data 0x10000000"
		
#define ASM_OUTPUT_LABELREF(stream, name) \
	fprintf(stream,"%s",name);

/* ------------------------------------------------------------------------------*
 * 			Misc 
 * ------------------------------------------------------------------------------*/
#define DEFAULT_SIGNED_CHAR \
0

/* In  this level,  since  we  are not  supporting  any addressing  mode
 * involving  offsets (in  fact  we are  not  supporting ANY  addressing
 * mode), no  constant value is  defined. Later  on we will  define this
 * macro depending  upon constant  values permitted in  addressing modes
 * supported.*/
#define CONST_OK_FOR_LETTER_P(VALUE, CH)                                /* D */\
0

/* Double is not supported in level0. */
#define CONST_DOUBLE_OK_FOR_CONSTRAINT_P(op,ch,p)                       /* D */\
0

#define TRULY_NOOP_TRUNCATION(in,out) \
1

/* To support  variants of targets  which can be chosen  through command        
 * line arguments */
//#define TARGET_SWITCHES                                                 /* D */\
 //{"", 0, ""}


#define TARGET_CPU_CPP_BUILTINS()\
do                                            \
  {                                           \
      builtin_define_std ("spim");           \
      builtin_assert ("cpu=spim");	      \
  }                                           \
  while (0)


/* This macro has been defined to eliminate call to __main function from `main'. */
#define HAS_INIT_SECTION

#define TRAMPOLINE_SIZE 32 

#ifdef _M_USE_POISONED
#define LEGITIMATE_CONSTANT_P(x) \
legitimate_constant_p(x)
#endif


