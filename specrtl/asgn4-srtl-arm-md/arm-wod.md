;;- Machine description for ARM for GNU compiler
;;  Copyright (C) 1991-2013 Free Software Foundation, Inc.
;;  Contributed by Pieter `Tiggr' Schoenmakers (rcpieter@win.tue.nl)
;;  and Martin Simmons (@harleqn.co.uk).
;;  More major hacks by Richard Earnshaw (rearnsha@arm.com).

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

//Pattern#1
;;- See file "rtl.def" for documentation on define_insn, match_*, et. al.


;;---------------------------------------------------------------------------
;; Constants

;; Register numbers -- All machine registers should be defined here
//Pattern#2
(define_constants
  [(R0_REGNUM         0)	; First CORE register
   (R1_REGNUM	      1)	; Second CORE register
   (IP_REGNUM	     12)	; Scratch register
   (SP_REGNUM	     13)	; Stack pointer
   (LR_REGNUM        14)	; Return address register
   (PC_REGNUM	     15)	; Program counter
   (LAST_ARM_REGNUM  15)	;
   (CC_REGNUM       100)	; Condition code pseudo register
   (VFPCC_REGNUM    101)	; VFP Condition code pseudo register
  ]
)
;; 3rd operand to select_dominance_cc_mode
//Pattern#3
(define_constants
  [(DOM_CC_X_AND_Y  0)
   (DOM_CC_NX_OR_Y  1)
   (DOM_CC_X_OR_Y   2)
  ]
)
;; conditional compare combination
//Pattern#4
(define_constants
  [(CMP_CMP 0)
   (CMN_CMP 1)
   (CMP_CMN 2)
   (CMN_CMN 3)
   (NUM_OF_COND_CMP 4)
  ]
)


;;---------------------------------------------------------------------------
;; Attributes

;; Processor type.  This is created automatically from arm-cores.def.
(include "arm-tune.md")

; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#5
(define_attr "is_thumb" "no,yes" (const (symbol_ref "thumb_code")))

; IS_ARCH6 is set to 'yes' when we are generating code form ARMv6.
//Pattern#6
(define_attr "is_arch6" "no,yes" (const (symbol_ref "arm_arch6")))

; IS_THUMB1 is set to 'yes' iff we are generating Thumb-1 code.
//Pattern#7
(define_attr "is_thumb1" "no,yes" (const (symbol_ref "thumb1_code")))

;; Operand number of an input operand that is shifted.  Zero if the
;; given instruction does not shift one of its input operands.
//Pattern#8
(define_attr "shift" "" (const_int 0))

; Floating Point Unit.  If we only have floating point emulation, then there
; is no point in scheduling the floating point insns.  (Well, for best
; performance we should try and group them together).
//Pattern#9
(define_attr "fpu" "none,vfp"
  (const (symbol_ref "arm_fpu_attr")))

; LENGTH of an instruction (in bytes)
//Pattern#10
(define_attr "length" ""
  (const_int 4))

; The architecture which supports the instruction (or alternative).
; This can be "a" for ARM, "t" for either of the Thumbs, "32" for
; TARGET_32BIT, "t1" or "t2" to specify a specific Thumb mode.  "v6"
; for ARM or Thumb-2 with arm_arch6, and nov6 for ARM without
; arm_arch6.  This attribute is used to compute attribute "enabled",
; use type "any" to enable an alternative in all cases.
//Pattern#11
(define_attr "arch" "any,a,t,32,t1,t2,v6,nov6,onlya8,neon_onlya8,nota8,neon_nota8,iwmmxt,iwmmxt2"
  (const_string "any"))

//Pattern#12
(define_attr "arch_enabled" "no,yes"
  (cond [(eq_attr "arch" "any")
	 (const_string "yes")

	 (and (eq_attr "arch" "a")
	      (match_test "TARGET_ARM"))
	 (const_string "yes")

	 (and (eq_attr "arch" "t")
	      (match_test "TARGET_THUMB"))
	 (const_string "yes")

	 (and (eq_attr "arch" "t1")
	      (match_test "TARGET_THUMB1"))
	 (const_string "yes")

	 (and (eq_attr "arch" "t2")
	      (match_test "TARGET_THUMB2"))
	 (const_string "yes")

	 (and (eq_attr "arch" "32")
	      (match_test "TARGET_32BIT"))
	 (const_string "yes")

	 (and (eq_attr "arch" "v6")
	      (match_test "TARGET_32BIT && arm_arch6"))
	 (const_string "yes")

	 (and (eq_attr "arch" "nov6")
	      (match_test "TARGET_32BIT && !arm_arch6"))
	 (const_string "yes")

	 (and (eq_attr "arch" "onlya8")
	      (eq_attr "tune" "cortexa8"))
	 (const_string "yes")

	 (and (eq_attr "arch" "neon_onlya8")
	      (eq_attr "tune" "cortexa8")
	      (match_test "TARGET_NEON"))
	 (const_string "yes")

	 (and (eq_attr "arch" "nota8")
	      (not (eq_attr "tune" "cortexa8")))
	 (const_string "yes")

	 (and (eq_attr "arch" "neon_nota8")
	      (not (eq_attr "tune" "cortexa8"))
	      (match_test "TARGET_NEON"))
	 (const_string "yes")

	 (and (eq_attr "arch" "iwmmxt2")
	      (match_test "TARGET_REALLY_IWMMXT2"))
	 (const_string "yes")]

	(const_string "no")))

//Pattern#13
(define_attr "opt" "any,speed,size"
  (const_string "any"))

//Pattern#14
(define_attr "opt_enabled" "no,yes"
  (cond [(eq_attr "opt" "any")
         (const_string "yes")

	 (and (eq_attr "opt" "speed")
	      (match_test "optimize_function_for_speed_p (cfun)"))
	 (const_string "yes")

	 (and (eq_attr "opt" "size")
	      (match_test "optimize_function_for_size_p (cfun)"))
	 (const_string "yes")]
	(const_string "no")))

; Allows an insn to disable certain alternatives for reasons other than
; arch support.
//Pattern#15
(define_attr "insn_enabled" "no,yes"
  (const_string "yes"))

; Enable all alternatives that are both arch_enabled and insn_enabled.
//Pattern#16
 (define_attr "enabled" "no,yes"
   (cond [(eq_attr "insn_enabled" "no")
	  (const_string "no")

	  (eq_attr "arch_enabled" "no")
	  (const_string "no")

	  (eq_attr "opt_enabled" "no")
	  (const_string "no")]
	 (const_string "yes")))

; POOL_RANGE is how far away from a constant pool entry that this insn
; can be placed.  If the distance is zero, then this insn will never
; reference the pool.
; Note that for Thumb constant pools the PC value is rounded down to the
; nearest multiple of four.  Therefore, THUMB2_POOL_RANGE (and POOL_RANGE for
; Thumb insns) should be set to <max_range> - 2.
; NEG_POOL_RANGE is nonzero for insns that can reference a constant pool entry
; before its address.  It is set to <max_range> - (8 + <data_size>).
//Pattern#17
(define_attr "arm_pool_range" "" (const_int 0))
//Pattern#18
(define_attr "thumb2_pool_range" "" (const_int 0))
//Pattern#19
(define_attr "arm_neg_pool_range" "" (const_int 0))
//Pattern#20
(define_attr "thumb2_neg_pool_range" "" (const_int 0))

//Pattern#21
(define_attr "pool_range" ""
  (cond [(eq_attr "is_thumb" "yes") (attr "thumb2_pool_range")]
	(attr "arm_pool_range")))
//Pattern#22
(define_attr "neg_pool_range" ""
  (cond [(eq_attr "is_thumb" "yes") (attr "thumb2_neg_pool_range")]
	(attr "arm_neg_pool_range")))

; An assembler sequence may clobber the condition codes without us knowing.
; If such an insn references the pool, then we have no way of knowing how,
; so use the most conservative value for pool_range.
//Pattern#23
(define_asm_attributes
 [(set_attr "conds" "clob")
  (set_attr "length" "4")
  (set_attr "pool_range" "250")])

;; The instruction used to implement a particular pattern.  This
;; information is used by pipeline descriptions to provide accurate
;; scheduling information.

//Pattern#24
(define_attr "insn"
        "mov,mvn,smulxy,smlaxy,smlalxy,smulwy,smlawx,mul,muls,mla,mlas,umull,umulls,umlal,umlals,smull,smulls,smlal,smlals,smlawy,smuad,smuadx,smlad,smladx,smusd,smusdx,smlsd,smlsdx,smmul,smmulr,smmla,umaal,smlald,smlsld,clz,mrs,msr,xtab,sdiv,udiv,sat,other"
        (const_string "other"))

; TYPE attribute is used to detect floating point instructions which, if
; running on a co-processor can run in parallel with other, basic instructions
; If write-buffer scheduling is enabled then it can also be used in the
; scheduling of writes.

; Classification of each insn
; Note: vfp.md has different meanings for some of these, and some further
; types as well.  See that file for details.
; simple_alu_imm  a simple alu instruction that doesn't hit memory or fp
;               regs or have a shifted source operand and has an immediate
;               operand. This currently only tracks very basic immediate
;               alu operations.
; alu_reg       any alu instruction that doesn't hit memory or fp
;               regs or have a shifted source operand
;               and does not have an immediate operand. This is
;               also the default
; simple_alu_shift covers UXTH, UXTB, SXTH, SXTB
; alu_shift	any data instruction that doesn't hit memory or fp
;		regs, but has a source operand shifted by a constant
; alu_shift_reg	any data instruction that doesn't hit memory or fp
;		regs, but has a source operand shifted by a register value
; mult		a multiply instruction
; block		blockage insn, this blocks all functional units
; float		a floating point arithmetic operation (subject to expansion)
; fdivd		DFmode floating point division
; fdivs		SFmode floating point division
; f_load[sd]	A single/double load from memory. Used for VFP unit.
; f_store[sd]	A single/double store to memory. Used for VFP unit.
; f_flag	a transfer of co-processor flags to the CPSR
; f_2_r		transfer float to core (no memory needed)
; r_2_f		transfer core to float
; f_cvt		convert floating<->integral
; branch	a branch
; call		a subroutine call
; load_byte	load byte(s) from memory to arm registers
; load1		load 1 word from memory to arm registers
; load2         load 2 words from memory to arm registers
; load3         load 3 words from memory to arm registers
; load4         load 4 words from memory to arm registers
; store		store 1 word to memory from arm registers
; store2	store 2 words
; store3	store 3 words
; store4	store 4 (or more) words
;

//Pattern#25
(define_attr "type"
 "simple_alu_imm,\
  alu_reg,\
  simple_alu_shift,\
  alu_shift,\
  alu_shift_reg,\
  mult,\
  block,\
  float,\
  fdivd,\
  fdivs,\
  fmuls,\
  fmuld,\
  fmacs,\
  fmacd,\
  ffmas,\
  ffmad,\
  f_rints,\
  f_rintd,\
  f_minmaxs,\
  f_minmaxd,\
  f_flag,\
  f_loads,\
  f_loadd,\
  f_stores,\
  f_stored,\
  f_2_r,\
  r_2_f,\
  f_cvt,\
  branch,\
  call,\
  load_byte,\
  load1,\
  load2,\
  load3,\
  load4,\
  store1,\
  store2,\
  store3,\
  store4,\
  fconsts,\
  fconstd,\
  fadds,\
  faddd,\
  ffariths,\
  ffarithd,\
  fcmps,\
  fcmpd,\
  fcpys"
 (if_then_else 
    (eq_attr "insn" "smulxy,smlaxy,smlalxy,smulwy,smlawx,mul,muls,mla,mlas,\
	     	     umull,umulls,umlal,umlals,smull,smulls,smlal,smlals")
    (const_string "mult")
    (const_string "alu_reg")))

; Is this an (integer side) multiply with a 64-bit result?
//Pattern#26
(define_attr "mul64" "no,yes"
  (if_then_else
    (eq_attr "insn"
     "smlalxy,umull,umulls,umlal,umlals,smull,smulls,smlal,smlals")
    (const_string "yes")
    (const_string "no")))

; wtype for WMMX insn scheduling purposes.
//Pattern#27
(define_attr "wtype"
        "none,wor,wxor,wand,wandn,wmov,tmcrr,tmrrc,wldr,wstr,tmcr,tmrc,wadd,wsub,wmul,wmac,wavg2,tinsr,textrm,wshufh,wcmpeq,wcmpgt,wmax,wmin,wpack,wunpckih,wunpckil,wunpckeh,wunpckel,wror,wsra,wsrl,wsll,wmadd,tmia,tmiaph,tmiaxy,tbcst,tmovmsk,wacc,waligni,walignr,tandc,textrc,torc,torvsc,wsad,wabs,wabsdiff,waddsubhx,wsubaddhx,wavg4,wmulw,wqmulm,wqmulwm,waddbhus,wqmiaxy,wmiaxy,wmiawxy,wmerge" (const_string "none"))

; Load scheduling, set from the arm_ld_sched variable
; initialized by arm_option_override()
//Pattern#28
(define_attr "ldsched" "no,yes" (const (symbol_ref "arm_ld_sched")))

;; Classification of NEON instructions for scheduling purposes.
//Pattern#29
(define_attr "neon_type"
   "neon_int_1,\
   neon_int_2,\
   neon_int_3,\
   neon_int_4,\
   neon_int_5,\
   neon_vqneg_vqabs,\
   neon_vmov,\
   neon_vaba,\
   neon_vsma,\
   neon_vaba_qqq,\
   neon_mul_ddd_8_16_qdd_16_8_long_32_16_long,\
   neon_mul_qqq_8_16_32_ddd_32,\
   neon_mul_qdd_64_32_long_qqd_16_ddd_32_scalar_64_32_long_scalar,\
   neon_mla_ddd_8_16_qdd_16_8_long_32_16_long,\
   neon_mla_qqq_8_16,\
   neon_mla_ddd_32_qqd_16_ddd_32_scalar_qdd_64_32_long_scalar_qdd_64_32_long,\
   neon_mla_qqq_32_qqd_32_scalar,\
   neon_mul_ddd_16_scalar_32_16_long_scalar,\
   neon_mul_qqd_32_scalar,\
   neon_mla_ddd_16_scalar_qdd_32_16_long_scalar,\
   neon_shift_1,\
   neon_shift_2,\
   neon_shift_3,\
   neon_vshl_ddd,\
   neon_vqshl_vrshl_vqrshl_qqq,\
   neon_vsra_vrsra,\
   neon_fp_vadd_ddd_vabs_dd,\
   neon_fp_vadd_qqq_vabs_qq,\
   neon_fp_vsum,\
   neon_fp_vmul_ddd,\
   neon_fp_vmul_qqd,\
   neon_fp_vmla_ddd,\
   neon_fp_vmla_qqq,\
   neon_fp_vmla_ddd_scalar,\
   neon_fp_vmla_qqq_scalar,\
   neon_fp_vrecps_vrsqrts_ddd,\
   neon_fp_vrecps_vrsqrts_qqq,\
   neon_bp_simple,\
   neon_bp_2cycle,\
   neon_bp_3cycle,\
   neon_ldr,\
   neon_str,\
   neon_vld1_1_2_regs,\
   neon_vld1_3_4_regs,\
   neon_vld2_2_regs_vld1_vld2_all_lanes,\
   neon_vld2_4_regs,\
   neon_vld3_vld4,\
   neon_vst1_1_2_regs_vst2_2_regs,\
   neon_vst1_3_4_regs,\
   neon_vst2_4_regs_vst3_vst4,\
   neon_vst3_vst4,\
   neon_vld1_vld2_lane,\
   neon_vld3_vld4_lane,\
   neon_vst1_vst2_lane,\
   neon_vst3_vst4_lane,\
   neon_vld3_vld4_all_lanes,\
   neon_mcr,\
   neon_mcr_2_mcrr,\
   neon_mrc,\
   neon_mrrc,\
   neon_ldm_2,\
   neon_stm_2,\
   none"
 (const_string "none"))

; condition codes: this one is used by final_prescan_insn to speed up
; conditionalizing instructions.  It saves having to scan the rtl to see if
; it uses or alters the condition codes.
; 
; USE means that the condition codes are used by the insn in the process of
;   outputting code, this means (at present) that we can't use the insn in
;   inlined branches
;
; SET means that the purpose of the insn is to set the condition codes in a
;   well defined manner.
;
; CLOB means that the condition codes are altered in an undefined manner, if
;   they are altered at all
;
; UNCONDITIONAL means the instruction can not be conditionally executed and
;   that the instruction does not use or alter the condition codes.
;
; NOCOND means that the instruction does not use or alter the condition
;   codes but can be converted into a conditionally exectuted instruction.

//Pattern#30
(define_attr "conds" "use,set,clob,unconditional,nocond"
	(if_then_else
	 (ior (eq_attr "is_thumb1" "yes")
	      (eq_attr "type" "call"))
	 (const_string "clob")
	 (if_then_else (eq_attr "neon_type" "none")
	  (const_string "nocond")
	  (const_string "unconditional"))))

; Predicable means that the insn can be conditionally executed based on
; an automatically added predicate (additional patterns are generated by 
; gen...).  We default to 'no' because no Thumb patterns match this rule
; and not all ARM patterns do.
//Pattern#31
(define_attr "predicable" "no,yes" (const_string "no"))

; Only model the write buffer for ARM6 and ARM7.  Earlier processors don't
; have one.  Later ones, such as StrongARM, have write-back caches, so don't
; suffer blockages enough to warrant modelling this (and it can adversely
; affect the schedule).
//Pattern#32
(define_attr "model_wbuf" "no,yes" (const (symbol_ref "arm_tune_wbuf")))

; WRITE_CONFLICT implies that a read following an unrelated write is likely
; to stall the processor.  Used with model_wbuf above.
//Pattern#33
(define_attr "write_conflict" "no,yes"
  (if_then_else (eq_attr "type"
		 "block,call,load1")
		(const_string "yes")
		(const_string "no")))

; Classify the insns into those that take one cycle and those that take more
; than one on the main cpu execution unit.
//Pattern#34
(define_attr "core_cycles" "single,multi"
  (if_then_else (eq_attr "type"
		 "simple_alu_imm,alu_reg,\
                  simple_alu_shift,alu_shift,\
                  float,fdivd,fdivs")
		(const_string "single")
	        (const_string "multi")))

;; FAR_JUMP is "yes" if a BL instruction is used to generate a branch to a
;; distant label.  Only applicable to Thumb code.
//Pattern#35
(define_attr "far_jump" "yes,no" (const_string "no"))


;; The number of machine instructions this pattern expands to.
;; Used for Thumb-2 conditional execution.
//Pattern#36
(define_attr "ce_count" "" (const_int 1))

;;---------------------------------------------------------------------------
;; Unspecs

(include "unspecs.md")

;;---------------------------------------------------------------------------
;; Mode iterators

(include "iterators.md")

;;---------------------------------------------------------------------------
;; Predicates

(include "predicates.md")
(include "constraints.md")

;;---------------------------------------------------------------------------
;; Pipeline descriptions

//Pattern#37
(define_attr "tune_cortexr4" "yes,no"
  (const (if_then_else
	  (eq_attr "tune" "cortexr4,cortexr4f,cortexr5")
	  (const_string "yes")
	  (const_string "no"))))

;; True if the generic scheduling description should be used.

//Pattern#38
(define_attr "generic_sched" "yes,no"
  (const (if_then_else
          (ior (eq_attr "tune" "fa526,fa626,fa606te,fa626te,fmp626,fa726te,arm926ejs,arm1020e,arm1026ejs,arm1136js,arm1136jfs,cortexa5,cortexa7,cortexa8,cortexa9,cortexa15,cortexm4,marvell_pj4")
	       (eq_attr "tune_cortexr4" "yes"))
          (const_string "no")
          (const_string "yes"))))

//Pattern#39
(define_attr "generic_vfp" "yes,no"
  (const (if_then_else
	  (and (eq_attr "fpu" "vfp")
	       (eq_attr "tune" "!arm1020e,arm1022e,cortexa5,cortexa7,cortexa8,cortexa9,cortexm4,marvell_pj4")
	       (eq_attr "tune_cortexr4" "no"))
	  (const_string "yes")
	  (const_string "no"))))

(include "marvell-f-iwmmxt.md")
(include "arm-generic.md")
(include "arm926ejs.md")
(include "arm1020e.md")
(include "arm1026ejs.md")
(include "arm1136jfs.md")
(include "fa526.md")
(include "fa606te.md")
(include "fa626te.md")
(include "fmp626.md")
(include "fa726te.md")
(include "cortex-a5.md")
(include "cortex-a7.md")
(include "cortex-a8.md")
(include "cortex-a9.md")
(include "cortex-a15.md")
(include "cortex-r4.md")
(include "cortex-r4f.md")
(include "cortex-m4.md")
(include "cortex-m4-fpu.md")
(include "vfp11.md")
(include "marvell-pj4.md")


;;---------------------------------------------------------------------------
;; Insn patterns
;;
;; Addition insns.

;; Note: For DImode insns, there is normally no reason why operands should
;; not be in the same register, what we don't want is for something being
;; written to partially overlap something that is an input.

//Pattern#40
(define_expand "adddi3"
 [(parallel
   [(set (match_operand:DI           0 "s_register_operand" "")
	  (plus:DI (match_operand:DI 1 "s_register_operand" "")
	           (match_operand:DI 2 "arm_adddi_operand"  "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_EITHER"
  "
  if (TARGET_THUMB1)
    {
      if (!REG_P (operands[1]))
        operands[1] = force_reg (DImode, operands[1]);
      if (!REG_P (operands[2]))
        operands[2] = force_reg (DImode, operands[2]);
     }
  "
)

//Pattern#41
(define_insn "*thumb1_adddi3"
  [(set (match_operand:DI          0 "register_operand" "=l")
	(plus:DI (match_operand:DI 1 "register_operand" "%0")
		 (match_operand:DI 2 "register_operand" "l")))
   (clobber (reg:CC CC_REGNUM))
  ]
  "TARGET_THUMB1"
  "add\\t%Q0, %Q0, %Q2\;adc\\t%R0, %R0, %R2"
  [(set_attr "length" "4")]
)

//Pattern#42
(define_insn_and_split "*arm_adddi3"
  [(set (match_operand:DI          0 "s_register_operand" "=&r,&r,&r,&r,&r")
	(plus:DI (match_operand:DI 1 "s_register_operand" "%0, 0, r, 0, r")
		 (match_operand:DI 2 "arm_adddi_operand"  "r,  0, r, Dd, Dd")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && !TARGET_NEON"
  "#"
  "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))"
  [(parallel [(set (reg:CC_C CC_REGNUM)
		   (compare:CC_C (plus:SI (match_dup 1) (match_dup 2))
				 (match_dup 1)))
	      (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 3) (plus:SI (plus:SI (match_dup 4) (match_dup 5))
			       (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0))))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart_mode (SImode, DImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#43
(define_insn_and_split "*adddi_sesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(plus:DI (sign_extend:DI
		  (match_operand:SI 2 "s_register_operand" "r,r"))
		 (match_operand:DI 1 "s_register_operand" "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  [(parallel [(set (reg:CC_C CC_REGNUM)
		   (compare:CC_C (plus:SI (match_dup 1) (match_dup 2))
				 (match_dup 1)))
	      (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 3) (plus:SI (plus:SI (ashiftrt:SI (match_dup 2)
						     (const_int 31))
					(match_dup 4))
			       (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0))))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#44
(define_insn_and_split "*adddi_zesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(plus:DI (zero_extend:DI
		  (match_operand:SI 2 "s_register_operand" "r,r"))
		 (match_operand:DI 1 "s_register_operand" "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  [(parallel [(set (reg:CC_C CC_REGNUM)
		   (compare:CC_C (plus:SI (match_dup 1) (match_dup 2))
				 (match_dup 1)))
	      (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 3) (plus:SI (plus:SI (match_dup 4) (const_int 0))
			       (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0))))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#45
(define_expand "addsi3"
  [(set (match_operand:SI          0 "s_register_operand" "")
	(plus:SI (match_operand:SI 1 "s_register_operand" "")
		 (match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT && CONST_INT_P (operands[2]))
    {
      arm_split_constant (PLUS, SImode, NULL_RTX,
	                  INTVAL (operands[2]), operands[0], operands[1],
			  optimize && can_create_pseudo_p ());
      DONE;
    }
  "
)

; If there is a scratch available, this will be faster than synthesizing the
; addition.
//Pattern#46
(define_peephole2
  [(match_scratch:SI 3 "r")
   (set (match_operand:SI          0 "arm_general_register_operand" "")
	(plus:SI (match_operand:SI 1 "arm_general_register_operand" "")
		 (match_operand:SI 2 "const_int_operand"  "")))]
  "TARGET_32BIT &&
   !(const_ok_for_arm (INTVAL (operands[2]))
     || const_ok_for_arm (-INTVAL (operands[2])))
    && const_ok_for_arm (~INTVAL (operands[2]))"
  [(set (match_dup 3) (match_dup 2))
   (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 3)))]
  ""
)

;; The r/r/k alternative is required when reloading the address
;;  (plus (reg rN) (reg sp)) into (reg rN).  In this case reload will
;; put the duplicated register first, and not try the commutative version.
//Pattern#47
(define_insn_and_split "*arm_addsi3"
  [(set (match_operand:SI          0 "s_register_operand" "=rk, r,k, r,r, k, r, k,k,r, k, r")
	(plus:SI (match_operand:SI 1 "s_register_operand" "%0, rk,k, r,rk,k, rk,k,r,rk,k, rk")
		 (match_operand:SI 2 "reg_or_int_operand" "rk, rI,rI,k,Pj,Pj,L, L,L,PJ,PJ,?n")))]
  "TARGET_32BIT"
  "@
   add%?\\t%0, %0, %2
   add%?\\t%0, %1, %2
   add%?\\t%0, %1, %2
   add%?\\t%0, %2, %1
   addw%?\\t%0, %1, %2
   addw%?\\t%0, %1, %2
   sub%?\\t%0, %1, #%n2
   sub%?\\t%0, %1, #%n2
   sub%?\\t%0, %1, #%n2
   subw%?\\t%0, %1, #%n2
   subw%?\\t%0, %1, #%n2
   #"
  "TARGET_32BIT
   && CONST_INT_P (operands[2])
   && !const_ok_for_op (INTVAL (operands[2]), PLUS)
   && (reload_completed || !arm_eliminable_register (operands[1]))"
  [(clobber (const_int 0))]
  "
  arm_split_constant (PLUS, SImode, curr_insn,
	              INTVAL (operands[2]), operands[0],
		      operands[1], 0);
  DONE;
  "
  [(set_attr "length" "2,4,4,4,4,4,4,4,4,4,4,16")
   (set_attr "predicable" "yes")
   (set_attr "arch" "t2,*,*,*,t2,t2,*,*,a,t2,t2,*")
   (set (attr "type") (if_then_else (match_operand 2 "const_int_operand" "")
		      (const_string "simple_alu_imm")
		      (const_string "alu_reg")))
 ]
)

//Pattern#48
(define_insn_and_split "*thumb1_addsi3"
  [(set (match_operand:SI          0 "register_operand" "=l,l,l,*rk,*hk,l,k,l,l,l")
	(plus:SI (match_operand:SI 1 "register_operand" "%0,0,l,*0,*0,k,k,0,l,k")
		 (match_operand:SI 2 "nonmemory_operand" "I,J,lL,*hk,*rk,M,O,Pa,Pb,Pc")))]
  "TARGET_THUMB1"
  "*
   static const char * const asms[] = 
   {
     \"add\\t%0, %0, %2\",
     \"sub\\t%0, %0, #%n2\",
     \"add\\t%0, %1, %2\",
     \"add\\t%0, %0, %2\",
     \"add\\t%0, %0, %2\",
     \"add\\t%0, %1, %2\",
     \"add\\t%0, %1, %2\",
     \"#\",
     \"#\",
     \"#\"
   };
   if ((which_alternative == 2 || which_alternative == 6)
       && CONST_INT_P (operands[2])
       && INTVAL (operands[2]) < 0)
     return \"sub\\t%0, %1, #%n2\";
   return asms[which_alternative];
  "
  "&& reload_completed && CONST_INT_P (operands[2])
   && ((operands[1] != stack_pointer_rtx
        && (INTVAL (operands[2]) > 255 || INTVAL (operands[2]) < -255))
       || (operands[1] == stack_pointer_rtx
 	   && INTVAL (operands[2]) > 1020))"
  [(set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (plus:SI (match_dup 0) (match_dup 3)))]
  {
    HOST_WIDE_INT offset = INTVAL (operands[2]);
    if (operands[1] == stack_pointer_rtx)
      offset -= 1020;
    else
      {
        if (offset > 255)
	  offset = 255;
	else if (offset < -255)
	  offset = -255;
      }
    operands[3] = GEN_INT (offset);
    operands[2] = GEN_INT (INTVAL (operands[2]) - offset);
  }
  [(set_attr "length" "2,2,2,2,2,2,2,4,4,4")]
)

;; Reloading and elimination of the frame pointer can
;; sometimes cause this optimization to be missed.
//Pattern#49
(define_peephole2
  [(set (match_operand:SI 0 "arm_general_register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))
   (set (match_dup 0)
	(plus:SI (match_dup 0) (reg:SI SP_REGNUM)))]
  "TARGET_THUMB1
   && (unsigned HOST_WIDE_INT) (INTVAL (operands[1])) < 1024
   && (INTVAL (operands[1]) & 3) == 0"
  [(set (match_dup 0) (plus:SI (reg:SI SP_REGNUM) (match_dup 1)))]
  ""
)

//Pattern#50
(define_insn "addsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (match_operand:SI 1 "s_register_operand" "r, r,r")
		  (match_operand:SI 2 "arm_add_operand"    "I,L,r"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(plus:SI (match_dup 1) (match_dup 2)))]
  "TARGET_ARM"
  "@
   add%.\\t%0, %1, %2
   sub%.\\t%0, %1, #%n2
   add%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm, simple_alu_imm, *")]
)

//Pattern#51
(define_insn "*addsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (match_operand:SI 0 "s_register_operand" "r, r, r")
		  (match_operand:SI 1 "arm_add_operand"    "I,L, r"))
	 (const_int 0)))]
  "TARGET_ARM"
  "@
   cmn%?\\t%0, %1
   cmp%?\\t%0, #%n1
   cmn%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm, simple_alu_imm, *")
   ]
)

//Pattern#52
(define_insn "*compare_negsi_si"
  [(set (reg:CC_Z CC_REGNUM)
	(compare:CC_Z
	 (neg:SI (match_operand:SI 0 "s_register_operand" "r"))
	 (match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_32BIT"
  "cmn%?\\t%1, %0"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")]
)

;; This is the canonicalization of addsi3_compare0_for_combiner when the
;; addend is a constant.
//Pattern#53
(define_insn "*cmpsi2_addneg"
  [(set (reg:CC CC_REGNUM)
	(compare:CC
	 (match_operand:SI 1 "s_register_operand" "r,r")
	 (match_operand:SI 2 "arm_addimm_operand" "L,I")))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(plus:SI (match_dup 1)
		 (match_operand:SI 3 "arm_addimm_operand" "I,L")))]
  "TARGET_32BIT && INTVAL (operands[2]) == -INTVAL (operands[3])"
  "@
   add%.\\t%0, %1, %3
   sub%.\\t%0, %1, #%n3"
  [(set_attr "conds" "set")]
)

;; Convert the sequence
;;  sub  rd, rn, #1
;;  cmn  rd, #1	(equivalent to cmp rd, #-1)
;;  bne  dest
;; into
;;  subs rd, rn, #1
;;  bcs  dest	((unsigned)rn >= 1)
;; similarly for the beq variant using bcc.
;; This is a common looping idiom (while (n--))
//Pattern#54
(define_peephole2
  [(set (match_operand:SI 0 "arm_general_register_operand" "")
	(plus:SI (match_operand:SI 1 "arm_general_register_operand" "")
		 (const_int -1)))
   (set (match_operand 2 "cc_register" "")
	(compare (match_dup 0) (const_int -1)))
   (set (pc)
	(if_then_else (match_operator 3 "equality_operator"
		       [(match_dup 2) (const_int 0)])
		      (match_operand 4 "" "")
		      (match_operand 5 "" "")))]
  "TARGET_32BIT && peep2_reg_dead_p (3, operands[2])"
  [(parallel[
    (set (match_dup 2)
	 (compare:CC
	  (match_dup 1) (const_int 1)))
    (set (match_dup 0) (plus:SI (match_dup 1) (const_int -1)))])
   (set (pc)
	(if_then_else (match_op_dup 3 [(match_dup 2) (const_int 0)])
		      (match_dup 4)
		      (match_dup 5)))]
  "operands[2] = gen_rtx_REG (CCmode, CC_REGNUM);
   operands[3] = gen_rtx_fmt_ee ((GET_CODE (operands[3]) == NE
				  ? GEU : LTU),
				 VOIDmode, 
				 operands[2], const0_rtx);"
)

;; The next four insns work because they compare the result with one of
;; the operands, and we know that the use of the condition code is
;; either GEU or LTU, so we can use the carry flag from the addition
;; instead of doing the compare a second time.
//Pattern#55
(define_insn "*addsi3_compare_op1"
  [(set (reg:CC_C CC_REGNUM)
	(compare:CC_C
	 (plus:SI (match_operand:SI 1 "s_register_operand" "r,r,r")
		  (match_operand:SI 2 "arm_add_operand" "I,L,r"))
	 (match_dup 1)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(plus:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "@
   add%.\\t%0, %1, %2
   sub%.\\t%0, %1, #%n2
   add%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type"  "simple_alu_imm,simple_alu_imm,*")]
)

//Pattern#56
(define_insn "*addsi3_compare_op2"
  [(set (reg:CC_C CC_REGNUM)
	(compare:CC_C
	 (plus:SI (match_operand:SI 1 "s_register_operand" "r,r,r")
		  (match_operand:SI 2 "arm_add_operand" "I,L,r"))
	 (match_dup 2)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(plus:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "@
   add%.\\t%0, %1, %2
   add%.\\t%0, %1, %2
   sub%.\\t%0, %1, #%n2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*")]
)

//Pattern#57
(define_insn "*compare_addsi2_op0"
  [(set (reg:CC_C CC_REGNUM)
	(compare:CC_C
	 (plus:SI (match_operand:SI 0 "s_register_operand" "r,r,r")
		  (match_operand:SI 1 "arm_add_operand" "I,L,r"))
	 (match_dup 0)))]
  "TARGET_32BIT"
  "@
   cmn%?\\t%0, %1
   cmp%?\\t%0, #%n1
   cmn%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*")]
)

//Pattern#58
(define_insn "*compare_addsi2_op1"
  [(set (reg:CC_C CC_REGNUM)
	(compare:CC_C
	 (plus:SI (match_operand:SI 0 "s_register_operand" "r,r,r")
		  (match_operand:SI 1 "arm_add_operand" "I,L,r"))
	 (match_dup 1)))]
  "TARGET_32BIT"
  "@
   cmn%?\\t%0, %1
   cmp%?\\t%0, #%n1
   cmn%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*")]
)

//Pattern#59
(define_insn "*addsi3_carryin_<optab>"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(plus:SI (plus:SI (match_operand:SI 1 "s_register_operand" "%r,r")
			  (match_operand:SI 2 "arm_not_operand" "rI,K"))
		 (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0))))]
  "TARGET_32BIT"
  "@
   adc%?\\t%0, %1, %2
   sbc%?\\t%0, %1, #%B2"
  [(set_attr "conds" "use")]
)

//Pattern#60
(define_insn "*addsi3_carryin_alt2_<optab>"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(plus:SI (plus:SI (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0))
			  (match_operand:SI 1 "s_register_operand" "%r,r"))
		 (match_operand:SI 2 "arm_rhs_operand" "rI,K")))]
  "TARGET_32BIT"
  "@
   adc%?\\t%0, %1, %2
   sbc%?\\t%0, %1, #%B2"
  [(set_attr "conds" "use")]
)

//Pattern#61
(define_insn "*addsi3_carryin_shift_<optab>"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (plus:SI
		  (match_operator:SI 2 "shift_operator"
		    [(match_operand:SI 3 "s_register_operand" "r")
		     (match_operand:SI 4 "reg_or_int_operand" "rM")])
		  (match_operand:SI 1 "s_register_operand" "r"))
		 (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0))))]
  "TARGET_32BIT"
  "adc%?\\t%0, %1, %3%S2"
  [(set_attr "conds" "use")
   (set (attr "type") (if_then_else (match_operand 4 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#62
(define_insn "*addsi3_carryin_clobercc_<optab>"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (plus:SI (match_operand:SI 1 "s_register_operand" "%r")
			  (match_operand:SI 2 "arm_rhs_operand" "rI"))
		 (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0))))
   (clobber (reg:CC CC_REGNUM))]
   "TARGET_32BIT"
   "adc%.\\t%0, %1, %2"
   [(set_attr "conds" "set")]
)

//Pattern#63
(define_expand "incscc"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
        (plus:SI (match_operator:SI 2 "arm_comparison_operator"
                    [(match_operand:CC 3 "cc_register" "") (const_int 0)])
                 (match_operand:SI 1 "s_register_operand" "0,?r")))]
  "TARGET_32BIT"
  ""
)

//Pattern#64
(define_insn "*arm_incscc"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
        (plus:SI (match_operator:SI 2 "arm_comparison_operator"
                    [(match_operand:CC 3 "cc_register" "") (const_int 0)])
                 (match_operand:SI 1 "s_register_operand" "0,?r")))]
  "TARGET_ARM"
  "@
  add%d2\\t%0, %1, #1
  mov%D2\\t%0, %1\;add%d2\\t%0, %1, #1"
  [(set_attr "conds" "use")
   (set_attr "length" "4,8")]
)

; transform ((x << y) - 1) to ~(~(x-1) << y)  Where X is a constant.
//Pattern#65
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(plus:SI (ashift:SI (match_operand:SI 1 "const_int_operand" "")
			    (match_operand:SI 2 "s_register_operand" ""))
		 (const_int -1)))
   (clobber (match_operand:SI 3 "s_register_operand" ""))]
  "TARGET_32BIT"
  [(set (match_dup 3) (match_dup 1))
   (set (match_dup 0) (not:SI (ashift:SI (match_dup 3) (match_dup 2))))]
  "
  operands[1] = GEN_INT (~(INTVAL (operands[1]) - 1));
")

//Pattern#66
(define_expand "addsf3"
  [(set (match_operand:SF          0 "s_register_operand" "")
	(plus:SF (match_operand:SF 1 "s_register_operand" "")
		 (match_operand:SF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#67
(define_expand "adddf3"
  [(set (match_operand:DF          0 "s_register_operand" "")
	(plus:DF (match_operand:DF 1 "s_register_operand" "")
		 (match_operand:DF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")

//Pattern#68
(define_expand "subdi3"
 [(parallel
   [(set (match_operand:DI            0 "s_register_operand" "")
	  (minus:DI (match_operand:DI 1 "s_register_operand" "")
	            (match_operand:DI 2 "s_register_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_EITHER"
  "
  if (TARGET_THUMB1)
    {
      if (!REG_P (operands[1]))
        operands[1] = force_reg (DImode, operands[1]);
      if (!REG_P (operands[2]))
        operands[2] = force_reg (DImode, operands[2]);
     }
  "
)

//Pattern#69
(define_insn "*arm_subdi3"
  [(set (match_operand:DI           0 "s_register_operand" "=&r,&r,&r")
	(minus:DI (match_operand:DI 1 "s_register_operand" "0,r,0")
		  (match_operand:DI 2 "s_register_operand" "r,0,0")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && !TARGET_NEON"
  "subs\\t%Q0, %Q1, %Q2\;sbc\\t%R0, %R1, %R2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#70
(define_insn "*thumb_subdi3"
  [(set (match_operand:DI           0 "register_operand" "=l")
	(minus:DI (match_operand:DI 1 "register_operand"  "0")
		  (match_operand:DI 2 "register_operand"  "l")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_THUMB1"
  "sub\\t%Q0, %Q0, %Q2\;sbc\\t%R0, %R0, %R2"
  [(set_attr "length" "4")]
)

//Pattern#71
(define_insn "*subdi_di_zesidi"
  [(set (match_operand:DI           0 "s_register_operand" "=&r,&r")
	(minus:DI (match_operand:DI 1 "s_register_operand"  "0,r")
		  (zero_extend:DI
		   (match_operand:SI 2 "s_register_operand"  "r,r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "subs\\t%Q0, %Q1, %2\;sbc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#72
(define_insn "*subdi_di_sesidi"
  [(set (match_operand:DI            0 "s_register_operand" "=&r,&r")
	(minus:DI (match_operand:DI  1 "s_register_operand"  "0,r")
		  (sign_extend:DI
		   (match_operand:SI 2 "s_register_operand"  "r,r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "subs\\t%Q0, %Q1, %2\;sbc\\t%R0, %R1, %2, asr #31"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#73
(define_insn "*subdi_zesidi_di"
  [(set (match_operand:DI            0 "s_register_operand" "=&r,&r")
	(minus:DI (zero_extend:DI
		   (match_operand:SI 2 "s_register_operand"  "r,r"))
		  (match_operand:DI  1 "s_register_operand" "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, %2\;rsc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#74
(define_insn "*subdi_sesidi_di"
  [(set (match_operand:DI            0 "s_register_operand" "=&r,&r")
	(minus:DI (sign_extend:DI
		   (match_operand:SI 2 "s_register_operand"   "r,r"))
		  (match_operand:DI  1 "s_register_operand"  "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, %2\;rsc\\t%R0, %R1, %2, asr #31"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#75
(define_insn "*subdi_zesidi_zesidi"
  [(set (match_operand:DI            0 "s_register_operand" "=r")
	(minus:DI (zero_extend:DI
		   (match_operand:SI 1 "s_register_operand"  "r"))
		  (zero_extend:DI
		   (match_operand:SI 2 "s_register_operand"  "r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "subs\\t%Q0, %1, %2\;sbc\\t%R0, %1, %1"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#76
(define_expand "subsi3"
  [(set (match_operand:SI           0 "s_register_operand" "")
	(minus:SI (match_operand:SI 1 "reg_or_int_operand" "")
		  (match_operand:SI 2 "s_register_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[1]))
    {
      if (TARGET_32BIT)
        {
          arm_split_constant (MINUS, SImode, NULL_RTX,
	                      INTVAL (operands[1]), operands[0],
	  		      operands[2], optimize && can_create_pseudo_p ());
          DONE;
	}
      else /* TARGET_THUMB1 */
        operands[1] = force_reg (SImode, operands[1]);
    }
  "
)

//Pattern#77
(define_insn "thumb1_subsi3_insn"
  [(set (match_operand:SI           0 "register_operand" "=l")
	(minus:SI (match_operand:SI 1 "register_operand" "l")
		  (match_operand:SI 2 "reg_or_int_operand" "lPd")))]
  "TARGET_THUMB1"
  "sub\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

; ??? Check Thumb-2 split length
//Pattern#78
(define_insn_and_split "*arm_subsi3_insn"
  [(set (match_operand:SI           0 "s_register_operand" "=r,r,r,rk,r")
	(minus:SI (match_operand:SI 1 "reg_or_int_operand" "rI,r,r,k,?n")
		  (match_operand:SI 2 "reg_or_int_operand" "r,I,r,r, r")))]
  "TARGET_32BIT"
  "@
   rsb%?\\t%0, %2, %1
   sub%?\\t%0, %1, %2
   sub%?\\t%0, %1, %2
   sub%?\\t%0, %1, %2
   #"
  "&& (CONST_INT_P (operands[1])
       && !const_ok_for_arm (INTVAL (operands[1])))"
  [(clobber (const_int 0))]
  "
  arm_split_constant (MINUS, SImode, curr_insn,
                      INTVAL (operands[1]), operands[0], operands[2], 0);
  DONE;
  "
  [(set_attr "length" "4,4,4,4,16")
   (set_attr "predicable" "yes")
   (set_attr "type"  "*,simple_alu_imm,*,*,*")]
)

//Pattern#79
(define_peephole2
  [(match_scratch:SI 3 "r")
   (set (match_operand:SI 0 "arm_general_register_operand" "")
	(minus:SI (match_operand:SI 1 "const_int_operand" "")
		  (match_operand:SI 2 "arm_general_register_operand" "")))]
  "TARGET_32BIT
   && !const_ok_for_arm (INTVAL (operands[1]))
   && const_ok_for_arm (~INTVAL (operands[1]))"
  [(set (match_dup 3) (match_dup 1))
   (set (match_dup 0) (minus:SI (match_dup 3) (match_dup 2)))]
  ""
)

//Pattern#80
(define_insn "*subsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (minus:SI (match_operand:SI 1 "arm_rhs_operand" "r,r,I")
		   (match_operand:SI 2 "arm_rhs_operand" "I,r,r"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(minus:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "@
   sub%.\\t%0, %1, %2
   sub%.\\t%0, %1, %2
   rsb%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "type"  "simple_alu_imm,*,*")]
)

//Pattern#81
(define_insn "*subsi3_compare"
  [(set (reg:CC CC_REGNUM)
	(compare:CC (match_operand:SI 1 "arm_rhs_operand" "r,r,I")
		    (match_operand:SI 2 "arm_rhs_operand" "I,r,r")))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(minus:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "@
   sub%.\\t%0, %1, %2
   sub%.\\t%0, %1, %2
   rsb%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,*,*")]
)

//Pattern#82
(define_expand "decscc"
  [(set (match_operand:SI            0 "s_register_operand" "=r,r")
        (minus:SI (match_operand:SI  1 "s_register_operand" "0,?r")
		  (match_operator:SI 2 "arm_comparison_operator"
                   [(match_operand   3 "cc_register" "") (const_int 0)])))]
  "TARGET_32BIT"
  ""
)

//Pattern#83
(define_insn "*arm_decscc"
  [(set (match_operand:SI            0 "s_register_operand" "=r,r")
        (minus:SI (match_operand:SI  1 "s_register_operand" "0,?r")
		  (match_operator:SI 2 "arm_comparison_operator"
                   [(match_operand   3 "cc_register" "") (const_int 0)])))]
  "TARGET_ARM"
  "@
   sub%d2\\t%0, %1, #1
   mov%D2\\t%0, %1\;sub%d2\\t%0, %1, #1"
  [(set_attr "conds" "use")
   (set_attr "length" "*,8")
   (set_attr "type" "simple_alu_imm,*")]
)

//Pattern#84
(define_expand "subsf3"
  [(set (match_operand:SF           0 "s_register_operand" "")
	(minus:SF (match_operand:SF 1 "s_register_operand" "")
		  (match_operand:SF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#85
(define_expand "subdf3"
  [(set (match_operand:DF           0 "s_register_operand" "")
	(minus:DF (match_operand:DF 1 "s_register_operand" "")
		  (match_operand:DF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")


;; Multiplication insns

//Pattern#86
(define_expand "mulsi3"
  [(set (match_operand:SI          0 "s_register_operand" "")
	(mult:SI (match_operand:SI 2 "s_register_operand" "")
		 (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_EITHER"
  ""
)

;; Use `&' and then `0' to prevent the operands 0 and 1 being the same
//Pattern#87
(define_insn "*arm_mulsi3"
  [(set (match_operand:SI          0 "s_register_operand" "=&r,&r")
	(mult:SI (match_operand:SI 2 "s_register_operand" "r,r")
		 (match_operand:SI 1 "s_register_operand" "%0,r")))]
  "TARGET_32BIT && !arm_arch6"
  "mul%?\\t%0, %2, %1"
  [(set_attr "insn" "mul")
   (set_attr "predicable" "yes")]
)

//Pattern#88
(define_insn "*arm_mulsi3_v6"
  [(set (match_operand:SI          0 "s_register_operand" "=r")
	(mult:SI (match_operand:SI 1 "s_register_operand" "r")
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_32BIT && arm_arch6"
  "mul%?\\t%0, %1, %2"
  [(set_attr "insn" "mul")
   (set_attr "predicable" "yes")]
)

; Unfortunately with the Thumb the '&'/'0' trick can fails when operands 
; 1 and 2; are the same, because reload will make operand 0 match 
; operand 1 without realizing that this conflicts with operand 2.  We fix 
; this by adding another alternative to match this case, and then `reload' 
; it ourselves.  This alternative must come first.
//Pattern#89
(define_insn "*thumb_mulsi3"
  [(set (match_operand:SI          0 "register_operand" "=&l,&l,&l")
	(mult:SI (match_operand:SI 1 "register_operand" "%l,*h,0")
		 (match_operand:SI 2 "register_operand" "l,l,l")))]
  "TARGET_THUMB1 && !arm_arch6"
  "*
  if (which_alternative < 2)
    return \"mov\\t%0, %1\;mul\\t%0, %2\";
  else
    return \"mul\\t%0, %2\";
  "
  [(set_attr "length" "4,4,2")
   (set_attr "insn" "mul")]
)

//Pattern#90
(define_insn "*thumb_mulsi3_v6"
  [(set (match_operand:SI          0 "register_operand" "=l,l,l")
	(mult:SI (match_operand:SI 1 "register_operand" "0,l,0")
		 (match_operand:SI 2 "register_operand" "l,0,0")))]
  "TARGET_THUMB1 && arm_arch6"
  "@
   mul\\t%0, %2
   mul\\t%0, %1
   mul\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "insn" "mul")]
)

//Pattern#91
(define_insn "*mulsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (mult:SI
			  (match_operand:SI 2 "s_register_operand" "r,r")
			  (match_operand:SI 1 "s_register_operand" "%0,r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=&r,&r")
	(mult:SI (match_dup 2) (match_dup 1)))]
  "TARGET_ARM && !arm_arch6"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)

//Pattern#92
(define_insn "*mulsi3_compare0_v6"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (mult:SI
			  (match_operand:SI 2 "s_register_operand" "r")
			  (match_operand:SI 1 "s_register_operand" "r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (match_dup 2) (match_dup 1)))]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)

//Pattern#93
(define_insn "*mulsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (mult:SI
			  (match_operand:SI 2 "s_register_operand" "r,r")
			  (match_operand:SI 1 "s_register_operand" "%0,r"))
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=&r,&r"))]
  "TARGET_ARM && !arm_arch6"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)

//Pattern#94
(define_insn "*mulsi_compare0_scratch_v6"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (mult:SI
			  (match_operand:SI 2 "s_register_operand" "r")
			  (match_operand:SI 1 "s_register_operand" "r"))
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)

;; Unnamed templates to match MLA instruction.

//Pattern#95
(define_insn "*mulsi3addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r,&r")
	(plus:SI
	  (mult:SI (match_operand:SI 2 "s_register_operand" "r,r,r,r")
		   (match_operand:SI 1 "s_register_operand" "%0,r,0,r"))
	  (match_operand:SI 3 "s_register_operand" "r,r,0,0")))]
  "TARGET_32BIT && !arm_arch6"
  "mla%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)

//Pattern#96
(define_insn "*mulsi3addsi_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI
	  (mult:SI (match_operand:SI 2 "s_register_operand" "r")
		   (match_operand:SI 1 "s_register_operand" "r"))
	  (match_operand:SI 3 "s_register_operand" "r")))]
  "TARGET_32BIT && arm_arch6"
  "mla%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)

//Pattern#97
(define_insn "*mulsi3addsi_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (mult:SI
		   (match_operand:SI 2 "s_register_operand" "r,r,r,r")
		   (match_operand:SI 1 "s_register_operand" "%0,r,0,r"))
		  (match_operand:SI 3 "s_register_operand" "r,r,0,0"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r,&r")
	(plus:SI (mult:SI (match_dup 2) (match_dup 1))
		 (match_dup 3)))]
  "TARGET_ARM && arm_arch6"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)

//Pattern#98
(define_insn "*mulsi3addsi_compare0_v6"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (mult:SI
		   (match_operand:SI 2 "s_register_operand" "r")
		   (match_operand:SI 1 "s_register_operand" "r"))
		  (match_operand:SI 3 "s_register_operand" "r"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (mult:SI (match_dup 2) (match_dup 1))
		 (match_dup 3)))]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)

//Pattern#99
(define_insn "*mulsi3addsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (mult:SI
		   (match_operand:SI 2 "s_register_operand" "r,r,r,r")
		   (match_operand:SI 1 "s_register_operand" "%0,r,0,r"))
		  (match_operand:SI 3 "s_register_operand" "?r,r,0,0"))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=&r,&r,&r,&r"))]
  "TARGET_ARM && !arm_arch6"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)

//Pattern#100
(define_insn "*mulsi3addsi_compare0_scratch_v6"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (mult:SI
		   (match_operand:SI 2 "s_register_operand" "r")
		   (match_operand:SI 1 "s_register_operand" "r"))
		  (match_operand:SI 3 "s_register_operand" "r"))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)

//Pattern#101
(define_insn "*mulsi3subsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(minus:SI
	  (match_operand:SI 3 "s_register_operand" "r")
	  (mult:SI (match_operand:SI 2 "s_register_operand" "r")
		   (match_operand:SI 1 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch_thumb2"
  "mls%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)

//Pattern#102
(define_expand "maddsidi4"
  [(set (match_operand:DI 0 "s_register_operand" "")
	(plus:DI
	 (mult:DI
	  (sign_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	  (sign_extend:DI (match_operand:SI 2 "s_register_operand" "")))
	 (match_operand:DI 3 "s_register_operand" "")))]
  "TARGET_32BIT && arm_arch3m"
  "")

//Pattern#103
(define_insn "*mulsidi3adddi"
  [(set (match_operand:DI 0 "s_register_operand" "=&r")
	(plus:DI
	 (mult:DI
	  (sign_extend:DI (match_operand:SI 2 "s_register_operand" "%r"))
	  (sign_extend:DI (match_operand:SI 3 "s_register_operand" "r")))
	 (match_operand:DI 1 "s_register_operand" "0")))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "smlal")
   (set_attr "predicable" "yes")]
)

//Pattern#104
(define_insn "*mulsidi3adddi_v6"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	 (mult:DI
	  (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r"))
	  (sign_extend:DI (match_operand:SI 3 "s_register_operand" "r")))
	 (match_operand:DI 1 "s_register_operand" "0")))]
  "TARGET_32BIT && arm_arch6"
  "smlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "smlal")
   (set_attr "predicable" "yes")]
)

;; 32x32->64 widening multiply.
;; As with mulsi3, the only difference between the v3-5 and v6+
;; versions of these patterns is the requirement that the output not
;; overlap the inputs, but that still means we have to have a named
;; expander and two different starred insns.

//Pattern#105
(define_expand "mulsidi3"
  [(set (match_operand:DI 0 "s_register_operand" "")
	(mult:DI
	 (sign_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	 (sign_extend:DI (match_operand:SI 2 "s_register_operand" ""))))]
  "TARGET_32BIT && arm_arch3m"
  ""
)

//Pattern#106
(define_insn "*mulsidi3_nov6"
  [(set (match_operand:DI 0 "s_register_operand" "=&r")
	(mult:DI
	 (sign_extend:DI (match_operand:SI 1 "s_register_operand" "%r"))
	 (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)

//Pattern#107
(define_insn "*mulsidi3_v6"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(mult:DI
	 (sign_extend:DI (match_operand:SI 1 "s_register_operand" "r"))
	 (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch6"
  "smull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)

//Pattern#108
(define_expand "umulsidi3"
  [(set (match_operand:DI 0 "s_register_operand" "")
	(mult:DI
	 (zero_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	 (zero_extend:DI (match_operand:SI 2 "s_register_operand" ""))))]
  "TARGET_32BIT && arm_arch3m"
  ""
)

//Pattern#109
(define_insn "*umulsidi3_nov6"
  [(set (match_operand:DI 0 "s_register_operand" "=&r")
	(mult:DI
	 (zero_extend:DI (match_operand:SI 1 "s_register_operand" "%r"))
	 (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)

//Pattern#110
(define_insn "*umulsidi3_v6"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(mult:DI
	 (zero_extend:DI (match_operand:SI 1 "s_register_operand" "r"))
	 (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch6"
  "umull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)

//Pattern#111
(define_expand "umaddsidi4"
  [(set (match_operand:DI 0 "s_register_operand" "")
	(plus:DI
	 (mult:DI
	  (zero_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" "")))
	 (match_operand:DI 3 "s_register_operand" "")))]
  "TARGET_32BIT && arm_arch3m"
  "")

//Pattern#112
(define_insn "*umulsidi3adddi"
  [(set (match_operand:DI 0 "s_register_operand" "=&r")
	(plus:DI
	 (mult:DI
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" "%r"))
	  (zero_extend:DI (match_operand:SI 3 "s_register_operand" "r")))
	 (match_operand:DI 1 "s_register_operand" "0")))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "umlal")
   (set_attr "predicable" "yes")]
)

//Pattern#113
(define_insn "*umulsidi3adddi_v6"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	 (mult:DI
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r"))
	  (zero_extend:DI (match_operand:SI 3 "s_register_operand" "r")))
	 (match_operand:DI 1 "s_register_operand" "0")))]
  "TARGET_32BIT && arm_arch6"
  "umlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "umlal")
   (set_attr "predicable" "yes")]
)

//Pattern#114
(define_expand "smulsi3_highpart"
  [(parallel
    [(set (match_operand:SI 0 "s_register_operand" "")
	  (truncate:SI
	   (lshiftrt:DI
	    (mult:DI
	     (sign_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	     (sign_extend:DI (match_operand:SI 2 "s_register_operand" "")))
	    (const_int 32))))
     (clobber (match_scratch:SI 3 ""))])]
  "TARGET_32BIT && arm_arch3m"
  ""
)

//Pattern#115
(define_insn "*smulsi3_highpart_nov6"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r")
	(truncate:SI
	 (lshiftrt:DI
	  (mult:DI
	   (sign_extend:DI (match_operand:SI 1 "s_register_operand" "%0,r"))
	   (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r,r")))
	  (const_int 32))))
   (clobber (match_scratch:SI 3 "=&r,&r"))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)

//Pattern#116
(define_insn "*smulsi3_highpart_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(truncate:SI
	 (lshiftrt:DI
	  (mult:DI
	   (sign_extend:DI (match_operand:SI 1 "s_register_operand" "r"))
	   (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r")))
	  (const_int 32))))
   (clobber (match_scratch:SI 3 "=r"))]
  "TARGET_32BIT && arm_arch6"
  "smull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)

//Pattern#117
(define_expand "umulsi3_highpart"
  [(parallel
    [(set (match_operand:SI 0 "s_register_operand" "")
	  (truncate:SI
	   (lshiftrt:DI
	    (mult:DI
	     (zero_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	      (zero_extend:DI (match_operand:SI 2 "s_register_operand" "")))
	    (const_int 32))))
     (clobber (match_scratch:SI 3 ""))])]
  "TARGET_32BIT && arm_arch3m"
  ""
)

//Pattern#118
(define_insn "*umulsi3_highpart_nov6"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r")
	(truncate:SI
	 (lshiftrt:DI
	  (mult:DI
	   (zero_extend:DI (match_operand:SI 1 "s_register_operand" "%0,r"))
	   (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r,r")))
	  (const_int 32))))
   (clobber (match_scratch:SI 3 "=&r,&r"))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)

//Pattern#119
(define_insn "*umulsi3_highpart_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(truncate:SI
	 (lshiftrt:DI
	  (mult:DI
	   (zero_extend:DI (match_operand:SI 1 "s_register_operand" "r"))
	   (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r")))
	  (const_int 32))))
   (clobber (match_scratch:SI 3 "=r"))]
  "TARGET_32BIT && arm_arch6"
  "umull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)

//Pattern#120
(define_insn "mulhisi3"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (sign_extend:SI
		  (match_operand:HI 1 "s_register_operand" "%r"))
		 (sign_extend:SI
		  (match_operand:HI 2 "s_register_operand" "r"))))]
  "TARGET_DSP_MULTIPLY"
  "smulbb%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)

//Pattern#121
(define_insn "*mulhisi3tb"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (ashiftrt:SI
		  (match_operand:SI 1 "s_register_operand" "r")
		  (const_int 16))
		 (sign_extend:SI
		  (match_operand:HI 2 "s_register_operand" "r"))))]
  "TARGET_DSP_MULTIPLY"
  "smultb%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)

//Pattern#122
(define_insn "*mulhisi3bt"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (sign_extend:SI
		  (match_operand:HI 1 "s_register_operand" "r"))
		 (ashiftrt:SI
		  (match_operand:SI 2 "s_register_operand" "r")
		  (const_int 16))))]
  "TARGET_DSP_MULTIPLY"
  "smulbt%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)

//Pattern#123
(define_insn "*mulhisi3tt"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (ashiftrt:SI
		  (match_operand:SI 1 "s_register_operand" "r")
		  (const_int 16))
		 (ashiftrt:SI
		  (match_operand:SI 2 "s_register_operand" "r")
		  (const_int 16))))]
  "TARGET_DSP_MULTIPLY"
  "smultt%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)

//Pattern#124
(define_insn "maddhisi4"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (mult:SI (sign_extend:SI
			   (match_operand:HI 1 "s_register_operand" "r"))
			  (sign_extend:SI
			   (match_operand:HI 2 "s_register_operand" "r")))
		 (match_operand:SI 3 "s_register_operand" "r")))]
  "TARGET_DSP_MULTIPLY"
  "smlabb%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)

;; Note: there is no maddhisi4ibt because this one is canonical form
//Pattern#125
(define_insn "*maddhisi4tb"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (mult:SI (ashiftrt:SI
			   (match_operand:SI 1 "s_register_operand" "r")
			   (const_int 16))
			  (sign_extend:SI
			   (match_operand:HI 2 "s_register_operand" "r")))
		 (match_operand:SI 3 "s_register_operand" "r")))]
  "TARGET_DSP_MULTIPLY"
  "smlatb%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)

//Pattern#126
(define_insn "*maddhisi4tt"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (mult:SI (ashiftrt:SI
			   (match_operand:SI 1 "s_register_operand" "r")
			   (const_int 16))
			  (ashiftrt:SI
			   (match_operand:SI 2 "s_register_operand" "r")
			   (const_int 16)))
		 (match_operand:SI 3 "s_register_operand" "r")))]
  "TARGET_DSP_MULTIPLY"
  "smlatt%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)

//Pattern#127
(define_insn "maddhidi4"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	  (mult:DI (sign_extend:DI
	 	    (match_operand:HI 1 "s_register_operand" "r"))
		   (sign_extend:DI
		    (match_operand:HI 2 "s_register_operand" "r")))
	  (match_operand:DI 3 "s_register_operand" "0")))]
  "TARGET_DSP_MULTIPLY"
  "smlalbb%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")])

;; Note: there is no maddhidi4ibt because this one is canonical form
//Pattern#128
(define_insn "*maddhidi4tb"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	  (mult:DI (sign_extend:DI
		    (ashiftrt:SI
		     (match_operand:SI 1 "s_register_operand" "r")
		     (const_int 16)))
		   (sign_extend:DI
		    (match_operand:HI 2 "s_register_operand" "r")))
	  (match_operand:DI 3 "s_register_operand" "0")))]
  "TARGET_DSP_MULTIPLY"
  "smlaltb%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")])

//Pattern#129
(define_insn "*maddhidi4tt"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	  (mult:DI (sign_extend:DI
		    (ashiftrt:SI
		     (match_operand:SI 1 "s_register_operand" "r")
		     (const_int 16)))
		   (sign_extend:DI
		    (ashiftrt:SI
		     (match_operand:SI 2 "s_register_operand" "r")
		     (const_int 16))))
	  (match_operand:DI 3 "s_register_operand" "0")))]
  "TARGET_DSP_MULTIPLY"
  "smlaltt%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")])

//Pattern#130
(define_expand "mulsf3"
  [(set (match_operand:SF          0 "s_register_operand" "")
	(mult:SF (match_operand:SF 1 "s_register_operand" "")
		 (match_operand:SF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#131
(define_expand "muldf3"
  [(set (match_operand:DF          0 "s_register_operand" "")
	(mult:DF (match_operand:DF 1 "s_register_operand" "")
		 (match_operand:DF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")

;; Division insns

//Pattern#132
(define_expand "divsf3"
  [(set (match_operand:SF 0 "s_register_operand" "")
	(div:SF (match_operand:SF 1 "s_register_operand" "")
		(match_operand:SF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  "")

//Pattern#133
(define_expand "divdf3"
  [(set (match_operand:DF 0 "s_register_operand" "")
	(div:DF (match_operand:DF 1 "s_register_operand" "")
		(match_operand:DF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  "")

;; Boolean and,ior,xor insns

;; Split up double word logical operations

;; Split up simple DImode logical operations.  Simply perform the logical
;; operation on the upper and lower halves of the registers.
//Pattern#134
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(match_operator:DI 6 "logical_binary_operator"
	  [(match_operand:DI 1 "s_register_operand" "")
	   (match_operand:DI 2 "s_register_operand" "")]))]
  "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))
   && ! IS_IWMMXT_REGNUM (REGNO (operands[0]))"
  [(set (match_dup 0) (match_op_dup:SI 6 [(match_dup 1) (match_dup 2)]))
   (set (match_dup 3) (match_op_dup:SI 6 [(match_dup 4) (match_dup 5)]))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
)

//Pattern#135
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(match_operator:DI 6 "logical_binary_operator"
	  [(sign_extend:DI (match_operand:SI 2 "s_register_operand" ""))
	   (match_operand:DI 1 "s_register_operand" "")]))]
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 0) (match_op_dup:SI 6 [(match_dup 1) (match_dup 2)]))
   (set (match_dup 3) (match_op_dup:SI 6
			[(ashiftrt:SI (match_dup 2) (const_int 31))
			 (match_dup 4)]))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
)

;; The zero extend of operand 2 means we can just copy the high part of
;; operand1 into operand0.
//Pattern#136
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(ior:DI
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" ""))
	  (match_operand:DI 1 "s_register_operand" "")))]
  "TARGET_32BIT && operands[0] != operands[1] && reload_completed"
  [(set (match_dup 0) (ior:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 3) (match_dup 4))]
  "
  {
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
)

;; The zero extend of operand 2 means we can just copy the high part of
;; operand1 into operand0.
//Pattern#137
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(xor:DI
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" ""))
	  (match_operand:DI 1 "s_register_operand" "")))]
  "TARGET_32BIT && operands[0] != operands[1] && reload_completed"
  [(set (match_dup 0) (xor:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 3) (match_dup 4))]
  "
  {
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
)

//Pattern#138
(define_expand "anddi3"
  [(set (match_operand:DI         0 "s_register_operand" "")
	(and:DI (match_operand:DI 1 "s_register_operand" "")
		(match_operand:DI 2 "neon_inv_logic_op2" "")))]
  "TARGET_32BIT"
  ""
)

//Pattern#139
(define_insn "*anddi3_insn"
  [(set (match_operand:DI         0 "s_register_operand" "=&r,&r")
	(and:DI (match_operand:DI 1 "s_register_operand"  "%0,r")
		(match_operand:DI 2 "s_register_operand"   "r,r")))]
  "TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON"
  "#"
  [(set_attr "length" "8")]
)

//Pattern#140
(define_insn_and_split "*anddi_zesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(and:DI (zero_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  ; The zero extend of operand 2 clears the high word of the output
  ; operand.
  [(set (match_dup 0) (and:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 3) (const_int 0))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
  [(set_attr "length" "8")]
)

//Pattern#141
(define_insn "*anddi_sesdi_di"
  [(set (match_operand:DI          0 "s_register_operand" "=&r,&r")
	(and:DI (sign_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI  1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  [(set_attr "length" "8")]
)

//Pattern#142
(define_expand "andsi3"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(and:SI (match_operand:SI 1 "s_register_operand" "")
		(match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT)
    {
      if (CONST_INT_P (operands[2]))
        {
	  if (INTVAL (operands[2]) == 255 && arm_arch6)
	    {
	      operands[1] = convert_to_mode (QImode, operands[1], 1);
	      emit_insn (gen_thumb2_zero_extendqisi2_v6 (operands[0],
							 operands[1]));
	    }
	  else
	    arm_split_constant (AND, SImode, NULL_RTX,
				INTVAL (operands[2]), operands[0],
				operands[1],
				optimize && can_create_pseudo_p ());

          DONE;
        }
    }
  else /* TARGET_THUMB1 */
    {
      if (!CONST_INT_P (operands[2]))
        {
          rtx tmp = force_reg (SImode, operands[2]);
	  if (rtx_equal_p (operands[0], operands[1]))
	    operands[2] = tmp;
	  else
	    {
              operands[2] = operands[1];
              operands[1] = tmp;
	    }
        }
      else
        {
          int i;
	  
          if (((unsigned HOST_WIDE_INT) ~INTVAL (operands[2])) < 256)
  	    {
	      operands[2] = force_reg (SImode,
				       GEN_INT (~INTVAL (operands[2])));
	      
	      emit_insn (gen_thumb1_bicsi3 (operands[0], operands[2], operands[1]));
	      
	      DONE;
	    }

          for (i = 9; i <= 31; i++)
	    {
	      if ((((HOST_WIDE_INT) 1) << i) - 1 == INTVAL (operands[2]))
	        {
	          emit_insn (gen_extzv (operands[0], operands[1], GEN_INT (i),
			 	        const0_rtx));
	          DONE;
	        }
	      else if ((((HOST_WIDE_INT) 1) << i) - 1
		       == ~INTVAL (operands[2]))
	        {
	          rtx shift = GEN_INT (i);
	          rtx reg = gen_reg_rtx (SImode);

	          emit_insn (gen_lshrsi3 (reg, operands[1], shift));
	          emit_insn (gen_ashlsi3 (operands[0], reg, shift));
		  
	          DONE;
	        }
	    }

          operands[2] = force_reg (SImode, operands[2]);
        }
    }
  "
)

; ??? Check split length for Thumb-2
//Pattern#143
(define_insn_and_split "*arm_andsi3_insn"
  [(set (match_operand:SI         0 "s_register_operand" "=r,r,r,r")
	(and:SI (match_operand:SI 1 "s_register_operand" "r,r,r,r")
		(match_operand:SI 2 "reg_or_int_operand" "I,K,r,?n")))]
  "TARGET_32BIT"
  "@
   and%?\\t%0, %1, %2
   bic%?\\t%0, %1, #%B2
   and%?\\t%0, %1, %2
   #"
  "TARGET_32BIT
   && CONST_INT_P (operands[2])
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2])))"
  [(clobber (const_int 0))]
  "
  arm_split_constant  (AND, SImode, curr_insn, 
	               INTVAL (operands[2]), operands[0], operands[1], 0);
  DONE;
  "
  [(set_attr "length" "4,4,4,16")
   (set_attr "predicable" "yes")
   (set_attr "type" 	"simple_alu_imm,simple_alu_imm,*,simple_alu_imm")]
)

//Pattern#144
(define_insn "*thumb1_andsi3_insn"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(and:SI (match_operand:SI 1 "register_operand" "%0")
		(match_operand:SI 2 "register_operand" "l")))]
  "TARGET_THUMB1"
  "and\\t%0, %2"
  [(set_attr "length" "2")
   (set_attr "type"  "simple_alu_imm")
   (set_attr "conds" "set")])

//Pattern#145
(define_insn "*andsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (and:SI (match_operand:SI 1 "s_register_operand" "r,r,r")
		 (match_operand:SI 2 "arm_not_operand" "I,K,r"))
	 (const_int 0)))
   (set (match_operand:SI          0 "s_register_operand" "=r,r,r")
	(and:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "@
   and%.\\t%0, %1, %2
   bic%.\\t%0, %1, #%B2
   and%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*")]
)

//Pattern#146
(define_insn "*andsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (and:SI (match_operand:SI 0 "s_register_operand" "r,r,r")
		 (match_operand:SI 1 "arm_not_operand" "I,K,r"))
	 (const_int 0)))
   (clobber (match_scratch:SI 2 "=X,r,X"))]
  "TARGET_32BIT"
  "@
   tst%?\\t%0, %1
   bic%.\\t%2, %0, #%B1
   tst%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "type"  "simple_alu_imm,simple_alu_imm,*")]
)

//Pattern#147
(define_insn "*zeroextractsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (zero_extract:SI
			  (match_operand:SI 0 "s_register_operand" "r")
		 	  (match_operand 1 "const_int_operand" "n")
			  (match_operand 2 "const_int_operand" "n"))
			 (const_int 0)))]
  "TARGET_32BIT
  && (INTVAL (operands[2]) >= 0 && INTVAL (operands[2]) < 32
      && INTVAL (operands[1]) > 0 
      && INTVAL (operands[1]) + (INTVAL (operands[2]) & 1) <= 8
      && INTVAL (operands[1]) + INTVAL (operands[2]) <= 32)"
  "*
  operands[1] = GEN_INT (((1 << INTVAL (operands[1])) - 1)
			 << INTVAL (operands[2]));
  output_asm_insn (\"tst%?\\t%0, %1\", operands);
  return \"\";
  "
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm")]
)

//Pattern#148
(define_insn_and_split "*ne_zeroextractsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(ne:SI (zero_extract:SI
		(match_operand:SI 1 "s_register_operand" "r")
		(match_operand:SI 2 "const_int_operand" "n")
		(match_operand:SI 3 "const_int_operand" "n"))
	       (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)"
  "#"
  "TARGET_32BIT
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)"
  [(parallel [(set (reg:CC_NOOV CC_REGNUM)
		   (compare:CC_NOOV (and:SI (match_dup 1) (match_dup 2))
				    (const_int 0)))
	      (set (match_dup 0) (and:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 0)
	(if_then_else:SI (eq (reg:CC_NOOV CC_REGNUM) (const_int 0))
			 (match_dup 0) (const_int 1)))]
  "
  operands[2] = GEN_INT (((1 << INTVAL (operands[2])) - 1)
			 << INTVAL (operands[3])); 
  "
  [(set_attr "conds" "clob")
   (set (attr "length")
	(if_then_else (eq_attr "is_thumb" "yes")
		      (const_int 12)
		      (const_int 8)))]
)

//Pattern#149
(define_insn_and_split "*ne_zeroextractsi_shifted"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(ne:SI (zero_extract:SI
		(match_operand:SI 1 "s_register_operand" "r")
		(match_operand:SI 2 "const_int_operand" "n")
		(const_int 0))
	       (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  "TARGET_ARM"
  [(parallel [(set (reg:CC_NOOV CC_REGNUM)
		   (compare:CC_NOOV (ashift:SI (match_dup 1) (match_dup 2))
				    (const_int 0)))
	      (set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 0)
	(if_then_else:SI (eq (reg:CC_NOOV CC_REGNUM) (const_int 0))
			 (match_dup 0) (const_int 1)))]
  "
  operands[2] = GEN_INT (32 - INTVAL (operands[2]));
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#150
(define_insn_and_split "*ite_ne_zeroextractsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI (ne (zero_extract:SI
			      (match_operand:SI 1 "s_register_operand" "r")
			      (match_operand:SI 2 "const_int_operand" "n")
			      (match_operand:SI 3 "const_int_operand" "n"))
			     (const_int 0))
			 (match_operand:SI 4 "arm_not_operand" "rIK")
			 (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)
   && !reg_overlap_mentioned_p (operands[0], operands[4])"
  "#"
  "TARGET_ARM
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)
   && !reg_overlap_mentioned_p (operands[0], operands[4])"
  [(parallel [(set (reg:CC_NOOV CC_REGNUM)
		   (compare:CC_NOOV (and:SI (match_dup 1) (match_dup 2))
				    (const_int 0)))
	      (set (match_dup 0) (and:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 0)
	(if_then_else:SI (eq (reg:CC_NOOV CC_REGNUM) (const_int 0))
			 (match_dup 0) (match_dup 4)))]
  "
  operands[2] = GEN_INT (((1 << INTVAL (operands[2])) - 1)
			 << INTVAL (operands[3])); 
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#151
(define_insn_and_split "*ite_ne_zeroextractsi_shifted"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI (ne (zero_extract:SI
			      (match_operand:SI 1 "s_register_operand" "r")
			      (match_operand:SI 2 "const_int_operand" "n")
			      (const_int 0))
			     (const_int 0))
			 (match_operand:SI 3 "arm_not_operand" "rIK")
			 (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM && !reg_overlap_mentioned_p (operands[0], operands[3])"
  "#"
  "TARGET_ARM && !reg_overlap_mentioned_p (operands[0], operands[3])"
  [(parallel [(set (reg:CC_NOOV CC_REGNUM)
		   (compare:CC_NOOV (ashift:SI (match_dup 1) (match_dup 2))
				    (const_int 0)))
	      (set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 0)
	(if_then_else:SI (eq (reg:CC_NOOV CC_REGNUM) (const_int 0))
			 (match_dup 0) (match_dup 3)))]
  "
  operands[2] = GEN_INT (32 - INTVAL (operands[2]));
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#152
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extract:SI (match_operand:SI 1 "s_register_operand" "")
			 (match_operand:SI 2 "const_int_operand" "")
			 (match_operand:SI 3 "const_int_operand" "")))
   (clobber (match_operand:SI 4 "s_register_operand" ""))]
  "TARGET_THUMB1"
  [(set (match_dup 4) (ashift:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (lshiftrt:SI (match_dup 4) (match_dup 3)))]
  "{
     HOST_WIDE_INT temp = INTVAL (operands[2]);

     operands[2] = GEN_INT (32 - temp - INTVAL (operands[3]));
     operands[3] = GEN_INT (32 - temp);
   }"
)

;; ??? Use Thumb-2 has bitfield insert/extract instructions.
//Pattern#153
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "shiftable_operator"
	 [(zero_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))
	  (match_operand:SI 5 "s_register_operand" "")]))
   (clobber (match_operand:SI 6 "s_register_operand" ""))]
  "TARGET_ARM"
  [(set (match_dup 6) (ashift:SI (match_dup 2) (match_dup 3)))
   (set (match_dup 0)
	(match_op_dup 1
	 [(lshiftrt:SI (match_dup 6) (match_dup 4))
	  (match_dup 5)]))]
  "{
     HOST_WIDE_INT temp = INTVAL (operands[3]);

     operands[3] = GEN_INT (32 - temp - INTVAL (operands[4]));
     operands[4] = GEN_INT (32 - temp);
   }"
)
  
//Pattern#154
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(sign_extract:SI (match_operand:SI 1 "s_register_operand" "")
			 (match_operand:SI 2 "const_int_operand" "")
			 (match_operand:SI 3 "const_int_operand" "")))]
  "TARGET_THUMB1"
  [(set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (match_dup 3)))]
  "{
     HOST_WIDE_INT temp = INTVAL (operands[2]);

     operands[2] = GEN_INT (32 - temp - INTVAL (operands[3]));
     operands[3] = GEN_INT (32 - temp);
   }"
)

//Pattern#155
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "shiftable_operator"
	 [(sign_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))
	  (match_operand:SI 5 "s_register_operand" "")]))
   (clobber (match_operand:SI 6 "s_register_operand" ""))]
  "TARGET_ARM"
  [(set (match_dup 6) (ashift:SI (match_dup 2) (match_dup 3)))
   (set (match_dup 0)
	(match_op_dup 1
	 [(ashiftrt:SI (match_dup 6) (match_dup 4))
	  (match_dup 5)]))]
  "{
     HOST_WIDE_INT temp = INTVAL (operands[3]);

     operands[3] = GEN_INT (32 - temp - INTVAL (operands[4]));
     operands[4] = GEN_INT (32 - temp);
   }"
)
  
;;; ??? This pattern is bogus.  If operand3 has bits outside the range
;;; represented by the bitfield, then this will produce incorrect results.
;;; Somewhere, the value needs to be truncated.  On targets like the m68k,
;;; which have a real bit-field insert instruction, the truncation happens
;;; in the bit-field insert instruction itself.  Since arm does not have a
;;; bit-field insert instruction, we would have to emit code here to truncate
;;; the value before we insert.  This loses some of the advantage of having
;;; this insv pattern, so this pattern needs to be reevalutated.

//Pattern#156
(define_expand "insv"
  [(set (zero_extract (match_operand 0 "nonimmediate_operand" "")
                      (match_operand 1 "general_operand" "")
                      (match_operand 2 "general_operand" ""))
        (match_operand 3 "reg_or_int_operand" ""))]
  "TARGET_ARM || arm_arch_thumb2"
  "
  {
    int start_bit = INTVAL (operands[2]);
    int width = INTVAL (operands[1]);
    HOST_WIDE_INT mask = (((HOST_WIDE_INT)1) << width) - 1;
    rtx target, subtarget;

    if (arm_arch_thumb2)
      {
        if (unaligned_access && MEM_P (operands[0])
	    && s_register_operand (operands[3], GET_MODE (operands[3]))
	    && (width == 16 || width == 32) && (start_bit % BITS_PER_UNIT) == 0)
	  {
	    rtx base_addr;

	    if (BYTES_BIG_ENDIAN)
	      start_bit = GET_MODE_BITSIZE (GET_MODE (operands[3])) - width
			  - start_bit;

	    if (width == 32)
	      {
	        base_addr = adjust_address (operands[0], SImode,
					    start_bit / BITS_PER_UNIT);
		emit_insn (gen_unaligned_storesi (base_addr, operands[3]));
	      }
	    else
	      {
	        rtx tmp = gen_reg_rtx (HImode);

	        base_addr = adjust_address (operands[0], HImode,
					    start_bit / BITS_PER_UNIT);
		emit_move_insn (tmp, gen_lowpart (HImode, operands[3]));
		emit_insn (gen_unaligned_storehi (base_addr, tmp));
	      }
	    DONE;
	  }
	else if (s_register_operand (operands[0], GET_MODE (operands[0])))
	  {
	    bool use_bfi = TRUE;

	    if (CONST_INT_P (operands[3]))
	      {
		HOST_WIDE_INT val = INTVAL (operands[3]) & mask;

		if (val == 0)
		  {
		    emit_insn (gen_insv_zero (operands[0], operands[1],
					      operands[2]));
		    DONE;
		  }

		/* See if the set can be done with a single orr instruction.  */
		if (val == mask && const_ok_for_arm (val << start_bit))
		  use_bfi = FALSE;
	      }

	    if (use_bfi)
	      {
		if (!REG_P (operands[3]))
		  operands[3] = force_reg (SImode, operands[3]);

		emit_insn (gen_insv_t2 (operands[0], operands[1], operands[2],
					operands[3]));
		DONE;
	      }
	  }
	else
	  FAIL;
      }

    if (!s_register_operand (operands[0], GET_MODE (operands[0])))
      FAIL;

    target = copy_rtx (operands[0]);
    /* Avoid using a subreg as a subtarget, and avoid writing a paradoxical 
       subreg as the final target.  */
    if (GET_CODE (target) == SUBREG)
      {
	subtarget = gen_reg_rtx (SImode);
	if (GET_MODE_SIZE (GET_MODE (SUBREG_REG (target)))
	    < GET_MODE_SIZE (SImode))
	  target = SUBREG_REG (target);
      }
    else
      subtarget = target;    

    if (CONST_INT_P (operands[3]))
      {
	/* Since we are inserting a known constant, we may be able to
	   reduce the number of bits that we have to clear so that
	   the mask becomes simple.  */
	/* ??? This code does not check to see if the new mask is actually
	   simpler.  It may not be.  */
	rtx op1 = gen_reg_rtx (SImode);
	/* ??? Truncate operand3 to fit in the bitfield.  See comment before
	   start of this pattern.  */
	HOST_WIDE_INT op3_value = mask & INTVAL (operands[3]);
	HOST_WIDE_INT mask2 = ((mask & ~op3_value) << start_bit);

	emit_insn (gen_andsi3 (op1, operands[0],
			       gen_int_mode (~mask2, SImode)));
	emit_insn (gen_iorsi3 (subtarget, op1,
			       gen_int_mode (op3_value << start_bit, SImode)));
      }
    else if (start_bit == 0
	     && !(const_ok_for_arm (mask)
		  || const_ok_for_arm (~mask)))
      {
	/* A Trick, since we are setting the bottom bits in the word,
	   we can shift operand[3] up, operand[0] down, OR them together
	   and rotate the result back again.  This takes 3 insns, and
	   the third might be mergeable into another op.  */
	/* The shift up copes with the possibility that operand[3] is
           wider than the bitfield.  */
	rtx op0 = gen_reg_rtx (SImode);
	rtx op1 = gen_reg_rtx (SImode);

	emit_insn (gen_ashlsi3 (op0, operands[3], GEN_INT (32 - width)));
	emit_insn (gen_lshrsi3 (op1, operands[0], operands[1]));
	emit_insn (gen_iorsi3  (op1, op1, op0));
	emit_insn (gen_rotlsi3 (subtarget, op1, operands[1]));
      }
    else if ((width + start_bit == 32)
	     && !(const_ok_for_arm (mask)
		  || const_ok_for_arm (~mask)))
      {
	/* Similar trick, but slightly less efficient.  */

	rtx op0 = gen_reg_rtx (SImode);
	rtx op1 = gen_reg_rtx (SImode);

	emit_insn (gen_ashlsi3 (op0, operands[3], GEN_INT (32 - width)));
	emit_insn (gen_ashlsi3 (op1, operands[0], operands[1]));
	emit_insn (gen_lshrsi3 (op1, op1, operands[1]));
	emit_insn (gen_iorsi3 (subtarget, op1, op0));
      }
    else
      {
	rtx op0 = gen_int_mode (mask, SImode);
	rtx op1 = gen_reg_rtx (SImode);
	rtx op2 = gen_reg_rtx (SImode);

	if (!(const_ok_for_arm (mask) || const_ok_for_arm (~mask)))
	  {
	    rtx tmp = gen_reg_rtx (SImode);

	    emit_insn (gen_movsi (tmp, op0));
	    op0 = tmp;
	  }

	/* Mask out any bits in operand[3] that are not needed.  */
	   emit_insn (gen_andsi3 (op1, operands[3], op0));

	if (CONST_INT_P (op0)
	    && (const_ok_for_arm (mask << start_bit)
		|| const_ok_for_arm (~(mask << start_bit))))
	  {
	    op0 = gen_int_mode (~(mask << start_bit), SImode);
	    emit_insn (gen_andsi3 (op2, operands[0], op0));
	  }
	else
	  {
	    if (CONST_INT_P (op0))
	      {
		rtx tmp = gen_reg_rtx (SImode);

		emit_insn (gen_movsi (tmp, op0));
		op0 = tmp;
	      }

	    if (start_bit != 0)
	      emit_insn (gen_ashlsi3 (op0, op0, operands[2]));
	    
	    emit_insn (gen_andsi_notsi_si (op2, operands[0], op0));
	  }

	if (start_bit != 0)
          emit_insn (gen_ashlsi3 (op1, op1, operands[2]));

	emit_insn (gen_iorsi3 (subtarget, op1, op2));
      }

    if (subtarget != target)
      {
	/* If TARGET is still a SUBREG, then it must be wider than a word,
	   so we must be careful only to set the subword we were asked to.  */
	if (GET_CODE (target) == SUBREG)
	  emit_move_insn (target, subtarget);
	else
	  emit_move_insn (target, gen_lowpart (GET_MODE (target), subtarget));
      }

    DONE;
  }"
)

//Pattern#157
(define_insn "insv_zero"
  [(set (zero_extract:SI (match_operand:SI 0 "s_register_operand" "+r")
                         (match_operand:SI 1 "const_int_operand" "M")
                         (match_operand:SI 2 "const_int_operand" "M"))
        (const_int 0))]
  "arm_arch_thumb2"
  "bfc%?\t%0, %2, %1"
  [(set_attr "length" "4")
   (set_attr "predicable" "yes")]
)

//Pattern#158
(define_insn "insv_t2"
  [(set (zero_extract:SI (match_operand:SI 0 "s_register_operand" "+r")
                         (match_operand:SI 1 "const_int_operand" "M")
                         (match_operand:SI 2 "const_int_operand" "M"))
        (match_operand:SI 3 "s_register_operand" "r"))]
  "arm_arch_thumb2"
  "bfi%?\t%0, %3, %2, %1"
  [(set_attr "length" "4")
   (set_attr "predicable" "yes")]
)

; constants for op 2 will never be given to these patterns.
//Pattern#159
(define_insn_and_split "*anddi_notdi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(and:DI (not:DI (match_operand:DI 1 "s_register_operand" "0,r"))
		(match_operand:DI 2 "s_register_operand" "r,0")))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))
   && ! IS_IWMMXT_REGNUM (REGNO (operands[0]))"
  [(set (match_dup 0) (and:SI (not:SI (match_dup 1)) (match_dup 2)))
   (set (match_dup 3) (and:SI (not:SI (match_dup 4)) (match_dup 5)))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)
  
//Pattern#160
(define_insn_and_split "*anddi_notzesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(and:DI (not:DI (zero_extend:DI
			 (match_operand:SI 2 "s_register_operand" "r,r")))
		(match_operand:DI 1 "s_register_operand" "0,?r")))]
  "TARGET_32BIT"
  "@
   bic%?\\t%Q0, %Q1, %2
   #"
  ; (not (zero_extend ...)) allows us to just copy the high word from
  ; operand1 to operand0.
  "TARGET_32BIT
   && reload_completed
   && operands[0] != operands[1]"
  [(set (match_dup 0) (and:SI (not:SI (match_dup 2)) (match_dup 1)))
   (set (match_dup 3) (match_dup 4))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
  [(set_attr "length" "4,8")
   (set_attr "predicable" "yes")]
)
  
//Pattern#161
(define_insn_and_split "*anddi_notsesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(and:DI (not:DI (sign_extend:DI
			 (match_operand:SI 2 "s_register_operand" "r,r")))
		(match_operand:DI 1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 0) (and:SI (not:SI (match_dup 2)) (match_dup 1)))
   (set (match_dup 3) (and:SI (not:SI
				(ashiftrt:SI (match_dup 2) (const_int 31)))
			       (match_dup 4)))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)
  
//Pattern#162
(define_insn "andsi_notsi_si"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(and:SI (not:SI (match_operand:SI 2 "s_register_operand" "r"))
		(match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_32BIT"
  "bic%?\\t%0, %1, %2"
  [(set_attr "predicable" "yes")]
)

//Pattern#163
(define_insn "thumb1_bicsi3"
  [(set (match_operand:SI                 0 "register_operand" "=l")
	(and:SI (not:SI (match_operand:SI 1 "register_operand" "l"))
		(match_operand:SI         2 "register_operand" "0")))]
  "TARGET_THUMB1"
  "bic\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#164
(define_insn "andsi_not_shiftsi_si"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(and:SI (not:SI (match_operator:SI 4 "shift_operator"
			 [(match_operand:SI 2 "s_register_operand" "r")
			  (match_operand:SI 3 "arm_rhs_operand" "rM")]))
		(match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_ARM"
  "bic%?\\t%0, %1, %2%S4"
  [(set_attr "predicable" "yes")
   (set_attr "shift" "2")
   (set (attr "type") (if_then_else (match_operand 3 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#165
(define_insn "*andsi_notsi_si_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (and:SI (not:SI (match_operand:SI 2 "s_register_operand" "r"))
		 (match_operand:SI 1 "s_register_operand" "r"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(and:SI (not:SI (match_dup 2)) (match_dup 1)))]
  "TARGET_32BIT"
  "bic%.\\t%0, %1, %2"
  [(set_attr "conds" "set")]
)

//Pattern#166
(define_insn "*andsi_notsi_si_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (and:SI (not:SI (match_operand:SI 2 "s_register_operand" "r"))
		 (match_operand:SI 1 "s_register_operand" "r"))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_32BIT"
  "bic%.\\t%0, %1, %2"
  [(set_attr "conds" "set")]
)

//Pattern#167
(define_expand "iordi3"
  [(set (match_operand:DI         0 "s_register_operand" "")
	(ior:DI (match_operand:DI 1 "s_register_operand" "")
		(match_operand:DI 2 "neon_logic_op2" "")))]
  "TARGET_32BIT"
  ""
)

//Pattern#168
(define_insn "*iordi3_insn"
  [(set (match_operand:DI         0 "s_register_operand" "=&r,&r")
	(ior:DI (match_operand:DI 1 "s_register_operand"  "%0,r")
		(match_operand:DI 2 "s_register_operand"   "r,r")))]
  "TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON"
  "#"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)

//Pattern#169
(define_insn "*iordi_zesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(ior:DI (zero_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,?r")))]
  "TARGET_32BIT"
  "@
   orr%?\\t%Q0, %Q1, %2
   #"
  [(set_attr "length" "4,8")
   (set_attr "predicable" "yes")]
)

//Pattern#170
(define_insn "*iordi_sesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(ior:DI (sign_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)

//Pattern#171
(define_expand "iorsi3"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(ior:SI (match_operand:SI 1 "s_register_operand" "")
		(match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[2]))
    {
      if (TARGET_32BIT)
        {
          arm_split_constant (IOR, SImode, NULL_RTX,
	                      INTVAL (operands[2]), operands[0], operands[1],
			      optimize && can_create_pseudo_p ());
          DONE;
	}
      else /* TARGET_THUMB1 */
        {
          rtx tmp = force_reg (SImode, operands[2]);
	  if (rtx_equal_p (operands[0], operands[1]))
	    operands[2] = tmp;
	  else
	    {
              operands[2] = operands[1];
              operands[1] = tmp;
	    }
        }
    }
  "
)

//Pattern#172
(define_insn_and_split "*iorsi3_insn"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r,r")
	(ior:SI (match_operand:SI 1 "s_register_operand" "%r,r,r,r")
		(match_operand:SI 2 "reg_or_int_operand" "I,K,r,?n")))]
  "TARGET_32BIT"
  "@
   orr%?\\t%0, %1, %2
   orn%?\\t%0, %1, #%B2
   orr%?\\t%0, %1, %2
   #"
  "TARGET_32BIT
   && CONST_INT_P (operands[2])
   && !(const_ok_for_arm (INTVAL (operands[2]))
        || (TARGET_THUMB2 && const_ok_for_arm (~INTVAL (operands[2]))))"
  [(clobber (const_int 0))]
{
  arm_split_constant (IOR, SImode, curr_insn, 
                      INTVAL (operands[2]), operands[0], operands[1], 0);
  DONE;
}
  [(set_attr "length" "4,4,4,16")
   (set_attr "arch" "32,t2,32,32")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*,*")]
)

//Pattern#173
(define_insn "*thumb1_iorsi3_insn"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(ior:SI (match_operand:SI 1 "register_operand" "%0")
		(match_operand:SI 2 "register_operand" "l")))]
  "TARGET_THUMB1"
  "orr\\t%0, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#174
(define_peephole2
  [(match_scratch:SI 3 "r")
   (set (match_operand:SI 0 "arm_general_register_operand" "")
	(ior:SI (match_operand:SI 1 "arm_general_register_operand" "")
		(match_operand:SI 2 "const_int_operand" "")))]
  "TARGET_ARM
   && !const_ok_for_arm (INTVAL (operands[2]))
   && const_ok_for_arm (~INTVAL (operands[2]))"
  [(set (match_dup 3) (match_dup 2))
   (set (match_dup 0) (ior:SI (match_dup 1) (match_dup 3)))]
  ""
)

//Pattern#175
(define_insn "*iorsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (ior:SI (match_operand:SI 1 "s_register_operand" "%r,r")
				 (match_operand:SI 2 "arm_rhs_operand" "I,r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(ior:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "orr%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,*")]
)

//Pattern#176
(define_insn "*iorsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (ior:SI (match_operand:SI 1 "s_register_operand" "%r,r")
				 (match_operand:SI 2 "arm_rhs_operand" "I,r"))
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r,r"))]
  "TARGET_32BIT"
  "orr%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm, *")]
)

//Pattern#177
(define_expand "xordi3"
  [(set (match_operand:DI         0 "s_register_operand" "")
	(xor:DI (match_operand:DI 1 "s_register_operand" "")
		(match_operand:DI 2 "s_register_operand" "")))]
  "TARGET_32BIT"
  ""
)

//Pattern#178
(define_insn "*xordi3_insn"
  [(set (match_operand:DI         0 "s_register_operand" "=&r,&r")
	(xor:DI (match_operand:DI 1 "s_register_operand"  "%0,r")
		(match_operand:DI 2 "s_register_operand"   "r,r")))]
  "TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON"
  "#"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)

//Pattern#179
(define_insn "*xordi_zesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(xor:DI (zero_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,?r")))]
  "TARGET_32BIT"
  "@
   eor%?\\t%Q0, %Q1, %2
   #"
  [(set_attr "length" "4,8")
   (set_attr "predicable" "yes")]
)

//Pattern#180
(define_insn "*xordi_sesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(xor:DI (sign_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)

//Pattern#181
(define_expand "xorsi3"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(xor:SI (match_operand:SI 1 "s_register_operand" "")
		(match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_EITHER"
  "if (CONST_INT_P (operands[2]))
    {
      if (TARGET_32BIT)
        {
          arm_split_constant (XOR, SImode, NULL_RTX,
	                      INTVAL (operands[2]), operands[0], operands[1],
			      optimize && can_create_pseudo_p ());
          DONE;
	}
      else /* TARGET_THUMB1 */
        {
          rtx tmp = force_reg (SImode, operands[2]);
	  if (rtx_equal_p (operands[0], operands[1]))
	    operands[2] = tmp;
	  else
	    {
              operands[2] = operands[1];
              operands[1] = tmp;
	    }
        }
    }"
)

//Pattern#182
(define_insn_and_split "*arm_xorsi3"
  [(set (match_operand:SI         0 "s_register_operand" "=r,r,r")
	(xor:SI (match_operand:SI 1 "s_register_operand" "%r,r,r")
		(match_operand:SI 2 "reg_or_int_operand" "I,r,?n")))]
  "TARGET_32BIT"
  "@
   eor%?\\t%0, %1, %2
   eor%?\\t%0, %1, %2
   #"
  "TARGET_32BIT
   && CONST_INT_P (operands[2])
   && !const_ok_for_arm (INTVAL (operands[2]))"
  [(clobber (const_int 0))]
{
  arm_split_constant (XOR, SImode, curr_insn,
                      INTVAL (operands[2]), operands[0], operands[1], 0);
  DONE;
}
  [(set_attr "length" "4,4,16")
   (set_attr "predicable" "yes")
   (set_attr "type"  "simple_alu_imm,*,*")]
)

//Pattern#183
(define_insn "*thumb1_xorsi3_insn"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(xor:SI (match_operand:SI 1 "register_operand" "%0")
		(match_operand:SI 2 "register_operand" "l")))]
  "TARGET_THUMB1"
  "eor\\t%0, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm")]
)

//Pattern#184
(define_insn "*xorsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (xor:SI (match_operand:SI 1 "s_register_operand" "r,r")
				 (match_operand:SI 2 "arm_rhs_operand" "I,r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(xor:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "eor%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,*")]
)

//Pattern#185
(define_insn "*xorsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (xor:SI (match_operand:SI 0 "s_register_operand" "r,r")
				 (match_operand:SI 1 "arm_rhs_operand" "I,r"))
			 (const_int 0)))]
  "TARGET_32BIT"
  "teq%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm, *")]
)

; By splitting (IOR (AND (NOT A) (NOT B)) C) as D = AND (IOR A B) (NOT C), 
; (NOT D) we can sometimes merge the final NOT into one of the following
; insns.

//Pattern#186
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(ior:SI (and:SI (not:SI (match_operand:SI 1 "s_register_operand" ""))
			(not:SI (match_operand:SI 2 "arm_rhs_operand" "")))
		(match_operand:SI 3 "arm_rhs_operand" "")))
   (clobber (match_operand:SI 4 "s_register_operand" ""))]
  "TARGET_32BIT"
  [(set (match_dup 4) (and:SI (ior:SI (match_dup 1) (match_dup 2))
			      (not:SI (match_dup 3))))
   (set (match_dup 0) (not:SI (match_dup 4)))]
  ""
)

//Pattern#187
(define_insn "*andsi_iorsi3_notsi"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r")
	(and:SI (ior:SI (match_operand:SI 1 "s_register_operand" "%0,r,r")
			(match_operand:SI 2 "arm_rhs_operand" "rI,0,rI"))
		(not:SI (match_operand:SI 3 "arm_rhs_operand" "rI,rI,rI"))))]
  "TARGET_32BIT"
  "orr%?\\t%0, %1, %2\;bic%?\\t%0, %0, %3"
  [(set_attr "length" "8")
   (set_attr "ce_count" "2")
   (set_attr "predicable" "yes")]
)

; ??? Are these four splitters still beneficial when the Thumb-2 bitfield
; insns are available?
//Pattern#188
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "logical_binary_operator"
	 [(zero_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))
	  (match_operator:SI 9 "logical_binary_operator"
	   [(lshiftrt:SI (match_operand:SI 5 "s_register_operand" "")
			 (match_operand:SI 6 "const_int_operand" ""))
	    (match_operand:SI 7 "s_register_operand" "")])]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])"
  [(set (match_dup 8)
	(match_op_dup 1
	 [(ashift:SI (match_dup 2) (match_dup 4))
	  (match_dup 5)]))
   (set (match_dup 0)
	(match_op_dup 1
	 [(lshiftrt:SI (match_dup 8) (match_dup 6))
	  (match_dup 7)]))]
  "
  operands[4] = GEN_INT (32 - (INTVAL (operands[3]) + INTVAL (operands[4])));
")

//Pattern#189
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "logical_binary_operator"
	 [(match_operator:SI 9 "logical_binary_operator"
	   [(lshiftrt:SI (match_operand:SI 5 "s_register_operand" "")
			 (match_operand:SI 6 "const_int_operand" ""))
	    (match_operand:SI 7 "s_register_operand" "")])
	  (zero_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])"
  [(set (match_dup 8)
	(match_op_dup 1
	 [(ashift:SI (match_dup 2) (match_dup 4))
	  (match_dup 5)]))
   (set (match_dup 0)
	(match_op_dup 1
	 [(lshiftrt:SI (match_dup 8) (match_dup 6))
	  (match_dup 7)]))]
  "
  operands[4] = GEN_INT (32 - (INTVAL (operands[3]) + INTVAL (operands[4])));
")

//Pattern#190
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "logical_binary_operator"
	 [(sign_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))
	  (match_operator:SI 9 "logical_binary_operator"
	   [(ashiftrt:SI (match_operand:SI 5 "s_register_operand" "")
			 (match_operand:SI 6 "const_int_operand" ""))
	    (match_operand:SI 7 "s_register_operand" "")])]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])"
  [(set (match_dup 8)
	(match_op_dup 1
	 [(ashift:SI (match_dup 2) (match_dup 4))
	  (match_dup 5)]))
   (set (match_dup 0)
	(match_op_dup 1
	 [(ashiftrt:SI (match_dup 8) (match_dup 6))
	  (match_dup 7)]))]
  "
  operands[4] = GEN_INT (32 - (INTVAL (operands[3]) + INTVAL (operands[4])));
")

//Pattern#191
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "logical_binary_operator"
	 [(match_operator:SI 9 "logical_binary_operator"
	   [(ashiftrt:SI (match_operand:SI 5 "s_register_operand" "")
			 (match_operand:SI 6 "const_int_operand" ""))
	    (match_operand:SI 7 "s_register_operand" "")])
	  (sign_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])"
  [(set (match_dup 8)
	(match_op_dup 1
	 [(ashift:SI (match_dup 2) (match_dup 4))
	  (match_dup 5)]))
   (set (match_dup 0)
	(match_op_dup 1
	 [(ashiftrt:SI (match_dup 8) (match_dup 6))
	  (match_dup 7)]))]
  "
  operands[4] = GEN_INT (32 - (INTVAL (operands[3]) + INTVAL (operands[4])));
")


;; Minimum and maximum insns

//Pattern#192
(define_expand "smaxsi3"
  [(parallel [
    (set (match_operand:SI 0 "s_register_operand" "")
	 (smax:SI (match_operand:SI 1 "s_register_operand" "")
		  (match_operand:SI 2 "arm_rhs_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_32BIT"
  "
  if (operands[2] == const0_rtx || operands[2] == constm1_rtx)
    {
      /* No need for a clobber of the condition code register here.  */
      emit_insn (gen_rtx_SET (VOIDmode, operands[0],
			      gen_rtx_SMAX (SImode, operands[1],
					    operands[2])));
      DONE;
    }
")

//Pattern#193
(define_insn "*smax_0"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(smax:SI (match_operand:SI 1 "s_register_operand" "r")
		 (const_int 0)))]
  "TARGET_32BIT"
  "bic%?\\t%0, %1, %1, asr #31"
  [(set_attr "predicable" "yes")]
)

//Pattern#194
(define_insn "*smax_m1"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(smax:SI (match_operand:SI 1 "s_register_operand" "r")
		 (const_int -1)))]
  "TARGET_32BIT"
  "orr%?\\t%0, %1, %1, asr #31"
  [(set_attr "predicable" "yes")]
)

//Pattern#195
(define_insn "*arm_smax_insn"
  [(set (match_operand:SI          0 "s_register_operand" "=r,r")
	(smax:SI (match_operand:SI 1 "s_register_operand"  "%0,?r")
		 (match_operand:SI 2 "arm_rhs_operand"    "rI,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%1, %2\;movlt\\t%0, %2
   cmp\\t%1, %2\;movge\\t%0, %1\;movlt\\t%0, %2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#196
(define_expand "sminsi3"
  [(parallel [
    (set (match_operand:SI 0 "s_register_operand" "")
	 (smin:SI (match_operand:SI 1 "s_register_operand" "")
		  (match_operand:SI 2 "arm_rhs_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_32BIT"
  "
  if (operands[2] == const0_rtx)
    {
      /* No need for a clobber of the condition code register here.  */
      emit_insn (gen_rtx_SET (VOIDmode, operands[0],
			      gen_rtx_SMIN (SImode, operands[1],
					    operands[2])));
      DONE;
    }
")

//Pattern#197
(define_insn "*smin_0"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(smin:SI (match_operand:SI 1 "s_register_operand" "r")
		 (const_int 0)))]
  "TARGET_32BIT"
  "and%?\\t%0, %1, %1, asr #31"
  [(set_attr "predicable" "yes")]
)

//Pattern#198
(define_insn "*arm_smin_insn"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(smin:SI (match_operand:SI 1 "s_register_operand" "%0,?r")
		 (match_operand:SI 2 "arm_rhs_operand" "rI,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%1, %2\;movge\\t%0, %2
   cmp\\t%1, %2\;movlt\\t%0, %1\;movge\\t%0, %2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#199
(define_expand "umaxsi3"
  [(parallel [
    (set (match_operand:SI 0 "s_register_operand" "")
	 (umax:SI (match_operand:SI 1 "s_register_operand" "")
		  (match_operand:SI 2 "arm_rhs_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_32BIT"
  ""
)

//Pattern#200
(define_insn "*arm_umaxsi3"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(umax:SI (match_operand:SI 1 "s_register_operand" "0,r,?r")
		 (match_operand:SI 2 "arm_rhs_operand" "rI,0,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%1, %2\;movcc\\t%0, %2
   cmp\\t%1, %2\;movcs\\t%0, %1
   cmp\\t%1, %2\;movcs\\t%0, %1\;movcc\\t%0, %2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,8,12")]
)

//Pattern#201
(define_expand "uminsi3"
  [(parallel [
    (set (match_operand:SI 0 "s_register_operand" "")
	 (umin:SI (match_operand:SI 1 "s_register_operand" "")
		  (match_operand:SI 2 "arm_rhs_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_32BIT"
  ""
)

//Pattern#202
(define_insn "*arm_uminsi3"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(umin:SI (match_operand:SI 1 "s_register_operand" "0,r,?r")
		 (match_operand:SI 2 "arm_rhs_operand" "rI,0,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%1, %2\;movcs\\t%0, %2
   cmp\\t%1, %2\;movcc\\t%0, %1
   cmp\\t%1, %2\;movcc\\t%0, %1\;movcs\\t%0, %2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,8,12")]
)

//Pattern#203
(define_insn "*store_minmaxsi"
  [(set (match_operand:SI 0 "memory_operand" "=m")
	(match_operator:SI 3 "minmax_operator"
	 [(match_operand:SI 1 "s_register_operand" "r")
	  (match_operand:SI 2 "s_register_operand" "r")]))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "*
  operands[3] = gen_rtx_fmt_ee (minmax_code (operands[3]), SImode,
				operands[1], operands[2]);
  output_asm_insn (\"cmp\\t%1, %2\", operands);
  if (TARGET_THUMB2)
    output_asm_insn (\"ite\t%d3\", operands);
  output_asm_insn (\"str%d3\\t%1, %0\", operands);
  output_asm_insn (\"str%D3\\t%2, %0\", operands);
  return \"\";
  "
  [(set_attr "conds" "clob")
   (set (attr "length")
	(if_then_else (eq_attr "is_thumb" "yes")
		      (const_int 14)
		      (const_int 12)))
   (set_attr "type" "store1")]
)

; Reject the frame pointer in operand[1], since reloading this after
; it has been eliminated can cause carnage.
//Pattern#204
(define_insn "*minmax_arithsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(match_operator:SI 4 "shiftable_operator"
	 [(match_operator:SI 5 "minmax_operator"
	   [(match_operand:SI 2 "s_register_operand" "r,r")
	    (match_operand:SI 3 "arm_rhs_operand" "rI,rI")])
	  (match_operand:SI 1 "s_register_operand" "0,?r")]))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && !arm_eliminable_register (operands[1])"
  "*
  {
    enum rtx_code code = GET_CODE (operands[4]);
    bool need_else;

    if (which_alternative != 0 || operands[3] != const0_rtx
        || (code != PLUS && code != IOR && code != XOR))
      need_else = true;
    else
      need_else = false;

    operands[5] = gen_rtx_fmt_ee (minmax_code (operands[5]), SImode,
				  operands[2], operands[3]);
    output_asm_insn (\"cmp\\t%2, %3\", operands);
    if (TARGET_THUMB2)
      {
	if (need_else)
	  output_asm_insn (\"ite\\t%d5\", operands);
	else
	  output_asm_insn (\"it\\t%d5\", operands);
      }
    output_asm_insn (\"%i4%d5\\t%0, %1, %2\", operands);
    if (need_else)
      output_asm_insn (\"%i4%D5\\t%0, %1, %3\", operands);
    return \"\";
  }"
  [(set_attr "conds" "clob")
   (set (attr "length")
	(if_then_else (eq_attr "is_thumb" "yes")
		      (const_int 14)
		      (const_int 12)))]
)

//Pattern#205
(define_code_iterator SAT [smin smax])
//Pattern#206
(define_code_iterator SATrev [smin smax])
//Pattern#207
(define_code_attr SATlo [(smin "1") (smax "2")])
//Pattern#208
(define_code_attr SAThi [(smin "2") (smax "1")])

//Pattern#209
(define_insn "*satsi_<SAT:code>"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (SAT:SI (SATrev:SI (match_operand:SI 3 "s_register_operand" "r")
                           (match_operand:SI 1 "const_int_operand" "i"))
                (match_operand:SI 2 "const_int_operand" "i")))]
  "TARGET_32BIT && arm_arch6 && <SAT:CODE> != <SATrev:CODE>
   && arm_sat_operator_match (operands[<SAT:SATlo>], operands[<SAT:SAThi>], NULL, NULL)"
{
  int mask;
  bool signed_sat;
  if (!arm_sat_operator_match (operands[<SAT:SATlo>], operands[<SAT:SAThi>],
                               &mask, &signed_sat))
    gcc_unreachable ();

  operands[1] = GEN_INT (mask);
  if (signed_sat)
    return "ssat%?\t%0, %1, %3";
  else
    return "usat%?\t%0, %1, %3";
}
  [(set_attr "predicable" "yes")
   (set_attr "insn" "sat")])

//Pattern#210
(define_insn "*satsi_<SAT:code>_shift"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (SAT:SI (SATrev:SI (match_operator:SI 3 "sat_shift_operator"
                             [(match_operand:SI 4 "s_register_operand" "r")
                              (match_operand:SI 5 "const_int_operand" "i")])
                           (match_operand:SI 1 "const_int_operand" "i"))
                (match_operand:SI 2 "const_int_operand" "i")))]
  "TARGET_32BIT && arm_arch6 && <SAT:CODE> != <SATrev:CODE>
   && arm_sat_operator_match (operands[<SAT:SATlo>], operands[<SAT:SAThi>], NULL, NULL)"
{
  int mask;
  bool signed_sat;
  if (!arm_sat_operator_match (operands[<SAT:SATlo>], operands[<SAT:SAThi>],
                               &mask, &signed_sat))
    gcc_unreachable ();

  operands[1] = GEN_INT (mask);
  if (signed_sat)
    return "ssat%?\t%0, %1, %4%S3";
  else
    return "usat%?\t%0, %1, %4%S3";
}
  [(set_attr "predicable" "yes")
   (set_attr "insn" "sat")
   (set_attr "shift" "3")
   (set_attr "type" "alu_shift")])

;; Shift and rotation insns

//Pattern#211
(define_expand "ashldi3"
  [(set (match_operand:DI            0 "s_register_operand" "")
        (ashift:DI (match_operand:DI 1 "s_register_operand" "")
                   (match_operand:SI 2 "general_operand" "")))]
  "TARGET_32BIT"
  "
  if (TARGET_NEON)
    {
      /* Delay the decision whether to use NEON or core-regs until
	 register allocation.  */
      emit_insn (gen_ashldi3_neon (operands[0], operands[1], operands[2]));
      DONE;
    }
  else
    {
      /* Only the NEON case can handle in-memory shift counts.  */
      if (!reg_or_int_operand (operands[2], SImode))
        operands[2] = force_reg (SImode, operands[2]);
    }

  if (!CONST_INT_P (operands[2]) && TARGET_REALLY_IWMMXT)
    ; /* No special preparation statements; expand pattern as above.  */
  else
    {
      rtx scratch1, scratch2;

      if (CONST_INT_P (operands[2])
	  && (HOST_WIDE_INT) INTVAL (operands[2]) == 1)
        {
          emit_insn (gen_arm_ashldi3_1bit (operands[0], operands[1]));
          DONE;
        }

      /* Ideally we should use iwmmxt here if we could know that operands[1]
         ends up already living in an iwmmxt register. Otherwise it's
         cheaper to have the alternate code being generated than moving
         values to iwmmxt regs and back.  */

      /* If we're optimizing for size, we prefer the libgcc calls.  */
      if (optimize_function_for_size_p (cfun))
	FAIL;

      /* Expand operation using core-registers.
	 'FAIL' would achieve the same thing, but this is a bit smarter.  */
      scratch1 = gen_reg_rtx (SImode);
      scratch2 = gen_reg_rtx (SImode);
      arm_emit_coreregs_64bit_shift (ASHIFT, operands[0], operands[1],
				     operands[2], scratch1, scratch2);
      DONE;
    }
  "
)

//Pattern#212
(define_insn "arm_ashldi3_1bit"
  [(set (match_operand:DI            0 "s_register_operand" "=r,&r")
        (ashift:DI (match_operand:DI 1 "s_register_operand" "0,r")
                   (const_int 1)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "movs\\t%Q0, %Q1, asl #1\;adc\\t%R0, %R1, %R1"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#213
(define_expand "ashlsi3"
  [(set (match_operand:SI            0 "s_register_operand" "")
	(ashift:SI (match_operand:SI 1 "s_register_operand" "")
		   (match_operand:SI 2 "arm_rhs_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[2])
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    {
      emit_insn (gen_movsi (operands[0], const0_rtx));
      DONE;
    }
  "
)

//Pattern#214
(define_insn "*thumb1_ashlsi3"
  [(set (match_operand:SI            0 "register_operand" "=l,l")
	(ashift:SI (match_operand:SI 1 "register_operand" "l,0")
		   (match_operand:SI 2 "nonmemory_operand" "N,l")))]
  "TARGET_THUMB1"
  "lsl\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#215
(define_expand "ashrdi3"
  [(set (match_operand:DI              0 "s_register_operand" "")
        (ashiftrt:DI (match_operand:DI 1 "s_register_operand" "")
                     (match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_32BIT"
  "
  if (TARGET_NEON)
    {
      /* Delay the decision whether to use NEON or core-regs until
	 register allocation.  */
      emit_insn (gen_ashrdi3_neon (operands[0], operands[1], operands[2]));
      DONE;
    }

  if (!CONST_INT_P (operands[2]) && TARGET_REALLY_IWMMXT)
    ; /* No special preparation statements; expand pattern as above.  */
  else
    {
      rtx scratch1, scratch2;

      if (CONST_INT_P (operands[2])
	  && (HOST_WIDE_INT) INTVAL (operands[2]) == 1)
        {
          emit_insn (gen_arm_ashrdi3_1bit (operands[0], operands[1]));
          DONE;
        }

      /* Ideally we should use iwmmxt here if we could know that operands[1]
         ends up already living in an iwmmxt register. Otherwise it's
         cheaper to have the alternate code being generated than moving
         values to iwmmxt regs and back.  */

      /* If we're optimizing for size, we prefer the libgcc calls.  */
      if (optimize_function_for_size_p (cfun))
	FAIL;

      /* Expand operation using core-registers.
	 'FAIL' would achieve the same thing, but this is a bit smarter.  */
      scratch1 = gen_reg_rtx (SImode);
      scratch2 = gen_reg_rtx (SImode);
      arm_emit_coreregs_64bit_shift (ASHIFTRT, operands[0], operands[1],
				     operands[2], scratch1, scratch2);
      DONE;
    }
  "
)

//Pattern#216
(define_insn "arm_ashrdi3_1bit"
  [(set (match_operand:DI              0 "s_register_operand" "=r,&r")
        (ashiftrt:DI (match_operand:DI 1 "s_register_operand" "0,r")
                     (const_int 1)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "movs\\t%R0, %R1, asr #1\;mov\\t%Q0, %Q1, rrx"
  [(set_attr "conds" "clob")
   (set_attr "insn" "mov")
   (set_attr "length" "8")]
)

//Pattern#217
(define_expand "ashrsi3"
  [(set (match_operand:SI              0 "s_register_operand" "")
	(ashiftrt:SI (match_operand:SI 1 "s_register_operand" "")
		     (match_operand:SI 2 "arm_rhs_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[2])
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    operands[2] = GEN_INT (31);
  "
)

//Pattern#218
(define_insn "*thumb1_ashrsi3"
  [(set (match_operand:SI              0 "register_operand" "=l,l")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "l,0")
		     (match_operand:SI 2 "nonmemory_operand" "N,l")))]
  "TARGET_THUMB1"
  "asr\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#219
(define_expand "lshrdi3"
  [(set (match_operand:DI              0 "s_register_operand" "")
        (lshiftrt:DI (match_operand:DI 1 "s_register_operand" "")
                     (match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_32BIT"
  "
  if (TARGET_NEON)
    {
      /* Delay the decision whether to use NEON or core-regs until
	 register allocation.  */
      emit_insn (gen_lshrdi3_neon (operands[0], operands[1], operands[2]));
      DONE;
    }

  if (!CONST_INT_P (operands[2]) && TARGET_REALLY_IWMMXT)
    ; /* No special preparation statements; expand pattern as above.  */
  else
    {
      rtx scratch1, scratch2;

      if (CONST_INT_P (operands[2])
	  && (HOST_WIDE_INT) INTVAL (operands[2]) == 1)
        {
          emit_insn (gen_arm_lshrdi3_1bit (operands[0], operands[1]));
          DONE;
        }

      /* Ideally we should use iwmmxt here if we could know that operands[1]
         ends up already living in an iwmmxt register. Otherwise it's
         cheaper to have the alternate code being generated than moving
         values to iwmmxt regs and back.  */

      /* If we're optimizing for size, we prefer the libgcc calls.  */
      if (optimize_function_for_size_p (cfun))
	FAIL;

      /* Expand operation using core-registers.
	 'FAIL' would achieve the same thing, but this is a bit smarter.  */
      scratch1 = gen_reg_rtx (SImode);
      scratch2 = gen_reg_rtx (SImode);
      arm_emit_coreregs_64bit_shift (LSHIFTRT, operands[0], operands[1],
				     operands[2], scratch1, scratch2);
      DONE;
    }
  "
)

//Pattern#220
(define_insn "arm_lshrdi3_1bit"
  [(set (match_operand:DI              0 "s_register_operand" "=r,&r")
        (lshiftrt:DI (match_operand:DI 1 "s_register_operand" "0,r")
                     (const_int 1)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "movs\\t%R0, %R1, lsr #1\;mov\\t%Q0, %Q1, rrx"
  [(set_attr "conds" "clob")
   (set_attr "insn" "mov")
   (set_attr "length" "8")]
)

//Pattern#221
(define_expand "lshrsi3"
  [(set (match_operand:SI              0 "s_register_operand" "")
	(lshiftrt:SI (match_operand:SI 1 "s_register_operand" "")
		     (match_operand:SI 2 "arm_rhs_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[2])
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    {
      emit_insn (gen_movsi (operands[0], const0_rtx));
      DONE;
    }
  "
)

//Pattern#222
(define_insn "*thumb1_lshrsi3"
  [(set (match_operand:SI              0 "register_operand" "=l,l")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "l,0")
		     (match_operand:SI 2 "nonmemory_operand" "N,l")))]
  "TARGET_THUMB1"
  "lsr\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#223
(define_expand "rotlsi3"
  [(set (match_operand:SI              0 "s_register_operand" "")
	(rotatert:SI (match_operand:SI 1 "s_register_operand" "")
		     (match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_32BIT"
  "
  if (CONST_INT_P (operands[2]))
    operands[2] = GEN_INT ((32 - INTVAL (operands[2])) % 32);
  else
    {
      rtx reg = gen_reg_rtx (SImode);
      emit_insn (gen_subsi3 (reg, GEN_INT (32), operands[2]));
      operands[2] = reg;
    }
  "
)

//Pattern#224
(define_expand "rotrsi3"
  [(set (match_operand:SI              0 "s_register_operand" "")
	(rotatert:SI (match_operand:SI 1 "s_register_operand" "")
		     (match_operand:SI 2 "arm_rhs_operand" "")))]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT)
    {
      if (CONST_INT_P (operands[2])
          && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
        operands[2] = GEN_INT (INTVAL (operands[2]) % 32);
    }
  else /* TARGET_THUMB1 */
    {
      if (CONST_INT_P (operands [2]))
        operands [2] = force_reg (SImode, operands[2]);
    }
  "
)

//Pattern#225
(define_insn "*thumb1_rotrsi3"
  [(set (match_operand:SI              0 "register_operand" "=l")
	(rotatert:SI (match_operand:SI 1 "register_operand" "0")
		     (match_operand:SI 2 "register_operand" "l")))]
  "TARGET_THUMB1"
  "ror\\t%0, %0, %2"
  [(set_attr "length" "2")]
)

//Pattern#226
(define_insn "*arm_shiftsi3"
  [(set (match_operand:SI   0 "s_register_operand" "=r")
	(match_operator:SI  3 "shift_operator"
	 [(match_operand:SI 1 "s_register_operand"  "r")
	  (match_operand:SI 2 "reg_or_int_operand" "rM")]))]
  "TARGET_32BIT"
  "* return arm_output_shift(operands, 0);"
  [(set_attr "predicable" "yes")
   (set_attr "shift" "1")
   (set (attr "type") (if_then_else (match_operand 2 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#227
(define_insn "*shiftsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (match_operator:SI 3 "shift_operator"
			  [(match_operand:SI 1 "s_register_operand" "r")
			   (match_operand:SI 2 "arm_rhs_operand" "rM")])
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(match_op_dup 3 [(match_dup 1) (match_dup 2)]))]
  "TARGET_32BIT"
  "* return arm_output_shift(operands, 1);"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")
   (set (attr "type") (if_then_else (match_operand 2 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#228
(define_insn "*shiftsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (match_operator:SI 3 "shift_operator"
			  [(match_operand:SI 1 "s_register_operand" "r")
			   (match_operand:SI 2 "arm_rhs_operand" "rM")])
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_32BIT"
  "* return arm_output_shift(operands, 1);"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")]
)

//Pattern#229
(define_insn "*not_shiftsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(not:SI (match_operator:SI 3 "shift_operator"
		 [(match_operand:SI 1 "s_register_operand" "r,r")
		  (match_operand:SI 2 "shift_amount_operand" "M,rM")])))]
  "TARGET_32BIT"
  "mvn%?\\t%0, %1%S3"
  [(set_attr "predicable" "yes")
   (set_attr "shift" "1")
   (set_attr "insn" "mvn")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#230
(define_insn "*not_shiftsi_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (not:SI (match_operator:SI 3 "shift_operator"
		  [(match_operand:SI 1 "s_register_operand" "r,r")
		   (match_operand:SI 2 "shift_amount_operand" "M,rM")]))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(not:SI (match_op_dup 3 [(match_dup 1) (match_dup 2)])))]
  "TARGET_32BIT"
  "mvn%.\\t%0, %1%S3"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")
   (set_attr "insn" "mvn")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#231
(define_insn "*not_shiftsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (not:SI (match_operator:SI 3 "shift_operator"
		  [(match_operand:SI 1 "s_register_operand" "r,r")
		   (match_operand:SI 2 "shift_amount_operand" "M,rM")]))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r,r"))]
  "TARGET_32BIT"
  "mvn%.\\t%0, %1%S3"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")
   (set_attr "insn" "mvn")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

;; We don't really have extzv, but defining this using shifts helps
;; to reduce register pressure later on.

//Pattern#232
(define_expand "extzv"
  [(set (match_operand 0 "s_register_operand" "")
	(zero_extract (match_operand 1 "nonimmediate_operand" "")
		      (match_operand 2 "const_int_operand" "")
		      (match_operand 3 "const_int_operand" "")))]
  "TARGET_THUMB1 || arm_arch_thumb2"
  "
  {
    HOST_WIDE_INT lshift = 32 - INTVAL (operands[2]) - INTVAL (operands[3]);
    HOST_WIDE_INT rshift = 32 - INTVAL (operands[2]);
    
    if (arm_arch_thumb2)
      {
	HOST_WIDE_INT width = INTVAL (operands[2]);
	HOST_WIDE_INT bitpos = INTVAL (operands[3]);

	if (unaligned_access && MEM_P (operands[1])
	    && (width == 16 || width == 32) && (bitpos % BITS_PER_UNIT) == 0)
	  {
	    rtx base_addr;

	    if (BYTES_BIG_ENDIAN)
	      bitpos = GET_MODE_BITSIZE (GET_MODE (operands[0])) - width
		       - bitpos;

	    if (width == 32)
              {
		base_addr = adjust_address (operands[1], SImode,
					    bitpos / BITS_PER_UNIT);
		emit_insn (gen_unaligned_loadsi (operands[0], base_addr));
              }
	    else
              {
		rtx dest = operands[0];
		rtx tmp = gen_reg_rtx (SImode);

		/* We may get a paradoxical subreg here.  Strip it off.  */
		if (GET_CODE (dest) == SUBREG
		    && GET_MODE (dest) == SImode
		    && GET_MODE (SUBREG_REG (dest)) == HImode)
		  dest = SUBREG_REG (dest);

		if (GET_MODE_BITSIZE (GET_MODE (dest)) != width)
		  FAIL;

		base_addr = adjust_address (operands[1], HImode,
					    bitpos / BITS_PER_UNIT);
		emit_insn (gen_unaligned_loadhiu (tmp, base_addr));
		emit_move_insn (gen_lowpart (SImode, dest), tmp);
	      }
	    DONE;
	  }
	else if (s_register_operand (operands[1], GET_MODE (operands[1])))
	  {
	    emit_insn (gen_extzv_t2 (operands[0], operands[1], operands[2],
				     operands[3]));
	    DONE;
	  }
	else
	  FAIL;
      }
    
    if (!s_register_operand (operands[1], GET_MODE (operands[1])))
      FAIL;

    operands[3] = GEN_INT (rshift);
    
    if (lshift == 0)
      {
        emit_insn (gen_lshrsi3 (operands[0], operands[1], operands[3]));
        DONE;
      }
      
    emit_insn (gen_extzv_t1 (operands[0], operands[1], GEN_INT (lshift),
			     operands[3], gen_reg_rtx (SImode)));
    DONE;
  }"
)

;; Helper for extzv, for the Thumb-1 register-shifts case.

//Pattern#233
(define_expand "extzv_t1"
  [(set (match_operand:SI 4 "s_register_operand" "")
	(ashift:SI (match_operand:SI 1 "nonimmediate_operand" "")
		   (match_operand:SI 2 "const_int_operand" "")))
   (set (match_operand:SI 0 "s_register_operand" "")
	(lshiftrt:SI (match_dup 4)
		     (match_operand:SI 3 "const_int_operand" "")))]
  "TARGET_THUMB1"
  "")

//Pattern#234
(define_expand "extv"
  [(set (match_operand 0 "s_register_operand" "")
	(sign_extract (match_operand 1 "nonimmediate_operand" "")
		      (match_operand 2 "const_int_operand" "")
		      (match_operand 3 "const_int_operand" "")))]
  "arm_arch_thumb2"
{
  HOST_WIDE_INT width = INTVAL (operands[2]);
  HOST_WIDE_INT bitpos = INTVAL (operands[3]);

  if (unaligned_access && MEM_P (operands[1]) && (width == 16 || width == 32)
      && (bitpos % BITS_PER_UNIT)  == 0)
    {
      rtx base_addr;
      
      if (BYTES_BIG_ENDIAN)
	bitpos = GET_MODE_BITSIZE (GET_MODE (operands[0])) - width - bitpos;
      
      if (width == 32)
        {
	  base_addr = adjust_address (operands[1], SImode,
				      bitpos / BITS_PER_UNIT);
	  emit_insn (gen_unaligned_loadsi (operands[0], base_addr));
        }
      else
        {
	  rtx dest = operands[0];
	  rtx tmp = gen_reg_rtx (SImode);
	  
	  /* We may get a paradoxical subreg here.  Strip it off.  */
	  if (GET_CODE (dest) == SUBREG
	      && GET_MODE (dest) == SImode
	      && GET_MODE (SUBREG_REG (dest)) == HImode)
	    dest = SUBREG_REG (dest);
	  
	  if (GET_MODE_BITSIZE (GET_MODE (dest)) != width)
	    FAIL;
	  
	  base_addr = adjust_address (operands[1], HImode,
				      bitpos / BITS_PER_UNIT);
	  emit_insn (gen_unaligned_loadhis (tmp, base_addr));
	  emit_move_insn (gen_lowpart (SImode, dest), tmp);
	}

      DONE;
    }
  else if (!s_register_operand (operands[1], GET_MODE (operands[1])))
    FAIL;
  else if (GET_MODE (operands[0]) == SImode
	   && GET_MODE (operands[1]) == SImode)
    {
      emit_insn (gen_extv_regsi (operands[0], operands[1], operands[2],
				 operands[3]));
      DONE;
    }

  FAIL;
})

; Helper to expand register forms of extv with the proper modes.

//Pattern#235
(define_expand "extv_regsi"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(sign_extract:SI (match_operand:SI 1 "s_register_operand" "")
			 (match_operand 2 "const_int_operand" "")
			 (match_operand 3 "const_int_operand" "")))]
  ""
{
})

; ARMv6+ unaligned load/store instructions (used for packed structure accesses).

//Pattern#236
(define_insn "unaligned_loadsi"
  [(set (match_operand:SI 0 "s_register_operand" "=l,r")
	(unspec:SI [(match_operand:SI 1 "memory_operand" "Uw,m")]
		   UNSPEC_UNALIGNED_LOAD))]
  "unaligned_access && TARGET_32BIT"
  "ldr%?\t%0, %1\t@ unaligned"
  [(set_attr "arch" "t2,any")
   (set_attr "length" "2,4")
   (set_attr "predicable" "yes")
   (set_attr "type" "load1")])

//Pattern#237
(define_insn "unaligned_loadhis"
  [(set (match_operand:SI 0 "s_register_operand" "=l,r")
	(sign_extend:SI
	  (unspec:HI [(match_operand:HI 1 "memory_operand" "Uw,m")]
		     UNSPEC_UNALIGNED_LOAD)))]
  "unaligned_access && TARGET_32BIT"
  "ldr%(sh%)\t%0, %1\t@ unaligned"
  [(set_attr "arch" "t2,any")
   (set_attr "length" "2,4")
   (set_attr "predicable" "yes")
   (set_attr "type" "load_byte")])

//Pattern#238
(define_insn "unaligned_loadhiu"
  [(set (match_operand:SI 0 "s_register_operand" "=l,r")
	(zero_extend:SI
	  (unspec:HI [(match_operand:HI 1 "memory_operand" "Uw,m")]
		     UNSPEC_UNALIGNED_LOAD)))]
  "unaligned_access && TARGET_32BIT"
  "ldr%(h%)\t%0, %1\t@ unaligned"
  [(set_attr "arch" "t2,any")
   (set_attr "length" "2,4")
   (set_attr "predicable" "yes")
   (set_attr "type" "load_byte")])

//Pattern#239
(define_insn "unaligned_storesi"
  [(set (match_operand:SI 0 "memory_operand" "=Uw,m")
	(unspec:SI [(match_operand:SI 1 "s_register_operand" "l,r")]
		   UNSPEC_UNALIGNED_STORE))]
  "unaligned_access && TARGET_32BIT"
  "str%?\t%1, %0\t@ unaligned"
  [(set_attr "arch" "t2,any")
   (set_attr "length" "2,4")
   (set_attr "predicable" "yes")
   (set_attr "type" "store1")])

//Pattern#240
(define_insn "unaligned_storehi"
  [(set (match_operand:HI 0 "memory_operand" "=Uw,m")
	(unspec:HI [(match_operand:HI 1 "s_register_operand" "l,r")]
		   UNSPEC_UNALIGNED_STORE))]
  "unaligned_access && TARGET_32BIT"
  "str%(h%)\t%1, %0\t@ unaligned"
  [(set_attr "arch" "t2,any")
   (set_attr "length" "2,4")
   (set_attr "predicable" "yes")
   (set_attr "type" "store1")])

//Pattern#241
(define_insn "*extv_reg"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(sign_extract:SI (match_operand:SI 1 "s_register_operand" "r")
                         (match_operand:SI 2 "const_int_operand" "M")
                         (match_operand:SI 3 "const_int_operand" "M")))]
  "arm_arch_thumb2"
  "sbfx%?\t%0, %1, %3, %2"
  [(set_attr "length" "4")
   (set_attr "predicable" "yes")]
)

//Pattern#242
(define_insn "extzv_t2"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(zero_extract:SI (match_operand:SI 1 "s_register_operand" "r")
                         (match_operand:SI 2 "const_int_operand" "M")
                         (match_operand:SI 3 "const_int_operand" "M")))]
  "arm_arch_thumb2"
  "ubfx%?\t%0, %1, %3, %2"
  [(set_attr "length" "4")
   (set_attr "predicable" "yes")]
)


;; Division instructions
//Pattern#243
(define_insn "divsi3"
  [(set (match_operand:SI	  0 "s_register_operand" "=r")
	(div:SI (match_operand:SI 1 "s_register_operand"  "r")
		(match_operand:SI 2 "s_register_operand"  "r")))]
  "TARGET_IDIV"
  "sdiv%?\t%0, %1, %2"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "sdiv")]
)

//Pattern#244
(define_insn "udivsi3"
  [(set (match_operand:SI	   0 "s_register_operand" "=r")
	(udiv:SI (match_operand:SI 1 "s_register_operand"  "r")
		 (match_operand:SI 2 "s_register_operand"  "r")))]
  "TARGET_IDIV"
  "udiv%?\t%0, %1, %2"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "udiv")]
)


;; Unary arithmetic insns

//Pattern#245
(define_expand "negdi2"
 [(parallel
   [(set (match_operand:DI 0 "s_register_operand" "")
	 (neg:DI (match_operand:DI 1 "s_register_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_EITHER"
  {
    if (TARGET_NEON)
      {
        emit_insn (gen_negdi2_neon (operands[0], operands[1]));
	DONE;
      }
  }
)

;; The constraints here are to prevent a *partial* overlap (where %Q0 == %R1).
;; The first alternative allows the common case of a *full* overlap.
//Pattern#246
(define_insn "*arm_negdi2"
  [(set (match_operand:DI         0 "s_register_operand" "=r,&r")
	(neg:DI (match_operand:DI 1 "s_register_operand"  "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, #0\;rsc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#247
(define_insn "*thumb1_negdi2"
  [(set (match_operand:DI 0 "register_operand" "=&l")
	(neg:DI (match_operand:DI 1 "register_operand" "l")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_THUMB1"
  "mov\\t%R0, #0\;neg\\t%Q0, %Q1\;sbc\\t%R0, %R1"
  [(set_attr "length" "6")]
)

//Pattern#248
(define_expand "negsi2"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(neg:SI (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_EITHER"
  ""
)

//Pattern#249
(define_insn "*arm_negsi2"
  [(set (match_operand:SI         0 "s_register_operand" "=r")
	(neg:SI (match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_32BIT"
  "rsb%?\\t%0, %1, #0"
  [(set_attr "predicable" "yes")]
)

//Pattern#250
(define_insn "*thumb1_negsi2"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(neg:SI (match_operand:SI 1 "register_operand" "l")))]
  "TARGET_THUMB1"
  "neg\\t%0, %1"
  [(set_attr "length" "2")]
)

//Pattern#251
(define_expand "negsf2"
  [(set (match_operand:SF         0 "s_register_operand" "")
	(neg:SF (match_operand:SF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  ""
)

//Pattern#252
(define_expand "negdf2"
  [(set (match_operand:DF         0 "s_register_operand" "")
	(neg:DF (match_operand:DF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  "")

;; abssi2 doesn't really clobber the condition codes if a different register
;; is being set.  To keep things simple, assume during rtl manipulations that
;; it does, but tell the final scan operator the truth.  Similarly for
;; (neg (abs...))

//Pattern#253
(define_expand "abssi2"
  [(parallel
    [(set (match_operand:SI         0 "s_register_operand" "")
	  (abs:SI (match_operand:SI 1 "s_register_operand" "")))
     (clobber (match_dup 2))])]
  "TARGET_EITHER"
  "
  if (TARGET_THUMB1)
    operands[2] = gen_rtx_SCRATCH (SImode);
  else
    operands[2] = gen_rtx_REG (CCmode, CC_REGNUM);
")

//Pattern#254
(define_insn "*arm_abssi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,&r")
	(abs:SI (match_operand:SI 1 "s_register_operand" "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%0, #0\;rsblt\\t%0, %0, #0
   eor%?\\t%0, %1, %1, asr #31\;sub%?\\t%0, %0, %1, asr #31"
  [(set_attr "conds" "clob,*")
   (set_attr "shift" "1")
   (set_attr "predicable" "no, yes")
   (set_attr "length" "8")]
)

//Pattern#255
(define_insn_and_split "*thumb1_abssi2"
  [(set (match_operand:SI 0 "s_register_operand" "=l")
	(abs:SI (match_operand:SI 1 "s_register_operand" "l")))
   (clobber (match_scratch:SI 2 "=&l"))]
  "TARGET_THUMB1"
  "#"
  "TARGET_THUMB1 && reload_completed"
  [(set (match_dup 2) (ashiftrt:SI (match_dup 1) (const_int 31)))
   (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (xor:SI (match_dup 0) (match_dup 2)))]
  ""
  [(set_attr "length" "6")]
)

//Pattern#256
(define_insn "*arm_neg_abssi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,&r")
	(neg:SI (abs:SI (match_operand:SI 1 "s_register_operand" "0,r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%0, #0\;rsbgt\\t%0, %0, #0
   eor%?\\t%0, %1, %1, asr #31\;rsb%?\\t%0, %0, %1, asr #31"
  [(set_attr "conds" "clob,*")
   (set_attr "shift" "1")
   (set_attr "predicable" "no, yes")
   (set_attr "length" "8")]
)

//Pattern#257
(define_insn_and_split "*thumb1_neg_abssi2"
  [(set (match_operand:SI 0 "s_register_operand" "=l")
	(neg:SI (abs:SI (match_operand:SI 1 "s_register_operand" "l"))))
   (clobber (match_scratch:SI 2 "=&l"))]
  "TARGET_THUMB1"
  "#"
  "TARGET_THUMB1 && reload_completed"
  [(set (match_dup 2) (ashiftrt:SI (match_dup 1) (const_int 31)))
   (set (match_dup 0) (minus:SI (match_dup 2) (match_dup 1)))
   (set (match_dup 0) (xor:SI (match_dup 0) (match_dup 2)))]
  ""
  [(set_attr "length" "6")]
)

//Pattern#258
(define_expand "abssf2"
  [(set (match_operand:SF         0 "s_register_operand" "")
	(abs:SF (match_operand:SF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "")

//Pattern#259
(define_expand "absdf2"
  [(set (match_operand:DF         0 "s_register_operand" "")
	(abs:DF (match_operand:DF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "")

//Pattern#260
(define_expand "sqrtsf2"
  [(set (match_operand:SF 0 "s_register_operand" "")
	(sqrt:SF (match_operand:SF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  "")

//Pattern#261
(define_expand "sqrtdf2"
  [(set (match_operand:DF 0 "s_register_operand" "")
	(sqrt:DF (match_operand:DF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  "")

//Pattern#262
(define_insn_and_split "one_cmpldi2"
  [(set (match_operand:DI 0 "s_register_operand"	 "=w,&r,&r,?w")
	(not:DI (match_operand:DI 1 "s_register_operand" " w, 0, r, w")))]
  "TARGET_32BIT"
  "@
   vmvn\t%P0, %P1
   #
   #
   vmvn\t%P0, %P1"
  "TARGET_32BIT && reload_completed
   && arm_general_register_operand (operands[0], DImode)"
  [(set (match_dup 0) (not:SI (match_dup 1)))
   (set (match_dup 2) (not:SI (match_dup 3)))]
  "
  {
    operands[2] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[3] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
  [(set_attr "length" "*,8,8,*")
   (set_attr "predicable" "no,yes,yes,no")
   (set_attr "neon_type" "neon_int_1,*,*,neon_int_1")
   (set_attr "arch" "neon_nota8,*,*,neon_onlya8")]
)

//Pattern#263
(define_expand "one_cmplsi2"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(not:SI (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_EITHER"
  ""
)

//Pattern#264
(define_insn "*arm_one_cmplsi2"
  [(set (match_operand:SI         0 "s_register_operand" "=r")
	(not:SI (match_operand:SI 1 "s_register_operand"  "r")))]
  "TARGET_32BIT"
  "mvn%?\\t%0, %1"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "mvn")]
)

//Pattern#265
(define_insn "*thumb1_one_cmplsi2"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(not:SI (match_operand:SI 1 "register_operand"  "l")))]
  "TARGET_THUMB1"
  "mvn\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "insn" "mvn")]
)

//Pattern#266
(define_insn "*notsi_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (not:SI (match_operand:SI 1 "s_register_operand" "r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(not:SI (match_dup 1)))]
  "TARGET_32BIT"
  "mvn%.\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "mvn")]
)

//Pattern#267
(define_insn "*notsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (not:SI (match_operand:SI 1 "s_register_operand" "r"))
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_32BIT"
  "mvn%.\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "mvn")]
)

;; Fixed <--> Floating conversion insns

//Pattern#268
(define_expand "floatsihf2"
  [(set (match_operand:HF           0 "general_operand" "")
	(float:HF (match_operand:SI 1 "general_operand" "")))]
  "TARGET_EITHER"
  "
  {
    rtx op1 = gen_reg_rtx (SFmode);
    expand_float (op1, operands[1], 0);
    op1 = convert_to_mode (HFmode, op1, 0);
    emit_move_insn (operands[0], op1);
    DONE;
  }"
)

//Pattern#269
(define_expand "floatdihf2"
  [(set (match_operand:HF           0 "general_operand" "")
	(float:HF (match_operand:DI 1 "general_operand" "")))]
  "TARGET_EITHER"
  "
  {
    rtx op1 = gen_reg_rtx (SFmode);
    expand_float (op1, operands[1], 0);
    op1 = convert_to_mode (HFmode, op1, 0);
    emit_move_insn (operands[0], op1);
    DONE;
  }"
)

//Pattern#270
(define_expand "floatsisf2"
  [(set (match_operand:SF           0 "s_register_operand" "")
	(float:SF (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#271
(define_expand "floatsidf2"
  [(set (match_operand:DF           0 "s_register_operand" "")
	(float:DF (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")

//Pattern#272
(define_expand "fix_trunchfsi2"
  [(set (match_operand:SI         0 "general_operand" "")
	(fix:SI (fix:HF (match_operand:HF 1 "general_operand"  ""))))]
  "TARGET_EITHER"
  "
  {
    rtx op1 = convert_to_mode (SFmode, operands[1], 0);
    expand_fix (operands[0], op1, 0);
    DONE;
  }"
)

//Pattern#273
(define_expand "fix_trunchfdi2"
  [(set (match_operand:DI         0 "general_operand" "")
	(fix:DI (fix:HF (match_operand:HF 1 "general_operand"  ""))))]
  "TARGET_EITHER"
  "
  {
    rtx op1 = convert_to_mode (SFmode, operands[1], 0);
    expand_fix (operands[0], op1, 0);
    DONE;
  }"
)

//Pattern#274
(define_expand "fix_truncsfsi2"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(fix:SI (fix:SF (match_operand:SF 1 "s_register_operand"  ""))))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#275
(define_expand "fix_truncdfsi2"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(fix:SI (fix:DF (match_operand:DF 1 "s_register_operand"  ""))))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")

;; Truncation insns

//Pattern#276
(define_expand "truncdfsf2"
  [(set (match_operand:SF  0 "s_register_operand" "")
	(float_truncate:SF
 	 (match_operand:DF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  ""
)

/* DFmode -> HFmode conversions have to go through SFmode.  */
//Pattern#277
(define_expand "truncdfhf2"
  [(set (match_operand:HF  0 "general_operand" "")
	(float_truncate:HF
 	 (match_operand:DF 1 "general_operand" "")))]
  "TARGET_EITHER"
  "
  {
    rtx op1;
    op1 = convert_to_mode (SFmode, operands[1], 0);
    op1 = convert_to_mode (HFmode, op1, 0);
    emit_move_insn (operands[0], op1);
    DONE;
  }"
)

;; Zero and sign extension instructions.

//Pattern#278
(define_insn "zero_extend<mode>di2"
  [(set (match_operand:DI 0 "s_register_operand" "=w,r,?r,w")
        (zero_extend:DI (match_operand:QHSI 1 "<qhs_zextenddi_op>"
					    "<qhs_zextenddi_cstr>")))]
  "TARGET_32BIT <qhs_zextenddi_cond>"
  "#"
  [(set_attr "length" "8,4,8,8")
   (set_attr "arch" "neon_nota8,*,*,neon_onlya8")
   (set_attr "ce_count" "2")
   (set_attr "predicable" "yes")]
)

//Pattern#279
(define_insn "extend<mode>di2"
  [(set (match_operand:DI 0 "s_register_operand" "=w,r,?r,?r,w")
        (sign_extend:DI (match_operand:QHSI 1 "<qhs_extenddi_op>"
					    "<qhs_extenddi_cstr>")))]
  "TARGET_32BIT <qhs_sextenddi_cond>"
  "#"
  [(set_attr "length" "8,4,8,8,8")
   (set_attr "ce_count" "2")
   (set_attr "shift" "1")
   (set_attr "predicable" "yes")
   (set_attr "arch" "neon_nota8,*,a,t,neon_onlya8")]
)

;; Splits for all extensions to DImode
//Pattern#280
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
        (zero_extend:DI (match_operand 1 "nonimmediate_operand" "")))]
  "TARGET_32BIT && reload_completed && !IS_VFP_REGNUM (REGNO (operands[0]))"
  [(set (match_dup 0) (match_dup 1))]
{
  rtx lo_part = gen_lowpart (SImode, operands[0]);
  enum machine_mode src_mode = GET_MODE (operands[1]);

  if (REG_P (operands[0])
      && !reg_overlap_mentioned_p (operands[0], operands[1]))
    emit_clobber (operands[0]);
  if (!REG_P (lo_part) || src_mode != SImode
      || !rtx_equal_p (lo_part, operands[1]))
    {
      if (src_mode == SImode)
        emit_move_insn (lo_part, operands[1]);
      else
        emit_insn (gen_rtx_SET (VOIDmode, lo_part,
				gen_rtx_ZERO_EXTEND (SImode, operands[1])));
      operands[1] = lo_part;
    }
  operands[0] = gen_highpart (SImode, operands[0]);
  operands[1] = const0_rtx;
})

//Pattern#281
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
        (sign_extend:DI (match_operand 1 "nonimmediate_operand" "")))]
  "TARGET_32BIT && reload_completed && !IS_VFP_REGNUM (REGNO (operands[0]))"
  [(set (match_dup 0) (ashiftrt:SI (match_dup 1) (const_int 31)))]
{
  rtx lo_part = gen_lowpart (SImode, operands[0]);
  enum machine_mode src_mode = GET_MODE (operands[1]);

  if (REG_P (operands[0])
      && !reg_overlap_mentioned_p (operands[0], operands[1]))
    emit_clobber (operands[0]);

  if (!REG_P (lo_part) || src_mode != SImode
      || !rtx_equal_p (lo_part, operands[1]))
    {
      if (src_mode == SImode)
        emit_move_insn (lo_part, operands[1]);
      else
        emit_insn (gen_rtx_SET (VOIDmode, lo_part,
				gen_rtx_SIGN_EXTEND (SImode, operands[1])));
      operands[1] = lo_part;
    }
  operands[0] = gen_highpart (SImode, operands[0]);
})

//Pattern#282
(define_expand "zero_extendhisi2"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "")))]
  "TARGET_EITHER"
{
  if (TARGET_ARM && !arm_arch4 && MEM_P (operands[1]))
    {
      emit_insn (gen_movhi_bytes (operands[0], operands[1]));
      DONE;
    }
  if (!arm_arch6 && !MEM_P (operands[1]))
    {
      rtx t = gen_lowpart (SImode, operands[1]);
      rtx tmp = gen_reg_rtx (SImode);
      emit_insn (gen_ashlsi3 (tmp, t, GEN_INT (16)));
      emit_insn (gen_lshrsi3 (operands[0], tmp, GEN_INT (16)));
      DONE;
    }
})

//Pattern#283
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (match_operand:HI 1 "s_register_operand" "")))]
  "!TARGET_THUMB2 && !arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 16)))
   (set (match_dup 0) (lshiftrt:SI (match_dup 0) (const_int 16)))]
{
  operands[2] = gen_lowpart (SImode, operands[1]);
})

//Pattern#284
(define_insn "*thumb1_zero_extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "=l,l")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "l,m")))]
  "TARGET_THUMB1"
{
  rtx mem;

  if (which_alternative == 0 && arm_arch6)
    return "uxth\t%0, %1";
  if (which_alternative == 0)
    return "#";

  mem = XEXP (operands[1], 0);

  if (GET_CODE (mem) == CONST)
    mem = XEXP (mem, 0);
    
  if (GET_CODE (mem) == PLUS)
    {
      rtx a = XEXP (mem, 0);

      /* This can happen due to bugs in reload.  */
      if (REG_P (a) && REGNO (a) == SP_REGNUM)
        {
          rtx ops[2];
          ops[0] = operands[0];
          ops[1] = a;
      
          output_asm_insn ("mov\t%0, %1", ops);

          XEXP (mem, 0) = operands[0];
       }
    }
    
  return "ldrh\t%0, %1";
}
  [(set_attr_alternative "length"
			 [(if_then_else (eq_attr "is_arch6" "yes")
				       (const_int 2) (const_int 4))
			 (const_int 4)])
   (set_attr "type" "simple_alu_shift, load_byte")]
)

//Pattern#285
(define_insn "*arm_zero_extendhisi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && arm_arch4 && !arm_arch6"
  "@
   #
   ldr%(h%)\\t%0, %1"
  [(set_attr "type" "alu_shift,load_byte")
   (set_attr "predicable" "yes")]
)

//Pattern#286
(define_insn "*arm_zero_extendhisi2_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && arm_arch6"
  "@
   uxth%?\\t%0, %1
   ldr%(h%)\\t%0, %1"
  [(set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_shift,load_byte")]
)

//Pattern#287
(define_insn "*arm_zero_extendhisi2addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (zero_extend:SI (match_operand:HI 1 "s_register_operand" "r"))
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_INT_SIMD"
  "uxtah%?\\t%0, %2, %1"
  [(set_attr "type" "alu_shift")
   (set_attr "predicable" "yes")]
)

//Pattern#288
(define_expand "zero_extendqisi2"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "")))]
  "TARGET_EITHER"
{
  if (TARGET_ARM && !arm_arch6 && !MEM_P (operands[1]))
    {
      emit_insn (gen_andsi3 (operands[0],
			     gen_lowpart (SImode, operands[1]),
					  GEN_INT (255)));
      DONE;
    }
  if (!arm_arch6 && !MEM_P (operands[1]))
    {
      rtx t = gen_lowpart (SImode, operands[1]);
      rtx tmp = gen_reg_rtx (SImode);
      emit_insn (gen_ashlsi3 (tmp, t, GEN_INT (24)));
      emit_insn (gen_lshrsi3 (operands[0], tmp, GEN_INT (24)));
      DONE;
    }
})

//Pattern#289
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (match_operand:QI 1 "s_register_operand" "")))]
  "!arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 24)))
   (set (match_dup 0) (lshiftrt:SI (match_dup 0) (const_int 24)))]
{
  operands[2] = simplify_gen_subreg (SImode, operands[1], QImode, 0);
  if (TARGET_ARM)
    {
      emit_insn (gen_andsi3 (operands[0], operands[2], GEN_INT (255)));
      DONE;
    }
})

//Pattern#290
(define_insn "*thumb1_zero_extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "=l,l")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "l,m")))]
  "TARGET_THUMB1 && !arm_arch6"
  "@
   #
   ldrb\\t%0, %1"
  [(set_attr "length" "4,2")
   (set_attr "type" "alu_shift,load_byte")
   (set_attr "pool_range" "*,32")]
)

//Pattern#291
(define_insn "*thumb1_zero_extendqisi2_v6"
  [(set (match_operand:SI 0 "register_operand" "=l,l")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "l,m")))]
  "TARGET_THUMB1 && arm_arch6"
  "@
   uxtb\\t%0, %1
   ldrb\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "type" "simple_alu_shift,load_byte")]
)

//Pattern#292
(define_insn "*arm_zero_extendqisi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && !arm_arch6"
  "@
   #
   ldr%(b%)\\t%0, %1\\t%@ zero_extendqisi2"
  [(set_attr "length" "8,4")
   (set_attr "type" "alu_shift,load_byte")
   (set_attr "predicable" "yes")]
)

//Pattern#293
(define_insn "*arm_zero_extendqisi2_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && arm_arch6"
  "@
   uxtb%(%)\\t%0, %1
   ldr%(b%)\\t%0, %1\\t%@ zero_extendqisi2"
  [(set_attr "type" "simple_alu_shift,load_byte")
   (set_attr "predicable" "yes")]
)

//Pattern#294
(define_insn "*arm_zero_extendqisi2addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (zero_extend:SI (match_operand:QI 1 "s_register_operand" "r"))
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_INT_SIMD"
  "uxtab%?\\t%0, %2, %1"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "xtab")
   (set_attr "type" "alu_shift")]
)

//Pattern#295
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (subreg:QI (match_operand:SI 1 "" "") 0)))
   (clobber (match_operand:SI 2 "s_register_operand" ""))]
  "TARGET_32BIT && (!MEM_P (operands[1])) && ! BYTES_BIG_ENDIAN"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (and:SI (match_dup 2) (const_int 255)))]
  ""
)

//Pattern#296
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (subreg:QI (match_operand:SI 1 "" "") 3)))
   (clobber (match_operand:SI 2 "s_register_operand" ""))]
  "TARGET_32BIT && (!MEM_P (operands[1])) && BYTES_BIG_ENDIAN"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (and:SI (match_dup 2) (const_int 255)))]
  ""
)


//Pattern#297
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(ior_xor:SI (and:SI (ashift:SI
			     (match_operand:SI 1 "s_register_operand" "")
			     (match_operand:SI 2 "const_int_operand" ""))
			    (match_operand:SI 3 "const_int_operand" ""))
		    (zero_extend:SI
		     (match_operator 5 "subreg_lowpart_operator"
		      [(match_operand:SI 4 "s_register_operand" "")]))))]
  "TARGET_32BIT
   && ((unsigned HOST_WIDE_INT) INTVAL (operands[3])
       == (GET_MODE_MASK (GET_MODE (operands[5]))
           & (GET_MODE_MASK (GET_MODE (operands[5]))
	      << (INTVAL (operands[2])))))"
  [(set (match_dup 0) (ior_xor:SI (ashift:SI (match_dup 1) (match_dup 2))
				  (match_dup 4)))
   (set (match_dup 0) (zero_extend:SI (match_dup 5)))]
  "operands[5] = gen_lowpart (GET_MODE (operands[5]), operands[0]);"
)

//Pattern#298
(define_insn "*compareqi_eq0"
  [(set (reg:CC_Z CC_REGNUM)
	(compare:CC_Z (match_operand:QI 0 "s_register_operand" "r")
			 (const_int 0)))]
  "TARGET_32BIT"
  "tst%?\\t%0, #255"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")]
)

//Pattern#299
(define_expand "extendhisi2"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "")))]
  "TARGET_EITHER"
{
  if (TARGET_THUMB1)
    {
      emit_insn (gen_thumb1_extendhisi2 (operands[0], operands[1]));
      DONE;
    }
  if (MEM_P (operands[1]) && TARGET_ARM && !arm_arch4)
    {
      emit_insn (gen_extendhisi2_mem (operands[0], operands[1]));
      DONE;
    }

  if (!arm_arch6 && !MEM_P (operands[1]))
    {
      rtx t = gen_lowpart (SImode, operands[1]);
      rtx tmp = gen_reg_rtx (SImode);
      emit_insn (gen_ashlsi3 (tmp, t, GEN_INT (16)));
      emit_insn (gen_ashrsi3 (operands[0], tmp, GEN_INT (16)));
      DONE;
    }
})

//Pattern#300
(define_split
  [(parallel
    [(set (match_operand:SI 0 "register_operand" "")
	  (sign_extend:SI (match_operand:HI 1 "register_operand" "")))
     (clobber (match_scratch:SI 2 ""))])]
  "!arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 16)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (const_int 16)))]
{
  operands[2] = simplify_gen_subreg (SImode, operands[1], HImode, 0);
})

;; We used to have an early-clobber on the scratch register here.
;; However, there's a bug somewhere in reload which means that this
;; can be partially ignored during spill allocation if the memory
;; address also needs reloading; this causes us to die later on when
;; we try to verify the operands.  Fortunately, we don't really need
;; the early-clobber: we can always use operand 0 if operand 2
;; overlaps the address.
//Pattern#301
(define_insn "thumb1_extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "=l,l")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "l,m")))
   (clobber (match_scratch:SI 2 "=X,l"))]
  "TARGET_THUMB1"
  "*
  {
    rtx ops[4];
    rtx mem;

    if (which_alternative == 0 && !arm_arch6)
      return \"#\";
    if (which_alternative == 0)
      return \"sxth\\t%0, %1\";

    mem = XEXP (operands[1], 0);

    /* This code used to try to use 'V', and fix the address only if it was
       offsettable, but this fails for e.g. REG+48 because 48 is outside the
       range of QImode offsets, and offsettable_address_p does a QImode
       address check.  */
       
    if (GET_CODE (mem) == CONST)
      mem = XEXP (mem, 0);
    
    if (GET_CODE (mem) == LABEL_REF)
      return \"ldr\\t%0, %1\";
    
    if (GET_CODE (mem) == PLUS)
      {
        rtx a = XEXP (mem, 0);
        rtx b = XEXP (mem, 1);

        if (GET_CODE (a) == LABEL_REF
	    && CONST_INT_P (b))
          return \"ldr\\t%0, %1\";

        if (REG_P (b))
          return \"ldrsh\\t%0, %1\";
	  
        ops[1] = a;
        ops[2] = b;
      }
    else
      {
        ops[1] = mem;
        ops[2] = const0_rtx;
      }
      
    gcc_assert (REG_P (ops[1]));

    ops[0] = operands[0];
    if (reg_mentioned_p (operands[2], ops[1]))
      ops[3] = ops[0];
    else
      ops[3] = operands[2];
    output_asm_insn (\"mov\\t%3, %2\;ldrsh\\t%0, [%1, %3]\", ops);
    return \"\";
  }"
  [(set_attr_alternative "length"
			 [(if_then_else (eq_attr "is_arch6" "yes")
					(const_int 2) (const_int 4))
			  (const_int 4)])
   (set_attr "type" "simple_alu_shift,load_byte")
   (set_attr "pool_range" "*,1018")]
)

;; This pattern will only be used when ldsh is not available
//Pattern#302
(define_expand "extendhisi2_mem"
  [(set (match_dup 2) (zero_extend:SI (match_operand:HI 1 "" "")))
   (set (match_dup 3)
	(zero_extend:SI (match_dup 7)))
   (set (match_dup 6) (ashift:SI (match_dup 4) (const_int 24)))
   (set (match_operand:SI 0 "" "")
	(ior:SI (ashiftrt:SI (match_dup 6) (const_int 16)) (match_dup 5)))]
  "TARGET_ARM"
  "
  {
    rtx mem1, mem2;
    rtx addr = copy_to_mode_reg (SImode, XEXP (operands[1], 0));

    mem1 = change_address (operands[1], QImode, addr);
    mem2 = change_address (operands[1], QImode,
			   plus_constant (Pmode, addr, 1));
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = mem1;
    operands[2] = gen_reg_rtx (SImode);
    operands[3] = gen_reg_rtx (SImode);
    operands[6] = gen_reg_rtx (SImode);
    operands[7] = mem2;

    if (BYTES_BIG_ENDIAN)
      {
	operands[4] = operands[2];
	operands[5] = operands[3];
      }
    else
      {
	operands[4] = operands[3];
	operands[5] = operands[2];
      }
  }"
)

//Pattern#303
(define_split
  [(set (match_operand:SI 0 "register_operand" "")
	(sign_extend:SI (match_operand:HI 1 "register_operand" "")))]
  "!arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 16)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (const_int 16)))]
{
  operands[2] = simplify_gen_subreg (SImode, operands[1], HImode, 0);
})

//Pattern#304
(define_insn "*arm_extendhisi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && arm_arch4 && !arm_arch6"
  "@
   #
   ldr%(sh%)\\t%0, %1"
  [(set_attr "length" "8,4")
   (set_attr "type" "alu_shift,load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,256")
   (set_attr "neg_pool_range" "*,244")]
)

;; ??? Check Thumb-2 pool range
//Pattern#305
(define_insn "*arm_extendhisi2_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_32BIT && arm_arch6"
  "@
   sxth%?\\t%0, %1
   ldr%(sh%)\\t%0, %1"
  [(set_attr "type" "simple_alu_shift,load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,256")
   (set_attr "neg_pool_range" "*,244")]
)

//Pattern#306
(define_insn "*arm_extendhisi2addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (sign_extend:SI (match_operand:HI 1 "s_register_operand" "r"))
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_INT_SIMD"
  "sxtah%?\\t%0, %2, %1"
)

//Pattern#307
(define_expand "extendqihi2"
  [(set (match_dup 2)
	(ashift:SI (match_operand:QI 1 "arm_reg_or_extendqisi_mem_op" "")
		   (const_int 24)))
   (set (match_operand:HI 0 "s_register_operand" "")
	(ashiftrt:SI (match_dup 2)
		     (const_int 24)))]
  "TARGET_ARM"
  "
  {
    if (arm_arch4 && MEM_P (operands[1]))
      {
	emit_insn (gen_rtx_SET (VOIDmode,
				operands[0],
				gen_rtx_SIGN_EXTEND (HImode, operands[1])));
	DONE;
      }
    if (!s_register_operand (operands[1], QImode))
      operands[1] = copy_to_mode_reg (QImode, operands[1]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_reg_rtx (SImode);
  }"
)

//Pattern#308
(define_insn "*arm_extendqihi_insn"
  [(set (match_operand:HI 0 "s_register_operand" "=r")
	(sign_extend:HI (match_operand:QI 1 "arm_extendqisi_mem_op" "Uq")))]
  "TARGET_ARM && arm_arch4"
  "ldr%(sb%)\\t%0, %1"
  [(set_attr "type" "load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "256")
   (set_attr "neg_pool_range" "244")]
)

//Pattern#309
(define_expand "extendqisi2"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(sign_extend:SI (match_operand:QI 1 "arm_reg_or_extendqisi_mem_op" "")))]
  "TARGET_EITHER"
{
  if (!arm_arch4 && MEM_P (operands[1]))
    operands[1] = copy_to_mode_reg (QImode, operands[1]);

  if (!arm_arch6 && !MEM_P (operands[1]))
    {
      rtx t = gen_lowpart (SImode, operands[1]);
      rtx tmp = gen_reg_rtx (SImode);
      emit_insn (gen_ashlsi3 (tmp, t, GEN_INT (24)));
      emit_insn (gen_ashrsi3 (operands[0], tmp, GEN_INT (24)));
      DONE;
    }
})

//Pattern#310
(define_split
  [(set (match_operand:SI 0 "register_operand" "")
	(sign_extend:SI (match_operand:QI 1 "register_operand" "")))]
  "!arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 24)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (const_int 24)))]
{
  operands[2] = simplify_gen_subreg (SImode, operands[1], QImode, 0);
})

//Pattern#311
(define_insn "*arm_extendqisi"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(sign_extend:SI (match_operand:QI 1 "arm_reg_or_extendqisi_mem_op" "r,Uq")))]
  "TARGET_ARM && arm_arch4 && !arm_arch6"
  "@
   #
   ldr%(sb%)\\t%0, %1"
  [(set_attr "length" "8,4")
   (set_attr "type" "alu_shift,load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,256")
   (set_attr "neg_pool_range" "*,244")]
)

//Pattern#312
(define_insn "*arm_extendqisi_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(sign_extend:SI
	 (match_operand:QI 1 "arm_reg_or_extendqisi_mem_op" "r,Uq")))]
  "TARGET_ARM && arm_arch6"
  "@
   sxtb%?\\t%0, %1
   ldr%(sb%)\\t%0, %1"
  [(set_attr "type" "simple_alu_shift,load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,256")
   (set_attr "neg_pool_range" "*,244")]
)

//Pattern#313
(define_insn "*arm_extendqisi2addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (sign_extend:SI (match_operand:QI 1 "s_register_operand" "r"))
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_INT_SIMD"
  "sxtab%?\\t%0, %2, %1"
  [(set_attr "type" "alu_shift")
   (set_attr "insn" "xtab")
   (set_attr "predicable" "yes")]
)

//Pattern#314
(define_split
  [(set (match_operand:SI 0 "register_operand" "")
	(sign_extend:SI (match_operand:QI 1 "memory_operand" "")))]
  "TARGET_THUMB1 && reload_completed"
  [(set (match_dup 0) (match_dup 2))
   (set (match_dup 0) (sign_extend:SI (match_dup 3)))]
{
  rtx addr = XEXP (operands[1], 0);

  if (GET_CODE (addr) == CONST)
    addr = XEXP (addr, 0);

  if (GET_CODE (addr) == PLUS
      && REG_P (XEXP (addr, 0)) && REG_P (XEXP (addr, 1)))
    /* No split necessary.  */
    FAIL;

  if (GET_CODE (addr) == PLUS
      && !REG_P (XEXP (addr, 0)) && !REG_P (XEXP (addr, 1)))
    FAIL;

  if (reg_overlap_mentioned_p (operands[0], addr))
    {
      rtx t = gen_lowpart (QImode, operands[0]);
      emit_move_insn (t, operands[1]);
      emit_insn (gen_thumb1_extendqisi2 (operands[0], t));
      DONE;
    }

  if (REG_P (addr))
    {
      addr = gen_rtx_PLUS (Pmode, addr, operands[0]);
      operands[2] = const0_rtx;
    }
  else if (GET_CODE (addr) != PLUS)
    FAIL;
  else if (REG_P (XEXP (addr, 0)))
    {
      operands[2] = XEXP (addr, 1);
      addr = gen_rtx_PLUS (Pmode, XEXP (addr, 0), operands[0]);
    }
  else
    {
      operands[2] = XEXP (addr, 0);
      addr = gen_rtx_PLUS (Pmode, XEXP (addr, 1), operands[0]);
    }

  operands[3] = change_address (operands[1], QImode, addr);
})

//Pattern#315
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(plus:SI (match_dup 0) (match_operand 1 "const_int_operand")))
   (set (match_operand:SI 2 "register_operand" "") (const_int 0))
   (set (match_operand:SI 3 "register_operand" "")
	(sign_extend:SI (match_operand:QI 4 "memory_operand" "")))]
  "TARGET_THUMB1
   && GET_CODE (XEXP (operands[4], 0)) == PLUS
   && rtx_equal_p (operands[0], XEXP (XEXP (operands[4], 0), 0))
   && rtx_equal_p (operands[2], XEXP (XEXP (operands[4], 0), 1))
   && (peep2_reg_dead_p (3, operands[0])
       || rtx_equal_p (operands[0], operands[3]))
   && (peep2_reg_dead_p (3, operands[2])
       || rtx_equal_p (operands[2], operands[3]))"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 3) (sign_extend:SI (match_dup 4)))]
{
  rtx addr = gen_rtx_PLUS (Pmode, operands[0], operands[2]);
  operands[4] = change_address (operands[4], QImode, addr);
})

//Pattern#316
(define_insn "thumb1_extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "=l,l,l")
	(sign_extend:SI (match_operand:QI 1 "nonimmediate_operand" "l,V,m")))]
  "TARGET_THUMB1"
{
  rtx addr;

  if (which_alternative == 0 && arm_arch6)
    return "sxtb\\t%0, %1";
  if (which_alternative == 0)
    return "#";

  addr = XEXP (operands[1], 0);
  if (GET_CODE (addr) == PLUS
      && REG_P (XEXP (addr, 0)) && REG_P (XEXP (addr, 1)))
    return "ldrsb\\t%0, %1";
      
  return "#";
}
  [(set_attr_alternative "length"
			 [(if_then_else (eq_attr "is_arch6" "yes")
					(const_int 2) (const_int 4))
			  (const_int 2)
			  (if_then_else (eq_attr "is_arch6" "yes")
					(const_int 4) (const_int 6))])
   (set_attr "type" "simple_alu_shift,load_byte,load_byte")]
)

//Pattern#317
(define_expand "extendsfdf2"
  [(set (match_operand:DF                  0 "s_register_operand" "")
	(float_extend:DF (match_operand:SF 1 "s_register_operand"  "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  ""
)

/* HFmode -> DFmode conversions have to go through SFmode.  */
//Pattern#318
(define_expand "extendhfdf2"
  [(set (match_operand:DF                  0 "general_operand" "")
	(float_extend:DF (match_operand:HF 1 "general_operand"  "")))]
  "TARGET_EITHER"
  "
  {
    rtx op1;
    op1 = convert_to_mode (SFmode, operands[1], 0);
    op1 = convert_to_mode (DFmode, op1, 0);
    emit_insn (gen_movdf (operands[0], op1));
    DONE;
  }"
)

;; Move insns (including loads and stores)

;; XXX Just some ideas about movti.
;; I don't think these are a good idea on the arm, there just aren't enough
;; registers
//Pattern#319
;;(define_expand "loadti"
;;  [(set (match_operand:TI 0 "s_register_operand" "")
;;	(mem:TI (match_operand:SI 1 "address_operand" "")))]
;;  "" "")

//Pattern#320
;;(define_expand "storeti"
;;  [(set (mem:TI (match_operand:TI 0 "address_operand" ""))
;;	(match_operand:TI 1 "s_register_operand" ""))]
;;  "" "")

//Pattern#321
;;(define_expand "movti"
;;  [(set (match_operand:TI 0 "general_operand" "")
;;	(match_operand:TI 1 "general_operand" ""))]
;;  ""
;;  "
;;{
;;  rtx insn;
;;
;;  if (MEM_P (operands[0]) && MEM_P (operands[1]))
;;    operands[1] = copy_to_reg (operands[1]);
;;  if (MEM_P (operands[0]))
;;    insn = gen_storeti (XEXP (operands[0], 0), operands[1]);
;;  else if (MEM_P (operands[1]))
;;    insn = gen_loadti (operands[0], XEXP (operands[1], 0));
;;  else
;;    FAIL;
;;
;;  emit_insn (insn);
;;  DONE;
;;}")

;; Recognize garbage generated above.

//Pattern#322
;;(define_insn ""
;;  [(set (match_operand:TI 0 "general_operand" "=r,r,r,<,>,m")
;;	(match_operand:TI 1 "general_operand" "<,>,m,r,r,r"))]
;;  ""
;;  "*
;;  {
;;    register mem = (which_alternative < 3);
;;    register const char *template;
;;
;;    operands[mem] = XEXP (operands[mem], 0);
;;    switch (which_alternative)
;;      {
;;      case 0: template = \"ldmdb\\t%1!, %M0\"; break;
;;      case 1: template = \"ldmia\\t%1!, %M0\"; break;
;;      case 2: template = \"ldmia\\t%1, %M0\"; break;
;;      case 3: template = \"stmdb\\t%0!, %M1\"; break;
;;      case 4: template = \"stmia\\t%0!, %M1\"; break;
;;      case 5: template = \"stmia\\t%0, %M1\"; break;
;;      }
;;    output_asm_insn (template, operands);
;;    return \"\";
;;  }")

//Pattern#323
(define_expand "movdi"
  [(set (match_operand:DI 0 "general_operand" "")
	(match_operand:DI 1 "general_operand" ""))]
  "TARGET_EITHER"
  "
  if (can_create_pseudo_p ())
    {
      if (!REG_P (operands[0]))
	operands[1] = force_reg (DImode, operands[1]);
    }
  "
)

//Pattern#324
(define_insn "*arm_movdi"
  [(set (match_operand:DI 0 "nonimmediate_di_operand" "=r, r, r, r, m")
	(match_operand:DI 1 "di_operand"              "rDa,Db,Dc,mi,r"))]
  "TARGET_32BIT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && !TARGET_IWMMXT
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))"
  "*
  switch (which_alternative)
    {
    case 0:
    case 1:
    case 2:
      return \"#\";
    default:
      return output_move_double (operands, true, NULL);
    }
  "
  [(set_attr "length" "8,12,16,8,8")
   (set_attr "type" "*,*,*,load2,store2")
   (set_attr "arm_pool_range" "*,*,*,1020,*")
   (set_attr "arm_neg_pool_range" "*,*,*,1004,*")
   (set_attr "thumb2_pool_range" "*,*,*,4094,*")
   (set_attr "thumb2_neg_pool_range" "*,*,*,0,*")]
)

//Pattern#325
(define_split
  [(set (match_operand:ANY64 0 "arm_general_register_operand" "")
	(match_operand:ANY64 1 "const_double_operand" ""))]
  "TARGET_32BIT
   && reload_completed
   && (arm_const_double_inline_cost (operands[1])
       <= ((optimize_size || arm_ld_sched) ? 3 : 4))"
  [(const_int 0)]
  "
  arm_split_constant (SET, SImode, curr_insn,
		      INTVAL (gen_lowpart (SImode, operands[1])),
		      gen_lowpart (SImode, operands[0]), NULL_RTX, 0);
  arm_split_constant (SET, SImode, curr_insn,
		      INTVAL (gen_highpart_mode (SImode,
						 GET_MODE (operands[0]),
						 operands[1])),
		      gen_highpart (SImode, operands[0]), NULL_RTX, 0);
  DONE;
  "
)

; If optimizing for size, or if we have load delay slots, then 
; we want to split the constant into two separate operations. 
; In both cases this may split a trivial part into a single data op
; leaving a single complex constant to load.  We can also get longer
; offsets in a LDR which means we get better chances of sharing the pool
; entries.  Finally, we can normally do a better job of scheduling
; LDR instructions than we can with LDM.
; This pattern will only match if the one above did not.
//Pattern#326
(define_split
  [(set (match_operand:ANY64 0 "arm_general_register_operand" "")
	(match_operand:ANY64 1 "const_double_operand" ""))]
  "TARGET_ARM && reload_completed
   && arm_const_double_by_parts (operands[1])"
  [(set (match_dup 0) (match_dup 1))
   (set (match_dup 2) (match_dup 3))]
  "
  operands[2] = gen_highpart (SImode, operands[0]);
  operands[3] = gen_highpart_mode (SImode, GET_MODE (operands[0]),
				   operands[1]);
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  "
)

//Pattern#327
(define_split
  [(set (match_operand:ANY64 0 "arm_general_register_operand" "")
	(match_operand:ANY64 1 "arm_general_register_operand" ""))]
  "TARGET_EITHER && reload_completed"
  [(set (match_dup 0) (match_dup 1))
   (set (match_dup 2) (match_dup 3))]
  "
  operands[2] = gen_highpart (SImode, operands[0]);
  operands[3] = gen_highpart (SImode, operands[1]);
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = gen_lowpart (SImode, operands[1]);

  /* Handle a partial overlap.  */
  if (rtx_equal_p (operands[0], operands[3]))
    {
      rtx tmp0 = operands[0];
      rtx tmp1 = operands[1];

      operands[0] = operands[2];
      operands[1] = operands[3];
      operands[2] = tmp0;
      operands[3] = tmp1;
    }
  "
)

;; We can't actually do base+index doubleword loads if the index and
;; destination overlap.  Split here so that we at least have chance to
;; schedule.
//Pattern#328
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(mem:DI (plus:SI (match_operand:SI 1 "s_register_operand" "")
			 (match_operand:SI 2 "s_register_operand" ""))))]
  "TARGET_LDRD
  && reg_overlap_mentioned_p (operands[0], operands[1])
  && reg_overlap_mentioned_p (operands[0], operands[2])"
  [(set (match_dup 4)
	(plus:SI (match_dup 1)
		 (match_dup 2)))
   (set (match_dup 0)
	(mem:DI (match_dup 4)))]
  "
  operands[4] = gen_rtx_REG (SImode, REGNO(operands[0]));
  "
)

;;; ??? This should have alternatives for constants.
;;; ??? This was originally identical to the movdf_insn pattern.
;;; ??? The 'i' constraint looks funny, but it should always be replaced by
;;; thumb_reorg with a memory reference.
//Pattern#329
(define_insn "*thumb1_movdi_insn"
  [(set (match_operand:DI 0 "nonimmediate_operand" "=l,l,l,l,>,l, m,*r")
	(match_operand:DI 1 "general_operand"      "l, I,J,>,l,mi,l,*r"))]
  "TARGET_THUMB1
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))"
  "*
  {
  switch (which_alternative)
    {
    default:
    case 0:
      if (REGNO (operands[1]) == REGNO (operands[0]) + 1)
	return \"add\\t%0,  %1,  #0\;add\\t%H0, %H1, #0\";
      return   \"add\\t%H0, %H1, #0\;add\\t%0,  %1,  #0\";
    case 1:
      return \"mov\\t%Q0, %1\;mov\\t%R0, #0\";
    case 2:
      operands[1] = GEN_INT (- INTVAL (operands[1]));
      return \"mov\\t%Q0, %1\;neg\\t%Q0, %Q0\;asr\\t%R0, %Q0, #31\";
    case 3:
      return \"ldmia\\t%1, {%0, %H0}\";
    case 4:
      return \"stmia\\t%0, {%1, %H1}\";
    case 5:
      return thumb_load_double_from_address (operands);
    case 6:
      operands[2] = gen_rtx_MEM (SImode,
			     plus_constant (Pmode, XEXP (operands[0], 0), 4));
      output_asm_insn (\"str\\t%1, %0\;str\\t%H1, %2\", operands);
      return \"\";
    case 7:
      if (REGNO (operands[1]) == REGNO (operands[0]) + 1)
	return \"mov\\t%0, %1\;mov\\t%H0, %H1\";
      return \"mov\\t%H0, %H1\;mov\\t%0, %1\";
    }
  }"
  [(set_attr "length" "4,4,6,2,2,6,4,4")
   (set_attr "type" "*,*,*,load2,store2,load2,store2,*")
   (set_attr "insn" "*,mov,*,*,*,*,*,mov")
   (set_attr "pool_range" "*,*,*,*,*,1018,*,*")]
)

//Pattern#330
(define_expand "movsi"
  [(set (match_operand:SI 0 "general_operand" "")
        (match_operand:SI 1 "general_operand" ""))]
  "TARGET_EITHER"
  "
  {
  rtx base, offset, tmp;

  if (TARGET_32BIT)
    {
      /* Everything except mem = const or mem = mem can be done easily.  */
      if (MEM_P (operands[0]))
        operands[1] = force_reg (SImode, operands[1]);
      if (arm_general_register_operand (operands[0], SImode)
	  && CONST_INT_P (operands[1])
          && !(const_ok_for_arm (INTVAL (operands[1]))
               || const_ok_for_arm (~INTVAL (operands[1]))))
        {
           arm_split_constant (SET, SImode, NULL_RTX,
	                       INTVAL (operands[1]), operands[0], NULL_RTX,
			       optimize && can_create_pseudo_p ());
          DONE;
        }
    }
  else /* TARGET_THUMB1...  */
    {
      if (can_create_pseudo_p ())
        {
          if (!REG_P (operands[0]))
	    operands[1] = force_reg (SImode, operands[1]);
        }
    }

  if (ARM_OFFSETS_MUST_BE_WITHIN_SECTIONS_P)
    {
      split_const (operands[1], &base, &offset);
      if (GET_CODE (base) == SYMBOL_REF
	  && !offset_within_block_p (base, INTVAL (offset)))
	{
	  tmp = can_create_pseudo_p () ? gen_reg_rtx (SImode) : operands[0];
	  emit_move_insn (tmp, base);
	  emit_insn (gen_addsi3 (operands[0], tmp, offset));
	  DONE;
	}
    }

  /* Recognize the case where operand[1] is a reference to thread-local
     data and load its address to a register.  */
  if (arm_tls_referenced_p (operands[1]))
    {
      rtx tmp = operands[1];
      rtx addend = NULL;

      if (GET_CODE (tmp) == CONST && GET_CODE (XEXP (tmp, 0)) == PLUS)
        {
          addend = XEXP (XEXP (tmp, 0), 1);
          tmp = XEXP (XEXP (tmp, 0), 0);
        }

      gcc_assert (GET_CODE (tmp) == SYMBOL_REF);
      gcc_assert (SYMBOL_REF_TLS_MODEL (tmp) != 0);

      tmp = legitimize_tls_address (tmp,
				    !can_create_pseudo_p () ? operands[0] : 0);
      if (addend)
        {
          tmp = gen_rtx_PLUS (SImode, tmp, addend);
          tmp = force_operand (tmp, operands[0]);
        }
      operands[1] = tmp;
    }
  else if (flag_pic
	   && (CONSTANT_P (operands[1])
	       || symbol_mentioned_p (operands[1])
	       || label_mentioned_p (operands[1])))
      operands[1] = legitimize_pic_address (operands[1], SImode,
					    (!can_create_pseudo_p ()
					     ? operands[0]
					     : 0));
  }
  "
)

;; The ARM LO_SUM and HIGH are backwards - HIGH sets the low bits, and
;; LO_SUM adds in the high bits.  Fortunately these are opaque operations
;; so this does not matter.
//Pattern#331
(define_insn "*arm_movt"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=r")
	(lo_sum:SI (match_operand:SI 1 "nonimmediate_operand" "0")
		   (match_operand:SI 2 "general_operand"      "i")))]
  "arm_arch_thumb2"
  "movt%?\t%0, #:upper16:%c2"
  [(set_attr "predicable" "yes")
   (set_attr "length" "4")]
)

//Pattern#332
(define_insn "*arm_movsi_insn"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=rk,r,r,r,rk,m")
	(match_operand:SI 1 "general_operand"      "rk, I,K,j,mi,rk"))]
  "TARGET_ARM && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))"
  "@
   mov%?\\t%0, %1
   mov%?\\t%0, %1
   mvn%?\\t%0, #%B1
   movw%?\\t%0, %1
   ldr%?\\t%0, %1
   str%?\\t%1, %0"
  [(set_attr "type" "*,simple_alu_imm,simple_alu_imm,simple_alu_imm,load1,store1")
   (set_attr "insn" "mov,mov,mvn,mov,*,*")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,*,*,*,4096,*")
   (set_attr "neg_pool_range" "*,*,*,*,4084,*")]
)

//Pattern#333
(define_split
  [(set (match_operand:SI 0 "arm_general_register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "TARGET_32BIT
  && (!(const_ok_for_arm (INTVAL (operands[1]))
        || const_ok_for_arm (~INTVAL (operands[1]))))"
  [(clobber (const_int 0))]
  "
  arm_split_constant (SET, SImode, NULL_RTX, 
                      INTVAL (operands[1]), operands[0], NULL_RTX, 0);
  DONE;
  "
)

;; Split symbol_refs at the later stage (after cprop), instead of generating
;; movt/movw pair directly at expand.  Otherwise corresponding high_sum
;; and lo_sum would be merged back into memory load at cprop.  However,
;; if the default is to prefer movt/movw rather than a load from the constant
;; pool, the performance is better.
//Pattern#334
(define_split
  [(set (match_operand:SI 0 "arm_general_register_operand" "")
       (match_operand:SI 1 "general_operand" ""))]
  "TARGET_32BIT
   && TARGET_USE_MOVT && GET_CODE (operands[1]) == SYMBOL_REF
   && !flag_pic && !target_word_relocations
   && !arm_tls_referenced_p (operands[1])"
  [(clobber (const_int 0))]
{
  arm_emit_movpair (operands[0], operands[1]);
  DONE;
})

//Pattern#335
(define_insn "*thumb1_movsi_insn"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=l,l,l,l,l,>,l, m,*l*h*k")
	(match_operand:SI 1 "general_operand"      "l, I,J,K,>,l,mi,l,*l*h*k"))]
  "TARGET_THUMB1
   && (   register_operand (operands[0], SImode) 
       || register_operand (operands[1], SImode))"
  "@
   mov	%0, %1
   mov	%0, %1
   #
   #
   ldmia\\t%1, {%0}
   stmia\\t%0, {%1}
   ldr\\t%0, %1
   str\\t%1, %0
   mov\\t%0, %1"
  [(set_attr "length" "2,2,4,4,2,2,2,2,2")
   (set_attr "type" "*,*,*,*,load1,store1,load1,store1,*")
   (set_attr "pool_range" "*,*,*,*,*,*,1018,*,*")
   (set_attr "conds" "set,clob,*,*,nocond,nocond,nocond,nocond,nocond")])

//Pattern#336
(define_split 
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "TARGET_THUMB1 && satisfies_constraint_J (operands[1])"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (neg:SI (match_dup 2)))]
  "
  {
    operands[1] = GEN_INT (- INTVAL (operands[1]));
    operands[2] = can_create_pseudo_p () ? gen_reg_rtx (SImode) : operands[0];
  }"
)

//Pattern#337
(define_split 
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "TARGET_THUMB1 && satisfies_constraint_K (operands[1])"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (ashift:SI (match_dup 2) (match_dup 3)))]
  "
  {
    unsigned HOST_WIDE_INT val = INTVAL (operands[1]) & 0xffffffffu;
    unsigned HOST_WIDE_INT mask = 0xff;
    int i;
    
    for (i = 0; i < 25; i++)
      if ((val & (mask << i)) == val)
        break;

    /* Don't split if the shift is zero.  */
    if (i == 0)
      FAIL;

    operands[1] = GEN_INT (val >> i);
    operands[2] = can_create_pseudo_p () ? gen_reg_rtx (SImode) : operands[0];
    operands[3] = GEN_INT (i);
  }"
)

;; For thumb1 split imm move [256-510] into mov [1-255] and add #255
//Pattern#338
(define_split 
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "TARGET_THUMB1 && satisfies_constraint_Pe (operands[1])"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (plus:SI (match_dup 2) (match_dup 3)))]
  "
  {
    operands[1] = GEN_INT (INTVAL (operands[1]) - 255);
    operands[2] = can_create_pseudo_p () ? gen_reg_rtx (SImode) : operands[0];
    operands[3] = GEN_INT (255);
  }"
)

;; When generating pic, we need to load the symbol offset into a register.
;; So that the optimizer does not confuse this with a normal symbol load
;; we use an unspec.  The offset will be loaded from a constant pool entry,
;; since that is the only type of relocation we can use.

;; Wrap calculation of the whole PIC address in a single pattern for the
;; benefit of optimizers, particularly, PRE and HOIST.  Calculation of
;; a PIC address involves two loads from memory, so we want to CSE it
;; as often as possible.
;; This pattern will be split into one of the pic_load_addr_* patterns
;; and a move after GCSE optimizations.
;;
;; Note: Update arm.c: legitimize_pic_address() when changing this pattern.
//Pattern#339
(define_expand "calculate_pic_address"
  [(set (match_operand:SI 0 "register_operand" "")
	(mem:SI (plus:SI (match_operand:SI 1 "register_operand" "")
			 (unspec:SI [(match_operand:SI 2 "" "")]
				    UNSPEC_PIC_SYM))))]
  "flag_pic"
)

;; Split calculate_pic_address into pic_load_addr_* and a move.
//Pattern#340
(define_split
  [(set (match_operand:SI 0 "register_operand" "")
	(mem:SI (plus:SI (match_operand:SI 1 "register_operand" "")
			 (unspec:SI [(match_operand:SI 2 "" "")]
				    UNSPEC_PIC_SYM))))]
  "flag_pic"
  [(set (match_dup 3) (unspec:SI [(match_dup 2)] UNSPEC_PIC_SYM))
   (set (match_dup 0) (mem:SI (plus:SI (match_dup 1) (match_dup 3))))]
  "operands[3] = can_create_pseudo_p () ? gen_reg_rtx (SImode) : operands[0];"
)

;; operand1 is the memory address to go into 
;; pic_load_addr_32bit.
;; operand2 is the PIC label to be emitted 
;; from pic_add_dot_plus_eight.
;; We do this to allow hoisting of the entire insn.
//Pattern#341
(define_insn_and_split "pic_load_addr_unified"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,l")
	(unspec:SI [(match_operand:SI 1 "" "mX,mX,mX") 
		    (match_operand:SI 2 "" "")] 
		    UNSPEC_PIC_UNIFIED))]
 "flag_pic"
 "#"
 "&& reload_completed"
 [(set (match_dup 0) (unspec:SI [(match_dup 1)] UNSPEC_PIC_SYM))
  (set (match_dup 0) (unspec:SI [(match_dup 0) (match_dup 3)
       		     		 (match_dup 2)] UNSPEC_PIC_BASE))]
 "operands[3] = TARGET_THUMB ? GEN_INT (4) : GEN_INT (8);"
 [(set_attr "type" "load1,load1,load1")
  (set_attr "pool_range" "4096,4094,1022")
  (set_attr "neg_pool_range" "4084,0,0")
  (set_attr "arch"  "a,t2,t1")    
  (set_attr "length" "8,6,4")]
)

;; The rather odd constraints on the following are to force reload to leave
;; the insn alone, and to force the minipool generation pass to then move
;; the GOT symbol to memory.

//Pattern#342
(define_insn "pic_load_addr_32bit"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "" "mX")] UNSPEC_PIC_SYM))]
  "TARGET_32BIT && flag_pic"
  "ldr%?\\t%0, %1"
  [(set_attr "type" "load1")
   (set (attr "pool_range")
	(if_then_else (eq_attr "is_thumb" "no")
		      (const_int 4096)
		      (const_int 4094)))
   (set (attr "neg_pool_range")
	(if_then_else (eq_attr "is_thumb" "no")
		      (const_int 4084)
		      (const_int 0)))]
)

//Pattern#343
(define_insn "pic_load_addr_thumb1"
  [(set (match_operand:SI 0 "s_register_operand" "=l")
	(unspec:SI [(match_operand:SI 1 "" "mX")] UNSPEC_PIC_SYM))]
  "TARGET_THUMB1 && flag_pic"
  "ldr\\t%0, %1"
  [(set_attr "type" "load1")
   (set (attr "pool_range") (const_int 1018))]
)

//Pattern#344
(define_insn "pic_add_dot_plus_four"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "0")
		    (const_int 4)
		    (match_operand 2 "" "")]
		   UNSPEC_PIC_BASE))]
  "TARGET_THUMB"
  "*
  (*targetm.asm_out.internal_label) (asm_out_file, \"LPIC\",
				     INTVAL (operands[2]));
  return \"add\\t%0, %|pc\";
  "
  [(set_attr "length" "2")]
)

//Pattern#345
(define_insn "pic_add_dot_plus_eight"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (const_int 8)
		    (match_operand 2 "" "")]
		   UNSPEC_PIC_BASE))]
  "TARGET_ARM"
  "*
    (*targetm.asm_out.internal_label) (asm_out_file, \"LPIC\",
				       INTVAL (operands[2]));
    return \"add%?\\t%0, %|pc, %1\";
  "
  [(set_attr "predicable" "yes")]
)

//Pattern#346
(define_insn "tls_load_dot_plus_eight"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mem:SI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
			    (const_int 8)
			    (match_operand 2 "" "")]
			   UNSPEC_PIC_BASE)))]
  "TARGET_ARM"
  "*
    (*targetm.asm_out.internal_label) (asm_out_file, \"LPIC\",
				       INTVAL (operands[2]));
    return \"ldr%?\\t%0, [%|pc, %1]\t\t@ tls_load_dot_plus_eight\";
  "
  [(set_attr "predicable" "yes")]
)

;; PIC references to local variables can generate pic_add_dot_plus_eight
;; followed by a load.  These sequences can be crunched down to
;; tls_load_dot_plus_eight by a peephole.

//Pattern#347
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(unspec:SI [(match_operand:SI 3 "register_operand" "")
		    (const_int 8)
		    (match_operand 1 "" "")]
		   UNSPEC_PIC_BASE))
   (set (match_operand:SI 2 "arm_general_register_operand" "")
	(mem:SI (match_dup 0)))]
  "TARGET_ARM && peep2_reg_dead_p (2, operands[0])"
  [(set (match_dup 2)
	(mem:SI (unspec:SI [(match_dup 3)
			    (const_int 8)
			    (match_dup 1)]
			   UNSPEC_PIC_BASE)))]
  ""
)

//Pattern#348
(define_insn "pic_offset_arm"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mem:SI (plus:SI (match_operand:SI 1 "register_operand" "r")
			 (unspec:SI [(match_operand:SI 2 "" "X")]
				    UNSPEC_PIC_OFFSET))))]
  "TARGET_VXWORKS_RTP && TARGET_ARM && flag_pic"
  "ldr%?\\t%0, [%1,%2]"
  [(set_attr "type" "load1")]
)

//Pattern#349
(define_expand "builtin_setjmp_receiver"
  [(label_ref (match_operand 0 "" ""))]
  "flag_pic"
  "
{
  /* r3 is clobbered by set/longjmp, so we can use it as a scratch
     register.  */
  if (arm_pic_register != INVALID_REGNUM)
    arm_load_pic_register (1UL << 3);
  DONE;
}")

;; If copying one reg to another we can set the condition codes according to
;; its value.  Such a move is common after a return from subroutine and the
;; result is being tested against zero.

//Pattern#350
(define_insn "*movsi_compare0"
  [(set (reg:CC CC_REGNUM)
	(compare:CC (match_operand:SI 1 "s_register_operand" "0,r")
		    (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(match_dup 1))]
  "TARGET_32BIT"
  "@
   cmp%?\\t%0, #0
   sub%.\\t%0, %1, #0"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,simple_alu_imm")]
)

;; Subroutine to store a half word from a register into memory.
;; Operand 0 is the source register (HImode)
;; Operand 1 is the destination address in a register (SImode)

;; In both this routine and the next, we must be careful not to spill
;; a memory address of reg+large_const into a separate PLUS insn, since this
;; can generate unrecognizable rtl.

//Pattern#351
(define_expand "storehi"
  [;; store the low byte
   (set (match_operand 1 "" "") (match_dup 3))
   ;; extract the high byte
   (set (match_dup 2)
	(ashiftrt:SI (match_operand 0 "" "") (const_int 8)))
   ;; store the high byte
   (set (match_dup 4) (match_dup 5))]
  "TARGET_ARM"
  "
  {
    rtx op1 = operands[1];
    rtx addr = XEXP (op1, 0);
    enum rtx_code code = GET_CODE (addr);

    if ((code == PLUS && !CONST_INT_P (XEXP (addr, 1)))
	|| code == MINUS)
      op1 = replace_equiv_address (operands[1], force_reg (SImode, addr));

    operands[4] = adjust_address (op1, QImode, 1);
    operands[1] = adjust_address (operands[1], QImode, 0);
    operands[3] = gen_lowpart (QImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[2] = gen_reg_rtx (SImode);
    operands[5] = gen_lowpart (QImode, operands[2]);
  }"
)

//Pattern#352
(define_expand "storehi_bigend"
  [(set (match_dup 4) (match_dup 3))
   (set (match_dup 2)
	(ashiftrt:SI (match_operand 0 "" "") (const_int 8)))
   (set (match_operand 1 "" "")	(match_dup 5))]
  "TARGET_ARM"
  "
  {
    rtx op1 = operands[1];
    rtx addr = XEXP (op1, 0);
    enum rtx_code code = GET_CODE (addr);

    if ((code == PLUS && !CONST_INT_P (XEXP (addr, 1)))
	|| code == MINUS)
      op1 = replace_equiv_address (op1, force_reg (SImode, addr));

    operands[4] = adjust_address (op1, QImode, 1);
    operands[1] = adjust_address (operands[1], QImode, 0);
    operands[3] = gen_lowpart (QImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[2] = gen_reg_rtx (SImode);
    operands[5] = gen_lowpart (QImode, operands[2]);
  }"
)

;; Subroutine to store a half word integer constant into memory.
//Pattern#353
(define_expand "storeinthi"
  [(set (match_operand 0 "" "")
	(match_operand 1 "" ""))
   (set (match_dup 3) (match_dup 2))]
  "TARGET_ARM"
  "
  {
    HOST_WIDE_INT value = INTVAL (operands[1]);
    rtx addr = XEXP (operands[0], 0);
    rtx op0 = operands[0];
    enum rtx_code code = GET_CODE (addr);

    if ((code == PLUS && !CONST_INT_P (XEXP (addr, 1)))
	|| code == MINUS)
      op0 = replace_equiv_address (op0, force_reg (SImode, addr));

    operands[1] = gen_reg_rtx (SImode);
    if (BYTES_BIG_ENDIAN)
      {
	emit_insn (gen_movsi (operands[1], GEN_INT ((value >> 8) & 255)));
	if ((value & 255) == ((value >> 8) & 255))
	  operands[2] = operands[1];
	else
	  {
	    operands[2] = gen_reg_rtx (SImode);
	    emit_insn (gen_movsi (operands[2], GEN_INT (value & 255)));
	  }
      }
    else
      {
	emit_insn (gen_movsi (operands[1], GEN_INT (value & 255)));
	if ((value & 255) == ((value >> 8) & 255))
	  operands[2] = operands[1];
	else
	  {
	    operands[2] = gen_reg_rtx (SImode);
	    emit_insn (gen_movsi (operands[2], GEN_INT ((value >> 8) & 255)));
	  }
      }

    operands[3] = adjust_address (op0, QImode, 1);
    operands[0] = adjust_address (operands[0], QImode, 0);
    operands[2] = gen_lowpart (QImode, operands[2]);
    operands[1] = gen_lowpart (QImode, operands[1]);
  }"
)

//Pattern#354
(define_expand "storehi_single_op"
  [(set (match_operand:HI 0 "memory_operand" "")
	(match_operand:HI 1 "general_operand" ""))]
  "TARGET_32BIT && arm_arch4"
  "
  if (!s_register_operand (operands[1], HImode))
    operands[1] = copy_to_mode_reg (HImode, operands[1]);
  "
)

//Pattern#355
(define_expand "movhi"
  [(set (match_operand:HI 0 "general_operand" "")
	(match_operand:HI 1 "general_operand" ""))]
  "TARGET_EITHER"
  "
  if (TARGET_ARM)
    {
      if (can_create_pseudo_p ())
        {
          if (MEM_P (operands[0]))
	    {
	      if (arm_arch4)
	        {
	          emit_insn (gen_storehi_single_op (operands[0], operands[1]));
	          DONE;
	        }
	      if (CONST_INT_P (operands[1]))
	        emit_insn (gen_storeinthi (operands[0], operands[1]));
	      else
	        {
	          if (MEM_P (operands[1]))
		    operands[1] = force_reg (HImode, operands[1]);
	          if (BYTES_BIG_ENDIAN)
		    emit_insn (gen_storehi_bigend (operands[1], operands[0]));
	          else
		   emit_insn (gen_storehi (operands[1], operands[0]));
	        }
	      DONE;
	    }
          /* Sign extend a constant, and keep it in an SImode reg.  */
          else if (CONST_INT_P (operands[1]))
	    {
	      rtx reg = gen_reg_rtx (SImode);
	      HOST_WIDE_INT val = INTVAL (operands[1]) & 0xffff;

	      /* If the constant is already valid, leave it alone.  */
	      if (!const_ok_for_arm (val))
	        {
	          /* If setting all the top bits will make the constant 
		     loadable in a single instruction, then set them.  
		     Otherwise, sign extend the number.  */

	          if (const_ok_for_arm (~(val | ~0xffff)))
		    val |= ~0xffff;
	          else if (val & 0x8000)
		    val |= ~0xffff;
	        }

	      emit_insn (gen_movsi (reg, GEN_INT (val)));
	      operands[1] = gen_lowpart (HImode, reg);
	    }
	  else if (arm_arch4 && optimize && can_create_pseudo_p ()
		   && MEM_P (operands[1]))
	    {
	      rtx reg = gen_reg_rtx (SImode);

	      emit_insn (gen_zero_extendhisi2 (reg, operands[1]));
	      operands[1] = gen_lowpart (HImode, reg);
	    }
          else if (!arm_arch4)
	    {
	      if (MEM_P (operands[1]))
	        {
		  rtx base;
		  rtx offset = const0_rtx;
		  rtx reg = gen_reg_rtx (SImode);

		  if ((REG_P (base = XEXP (operands[1], 0))
		       || (GET_CODE (base) == PLUS
			   && (CONST_INT_P (offset = XEXP (base, 1)))
                           && ((INTVAL(offset) & 1) != 1)
			   && REG_P (base = XEXP (base, 0))))
		      && REGNO_POINTER_ALIGN (REGNO (base)) >= 32)
		    {
		      rtx new_rtx;

		      new_rtx = widen_memory_access (operands[1], SImode,
						     ((INTVAL (offset) & ~3)
						      - INTVAL (offset)));
		      emit_insn (gen_movsi (reg, new_rtx));
		      if (((INTVAL (offset) & 2) != 0)
			  ^ (BYTES_BIG_ENDIAN ? 1 : 0))
			{
			  rtx reg2 = gen_reg_rtx (SImode);

			  emit_insn (gen_lshrsi3 (reg2, reg, GEN_INT (16)));
			  reg = reg2;
			}
		    }
		  else
		    emit_insn (gen_movhi_bytes (reg, operands[1]));

		  operands[1] = gen_lowpart (HImode, reg);
	       }
	   }
        }
      /* Handle loading a large integer during reload.  */
      else if (CONST_INT_P (operands[1])
	       && !const_ok_for_arm (INTVAL (operands[1]))
	       && !const_ok_for_arm (~INTVAL (operands[1])))
        {
          /* Writing a constant to memory needs a scratch, which should
	     be handled with SECONDARY_RELOADs.  */
          gcc_assert (REG_P (operands[0]));

          operands[0] = gen_rtx_SUBREG (SImode, operands[0], 0);
          emit_insn (gen_movsi (operands[0], operands[1]));
          DONE;
       }
    }
  else if (TARGET_THUMB2)
    {
      /* Thumb-2 can do everything except mem=mem and mem=const easily.  */
      if (can_create_pseudo_p ())
	{
	  if (!REG_P (operands[0]))
	    operands[1] = force_reg (HImode, operands[1]);
          /* Zero extend a constant, and keep it in an SImode reg.  */
          else if (CONST_INT_P (operands[1]))
	    {
	      rtx reg = gen_reg_rtx (SImode);
	      HOST_WIDE_INT val = INTVAL (operands[1]) & 0xffff;

	      emit_insn (gen_movsi (reg, GEN_INT (val)));
	      operands[1] = gen_lowpart (HImode, reg);
	    }
	}
    }
  else /* TARGET_THUMB1 */
    {
      if (can_create_pseudo_p ())
        {
	  if (CONST_INT_P (operands[1]))
	    {
	      rtx reg = gen_reg_rtx (SImode);

	      emit_insn (gen_movsi (reg, operands[1]));
	      operands[1] = gen_lowpart (HImode, reg);
	    }

          /* ??? We shouldn't really get invalid addresses here, but this can
	     happen if we are passed a SP (never OK for HImode/QImode) or 
	     virtual register (also rejected as illegitimate for HImode/QImode)
	     relative address.  */
          /* ??? This should perhaps be fixed elsewhere, for instance, in
	     fixup_stack_1, by checking for other kinds of invalid addresses,
	     e.g. a bare reference to a virtual register.  This may confuse the
	     alpha though, which must handle this case differently.  */
          if (MEM_P (operands[0])
	      && !memory_address_p (GET_MODE (operands[0]),
				    XEXP (operands[0], 0)))
	    operands[0]
	      = replace_equiv_address (operands[0],
				       copy_to_reg (XEXP (operands[0], 0)));
   
          if (MEM_P (operands[1])
	      && !memory_address_p (GET_MODE (operands[1]),
				    XEXP (operands[1], 0)))
	    operands[1]
	      = replace_equiv_address (operands[1],
				       copy_to_reg (XEXP (operands[1], 0)));

	  if (MEM_P (operands[1]) && optimize > 0)
	    {
	      rtx reg = gen_reg_rtx (SImode);

	      emit_insn (gen_zero_extendhisi2 (reg, operands[1]));
	      operands[1] = gen_lowpart (HImode, reg);
	    }

          if (MEM_P (operands[0]))
	    operands[1] = force_reg (HImode, operands[1]);
        }
      else if (CONST_INT_P (operands[1])
	        && !satisfies_constraint_I (operands[1]))
        {
	  /* Handle loading a large integer during reload.  */

          /* Writing a constant to memory needs a scratch, which should
	     be handled with SECONDARY_RELOADs.  */
          gcc_assert (REG_P (operands[0]));

          operands[0] = gen_rtx_SUBREG (SImode, operands[0], 0);
          emit_insn (gen_movsi (operands[0], operands[1]));
          DONE;
        }
    }
  "
)

//Pattern#356
(define_insn "*thumb1_movhi_insn"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=l,l,m,*r,*h,l")
	(match_operand:HI 1 "general_operand"       "l,m,l,*h,*r,I"))]
  "TARGET_THUMB1
   && (   register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode))"
  "*
  switch (which_alternative)
    {
    case 0: return \"add	%0, %1, #0\";
    case 2: return \"strh	%1, %0\";
    case 3: return \"mov	%0, %1\";
    case 4: return \"mov	%0, %1\";
    case 5: return \"mov	%0, %1\";
    default: gcc_unreachable ();
    case 1:
      /* The stack pointer can end up being taken as an index register.
          Catch this case here and deal with it.  */
      if (GET_CODE (XEXP (operands[1], 0)) == PLUS
	  && REG_P (XEXP (XEXP (operands[1], 0), 0))
	  && REGNO    (XEXP (XEXP (operands[1], 0), 0)) == SP_REGNUM)
        {
	  rtx ops[2];
          ops[0] = operands[0];
          ops[1] = XEXP (XEXP (operands[1], 0), 0);
      
          output_asm_insn (\"mov	%0, %1\", ops);

          XEXP (XEXP (operands[1], 0), 0) = operands[0];
    
	}
      return \"ldrh	%0, %1\";
    }"
  [(set_attr "length" "2,4,2,2,2,2")
   (set_attr "type" "*,load1,store1,*,*,*")
   (set_attr "conds" "clob,nocond,nocond,nocond,nocond,clob")])


//Pattern#357
(define_expand "movhi_bytes"
  [(set (match_dup 2) (zero_extend:SI (match_operand:HI 1 "" "")))
   (set (match_dup 3)
	(zero_extend:SI (match_dup 6)))
   (set (match_operand:SI 0 "" "")
	 (ior:SI (ashift:SI (match_dup 4) (const_int 8)) (match_dup 5)))]
  "TARGET_ARM"
  "
  {
    rtx mem1, mem2;
    rtx addr = copy_to_mode_reg (SImode, XEXP (operands[1], 0));

    mem1 = change_address (operands[1], QImode, addr);
    mem2 = change_address (operands[1], QImode,
			   plus_constant (Pmode, addr, 1));
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = mem1;
    operands[2] = gen_reg_rtx (SImode);
    operands[3] = gen_reg_rtx (SImode);
    operands[6] = mem2;

    if (BYTES_BIG_ENDIAN)
      {
	operands[4] = operands[2];
	operands[5] = operands[3];
      }
    else
      {
	operands[4] = operands[3];
	operands[5] = operands[2];
      }
  }"
)

//Pattern#358
(define_expand "movhi_bigend"
  [(set (match_dup 2)
	(rotate:SI (subreg:SI (match_operand:HI 1 "memory_operand" "") 0)
		   (const_int 16)))
   (set (match_dup 3)
	(ashiftrt:SI (match_dup 2) (const_int 16)))
   (set (match_operand:HI 0 "s_register_operand" "")
	(match_dup 4))]
  "TARGET_ARM"
  "
  operands[2] = gen_reg_rtx (SImode);
  operands[3] = gen_reg_rtx (SImode);
  operands[4] = gen_lowpart (HImode, operands[3]);
  "
)

;; Pattern to recognize insn generated default case above
//Pattern#359
(define_insn "*movhi_insn_arch4"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r,r,m,r")
	(match_operand:HI 1 "general_operand"      "rI,K,r,mi"))]
  "TARGET_ARM
   && arm_arch4
   && (register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode))"
  "@
   mov%?\\t%0, %1\\t%@ movhi
   mvn%?\\t%0, #%B1\\t%@ movhi
   str%(h%)\\t%1, %0\\t%@ movhi
   ldr%(h%)\\t%0, %1\\t%@ movhi"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "mov,mvn,*,*")
   (set_attr "pool_range" "*,*,*,256")
   (set_attr "neg_pool_range" "*,*,*,244")
   (set_attr_alternative "type"
                         [(if_then_else (match_operand 1 "const_int_operand" "")
                                        (const_string "simple_alu_imm" )
                                        (const_string "*"))
                          (const_string "simple_alu_imm")
                          (const_string "store1")
                          (const_string "load1")])]
)

//Pattern#360
(define_insn "*movhi_bytes"
  [(set (match_operand:HI 0 "s_register_operand" "=r,r,r")
	(match_operand:HI 1 "arm_rhs_operand"  "I,r,K"))]
  "TARGET_ARM"
  "@
   mov%?\\t%0, %1\\t%@ movhi
   mov%?\\t%0, %1\\t%@ movhi
   mvn%?\\t%0, #%B1\\t%@ movhi"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "mov, mov,mvn")
   (set_attr "type" "simple_alu_imm,*,simple_alu_imm")]
)

//Pattern#361
(define_expand "thumb_movhi_clobber"
  [(set (match_operand:HI     0 "memory_operand"   "")
	(match_operand:HI     1 "register_operand" ""))
   (clobber (match_operand:DI 2 "register_operand" ""))]
  "TARGET_THUMB1"
  "
  if (strict_memory_address_p (HImode, XEXP (operands[0], 0))
      && REGNO (operands[1]) <= LAST_LO_REGNUM)
    {
      emit_insn (gen_movhi (operands[0], operands[1]));
      DONE;
    }
  /* XXX Fixme, need to handle other cases here as well.  */
  gcc_unreachable ();
  "
)

;; We use a DImode scratch because we may occasionally need an additional
;; temporary if the address isn't offsettable -- push_reload doesn't seem
;; to take any notice of the "o" constraints on reload_memory_operand operand.
//Pattern#362
(define_expand "reload_outhi"
  [(parallel [(match_operand:HI 0 "arm_reload_memory_operand" "=o")
	      (match_operand:HI 1 "s_register_operand"        "r")
	      (match_operand:DI 2 "s_register_operand"        "=&l")])]
  "TARGET_EITHER"
  "if (TARGET_ARM)
     arm_reload_out_hi (operands);
   else
     thumb_reload_out_hi (operands);
  DONE;
  "
)

//Pattern#363
(define_expand "reload_inhi"
  [(parallel [(match_operand:HI 0 "s_register_operand" "=r")
	      (match_operand:HI 1 "arm_reload_memory_operand" "o")
	      (match_operand:DI 2 "s_register_operand" "=&r")])]
  "TARGET_EITHER"
  "
  if (TARGET_ARM)
    arm_reload_in_hi (operands);
  else
    thumb_reload_out_hi (operands);
  DONE;
")

//Pattern#364
(define_expand "movqi"
  [(set (match_operand:QI 0 "general_operand" "")
        (match_operand:QI 1 "general_operand" ""))]
  "TARGET_EITHER"
  "
  /* Everything except mem = const or mem = mem can be done easily */

  if (can_create_pseudo_p ())
    {
      if (CONST_INT_P (operands[1]))
	{
	  rtx reg = gen_reg_rtx (SImode);

	  /* For thumb we want an unsigned immediate, then we are more likely 
	     to be able to use a movs insn.  */
	  if (TARGET_THUMB)
	    operands[1] = GEN_INT (INTVAL (operands[1]) & 255);

	  emit_insn (gen_movsi (reg, operands[1]));
	  operands[1] = gen_lowpart (QImode, reg);
	}

      if (TARGET_THUMB)
	{
          /* ??? We shouldn't really get invalid addresses here, but this can
	     happen if we are passed a SP (never OK for HImode/QImode) or
	     virtual register (also rejected as illegitimate for HImode/QImode)
	     relative address.  */
          /* ??? This should perhaps be fixed elsewhere, for instance, in
	     fixup_stack_1, by checking for other kinds of invalid addresses,
	     e.g. a bare reference to a virtual register.  This may confuse the
	     alpha though, which must handle this case differently.  */
          if (MEM_P (operands[0])
	      && !memory_address_p (GET_MODE (operands[0]),
		  		     XEXP (operands[0], 0)))
	    operands[0]
	      = replace_equiv_address (operands[0],
				       copy_to_reg (XEXP (operands[0], 0)));
          if (MEM_P (operands[1])
	      && !memory_address_p (GET_MODE (operands[1]),
				    XEXP (operands[1], 0)))
	     operands[1]
	       = replace_equiv_address (operands[1],
					copy_to_reg (XEXP (operands[1], 0)));
	}

      if (MEM_P (operands[1]) && optimize > 0)
	{
	  rtx reg = gen_reg_rtx (SImode);

	  emit_insn (gen_zero_extendqisi2 (reg, operands[1]));
	  operands[1] = gen_lowpart (QImode, reg);
	}

      if (MEM_P (operands[0]))
	operands[1] = force_reg (QImode, operands[1]);
    }
  else if (TARGET_THUMB
	   && CONST_INT_P (operands[1])
	   && !satisfies_constraint_I (operands[1]))
    {
      /* Handle loading a large integer during reload.  */

      /* Writing a constant to memory needs a scratch, which should
	 be handled with SECONDARY_RELOADs.  */
      gcc_assert (REG_P (operands[0]));

      operands[0] = gen_rtx_SUBREG (SImode, operands[0], 0);
      emit_insn (gen_movsi (operands[0], operands[1]));
      DONE;
    }
  "
)


//Pattern#365
(define_insn "*arm_movqi_insn"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=r,r,r,l,Uu,r,m")
	(match_operand:QI 1 "general_operand" "r,I,K,Uu,l,m,r"))]
  "TARGET_32BIT
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))"
  "@
   mov%?\\t%0, %1
   mov%?\\t%0, %1
   mvn%?\\t%0, #%B1
   ldr%(b%)\\t%0, %1
   str%(b%)\\t%1, %0
   ldr%(b%)\\t%0, %1
   str%(b%)\\t%1, %0"
  [(set_attr "type" "*,simple_alu_imm,simple_alu_imm,load1, store1, load1, store1")
   (set_attr "insn" "mov,mov,mvn,*,*,*,*")
   (set_attr "predicable" "yes")
   (set_attr "arch" "any,any,any,t2,t2,any,any")
   (set_attr "length" "4,4,4,2,2,4,4")]
)

//Pattern#366
(define_insn "*thumb1_movqi_insn"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=l,l,m,*r,*h,l")
	(match_operand:QI 1 "general_operand"      "l, m,l,*h,*r,I"))]
  "TARGET_THUMB1
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))"
  "@
   add\\t%0, %1, #0
   ldrb\\t%0, %1
   strb\\t%1, %0
   mov\\t%0, %1
   mov\\t%0, %1
   mov\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "type" "simple_alu_imm,load1,store1,*,*,simple_alu_imm")
   (set_attr "insn" "*,*,*,mov,mov,mov")
   (set_attr "pool_range" "*,32,*,*,*,*")
   (set_attr "conds" "clob,nocond,nocond,nocond,nocond,clob")])

;; HFmode moves
//Pattern#367
(define_expand "movhf"
  [(set (match_operand:HF 0 "general_operand" "")
	(match_operand:HF 1 "general_operand" ""))]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT)
    {
      if (MEM_P (operands[0]))
        operands[1] = force_reg (HFmode, operands[1]);
    }
  else /* TARGET_THUMB1 */
    {
      if (can_create_pseudo_p ())
        {
           if (!REG_P (operands[0]))
	     operands[1] = force_reg (HFmode, operands[1]);
        }
    }
  "
)

//Pattern#368
(define_insn "*arm32_movhf"
  [(set (match_operand:HF 0 "nonimmediate_operand" "=r,m,r,r")
	(match_operand:HF 1 "general_operand"	   " m,r,r,F"))]
  "TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)
   && (	  s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode))"
  "*
  switch (which_alternative)
    {
    case 0:	/* ARM register from memory */
      return \"ldr%(h%)\\t%0, %1\\t%@ __fp16\";
    case 1:	/* memory from ARM register */
      return \"str%(h%)\\t%1, %0\\t%@ __fp16\";
    case 2:	/* ARM register from ARM register */
      return \"mov%?\\t%0, %1\\t%@ __fp16\";
    case 3:	/* ARM register from constant */
      {
	REAL_VALUE_TYPE r;
	long bits;
	rtx ops[4];

	REAL_VALUE_FROM_CONST_DOUBLE (r, operands[1]);
	bits = real_to_target (NULL, &r, HFmode);
	ops[0] = operands[0];
	ops[1] = GEN_INT (bits);
	ops[2] = GEN_INT (bits & 0xff00);
	ops[3] = GEN_INT (bits & 0x00ff);

	if (arm_arch_thumb2)
	  output_asm_insn (\"movw%?\\t%0, %1\", ops);
	else
	  output_asm_insn (\"mov%?\\t%0, %2\;orr%?\\t%0, %0, %3\", ops);
	return \"\";
       }
    default:
      gcc_unreachable ();
    }
  "
  [(set_attr "conds" "unconditional")
   (set_attr "type" "load1,store1,*,*")
   (set_attr "insn" "*,*,mov,mov")
   (set_attr "length" "4,4,4,8")
   (set_attr "predicable" "yes")]
)

//Pattern#369
(define_insn "*thumb1_movhf"
  [(set (match_operand:HF     0 "nonimmediate_operand" "=l,l,m,*r,*h")
	(match_operand:HF     1 "general_operand"      "l,mF,l,*h,*r"))]
  "TARGET_THUMB1
   && (	  s_register_operand (operands[0], HFmode) 
       || s_register_operand (operands[1], HFmode))"
  "*
  switch (which_alternative)
    {
    case 1:
      {
	rtx addr;
	gcc_assert (MEM_P (operands[1]));
	addr = XEXP (operands[1], 0);
	if (GET_CODE (addr) == LABEL_REF
	    || (GET_CODE (addr) == CONST
		&& GET_CODE (XEXP (addr, 0)) == PLUS
		&& GET_CODE (XEXP (XEXP (addr, 0), 0)) == LABEL_REF
		&& CONST_INT_P (XEXP (XEXP (addr, 0), 1))))
	  {
	    /* Constant pool entry.  */
	    return \"ldr\\t%0, %1\";
	  }
	return \"ldrh\\t%0, %1\";
      }
    case 2: return \"strh\\t%1, %0\";
    default: return \"mov\\t%0, %1\";
    }
  "
  [(set_attr "length" "2")
   (set_attr "type" "*,load1,store1,*,*")
   (set_attr "insn" "mov,*,*,mov,mov")
   (set_attr "pool_range" "*,1018,*,*,*")
   (set_attr "conds" "clob,nocond,nocond,nocond,nocond")])

//Pattern#370
(define_expand "movsf"
  [(set (match_operand:SF 0 "general_operand" "")
	(match_operand:SF 1 "general_operand" ""))]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT)
    {
      if (MEM_P (operands[0]))
        operands[1] = force_reg (SFmode, operands[1]);
    }
  else /* TARGET_THUMB1 */
    {
      if (can_create_pseudo_p ())
        {
           if (!REG_P (operands[0]))
	     operands[1] = force_reg (SFmode, operands[1]);
        }
    }
  "
)

;; Transform a floating-point move of a constant into a core register into
;; an SImode operation.
//Pattern#371
(define_split
  [(set (match_operand:SF 0 "arm_general_register_operand" "")
	(match_operand:SF 1 "immediate_operand" ""))]
  "TARGET_EITHER
   && reload_completed
   && CONST_DOUBLE_P (operands[1])"
  [(set (match_dup 2) (match_dup 3))]
  "
  operands[2] = gen_lowpart (SImode, operands[0]);
  operands[3] = gen_lowpart (SImode, operands[1]);
  if (operands[2] == 0 || operands[3] == 0)
    FAIL;
  "
)

//Pattern#372
(define_insn "*arm_movsf_soft_insn"
  [(set (match_operand:SF 0 "nonimmediate_operand" "=r,r,m")
	(match_operand:SF 1 "general_operand"  "r,mE,r"))]
  "TARGET_32BIT
   && TARGET_SOFT_FLOAT
   && (!MEM_P (operands[0])
       || register_operand (operands[1], SFmode))"
  "@
   mov%?\\t%0, %1
   ldr%?\\t%0, %1\\t%@ float
   str%?\\t%1, %0\\t%@ float"
  [(set_attr "predicable" "yes")
   (set_attr "type" "*,load1,store1")
   (set_attr "insn" "mov,*,*")
   (set_attr "arm_pool_range" "*,4096,*")
   (set_attr "thumb2_pool_range" "*,4094,*")
   (set_attr "arm_neg_pool_range" "*,4084,*")
   (set_attr "thumb2_neg_pool_range" "*,0,*")]
)

;;; ??? This should have alternatives for constants.
//Pattern#373
(define_insn "*thumb1_movsf_insn"
  [(set (match_operand:SF     0 "nonimmediate_operand" "=l,l,>,l, m,*r,*h")
	(match_operand:SF     1 "general_operand"      "l, >,l,mF,l,*h,*r"))]
  "TARGET_THUMB1
   && (   register_operand (operands[0], SFmode) 
       || register_operand (operands[1], SFmode))"
  "@
   add\\t%0, %1, #0
   ldmia\\t%1, {%0}
   stmia\\t%0, {%1}
   ldr\\t%0, %1
   str\\t%1, %0
   mov\\t%0, %1
   mov\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "type" "*,load1,store1,load1,store1,*,*")
   (set_attr "pool_range" "*,*,*,1018,*,*,*")
   (set_attr "insn" "*,*,*,*,*,mov,mov")
   (set_attr "conds" "clob,nocond,nocond,nocond,nocond,nocond,nocond")]
)

//Pattern#374
(define_expand "movdf"
  [(set (match_operand:DF 0 "general_operand" "")
	(match_operand:DF 1 "general_operand" ""))]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT)
    {
      if (MEM_P (operands[0]))
        operands[1] = force_reg (DFmode, operands[1]);
    }
  else /* TARGET_THUMB */
    {
      if (can_create_pseudo_p ())
        {
          if (!REG_P (operands[0]))
	    operands[1] = force_reg (DFmode, operands[1]);
        }
    }
  "
)

;; Reloading a df mode value stored in integer regs to memory can require a
;; scratch reg.
//Pattern#375
(define_expand "reload_outdf"
  [(match_operand:DF 0 "arm_reload_memory_operand" "=o")
   (match_operand:DF 1 "s_register_operand" "r")
   (match_operand:SI 2 "s_register_operand" "=&r")]
  "TARGET_THUMB2"
  "
  {
    enum rtx_code code = GET_CODE (XEXP (operands[0], 0));

    if (code == REG)
      operands[2] = XEXP (operands[0], 0);
    else if (code == POST_INC || code == PRE_DEC)
      {
	operands[0] = gen_rtx_SUBREG (DImode, operands[0], 0);
	operands[1] = gen_rtx_SUBREG (DImode, operands[1], 0);
	emit_insn (gen_movdi (operands[0], operands[1]));
	DONE;
      }
    else if (code == PRE_INC)
      {
	rtx reg = XEXP (XEXP (operands[0], 0), 0);

	emit_insn (gen_addsi3 (reg, reg, GEN_INT (8)));
	operands[2] = reg;
      }
    else if (code == POST_DEC)
      operands[2] = XEXP (XEXP (operands[0], 0), 0);
    else
      emit_insn (gen_addsi3 (operands[2], XEXP (XEXP (operands[0], 0), 0),
			     XEXP (XEXP (operands[0], 0), 1)));

    emit_insn (gen_rtx_SET (VOIDmode,
			    replace_equiv_address (operands[0], operands[2]),
			    operands[1]));

    if (code == POST_DEC)
      emit_insn (gen_addsi3 (operands[2], operands[2], GEN_INT (-8)));

    DONE;
  }"
)

//Pattern#376
(define_insn "*movdf_soft_insn"
  [(set (match_operand:DF 0 "nonimmediate_soft_df_operand" "=r,r,r,r,m")
	(match_operand:DF 1 "soft_df_operand" "rDa,Db,Dc,mF,r"))]
  "TARGET_32BIT && TARGET_SOFT_FLOAT
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))"
  "*
  switch (which_alternative)
    {
    case 0:
    case 1:
    case 2:
      return \"#\";
    default:
      return output_move_double (operands, true, NULL);
    }
  "
  [(set_attr "length" "8,12,16,8,8")
   (set_attr "type" "*,*,*,load2,store2")
   (set_attr "arm_pool_range" "*,*,*,1020,*")
   (set_attr "thumb2_pool_range" "*,*,*,1018,*")
   (set_attr "arm_neg_pool_range" "*,*,*,1004,*")
   (set_attr "thumb2_neg_pool_range" "*,*,*,0,*")]
)

;;; ??? This should have alternatives for constants.
;;; ??? This was originally identical to the movdi_insn pattern.
;;; ??? The 'F' constraint looks funny, but it should always be replaced by
;;; thumb_reorg with a memory reference.
//Pattern#377
(define_insn "*thumb_movdf_insn"
  [(set (match_operand:DF 0 "nonimmediate_operand" "=l,l,>,l, m,*r")
	(match_operand:DF 1 "general_operand"      "l, >,l,mF,l,*r"))]
  "TARGET_THUMB1
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))"
  "*
  switch (which_alternative)
    {
    default:
    case 0:
      if (REGNO (operands[1]) == REGNO (operands[0]) + 1)
	return \"add\\t%0, %1, #0\;add\\t%H0, %H1, #0\";
      return \"add\\t%H0, %H1, #0\;add\\t%0, %1, #0\";
    case 1:
      return \"ldmia\\t%1, {%0, %H0}\";
    case 2:
      return \"stmia\\t%0, {%1, %H1}\";
    case 3:
      return thumb_load_double_from_address (operands);
    case 4:
      operands[2] = gen_rtx_MEM (SImode,
				 plus_constant (Pmode,
						XEXP (operands[0], 0), 4));
      output_asm_insn (\"str\\t%1, %0\;str\\t%H1, %2\", operands);
      return \"\";
    case 5:
      if (REGNO (operands[1]) == REGNO (operands[0]) + 1)
	return \"mov\\t%0, %1\;mov\\t%H0, %H1\";
      return \"mov\\t%H0, %H1\;mov\\t%0, %1\";
    }
  "
  [(set_attr "length" "4,2,2,6,4,4")
   (set_attr "type" "*,load2,store2,load2,store2,*")
   (set_attr "insn" "*,*,*,*,*,mov")
   (set_attr "pool_range" "*,*,*,1018,*,*")]
)


;; load- and store-multiple insns
;; The arm can load/store any set of registers, provided that they are in
;; ascending order, but these expanders assume a contiguous set.

//Pattern#378
(define_expand "load_multiple"
  [(match_par_dup 3 [(set (match_operand:SI 0 "" "")
                          (match_operand:SI 1 "" ""))
                     (use (match_operand:SI 2 "" ""))])]
  "TARGET_32BIT"
{
  HOST_WIDE_INT offset = 0;

  /* Support only fixed point registers.  */
  if (!CONST_INT_P (operands[2])
      || INTVAL (operands[2]) > 14
      || INTVAL (operands[2]) < 2
      || !MEM_P (operands[1])
      || !REG_P (operands[0])
      || REGNO (operands[0]) > (LAST_ARM_REGNUM - 1)
      || REGNO (operands[0]) + INTVAL (operands[2]) > LAST_ARM_REGNUM)
    FAIL;

  operands[3]
    = arm_gen_load_multiple (arm_regs_in_sequence + REGNO (operands[0]),
			     INTVAL (operands[2]),
			     force_reg (SImode, XEXP (operands[1], 0)),
			     FALSE, operands[1], &offset);
})

//Pattern#379
(define_expand "store_multiple"
  [(match_par_dup 3 [(set (match_operand:SI 0 "" "")
                          (match_operand:SI 1 "" ""))
                     (use (match_operand:SI 2 "" ""))])]
  "TARGET_32BIT"
{
  HOST_WIDE_INT offset = 0;

  /* Support only fixed point registers.  */
  if (!CONST_INT_P (operands[2])
      || INTVAL (operands[2]) > 14
      || INTVAL (operands[2]) < 2
      || !REG_P (operands[1])
      || !MEM_P (operands[0])
      || REGNO (operands[1]) > (LAST_ARM_REGNUM - 1)
      || REGNO (operands[1]) + INTVAL (operands[2]) > LAST_ARM_REGNUM)
    FAIL;

  operands[3]
    = arm_gen_store_multiple (arm_regs_in_sequence + REGNO (operands[1]),
			      INTVAL (operands[2]),
			      force_reg (SImode, XEXP (operands[0], 0)),
			      FALSE, operands[0], &offset);
})


;; Move a block of memory if it is word aligned and MORE than 2 words long.
;; We could let this apply for blocks of less than this, but it clobbers so
;; many registers that there is then probably a better way.

//Pattern#380
(define_expand "movmemqi"
  [(match_operand:BLK 0 "general_operand" "")
   (match_operand:BLK 1 "general_operand" "")
   (match_operand:SI 2 "const_int_operand" "")
   (match_operand:SI 3 "const_int_operand" "")]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT)
    {
      if (arm_gen_movmemqi (operands))
        DONE;
      FAIL;
    }
  else /* TARGET_THUMB1 */
    {
      if (   INTVAL (operands[3]) != 4
          || INTVAL (operands[2]) > 48)
        FAIL;

      thumb_expand_movmemqi (operands);
      DONE;
    }
  "
)

;; Thumb block-move insns

//Pattern#381
(define_insn "movmem12b"
  [(set (mem:SI (match_operand:SI 2 "register_operand" "0"))
	(mem:SI (match_operand:SI 3 "register_operand" "1")))
   (set (mem:SI (plus:SI (match_dup 2) (const_int 4)))
	(mem:SI (plus:SI (match_dup 3) (const_int 4))))
   (set (mem:SI (plus:SI (match_dup 2) (const_int 8)))
	(mem:SI (plus:SI (match_dup 3) (const_int 8))))
   (set (match_operand:SI 0 "register_operand" "=l")
	(plus:SI (match_dup 2) (const_int 12)))
   (set (match_operand:SI 1 "register_operand" "=l")
	(plus:SI (match_dup 3) (const_int 12)))
   (clobber (match_scratch:SI 4 "=&l"))
   (clobber (match_scratch:SI 5 "=&l"))
   (clobber (match_scratch:SI 6 "=&l"))]
  "TARGET_THUMB1"
  "* return thumb_output_move_mem_multiple (3, operands);"
  [(set_attr "length" "4")
   ; This isn't entirely accurate...  It loads as well, but in terms of
   ; scheduling the following insn it is better to consider it as a store
   (set_attr "type" "store3")]
)

//Pattern#382
(define_insn "movmem8b"
  [(set (mem:SI (match_operand:SI 2 "register_operand" "0"))
	(mem:SI (match_operand:SI 3 "register_operand" "1")))
   (set (mem:SI (plus:SI (match_dup 2) (const_int 4)))
	(mem:SI (plus:SI (match_dup 3) (const_int 4))))
   (set (match_operand:SI 0 "register_operand" "=l")
	(plus:SI (match_dup 2) (const_int 8)))
   (set (match_operand:SI 1 "register_operand" "=l")
	(plus:SI (match_dup 3) (const_int 8)))
   (clobber (match_scratch:SI 4 "=&l"))
   (clobber (match_scratch:SI 5 "=&l"))]
  "TARGET_THUMB1"
  "* return thumb_output_move_mem_multiple (2, operands);"
  [(set_attr "length" "4")
   ; This isn't entirely accurate...  It loads as well, but in terms of
   ; scheduling the following insn it is better to consider it as a store
   (set_attr "type" "store2")]
)



;; Compare & branch insns
;; The range calculations are based as follows:
;; For forward branches, the address calculation returns the address of
;; the next instruction.  This is 2 beyond the branch instruction.
;; For backward branches, the address calculation returns the address of
;; the first instruction in this pattern (cmp).  This is 2 before the branch
;; instruction for the shortest sequence, and 4 before the branch instruction
;; if we have to jump around an unconditional branch.
;; To the basic branch range the PC offset must be added (this is +4).
;; So for forward branches we have 
;;   (pos_range - pos_base_offs + pc_offs) = (pos_range - 2 + 4).
;; And for backward branches we have 
;;   (neg_range - neg_base_offs + pc_offs) = (neg_range - (-2 or -4) + 4).
;;
;; For a 'b'       pos_range = 2046, neg_range = -2048 giving (-2040->2048).
;; For a 'b<cond>' pos_range = 254,  neg_range = -256  giving (-250 ->256).

//Pattern#383
(define_expand "cbranchsi4"
  [(set (pc) (if_then_else
	      (match_operator 0 "expandable_comparison_operator"
	       [(match_operand:SI 1 "s_register_operand" "")
	        (match_operand:SI 2 "nonmemory_operand" "")])
	      (label_ref (match_operand 3 "" ""))
	      (pc)))]
  "TARGET_EITHER"
  "
  if (!TARGET_THUMB1)
    {
      if (!arm_validize_comparison (&operands[0], &operands[1], &operands[2]))
        FAIL;
      emit_jump_insn (gen_cbranch_cc (operands[0], operands[1], operands[2],
				      operands[3]));
      DONE;
    }
  if (thumb1_cmpneg_operand (operands[2], SImode))
    {
      emit_jump_insn (gen_cbranchsi4_scratch (NULL, operands[1], operands[2],
					      operands[3], operands[0]));
      DONE;
    }
  if (!thumb1_cmp_operand (operands[2], SImode))
    operands[2] = force_reg (SImode, operands[2]);
  ")

;; A pattern to recognize a special situation and optimize for it.
;; On the thumb, zero-extension from memory is preferrable to sign-extension
;; due to the available addressing modes.  Hence, convert a signed comparison
;; with zero into an unsigned comparison with 127 if possible.
//Pattern#384
(define_expand "cbranchqi4"
  [(set (pc) (if_then_else
	      (match_operator 0 "lt_ge_comparison_operator"
	       [(match_operand:QI 1 "memory_operand" "")
	        (match_operand:QI 2 "const0_operand" "")])
	      (label_ref (match_operand 3 "" ""))
	      (pc)))]
  "TARGET_THUMB1"
{
  rtx xops[4];
  xops[1] = gen_reg_rtx (SImode);
  emit_insn (gen_zero_extendqisi2 (xops[1], operands[1]));
  xops[2] = GEN_INT (127);
  xops[0] = gen_rtx_fmt_ee (GET_CODE (operands[0]) == GE ? LEU : GTU,
			    VOIDmode, xops[1], xops[2]);
  xops[3] = operands[3];
  emit_insn (gen_cbranchsi4 (xops[0], xops[1], xops[2], xops[3]));
  DONE;
})

//Pattern#385
(define_expand "cbranchsf4"
  [(set (pc) (if_then_else
	      (match_operator 0 "expandable_comparison_operator"
	       [(match_operand:SF 1 "s_register_operand" "")
	        (match_operand:SF 2 "arm_float_compare_operand" "")])
	      (label_ref (match_operand 3 "" ""))
	      (pc)))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "emit_jump_insn (gen_cbranch_cc (operands[0], operands[1], operands[2],
				   operands[3])); DONE;"
)

//Pattern#386
(define_expand "cbranchdf4"
  [(set (pc) (if_then_else
	      (match_operator 0 "expandable_comparison_operator"
	       [(match_operand:DF 1 "s_register_operand" "")
	        (match_operand:DF 2 "arm_float_compare_operand" "")])
	      (label_ref (match_operand 3 "" ""))
	      (pc)))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "emit_jump_insn (gen_cbranch_cc (operands[0], operands[1], operands[2],
				   operands[3])); DONE;"
)

//Pattern#387
(define_expand "cbranchdi4"
  [(set (pc) (if_then_else
	      (match_operator 0 "expandable_comparison_operator"
	       [(match_operand:DI 1 "s_register_operand" "")
	        (match_operand:DI 2 "cmpdi_operand" "")])
	      (label_ref (match_operand 3 "" ""))
	      (pc)))]
  "TARGET_32BIT"
  "{
     if (!arm_validize_comparison (&operands[0], &operands[1], &operands[2]))
       FAIL;
     emit_jump_insn (gen_cbranch_cc (operands[0], operands[1], operands[2],
				       operands[3]));
     DONE;
   }"
)

//Pattern#388
(define_insn "cbranchsi4_insn"
  [(set (pc) (if_then_else
	      (match_operator 0 "arm_comparison_operator"
	       [(match_operand:SI 1 "s_register_operand" "l,l*h")
	        (match_operand:SI 2 "thumb1_cmp_operand" "lI*h,*r")])
	      (label_ref (match_operand 3 "" ""))
	      (pc)))]
  "TARGET_THUMB1"
{
  rtx t = cfun->machine->thumb1_cc_insn;
  if (t != NULL_RTX)
    {
      if (!rtx_equal_p (cfun->machine->thumb1_cc_op0, operands[1])
	  || !rtx_equal_p (cfun->machine->thumb1_cc_op1, operands[2]))
	t = NULL_RTX;
      if (cfun->machine->thumb1_cc_mode == CC_NOOVmode)
	{
	  if (!noov_comparison_operator (operands[0], VOIDmode))
	    t = NULL_RTX;
	}
      else if (cfun->machine->thumb1_cc_mode != CCmode)
	t = NULL_RTX;
    }
  if (t == NULL_RTX)
    {
      output_asm_insn ("cmp\t%1, %2", operands);
      cfun->machine->thumb1_cc_insn = insn;
      cfun->machine->thumb1_cc_op0 = operands[1];
      cfun->machine->thumb1_cc_op1 = operands[2];
      cfun->machine->thumb1_cc_mode = CCmode;
    }
  else
    /* Ensure we emit the right type of condition code on the jump.  */
    XEXP (operands[0], 0) = gen_rtx_REG (cfun->machine->thumb1_cc_mode,
					 CC_REGNUM);

  switch (get_attr_length (insn))
    {
    case 4:  return \"b%d0\\t%l3\";
    case 6:  return \"b%D0\\t.LCB%=\;b\\t%l3\\t%@long jump\\n.LCB%=:\";
    default: return \"b%D0\\t.LCB%=\;bl\\t%l3\\t%@far jump\\n.LCB%=:\";
    }
}
  [(set (attr "far_jump")
        (if_then_else
	    (eq_attr "length" "8")
	    (const_string "yes")
            (const_string "no")))
   (set (attr "length") 
        (if_then_else
	    (and (ge (minus (match_dup 3) (pc)) (const_int -250))
	         (le (minus (match_dup 3) (pc)) (const_int 256)))
	    (const_int 4)
	    (if_then_else
	        (and (ge (minus (match_dup 3) (pc)) (const_int -2040))
		     (le (minus (match_dup 3) (pc)) (const_int 2048)))
		(const_int 6)
		(const_int 8))))]
)

//Pattern#389
(define_insn "cbranchsi4_scratch"
  [(set (pc) (if_then_else
	      (match_operator 4 "arm_comparison_operator"
	       [(match_operand:SI 1 "s_register_operand" "l,0")
	        (match_operand:SI 2 "thumb1_cmpneg_operand" "L,J")])
	      (label_ref (match_operand 3 "" ""))
	      (pc)))
   (clobber (match_scratch:SI 0 "=l,l"))]
  "TARGET_THUMB1"
  "*
  output_asm_insn (\"add\\t%0, %1, #%n2\", operands);

  switch (get_attr_length (insn))
    {
    case 4:  return \"b%d4\\t%l3\";
    case 6:  return \"b%D4\\t.LCB%=\;b\\t%l3\\t%@long jump\\n.LCB%=:\";
    default: return \"b%D4\\t.LCB%=\;bl\\t%l3\\t%@far jump\\n.LCB%=:\";
    }
  "
  [(set (attr "far_jump")
        (if_then_else
	    (eq_attr "length" "8")
	    (const_string "yes")
            (const_string "no")))
   (set (attr "length") 
        (if_then_else
	    (and (ge (minus (match_dup 3) (pc)) (const_int -250))
	         (le (minus (match_dup 3) (pc)) (const_int 256)))
	    (const_int 4)
	    (if_then_else
	        (and (ge (minus (match_dup 3) (pc)) (const_int -2040))
		     (le (minus (match_dup 3) (pc)) (const_int 2048)))
		(const_int 6)
		(const_int 8))))]
)

//Pattern#390
(define_insn "*negated_cbranchsi4"
  [(set (pc)
	(if_then_else
	 (match_operator 0 "equality_operator"
	  [(match_operand:SI 1 "s_register_operand" "l")
	   (neg:SI (match_operand:SI 2 "s_register_operand" "l"))])
	 (label_ref (match_operand 3 "" ""))
	 (pc)))]
  "TARGET_THUMB1"
  "*
  output_asm_insn (\"cmn\\t%1, %2\", operands);
  switch (get_attr_length (insn))
    {
    case 4:  return \"b%d0\\t%l3\";
    case 6:  return \"b%D0\\t.LCB%=\;b\\t%l3\\t%@long jump\\n.LCB%=:\";
    default: return \"b%D0\\t.LCB%=\;bl\\t%l3\\t%@far jump\\n.LCB%=:\";
    }
  "
  [(set (attr "far_jump")
        (if_then_else
	    (eq_attr "length" "8")
	    (const_string "yes")
            (const_string "no")))
   (set (attr "length") 
        (if_then_else
	    (and (ge (minus (match_dup 3) (pc)) (const_int -250))
	         (le (minus (match_dup 3) (pc)) (const_int 256)))
	    (const_int 4)
	    (if_then_else
	        (and (ge (minus (match_dup 3) (pc)) (const_int -2040))
		     (le (minus (match_dup 3) (pc)) (const_int 2048)))
		(const_int 6)
		(const_int 8))))]
)

//Pattern#391
(define_insn "*tbit_cbranch"
  [(set (pc)
	(if_then_else
	 (match_operator 0 "equality_operator"
	  [(zero_extract:SI (match_operand:SI 1 "s_register_operand" "l")
			    (const_int 1)
			    (match_operand:SI 2 "const_int_operand" "i"))
	   (const_int 0)])
	 (label_ref (match_operand 3 "" ""))
	 (pc)))
   (clobber (match_scratch:SI 4 "=l"))]
  "TARGET_THUMB1"
  "*
  {
  rtx op[3];
  op[0] = operands[4];
  op[1] = operands[1];
  op[2] = GEN_INT (32 - 1 - INTVAL (operands[2]));

  output_asm_insn (\"lsl\\t%0, %1, %2\", op);
  switch (get_attr_length (insn))
    {
    case 4:  return \"b%d0\\t%l3\";
    case 6:  return \"b%D0\\t.LCB%=\;b\\t%l3\\t%@long jump\\n.LCB%=:\";
    default: return \"b%D0\\t.LCB%=\;bl\\t%l3\\t%@far jump\\n.LCB%=:\";
    }
  }"
  [(set (attr "far_jump")
        (if_then_else
	    (eq_attr "length" "8")
	    (const_string "yes")
            (const_string "no")))
   (set (attr "length") 
        (if_then_else
	    (and (ge (minus (match_dup 3) (pc)) (const_int -250))
	         (le (minus (match_dup 3) (pc)) (const_int 256)))
	    (const_int 4)
	    (if_then_else
	        (and (ge (minus (match_dup 3) (pc)) (const_int -2040))
		     (le (minus (match_dup 3) (pc)) (const_int 2048)))
		(const_int 6)
		(const_int 8))))]
)
  
//Pattern#392
(define_insn "*tlobits_cbranch"
  [(set (pc)
	(if_then_else
	 (match_operator 0 "equality_operator"
	  [(zero_extract:SI (match_operand:SI 1 "s_register_operand" "l")
			    (match_operand:SI 2 "const_int_operand" "i")
			    (const_int 0))
	   (const_int 0)])
	 (label_ref (match_operand 3 "" ""))
	 (pc)))
   (clobber (match_scratch:SI 4 "=l"))]
  "TARGET_THUMB1"
  "*
  {
  rtx op[3];
  op[0] = operands[4];
  op[1] = operands[1];
  op[2] = GEN_INT (32 - INTVAL (operands[2]));

  output_asm_insn (\"lsl\\t%0, %1, %2\", op);
  switch (get_attr_length (insn))
    {
    case 4:  return \"b%d0\\t%l3\";
    case 6:  return \"b%D0\\t.LCB%=\;b\\t%l3\\t%@long jump\\n.LCB%=:\";
    default: return \"b%D0\\t.LCB%=\;bl\\t%l3\\t%@far jump\\n.LCB%=:\";
    }
  }"
  [(set (attr "far_jump")
        (if_then_else
	    (eq_attr "length" "8")
	    (const_string "yes")
            (const_string "no")))
   (set (attr "length") 
        (if_then_else
	    (and (ge (minus (match_dup 3) (pc)) (const_int -250))
	         (le (minus (match_dup 3) (pc)) (const_int 256)))
	    (const_int 4)
	    (if_then_else
	        (and (ge (minus (match_dup 3) (pc)) (const_int -2040))
		     (le (minus (match_dup 3) (pc)) (const_int 2048)))
		(const_int 6)
		(const_int 8))))]
)

//Pattern#393
(define_insn "*tstsi3_cbranch"
  [(set (pc)
	(if_then_else
	 (match_operator 3 "equality_operator"
	  [(and:SI (match_operand:SI 0 "s_register_operand" "%l")
		   (match_operand:SI 1 "s_register_operand" "l"))
	   (const_int 0)])
	 (label_ref (match_operand 2 "" ""))
	 (pc)))]
  "TARGET_THUMB1"
  "*
  {
  output_asm_insn (\"tst\\t%0, %1\", operands);
  switch (get_attr_length (insn))
    {
    case 4:  return \"b%d3\\t%l2\";
    case 6:  return \"b%D3\\t.LCB%=\;b\\t%l2\\t%@long jump\\n.LCB%=:\";
    default: return \"b%D3\\t.LCB%=\;bl\\t%l2\\t%@far jump\\n.LCB%=:\";
    }
  }"
  [(set (attr "far_jump")
        (if_then_else
	    (eq_attr "length" "8")
	    (const_string "yes")
            (const_string "no")))
   (set (attr "length") 
        (if_then_else
	    (and (ge (minus (match_dup 2) (pc)) (const_int -250))
	         (le (minus (match_dup 2) (pc)) (const_int 256)))
	    (const_int 4)
	    (if_then_else
	        (and (ge (minus (match_dup 2) (pc)) (const_int -2040))
		     (le (minus (match_dup 2) (pc)) (const_int 2048)))
		(const_int 6)
		(const_int 8))))]
)
  
//Pattern#394
(define_insn "*cbranchne_decr1"
  [(set (pc)
	(if_then_else (match_operator 3 "equality_operator"
		       [(match_operand:SI 2 "s_register_operand" "l,l,1,l")
		        (const_int 0)])
		      (label_ref (match_operand 4 "" ""))
		      (pc)))
   (set (match_operand:SI 0 "thumb_cbrch_target_operand" "=l,*?h,*?m,*?m")
	(plus:SI (match_dup 2) (const_int -1)))
   (clobber (match_scratch:SI 1 "=X,l,&l,&l"))]
  "TARGET_THUMB1"
  "*
   {
     rtx cond[2];
     cond[0] = gen_rtx_fmt_ee ((GET_CODE (operands[3]) == NE
				? GEU : LTU),
			       VOIDmode, operands[2], const1_rtx);
     cond[1] = operands[4];

     if (which_alternative == 0)
       output_asm_insn (\"sub\\t%0, %2, #1\", operands);
     else if (which_alternative == 1)
       {
	 /* We must provide an alternative for a hi reg because reload 
	    cannot handle output reloads on a jump instruction, but we
	    can't subtract into that.  Fortunately a mov from lo to hi
	    does not clobber the condition codes.  */
	 output_asm_insn (\"sub\\t%1, %2, #1\", operands);
	 output_asm_insn (\"mov\\t%0, %1\", operands);
       }
     else
       {
	 /* Similarly, but the target is memory.  */
	 output_asm_insn (\"sub\\t%1, %2, #1\", operands);
	 output_asm_insn (\"str\\t%1, %0\", operands);
       }

     switch (get_attr_length (insn) - (which_alternative ? 2 : 0))
       {
	 case 4:
	   output_asm_insn (\"b%d0\\t%l1\", cond);
	   return \"\";
	 case 6:
	   output_asm_insn (\"b%D0\\t.LCB%=\", cond);
	   return \"b\\t%l4\\t%@long jump\\n.LCB%=:\";
	 default:
	   output_asm_insn (\"b%D0\\t.LCB%=\", cond);
	   return \"bl\\t%l4\\t%@far jump\\n.LCB%=:\";
       }
   }
  "
  [(set (attr "far_jump")
        (if_then_else
	    (ior (and (eq (symbol_ref ("which_alternative"))
	                  (const_int 0))
		      (eq_attr "length" "8"))
		 (eq_attr "length" "10"))
	    (const_string "yes")
            (const_string "no")))
   (set_attr_alternative "length"
      [
       ;; Alternative 0
       (if_then_else
	 (and (ge (minus (match_dup 4) (pc)) (const_int -250))
	      (le (minus (match_dup 4) (pc)) (const_int 256)))
	 (const_int 4)
	 (if_then_else
	   (and (ge (minus (match_dup 4) (pc)) (const_int -2040))
		(le (minus (match_dup 4) (pc)) (const_int 2048)))
	   (const_int 6)
	   (const_int 8)))
       ;; Alternative 1
       (if_then_else
	 (and (ge (minus (match_dup 4) (pc)) (const_int -248))
	      (le (minus (match_dup 4) (pc)) (const_int 256)))
	 (const_int 6)
	 (if_then_else
	   (and (ge (minus (match_dup 4) (pc)) (const_int -2038))
		(le (minus (match_dup 4) (pc)) (const_int 2048)))
	   (const_int 8)
	   (const_int 10)))
       ;; Alternative 2
       (if_then_else
	 (and (ge (minus (match_dup 4) (pc)) (const_int -248))
	      (le (minus (match_dup 4) (pc)) (const_int 256)))
	 (const_int 6)
	 (if_then_else
	   (and (ge (minus (match_dup 4) (pc)) (const_int -2038))
		(le (minus (match_dup 4) (pc)) (const_int 2048)))
	   (const_int 8)
	   (const_int 10)))
       ;; Alternative 3
       (if_then_else
	 (and (ge (minus (match_dup 4) (pc)) (const_int -248))
	      (le (minus (match_dup 4) (pc)) (const_int 256)))
	 (const_int 6)
	 (if_then_else
	   (and (ge (minus (match_dup 4) (pc)) (const_int -2038))
		(le (minus (match_dup 4) (pc)) (const_int 2048)))
	   (const_int 8)
	   (const_int 10)))])]
)

//Pattern#395
(define_insn "*addsi3_cbranch"
  [(set (pc)
	(if_then_else
	 (match_operator 4 "arm_comparison_operator"
	  [(plus:SI
	    (match_operand:SI 2 "s_register_operand" "%0,l,*l,1,1,1")
	    (match_operand:SI 3 "reg_or_int_operand" "IJ,lL,*l,lIJ,lIJ,lIJ"))
	   (const_int 0)])
	 (label_ref (match_operand 5 "" ""))
	 (pc)))
   (set
    (match_operand:SI 0 "thumb_cbrch_target_operand" "=l,l,*!h,*?h,*?m,*?m")
    (plus:SI (match_dup 2) (match_dup 3)))
   (clobber (match_scratch:SI 1 "=X,X,l,l,&l,&l"))]
  "TARGET_THUMB1
   && (GET_CODE (operands[4]) == EQ
       || GET_CODE (operands[4]) == NE
       || GET_CODE (operands[4]) == GE
       || GET_CODE (operands[4]) == LT)"
  "*
   {
     rtx cond[3];

     cond[0] = (which_alternative < 2) ? operands[0] : operands[1];
     cond[1] = operands[2];
     cond[2] = operands[3];

     if (CONST_INT_P (cond[2]) && INTVAL (cond[2]) < 0)
       output_asm_insn (\"sub\\t%0, %1, #%n2\", cond);
     else
       output_asm_insn (\"add\\t%0, %1, %2\", cond);

     if (which_alternative >= 2
	 && which_alternative < 4)
       output_asm_insn (\"mov\\t%0, %1\", operands);
     else if (which_alternative >= 4)
       output_asm_insn (\"str\\t%1, %0\", operands);

     switch (get_attr_length (insn) - ((which_alternative >= 2) ? 2 : 0))
       {
	 case 4:
	   return \"b%d4\\t%l5\";
	 case 6:
	   return \"b%D4\\t.LCB%=\;b\\t%l5\\t%@long jump\\n.LCB%=:\";
	 default:
	   return \"b%D4\\t.LCB%=\;bl\\t%l5\\t%@far jump\\n.LCB%=:\";
       }
   }
  "
  [(set (attr "far_jump")
        (if_then_else
	    (ior (and (lt (symbol_ref ("which_alternative"))
	                  (const_int 2))
		      (eq_attr "length" "8"))
		 (eq_attr "length" "10"))
	    (const_string "yes")
            (const_string "no")))
   (set (attr "length")
     (if_then_else
       (lt (symbol_ref ("which_alternative"))
		       (const_int 2))
       (if_then_else
	 (and (ge (minus (match_dup 5) (pc)) (const_int -250))
	      (le (minus (match_dup 5) (pc)) (const_int 256)))
	 (const_int 4)
	 (if_then_else
	   (and (ge (minus (match_dup 5) (pc)) (const_int -2040))
		(le (minus (match_dup 5) (pc)) (const_int 2048)))
	   (const_int 6)
	   (const_int 8)))
       (if_then_else
	 (and (ge (minus (match_dup 5) (pc)) (const_int -248))
	      (le (minus (match_dup 5) (pc)) (const_int 256)))
	 (const_int 6)
	 (if_then_else
	   (and (ge (minus (match_dup 5) (pc)) (const_int -2038))
		(le (minus (match_dup 5) (pc)) (const_int 2048)))
	   (const_int 8)
	   (const_int 10)))))]
)

//Pattern#396
(define_insn "*addsi3_cbranch_scratch"
  [(set (pc)
	(if_then_else
	 (match_operator 3 "arm_comparison_operator"
	  [(plus:SI
	    (match_operand:SI 1 "s_register_operand" "%l,l,l,0")
	    (match_operand:SI 2 "reg_or_int_operand" "J,l,L,IJ"))
	   (const_int 0)])
	 (label_ref (match_operand 4 "" ""))
	 (pc)))
   (clobber (match_scratch:SI 0 "=X,X,l,l"))]
  "TARGET_THUMB1
   && (GET_CODE (operands[3]) == EQ
       || GET_CODE (operands[3]) == NE
       || GET_CODE (operands[3]) == GE
       || GET_CODE (operands[3]) == LT)"
  "*
   {
     switch (which_alternative)
       {
       case 0:
	 output_asm_insn (\"cmp\t%1, #%n2\", operands);
	 break;
       case 1:
	 output_asm_insn (\"cmn\t%1, %2\", operands);
	 break;
       case 2:
	 if (INTVAL (operands[2]) < 0)
	   output_asm_insn (\"sub\t%0, %1, %2\", operands);
	 else
	   output_asm_insn (\"add\t%0, %1, %2\", operands);
	 break;
       case 3:
	 if (INTVAL (operands[2]) < 0)
	   output_asm_insn (\"sub\t%0, %0, %2\", operands);
	 else
	   output_asm_insn (\"add\t%0, %0, %2\", operands);
	 break;
       }

     switch (get_attr_length (insn))
       {
	 case 4:
	   return \"b%d3\\t%l4\";
	 case 6:
	   return \"b%D3\\t.LCB%=\;b\\t%l4\\t%@long jump\\n.LCB%=:\";
	 default:
	   return \"b%D3\\t.LCB%=\;bl\\t%l4\\t%@far jump\\n.LCB%=:\";
       }
   }
  "
  [(set (attr "far_jump")
        (if_then_else
	    (eq_attr "length" "8")
	    (const_string "yes")
            (const_string "no")))
   (set (attr "length")
       (if_then_else
	 (and (ge (minus (match_dup 4) (pc)) (const_int -250))
	      (le (minus (match_dup 4) (pc)) (const_int 256)))
	 (const_int 4)
	 (if_then_else
	   (and (ge (minus (match_dup 4) (pc)) (const_int -2040))
		(le (minus (match_dup 4) (pc)) (const_int 2048)))
	   (const_int 6)
	   (const_int 8))))]
)


;; Comparison and test insns

//Pattern#397
(define_insn "*arm_cmpsi_insn"
  [(set (reg:CC CC_REGNUM)
	(compare:CC (match_operand:SI 0 "s_register_operand" "l,r,r,r")
		    (match_operand:SI 1 "arm_add_operand"    "Py,r,rI,L")))]
  "TARGET_32BIT"
  "@
   cmp%?\\t%0, %1
   cmp%?\\t%0, %1
   cmp%?\\t%0, %1
   cmn%?\\t%0, #%n1"
  [(set_attr "conds" "set")
   (set_attr "arch" "t2,t2,any,any")
   (set_attr "length" "2,2,4,4")
   (set_attr "predicable" "yes")
   (set_attr "type" "*,*,*,simple_alu_imm")]
)

//Pattern#398
(define_insn "*cmpsi_shiftsi"
  [(set (reg:CC CC_REGNUM)
	(compare:CC (match_operand:SI   0 "s_register_operand" "r,r")
		    (match_operator:SI  3 "shift_operator"
		     [(match_operand:SI 1 "s_register_operand" "r,r")
		      (match_operand:SI 2 "shift_amount_operand" "M,rM")])))]
  "TARGET_32BIT"
  "cmp%?\\t%0, %1%S3"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#399
(define_insn "*cmpsi_shiftsi_swp"
  [(set (reg:CC_SWP CC_REGNUM)
	(compare:CC_SWP (match_operator:SI 3 "shift_operator"
			 [(match_operand:SI 1 "s_register_operand" "r,r")
			  (match_operand:SI 2 "shift_amount_operand" "M,rM")])
			(match_operand:SI 0 "s_register_operand" "r,r")))]
  "TARGET_32BIT"
  "cmp%?\\t%0, %1%S3"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#400
(define_insn "*arm_cmpsi_negshiftsi_si"
  [(set (reg:CC_Z CC_REGNUM)
	(compare:CC_Z
	 (neg:SI (match_operator:SI 1 "shift_operator"
		    [(match_operand:SI 2 "s_register_operand" "r")
		     (match_operand:SI 3 "reg_or_int_operand" "rM")]))
	 (match_operand:SI 0 "s_register_operand" "r")))]
  "TARGET_ARM"
  "cmn%?\\t%0, %2%S1"
  [(set_attr "conds" "set")
   (set (attr "type") (if_then_else (match_operand 3 "const_int_operand" "")
				    (const_string "alu_shift")
				    (const_string "alu_shift_reg")))
   (set_attr "predicable" "yes")]
)

;; DImode comparisons.  The generic code generates branches that
;; if-conversion can not reduce to a conditional compare, so we do
;; that directly.

//Pattern#401
(define_insn "*arm_cmpdi_insn"
  [(set (reg:CC_NCV CC_REGNUM)
	(compare:CC_NCV (match_operand:DI 0 "s_register_operand" "r")
			(match_operand:DI 1 "arm_di_operand"	   "rDi")))
   (clobber (match_scratch:SI 2 "=r"))]
  "TARGET_32BIT"
  "cmp\\t%Q0, %Q1\;sbcs\\t%2, %R0, %R1"
  [(set_attr "conds" "set")
   (set_attr "length" "8")]
)

//Pattern#402
(define_insn "*arm_cmpdi_unsigned"
  [(set (reg:CC_CZ CC_REGNUM)
	(compare:CC_CZ (match_operand:DI 0 "s_register_operand" "r")
		       (match_operand:DI 1 "arm_di_operand"	"rDi")))]
  "TARGET_32BIT"
  "cmp\\t%R0, %R1\;it eq\;cmpeq\\t%Q0, %Q1"
  [(set_attr "conds" "set")
   (set_attr "length" "8")]
)

//Pattern#403
(define_insn "*arm_cmpdi_zero"
  [(set (reg:CC_Z CC_REGNUM)
	(compare:CC_Z (match_operand:DI 0 "s_register_operand" "r")
		      (const_int 0)))
   (clobber (match_scratch:SI 1 "=r"))]
  "TARGET_32BIT"
  "orr%.\\t%1, %Q0, %R0"
  [(set_attr "conds" "set")]
)

//Pattern#404
(define_insn "*thumb_cmpdi_zero"
  [(set (reg:CC_Z CC_REGNUM)
	(compare:CC_Z (match_operand:DI 0 "s_register_operand" "l")
		      (const_int 0)))
   (clobber (match_scratch:SI 1 "=l"))]
  "TARGET_THUMB1"
  "orr\\t%1, %Q0, %R0"
  [(set_attr "conds" "set")
   (set_attr "length" "2")]
)

; This insn allows redundant compares to be removed by cse, nothing should
; ever appear in the output file since (set (reg x) (reg x)) is a no-op that
; is deleted later on. The match_dup will match the mode here, so that
; mode changes of the condition codes aren't lost by this even though we don't
; specify what they are.

//Pattern#405
(define_insn "*deleted_compare"
  [(set (match_operand 0 "cc_register" "") (match_dup 0))]
  "TARGET_32BIT"
  "\\t%@ deleted compare"
  [(set_attr "conds" "set")
   (set_attr "length" "0")]
)


;; Conditional branch insns

//Pattern#406
(define_expand "cbranch_cc"
  [(set (pc)
	(if_then_else (match_operator 0 "" [(match_operand 1 "" "")
					    (match_operand 2 "" "")])
		      (label_ref (match_operand 3 "" ""))
		      (pc)))]
  "TARGET_32BIT"
  "operands[1] = arm_gen_compare_reg (GET_CODE (operands[0]),
				      operands[1], operands[2], NULL_RTX);
   operands[2] = const0_rtx;"
)

;;
;; Patterns to match conditional branch insns.
;;

//Pattern#407
(define_insn "arm_cond_branch"
  [(set (pc)
	(if_then_else (match_operator 1 "arm_comparison_operator"
		       [(match_operand 2 "cc_register" "") (const_int 0)])
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  "TARGET_32BIT"
  "*
  if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
    {
      arm_ccfsm_state += 2;
      return \"\";
    }
  return \"b%d1\\t%l0\";
  "
  [(set_attr "conds" "use")
   (set_attr "type" "branch")
   (set (attr "length")
	(if_then_else
	   (and (match_test "TARGET_THUMB2")
		(and (ge (minus (match_dup 0) (pc)) (const_int -250))
		     (le (minus (match_dup 0) (pc)) (const_int 256))))
	   (const_int 2)
	   (const_int 4)))]
)

//Pattern#408
(define_insn "*arm_cond_branch_reversed"
  [(set (pc)
	(if_then_else (match_operator 1 "arm_comparison_operator"
		       [(match_operand 2 "cc_register" "") (const_int 0)])
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  "TARGET_32BIT"
  "*
  if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
    {
      arm_ccfsm_state += 2;
      return \"\";
    }
  return \"b%D1\\t%l0\";
  "
  [(set_attr "conds" "use")
   (set_attr "type" "branch")
   (set (attr "length")
	(if_then_else
	   (and (match_test "TARGET_THUMB2")
		(and (ge (minus (match_dup 0) (pc)) (const_int -250))
		     (le (minus (match_dup 0) (pc)) (const_int 256))))
	   (const_int 2)
	   (const_int 4)))]
)



; scc insns

//Pattern#409
(define_expand "cstore_cc"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "" [(match_operand 2 "" "")
				 (match_operand 3 "" "")]))]
  "TARGET_32BIT"
  "operands[2] = arm_gen_compare_reg (GET_CODE (operands[1]),
				      operands[2], operands[3], NULL_RTX);
   operands[3] = const0_rtx;"
)

//Pattern#410
(define_insn "*mov_scc"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(match_operator:SI 1 "arm_comparison_operator"
	 [(match_operand 2 "cc_register" "") (const_int 0)]))]
  "TARGET_ARM"
  "mov%D1\\t%0, #0\;mov%d1\\t%0, #1"
  [(set_attr "conds" "use")
   (set_attr "insn" "mov")
   (set_attr "length" "8")]
)

//Pattern#411
(define_insn "*mov_negscc"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(neg:SI (match_operator:SI 1 "arm_comparison_operator"
		 [(match_operand 2 "cc_register" "") (const_int 0)])))]
  "TARGET_ARM"
  "mov%D1\\t%0, #0\;mvn%d1\\t%0, #0"
  [(set_attr "conds" "use")
   (set_attr "insn" "mov")
   (set_attr "length" "8")]
)

//Pattern#412
(define_insn "*mov_notscc"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(not:SI (match_operator:SI 1 "arm_comparison_operator"
		 [(match_operand 2 "cc_register" "") (const_int 0)])))]
  "TARGET_ARM"
  "mvn%D1\\t%0, #0\;mvn%d1\\t%0, #1"
  [(set_attr "conds" "use")
   (set_attr "insn" "mov")
   (set_attr "length" "8")]
)

//Pattern#413
(define_expand "cstoresi4"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "expandable_comparison_operator"
	 [(match_operand:SI 2 "s_register_operand" "")
	  (match_operand:SI 3 "reg_or_int_operand" "")]))]
  "TARGET_32BIT || TARGET_THUMB1"
  "{
  rtx op3, scratch, scratch2;

  if (!TARGET_THUMB1)
    {
      if (!arm_add_operand (operands[3], SImode))
	operands[3] = force_reg (SImode, operands[3]);
      emit_insn (gen_cstore_cc (operands[0], operands[1],
				operands[2], operands[3]));
      DONE;
    }

  if (operands[3] == const0_rtx)
    {
      switch (GET_CODE (operands[1]))
	{
	case EQ:
	  emit_insn (gen_cstoresi_eq0_thumb1 (operands[0], operands[2]));
	  break;

	case NE:
	  emit_insn (gen_cstoresi_ne0_thumb1 (operands[0], operands[2]));
	  break;

	case LE:
          scratch = expand_binop (SImode, add_optab, operands[2], constm1_rtx,
				  NULL_RTX, 0, OPTAB_WIDEN);
          scratch = expand_binop (SImode, ior_optab, operands[2], scratch,
				  NULL_RTX, 0, OPTAB_WIDEN);
          expand_binop (SImode, lshr_optab, scratch, GEN_INT (31),
			operands[0], 1, OPTAB_WIDEN);
	  break;

        case GE:
          scratch = expand_unop (SImode, one_cmpl_optab, operands[2],
				 NULL_RTX, 1);
          expand_binop (SImode, lshr_optab, scratch, GEN_INT (31),
			NULL_RTX, 1, OPTAB_WIDEN);
          break;

        case GT:
          scratch = expand_binop (SImode, ashr_optab, operands[2],
				  GEN_INT (31), NULL_RTX, 0, OPTAB_WIDEN);
          scratch = expand_binop (SImode, sub_optab, scratch, operands[2],
				  NULL_RTX, 0, OPTAB_WIDEN);
          expand_binop (SImode, lshr_optab, scratch, GEN_INT (31), operands[0],
			0, OPTAB_WIDEN);
          break;

	/* LT is handled by generic code.  No need for unsigned with 0.  */
	default:
	  FAIL;
	}
      DONE;
    }

  switch (GET_CODE (operands[1]))
    {
    case EQ:
      scratch = expand_binop (SImode, sub_optab, operands[2], operands[3],
			      NULL_RTX, 0, OPTAB_WIDEN);
      emit_insn (gen_cstoresi_eq0_thumb1 (operands[0], scratch));
      break;

    case NE:
      scratch = expand_binop (SImode, sub_optab, operands[2], operands[3],
			      NULL_RTX, 0, OPTAB_WIDEN);
      emit_insn (gen_cstoresi_ne0_thumb1 (operands[0], scratch));
      break;

    case LE:
      op3 = force_reg (SImode, operands[3]);

      scratch = expand_binop (SImode, lshr_optab, operands[2], GEN_INT (31),
			      NULL_RTX, 1, OPTAB_WIDEN);
      scratch2 = expand_binop (SImode, ashr_optab, op3, GEN_INT (31),
			      NULL_RTX, 0, OPTAB_WIDEN);
      emit_insn (gen_thumb1_addsi3_addgeu (operands[0], scratch, scratch2,
					  op3, operands[2]));
      break;

    case GE:
      op3 = operands[3];
      if (!thumb1_cmp_operand (op3, SImode))
        op3 = force_reg (SImode, op3);
      scratch = expand_binop (SImode, ashr_optab, operands[2], GEN_INT (31),
			      NULL_RTX, 0, OPTAB_WIDEN);
      scratch2 = expand_binop (SImode, lshr_optab, op3, GEN_INT (31),
			       NULL_RTX, 1, OPTAB_WIDEN);
      emit_insn (gen_thumb1_addsi3_addgeu (operands[0], scratch, scratch2,
					  operands[2], op3));
      break;

    case LEU:
      op3 = force_reg (SImode, operands[3]);
      scratch = force_reg (SImode, const0_rtx);
      emit_insn (gen_thumb1_addsi3_addgeu (operands[0], scratch, scratch,
					  op3, operands[2]));
      break;

    case GEU:
      op3 = operands[3];
      if (!thumb1_cmp_operand (op3, SImode))
        op3 = force_reg (SImode, op3);
      scratch = force_reg (SImode, const0_rtx);
      emit_insn (gen_thumb1_addsi3_addgeu (operands[0], scratch, scratch,
					  operands[2], op3));
      break;

    case LTU:
      op3 = operands[3];
      if (!thumb1_cmp_operand (op3, SImode))
        op3 = force_reg (SImode, op3);
      scratch = gen_reg_rtx (SImode);
      emit_insn (gen_cstoresi_ltu_thumb1 (operands[0], operands[2], op3));
      break;

    case GTU:
      op3 = force_reg (SImode, operands[3]);
      scratch = gen_reg_rtx (SImode);
      emit_insn (gen_cstoresi_ltu_thumb1 (operands[0], op3, operands[2]));
      break;

    /* No good sequences for GT, LT.  */
    default:
      FAIL;
    }
  DONE;
}")

//Pattern#414
(define_expand "cstoresf4"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "expandable_comparison_operator"
	 [(match_operand:SF 2 "s_register_operand" "")
	  (match_operand:SF 3 "arm_float_compare_operand" "")]))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "emit_insn (gen_cstore_cc (operands[0], operands[1],
			     operands[2], operands[3])); DONE;"
)

//Pattern#415
(define_expand "cstoredf4"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "expandable_comparison_operator"
	 [(match_operand:DF 2 "s_register_operand" "")
	  (match_operand:DF 3 "arm_float_compare_operand" "")]))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "emit_insn (gen_cstore_cc (operands[0], operands[1],
			     operands[2], operands[3])); DONE;"
)

//Pattern#416
(define_expand "cstoredi4"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "expandable_comparison_operator"
	 [(match_operand:DI 2 "s_register_operand" "")
	  (match_operand:DI 3 "cmpdi_operand" "")]))]
  "TARGET_32BIT"
  "{
     if (!arm_validize_comparison (&operands[1],
     				   &operands[2],
				   &operands[3]))
       FAIL;
     emit_insn (gen_cstore_cc (operands[0], operands[1], operands[2],
		      	         operands[3]));
     DONE;
   }"
)

//Pattern#417
(define_expand "cstoresi_eq0_thumb1"
  [(parallel
    [(set (match_operand:SI 0 "s_register_operand" "")
	  (eq:SI (match_operand:SI 1 "s_register_operand" "")
		 (const_int 0)))
     (clobber (match_dup:SI 2))])]
  "TARGET_THUMB1"
  "operands[2] = gen_reg_rtx (SImode);"
)

//Pattern#418
(define_expand "cstoresi_ne0_thumb1"
  [(parallel
    [(set (match_operand:SI 0 "s_register_operand" "")
	  (ne:SI (match_operand:SI 1 "s_register_operand" "")
		 (const_int 0)))
     (clobber (match_dup:SI 2))])]
  "TARGET_THUMB1"
  "operands[2] = gen_reg_rtx (SImode);"
)

//Pattern#419
(define_insn "*cstoresi_eq0_thumb1_insn"
  [(set (match_operand:SI 0 "s_register_operand" "=&l,l")
	(eq:SI (match_operand:SI 1 "s_register_operand" "l,0")
	       (const_int 0)))
   (clobber (match_operand:SI 2 "s_register_operand" "=X,l"))]
  "TARGET_THUMB1"
  "@
   neg\\t%0, %1\;adc\\t%0, %0, %1
   neg\\t%2, %1\;adc\\t%0, %1, %2"
  [(set_attr "length" "4")]
)

//Pattern#420
(define_insn "*cstoresi_ne0_thumb1_insn"
  [(set (match_operand:SI 0 "s_register_operand" "=l")
	(ne:SI (match_operand:SI 1 "s_register_operand" "0")
	       (const_int 0)))
   (clobber (match_operand:SI 2 "s_register_operand" "=l"))]
  "TARGET_THUMB1"
  "sub\\t%2, %1, #1\;sbc\\t%0, %1, %2"
  [(set_attr "length" "4")]
)

;; Used as part of the expansion of thumb ltu and gtu sequences
//Pattern#421
(define_insn "cstoresi_nltu_thumb1"
  [(set (match_operand:SI 0 "s_register_operand" "=l,l")
        (neg:SI (ltu:SI (match_operand:SI 1 "s_register_operand" "l,*h")
			(match_operand:SI 2 "thumb1_cmp_operand" "lI*h,*r"))))]
  "TARGET_THUMB1"
  "cmp\\t%1, %2\;sbc\\t%0, %0, %0"
  [(set_attr "length" "4")]
)

//Pattern#422
(define_insn_and_split "cstoresi_ltu_thumb1"
  [(set (match_operand:SI 0 "s_register_operand" "=l,l")
        (ltu:SI (match_operand:SI 1 "s_register_operand" "l,*h")
		(match_operand:SI 2 "thumb1_cmp_operand" "lI*h,*r")))]
  "TARGET_THUMB1"
  "#"
  "TARGET_THUMB1"
  [(set (match_dup 3)
	(neg:SI (ltu:SI (match_dup 1) (match_dup 2))))
   (set (match_dup 0) (neg:SI (match_dup 3)))]
  "operands[3] = gen_reg_rtx (SImode);"
  [(set_attr "length" "4")]
)

;; Used as part of the expansion of thumb les sequence.
//Pattern#423
(define_insn "thumb1_addsi3_addgeu"
  [(set (match_operand:SI 0 "s_register_operand" "=l")
        (plus:SI (plus:SI (match_operand:SI 1 "s_register_operand" "%0")
			  (match_operand:SI 2 "s_register_operand" "l"))
		 (geu:SI (match_operand:SI 3 "s_register_operand" "l")
			 (match_operand:SI 4 "thumb1_cmp_operand" "lI"))))]
  "TARGET_THUMB1"
  "cmp\\t%3, %4\;adc\\t%0, %1, %2"
  [(set_attr "length" "4")]
)


;; Conditional move insns

//Pattern#424
(define_expand "movsicc"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(if_then_else:SI (match_operand 1 "expandable_comparison_operator" "")
			 (match_operand:SI 2 "arm_not_operand" "")
			 (match_operand:SI 3 "arm_not_operand" "")))]
  "TARGET_32BIT"
  "
  {
    enum rtx_code code;
    rtx ccreg;

    if (!arm_validize_comparison (&operands[1], &XEXP (operands[1], 0), 
       				  &XEXP (operands[1], 1)))
      FAIL;
    
    code = GET_CODE (operands[1]);
    ccreg = arm_gen_compare_reg (code, XEXP (operands[1], 0),
				 XEXP (operands[1], 1), NULL_RTX);
    operands[1] = gen_rtx_fmt_ee (code, VOIDmode, ccreg, const0_rtx);
  }"
)

//Pattern#425
(define_expand "movsfcc"
  [(set (match_operand:SF 0 "s_register_operand" "")
	(if_then_else:SF (match_operand 1 "expandable_comparison_operator" "")
			 (match_operand:SF 2 "s_register_operand" "")
			 (match_operand:SF 3 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
  {
    enum rtx_code code = GET_CODE (operands[1]);
    rtx ccreg;

    if (!arm_validize_comparison (&operands[1], &XEXP (operands[1], 0), 
       				  &XEXP (operands[1], 1)))
       FAIL;

    code = GET_CODE (operands[1]);
    ccreg = arm_gen_compare_reg (code, XEXP (operands[1], 0),
				 XEXP (operands[1], 1), NULL_RTX);
    operands[1] = gen_rtx_fmt_ee (code, VOIDmode, ccreg, const0_rtx);
  }"
)

//Pattern#426
(define_expand "movdfcc"
  [(set (match_operand:DF 0 "s_register_operand" "")
	(if_then_else:DF (match_operand 1 "expandable_comparison_operator" "")
			 (match_operand:DF 2 "s_register_operand" "")
			 (match_operand:DF 3 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  "
  {
    enum rtx_code code = GET_CODE (operands[1]);
    rtx ccreg;

    if (!arm_validize_comparison (&operands[1], &XEXP (operands[1], 0), 
       				  &XEXP (operands[1], 1)))
       FAIL;
    code = GET_CODE (operands[1]);
    ccreg = arm_gen_compare_reg (code, XEXP (operands[1], 0),
				 XEXP (operands[1], 1), NULL_RTX);
    operands[1] = gen_rtx_fmt_ee (code, VOIDmode, ccreg, const0_rtx);
  }"
)

//Pattern#427
(define_insn "*movsicc_insn"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r,r,r,r,r,r")
	(if_then_else:SI
	 (match_operator 3 "arm_comparison_operator"
	  [(match_operand 4 "cc_register" "") (const_int 0)])
	 (match_operand:SI 1 "arm_not_operand" "0,0,rI,K,rI,rI,K,K")
	 (match_operand:SI 2 "arm_not_operand" "rI,K,0,0,rI,K,rI,K")))]
  "TARGET_ARM"
  "@
   mov%D3\\t%0, %2
   mvn%D3\\t%0, #%B2
   mov%d3\\t%0, %1
   mvn%d3\\t%0, #%B1
   mov%d3\\t%0, %1\;mov%D3\\t%0, %2
   mov%d3\\t%0, %1\;mvn%D3\\t%0, #%B2
   mvn%d3\\t%0, #%B1\;mov%D3\\t%0, %2
   mvn%d3\\t%0, #%B1\;mvn%D3\\t%0, #%B2"
  [(set_attr "length" "4,4,4,4,8,8,8,8")
   (set_attr "conds" "use")
   (set_attr "insn" "mov,mvn,mov,mvn,mov,mov,mvn,mvn")
   (set_attr_alternative "type"
                         [(if_then_else (match_operand 2 "const_int_operand" "")
                                        (const_string "simple_alu_imm")
                                        (const_string "*"))
                          (const_string "simple_alu_imm")
                          (if_then_else (match_operand 1 "const_int_operand" "")
                                        (const_string "simple_alu_imm")
                                        (const_string "*"))
                          (const_string "simple_alu_imm")
                          (const_string "*")
                          (const_string "*")
                          (const_string "*")
                          (const_string "*")])]
)

//Pattern#428
(define_insn "*movsfcc_soft_insn"
  [(set (match_operand:SF 0 "s_register_operand" "=r,r")
	(if_then_else:SF (match_operator 3 "arm_comparison_operator"
			  [(match_operand 4 "cc_register" "") (const_int 0)])
			 (match_operand:SF 1 "s_register_operand" "0,r")
			 (match_operand:SF 2 "s_register_operand" "r,0")))]
  "TARGET_ARM && TARGET_SOFT_FLOAT"
  "@
   mov%D3\\t%0, %2
   mov%d3\\t%0, %1"
  [(set_attr "conds" "use")
   (set_attr "insn" "mov")]
)


;; Jump and linkage insns

//Pattern#429
(define_expand "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  "TARGET_EITHER"
  ""
)

//Pattern#430
(define_insn "*arm_jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  "TARGET_32BIT"
  "*
  {
    if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return \"\";
      }
    return \"b%?\\t%l0\";
  }
  "
  [(set_attr "predicable" "yes")
   (set (attr "length")
	(if_then_else
	   (and (match_test "TARGET_THUMB2")
		(and (ge (minus (match_dup 0) (pc)) (const_int -2044))
		     (le (minus (match_dup 0) (pc)) (const_int 2048))))
	   (const_int 2)
	   (const_int 4)))]
)

//Pattern#431
(define_insn "*thumb_jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  "TARGET_THUMB1"
  "*
  if (get_attr_length (insn) == 2)
    return \"b\\t%l0\";
  return \"bl\\t%l0\\t%@ far jump\";
  "
  [(set (attr "far_jump")
        (if_then_else
	    (eq_attr "length" "4")
	    (const_string "yes")
	    (const_string "no")))
   (set (attr "length") 
        (if_then_else
	    (and (ge (minus (match_dup 0) (pc)) (const_int -2044))
		 (le (minus (match_dup 0) (pc)) (const_int 2048)))
  	    (const_int 2)
	    (const_int 4)))]
)

//Pattern#432
(define_expand "call"
  [(parallel [(call (match_operand 0 "memory_operand" "")
	            (match_operand 1 "general_operand" ""))
	      (use (match_operand 2 "" ""))
	      (clobber (reg:SI LR_REGNUM))])]
  "TARGET_EITHER"
  "
  {
    rtx callee, pat;
    
    /* In an untyped call, we can get NULL for operand 2.  */
    if (operands[2] == NULL_RTX)
      operands[2] = const0_rtx;
      
    /* Decide if we should generate indirect calls by loading the
       32-bit address of the callee into a register before performing the
       branch and link.  */
    callee = XEXP (operands[0], 0);
    if (GET_CODE (callee) == SYMBOL_REF
	? arm_is_long_call_p (SYMBOL_REF_DECL (callee))
	: !REG_P (callee))
      XEXP (operands[0], 0) = force_reg (Pmode, callee);

    pat = gen_call_internal (operands[0], operands[1], operands[2]);
    arm_emit_call_insn (pat, XEXP (operands[0], 0));
    DONE;
  }"
)

//Pattern#433
(define_expand "call_internal"
  [(parallel [(call (match_operand 0 "memory_operand" "")
	            (match_operand 1 "general_operand" ""))
	      (use (match_operand 2 "" ""))
	      (clobber (reg:SI LR_REGNUM))])])

//Pattern#434
(define_insn "*call_reg_armv5"
  [(call (mem:SI (match_operand:SI 0 "s_register_operand" "r"))
         (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_ARM && arm_arch5"
  "blx%?\\t%0"
  [(set_attr "type" "call")]
)

//Pattern#435
(define_insn "*call_reg_arm"
  [(call (mem:SI (match_operand:SI 0 "s_register_operand" "r"))
         (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_ARM && !arm_arch5"
  "*
  return output_call (operands);
  "
  ;; length is worst case, normally it is only two
  [(set_attr "length" "12")
   (set_attr "type" "call")]
)


;; Note: not used for armv5+ because the sequence used (ldr pc, ...) is not
;; considered a function call by the branch predictor of some cores (PR40887).
;; Falls back to blx rN (*call_reg_armv5).

//Pattern#436
(define_insn "*call_mem"
  [(call (mem:SI (match_operand:SI 0 "call_memory_operand" "m"))
	 (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_ARM && !arm_arch5"
  "*
  return output_call_mem (operands);
  "
  [(set_attr "length" "12")
   (set_attr "type" "call")]
)

//Pattern#437
(define_insn "*call_reg_thumb1_v5"
  [(call (mem:SI (match_operand:SI 0 "register_operand" "l*r"))
	 (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_THUMB1 && arm_arch5"
  "blx\\t%0"
  [(set_attr "length" "2")
   (set_attr "type" "call")]
)

//Pattern#438
(define_insn "*call_reg_thumb1"
  [(call (mem:SI (match_operand:SI 0 "register_operand" "l*r"))
	 (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_THUMB1 && !arm_arch5"
  "*
  {
    if (!TARGET_CALLER_INTERWORKING)
      return thumb_call_via_reg (operands[0]);
    else if (operands[1] == const0_rtx)
      return \"bl\\t%__interwork_call_via_%0\";
    else if (frame_pointer_needed)
      return \"bl\\t%__interwork_r7_call_via_%0\";
    else
      return \"bl\\t%__interwork_r11_call_via_%0\";
  }"
  [(set_attr "type" "call")]
)

//Pattern#439
(define_expand "call_value"
  [(parallel [(set (match_operand       0 "" "")
	           (call (match_operand 1 "memory_operand" "")
		         (match_operand 2 "general_operand" "")))
	      (use (match_operand 3 "" ""))
	      (clobber (reg:SI LR_REGNUM))])]
  "TARGET_EITHER"
  "
  {
    rtx pat, callee;
    
    /* In an untyped call, we can get NULL for operand 2.  */
    if (operands[3] == 0)
      operands[3] = const0_rtx;
      
    /* Decide if we should generate indirect calls by loading the
       32-bit address of the callee into a register before performing the
       branch and link.  */
    callee = XEXP (operands[1], 0);
    if (GET_CODE (callee) == SYMBOL_REF
	? arm_is_long_call_p (SYMBOL_REF_DECL (callee))
	: !REG_P (callee))
      XEXP (operands[1], 0) = force_reg (Pmode, callee);

    pat = gen_call_value_internal (operands[0], operands[1],
				   operands[2], operands[3]);
    arm_emit_call_insn (pat, XEXP (operands[1], 0));
    DONE;
  }"
)

//Pattern#440
(define_expand "call_value_internal"
  [(parallel [(set (match_operand       0 "" "")
	           (call (match_operand 1 "memory_operand" "")
		         (match_operand 2 "general_operand" "")))
	      (use (match_operand 3 "" ""))
	      (clobber (reg:SI LR_REGNUM))])])

//Pattern#441
(define_insn "*call_value_reg_armv5"
  [(set (match_operand 0 "" "")
        (call (mem:SI (match_operand:SI 1 "s_register_operand" "r"))
	      (match_operand 2 "" "")))
   (use (match_operand 3 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_ARM && arm_arch5"
  "blx%?\\t%1"
  [(set_attr "type" "call")]
)

//Pattern#442
(define_insn "*call_value_reg_arm"
  [(set (match_operand 0 "" "")
        (call (mem:SI (match_operand:SI 1 "s_register_operand" "r"))
	      (match_operand 2 "" "")))
   (use (match_operand 3 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_ARM && !arm_arch5"
  "*
  return output_call (&operands[1]);
  "
  [(set_attr "length" "12")
   (set_attr "type" "call")]
)

;; Note: see *call_mem

//Pattern#443
(define_insn "*call_value_mem"
  [(set (match_operand 0 "" "")
	(call (mem:SI (match_operand:SI 1 "call_memory_operand" "m"))
	      (match_operand 2 "" "")))
   (use (match_operand 3 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_ARM && !arm_arch5 && (!CONSTANT_ADDRESS_P (XEXP (operands[1], 0)))"
  "*
  return output_call_mem (&operands[1]);
  "
  [(set_attr "length" "12")
   (set_attr "type" "call")]
)

//Pattern#444
(define_insn "*call_value_reg_thumb1_v5"
  [(set (match_operand 0 "" "")
	(call (mem:SI (match_operand:SI 1 "register_operand" "l*r"))
	      (match_operand 2 "" "")))
   (use (match_operand 3 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_THUMB1 && arm_arch5"
  "blx\\t%1"
  [(set_attr "length" "2")
   (set_attr "type" "call")]
)

//Pattern#445
(define_insn "*call_value_reg_thumb1"
  [(set (match_operand 0 "" "")
	(call (mem:SI (match_operand:SI 1 "register_operand" "l*r"))
	      (match_operand 2 "" "")))
   (use (match_operand 3 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_THUMB1 && !arm_arch5"
  "*
  {
    if (!TARGET_CALLER_INTERWORKING)
      return thumb_call_via_reg (operands[1]);
    else if (operands[2] == const0_rtx)
      return \"bl\\t%__interwork_call_via_%1\";
    else if (frame_pointer_needed)
      return \"bl\\t%__interwork_r7_call_via_%1\";
    else
      return \"bl\\t%__interwork_r11_call_via_%1\";
  }"
  [(set_attr "type" "call")]
)

;; Allow calls to SYMBOL_REFs specially as they are not valid general addresses
;; The 'a' causes the operand to be treated as an address, i.e. no '#' output.

//Pattern#446
(define_insn "*call_symbol"
  [(call (mem:SI (match_operand:SI 0 "" ""))
	 (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_32BIT
   && (GET_CODE (operands[0]) == SYMBOL_REF)
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0]))"
  "*
  {
    return NEED_PLT_RELOC ? \"bl%?\\t%a0(PLT)\" : \"bl%?\\t%a0\";
  }"
  [(set_attr "type" "call")]
)

//Pattern#447
(define_insn "*call_value_symbol"
  [(set (match_operand 0 "" "")
	(call (mem:SI (match_operand:SI 1 "" ""))
	(match_operand:SI 2 "" "")))
   (use (match_operand 3 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_32BIT
   && (GET_CODE (operands[1]) == SYMBOL_REF)
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1]))"
  "*
  {
    return NEED_PLT_RELOC ? \"bl%?\\t%a1(PLT)\" : \"bl%?\\t%a1\";
  }"
  [(set_attr "type" "call")]
)

//Pattern#448
(define_insn "*call_insn"
  [(call (mem:SI (match_operand:SI 0 "" ""))
	 (match_operand:SI 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_THUMB1
   && GET_CODE (operands[0]) == SYMBOL_REF
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0]))"
  "bl\\t%a0"
  [(set_attr "length" "4")
   (set_attr "type" "call")]
)

//Pattern#449
(define_insn "*call_value_insn"
  [(set (match_operand 0 "" "")
	(call (mem:SI (match_operand 1 "" ""))
	      (match_operand 2 "" "")))
   (use (match_operand 3 "" ""))
   (clobber (reg:SI LR_REGNUM))]
  "TARGET_THUMB1
   && GET_CODE (operands[1]) == SYMBOL_REF
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1]))"
  "bl\\t%a1"
  [(set_attr "length" "4")
   (set_attr "type" "call")]
)

;; We may also be able to do sibcalls for Thumb, but it's much harder...
//Pattern#450
(define_expand "sibcall"
  [(parallel [(call (match_operand 0 "memory_operand" "")
		    (match_operand 1 "general_operand" ""))
	      (return)
	      (use (match_operand 2 "" ""))])]
  "TARGET_32BIT"
  "
  {
    if (operands[2] == NULL_RTX)
      operands[2] = const0_rtx;
  }"
)

//Pattern#451
(define_expand "sibcall_value"
  [(parallel [(set (match_operand 0 "" "")
		   (call (match_operand 1 "memory_operand" "")
			 (match_operand 2 "general_operand" "")))
	      (return)
	      (use (match_operand 3 "" ""))])]
  "TARGET_32BIT"
  "
  {
    if (operands[3] == NULL_RTX)
      operands[3] = const0_rtx;
  }"
)

//Pattern#452
(define_insn "*sibcall_insn"
 [(call (mem:SI (match_operand:SI 0 "" "X"))
	(match_operand 1 "" ""))
  (return)
  (use (match_operand 2 "" ""))]
  "TARGET_32BIT && GET_CODE (operands[0]) == SYMBOL_REF"
  "*
  return NEED_PLT_RELOC ? \"b%?\\t%a0(PLT)\" : \"b%?\\t%a0\";
  "
  [(set_attr "type" "call")]
)

//Pattern#453
(define_insn "*sibcall_value_insn"
 [(set (match_operand 0 "" "")
       (call (mem:SI (match_operand:SI 1 "" "X"))
	     (match_operand 2 "" "")))
  (return)
  (use (match_operand 3 "" ""))]
  "TARGET_32BIT && GET_CODE (operands[1]) == SYMBOL_REF"
  "*
  return NEED_PLT_RELOC ? \"b%?\\t%a1(PLT)\" : \"b%?\\t%a1\";
  "
  [(set_attr "type" "call")]
)

//Pattern#454
(define_expand "return"
  [(return)]
  "(TARGET_ARM || (TARGET_THUMB2
                   && ARM_FUNC_TYPE (arm_current_func_type ()) == ARM_FT_NORMAL
                   && !IS_STACKALIGN (arm_current_func_type ())))
    && USE_RETURN_INSN (FALSE)"
  "
  {
    if (TARGET_THUMB2)
      {
        thumb2_expand_return ();
        DONE;
      }
  }
  "
)

;; Often the return insn will be the same as loading from memory, so set attr
//Pattern#455
(define_insn "*arm_return"
  [(return)]
  "TARGET_ARM && USE_RETURN_INSN (FALSE)"
  "*
  {
    if (arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return \"\";
      }
    return output_return_instruction (const_true_rtx, true, false, false);
  }"
  [(set_attr "type" "load1")
   (set_attr "length" "12")
   (set_attr "predicable" "yes")]
)

//Pattern#456
(define_insn "*cond_return"
  [(set (pc)
        (if_then_else (match_operator 0 "arm_comparison_operator"
		       [(match_operand 1 "cc_register" "") (const_int 0)])
                      (return)
                      (pc)))]
  "TARGET_ARM && USE_RETURN_INSN (TRUE)"
  "*
  {
    if (arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return \"\";
      }
    return output_return_instruction (operands[0], true, false, false);
  }"
  [(set_attr "conds" "use")
   (set_attr "length" "12")
   (set_attr "type" "load1")]
)

//Pattern#457
(define_insn "*cond_return_inverted"
  [(set (pc)
        (if_then_else (match_operator 0 "arm_comparison_operator"
		       [(match_operand 1 "cc_register" "") (const_int 0)])
                      (pc)
		      (return)))]
  "TARGET_ARM && USE_RETURN_INSN (TRUE)"
  "*
  {
    if (arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return \"\";
      }
    return output_return_instruction (operands[0], true, true, false);
  }"
  [(set_attr "conds" "use")
   (set_attr "length" "12")
   (set_attr "type" "load1")]
)

//Pattern#458
(define_insn "*arm_simple_return"
  [(simple_return)]
  "TARGET_ARM"
  "*
  {
    if (arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return \"\";
      }
    return output_return_instruction (const_true_rtx, true, false, true);
  }"
  [(set_attr "type" "branch")
   (set_attr "length" "4")
   (set_attr "predicable" "yes")]
)

;; Generate a sequence of instructions to determine if the processor is
;; in 26-bit or 32-bit mode, and return the appropriate return address
;; mask.

//Pattern#459
(define_expand "return_addr_mask"
  [(set (match_dup 1)
      (compare:CC_NOOV (unspec [(const_int 0)] UNSPEC_CHECK_ARCH)
		       (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "")
      (if_then_else:SI (eq (match_dup 1) (const_int 0))
		       (const_int -1)
		       (const_int 67108860)))] ; 0x03fffffc
  "TARGET_ARM"
  "
  operands[1] = gen_rtx_REG (CC_NOOVmode, CC_REGNUM);
  ")

//Pattern#460
(define_insn "*check_arch2"
  [(set (match_operand:CC_NOOV 0 "cc_register" "")
      (compare:CC_NOOV (unspec [(const_int 0)] UNSPEC_CHECK_ARCH)
		       (const_int 0)))]
  "TARGET_ARM"
  "teq\\t%|r0, %|r0\;teq\\t%|pc, %|pc"
  [(set_attr "length" "8")
   (set_attr "conds" "set")]
)

;; Call subroutine returning any type.

//Pattern#461
(define_expand "untyped_call"
  [(parallel [(call (match_operand 0 "" "")
		    (const_int 0))
	      (match_operand 1 "" "")
	      (match_operand 2 "" "")])]
  "TARGET_EITHER"
  "
  {
    int i;
    rtx par = gen_rtx_PARALLEL (VOIDmode,
				rtvec_alloc (XVECLEN (operands[2], 0)));
    rtx addr = gen_reg_rtx (Pmode);
    rtx mem;
    int size = 0;

    emit_move_insn (addr, XEXP (operands[1], 0));
    mem = change_address (operands[1], BLKmode, addr);

    for (i = 0; i < XVECLEN (operands[2], 0); i++)
      {
	rtx src = SET_SRC (XVECEXP (operands[2], 0, i));

	/* Default code only uses r0 as a return value, but we could
	   be using anything up to 4 registers.  */
	if (REGNO (src) == R0_REGNUM)
	  src = gen_rtx_REG (TImode, R0_REGNUM);

        XVECEXP (par, 0, i) = gen_rtx_EXPR_LIST (VOIDmode, src,
						 GEN_INT (size));
        size += GET_MODE_SIZE (GET_MODE (src));
      }

    emit_call_insn (GEN_CALL_VALUE (par, operands[0], const0_rtx, NULL,
				    const0_rtx));

    size = 0;

    for (i = 0; i < XVECLEN (par, 0); i++)
      {
	HOST_WIDE_INT offset = 0;
	rtx reg = XEXP (XVECEXP (par, 0, i), 0);

	if (size != 0)
	  emit_move_insn (addr, plus_constant (Pmode, addr, size));

	mem = change_address (mem, GET_MODE (reg), NULL);
	if (REGNO (reg) == R0_REGNUM)
	  {
	    /* On thumb we have to use a write-back instruction.  */
	    emit_insn (arm_gen_store_multiple (arm_regs_in_sequence, 4, addr,
 		       TARGET_THUMB ? TRUE : FALSE, mem, &offset));
	    size = TARGET_ARM ? 16 : 0;
	  }
	else
	  {
	    emit_move_insn (mem, reg);
	    size = GET_MODE_SIZE (GET_MODE (reg));
	  }
      }

    /* The optimizer does not know that the call sets the function value
       registers we stored in the result block.  We avoid problems by
       claiming that all hard registers are used and clobbered at this
       point.  */
    emit_insn (gen_blockage ());

    DONE;
  }"
)

//Pattern#462
(define_expand "untyped_return"
  [(match_operand:BLK 0 "memory_operand" "")
   (match_operand 1 "" "")]
  "TARGET_EITHER"
  "
  {
    int i;
    rtx addr = gen_reg_rtx (Pmode);
    rtx mem;
    int size = 0;

    emit_move_insn (addr, XEXP (operands[0], 0));
    mem = change_address (operands[0], BLKmode, addr);

    for (i = 0; i < XVECLEN (operands[1], 0); i++)
      {
	HOST_WIDE_INT offset = 0;
	rtx reg = SET_DEST (XVECEXP (operands[1], 0, i));

	if (size != 0)
	  emit_move_insn (addr, plus_constant (Pmode, addr, size));

	mem = change_address (mem, GET_MODE (reg), NULL);
	if (REGNO (reg) == R0_REGNUM)
	  {
	    /* On thumb we have to use a write-back instruction.  */
	    emit_insn (arm_gen_load_multiple (arm_regs_in_sequence, 4, addr,
 		       TARGET_THUMB ? TRUE : FALSE, mem, &offset));
	    size = TARGET_ARM ? 16 : 0;
	  }
	else
	  {
	    emit_move_insn (reg, mem);
	    size = GET_MODE_SIZE (GET_MODE (reg));
	  }
      }

    /* Emit USE insns before the return.  */
    for (i = 0; i < XVECLEN (operands[1], 0); i++)
      emit_use (SET_DEST (XVECEXP (operands[1], 0, i)));

    /* Construct the return.  */
    expand_naked_return ();

    DONE;
  }"
)

;; UNSPEC_VOLATILE is considered to use and clobber all hard registers and
;; all of memory.  This blocks insns from being moved across this point.

//Pattern#463
(define_insn "blockage"
  [(unspec_volatile [(const_int 0)] VUNSPEC_BLOCKAGE)]
  "TARGET_EITHER"
  ""
  [(set_attr "length" "0")
   (set_attr "type" "block")]
)

//Pattern#464
(define_expand "casesi"
  [(match_operand:SI 0 "s_register_operand" "")	; index to jump on
   (match_operand:SI 1 "const_int_operand" "")	; lower bound
   (match_operand:SI 2 "const_int_operand" "")	; total range
   (match_operand:SI 3 "" "")			; table label
   (match_operand:SI 4 "" "")]			; Out of range label
  "TARGET_32BIT || optimize_size || flag_pic"
  "
  {
    enum insn_code code;
    if (operands[1] != const0_rtx)
      {
	rtx reg = gen_reg_rtx (SImode);

	emit_insn (gen_addsi3 (reg, operands[0],
			       gen_int_mode (-INTVAL (operands[1]),
			       		     SImode)));
	operands[0] = reg;
      }

    if (TARGET_ARM)
      code = CODE_FOR_arm_casesi_internal;
    else if (TARGET_THUMB1)
      code = CODE_FOR_thumb1_casesi_internal_pic;
    else if (flag_pic)
      code = CODE_FOR_thumb2_casesi_internal_pic;
    else
      code = CODE_FOR_thumb2_casesi_internal;

    if (!insn_data[(int) code].operand[1].predicate(operands[2], SImode))
      operands[2] = force_reg (SImode, operands[2]);

    emit_jump_insn (GEN_FCN ((int) code) (operands[0], operands[2],
					  operands[3], operands[4]));
    DONE;
  }"
)

;; The USE in this pattern is needed to tell flow analysis that this is
;; a CASESI insn.  It has no other purpose.
//Pattern#465
(define_insn "arm_casesi_internal"
  [(parallel [(set (pc)
	       (if_then_else
		(leu (match_operand:SI 0 "s_register_operand" "r")
		     (match_operand:SI 1 "arm_rhs_operand" "rI"))
		(mem:SI (plus:SI (mult:SI (match_dup 0) (const_int 4))
				 (label_ref (match_operand 2 "" ""))))
		(label_ref (match_operand 3 "" ""))))
	      (clobber (reg:CC CC_REGNUM))
	      (use (label_ref (match_dup 2)))])]
  "TARGET_ARM"
  "*
    if (flag_pic)
      return \"cmp\\t%0, %1\;addls\\t%|pc, %|pc, %0, asl #2\;b\\t%l3\";
    return   \"cmp\\t%0, %1\;ldrls\\t%|pc, [%|pc, %0, asl #2]\;b\\t%l3\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "12")]
)

//Pattern#466
(define_expand "thumb1_casesi_internal_pic"
  [(match_operand:SI 0 "s_register_operand" "")
   (match_operand:SI 1 "thumb1_cmp_operand" "")
   (match_operand 2 "" "")
   (match_operand 3 "" "")]
  "TARGET_THUMB1"
  {
    rtx reg0;
    rtx test = gen_rtx_GTU (VOIDmode, operands[0], operands[1]);
    emit_jump_insn (gen_cbranchsi4 (test, operands[0], operands[1],
				    operands[3]));
    reg0 = gen_rtx_REG (SImode, 0);
    emit_move_insn (reg0, operands[0]);
    emit_jump_insn (gen_thumb1_casesi_dispatch (operands[2]/*, operands[3]*/));
    DONE;
  }
)

//Pattern#467
(define_insn "thumb1_casesi_dispatch"
  [(parallel [(set (pc) (unspec [(reg:SI 0)
				 (label_ref (match_operand 0 "" ""))
;;				 (label_ref (match_operand 1 "" ""))
]
			 UNSPEC_THUMB1_CASESI))
	      (clobber (reg:SI IP_REGNUM))
              (clobber (reg:SI LR_REGNUM))])]
  "TARGET_THUMB1"
  "* return thumb1_output_casesi(operands);"
  [(set_attr "length" "4")]
)

//Pattern#468
(define_expand "indirect_jump"
  [(set (pc)
	(match_operand:SI 0 "s_register_operand" ""))]
  "TARGET_EITHER"
  "
  /* Thumb-2 doesn't have mov pc, reg.  Explicitly set the low bit of the
     address and use bx.  */
  if (TARGET_THUMB2)
    {
      rtx tmp;
      tmp = gen_reg_rtx (SImode);
      emit_insn (gen_iorsi3 (tmp, operands[0], GEN_INT(1)));
      operands[0] = tmp;
    }
  "
)

;; NB Never uses BX.
//Pattern#469
(define_insn "*arm_indirect_jump"
  [(set (pc)
	(match_operand:SI 0 "s_register_operand" "r"))]
  "TARGET_ARM"
  "mov%?\\t%|pc, %0\\t%@ indirect register jump"
  [(set_attr "predicable" "yes")]
)

//Pattern#470
(define_insn "*load_indirect_jump"
  [(set (pc)
	(match_operand:SI 0 "memory_operand" "m"))]
  "TARGET_ARM"
  "ldr%?\\t%|pc, %0\\t%@ indirect memory jump"
  [(set_attr "type" "load1")
   (set_attr "pool_range" "4096")
   (set_attr "neg_pool_range" "4084")
   (set_attr "predicable" "yes")]
)

;; NB Never uses BX.
//Pattern#471
(define_insn "*thumb1_indirect_jump"
  [(set (pc)
	(match_operand:SI 0 "register_operand" "l*r"))]
  "TARGET_THUMB1"
  "mov\\tpc, %0"
  [(set_attr "conds" "clob")
   (set_attr "length" "2")]
)


;; Misc insns

