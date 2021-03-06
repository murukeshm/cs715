;; The dummy instruction has been supported to ensure that no empty
;; arrays are generated in source files of the generated compiler in the
;; initial levels..

(define_insn "dummy_pattern"
        [(reg:SI 0)]
        "1"
        "This stmnt should not be emitted!"
)

;; Functions in genemit.c read .md file for desired target and generates
;; gen_<insn-name> function  for each standard named  pattern defined in
;; .md file.  The function gen_<insn-name>  is in turn used  to generate
;; RTLs at the  time of transforming input program into  RTL. The source
;; files  cfgrtl.c,  cse.c,expmed.c which  contribute  in  cc1, use  the
;; function gen_jump to generate insn corresponding to jump instruction.
;; If this pattern is not  defined in machine description, the compiler,
;; fails in the linking phase because gen_jump is not defined.


;; For compiling _any_ program, jumps are a must.

(define_insn "jump"
        [(set (pc) (label_ref (match_operand 0 "" "")))]
        ""
	{
                return "j \\t%l0";
        }
)

(define_insn "indirect_jump"
	[(set (pc) (match_operand:SI 0 "register_operand" ""))]
	""
	"jr \\t%0"
)

(define_expand "epilogue"
        [(clobber (const_int 0))]
        ""
        {
                spim_epilogue();
                DONE;
        }
)

(define_insn "IITB_return"
	[(return)
	 (use (reg:SI 31))]
	""
	"jr \\t\\$ra"
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; This is the basic standard named pattern, which is matched at the time of 
;; GIMPLE to RTL conversion. Hence to add assignment operation in our machine
;; description, we have to define this standard pattern. We can have various
;; variants of this pattern depending upon which target instruction to emit,
;; and move patterns supported in the architecture. These variants can be
;; defined using define_insn patterns and constraints handling specific 
;; pattern.

;;movmisalignm is to be used for unaligned memory boundaries.

(define_insn "movqi"
	[(set (match_operand:QI 0 "nonimmediate_operand" "=r,m,r,r")
		(match_operand:QI 1 "general_operand" "m,r,i,r")
	)]
	""
	"@
	lw \\t%0, %m1
	sh \\t%1, %m0
	li \\t%0, %c1
        move \\t%0,%1"
)
(define_insn "movhi"
	[(set (match_operand:HI 0 "nonimmediate_operand" "=r,m,r,r")
		(match_operand:HI 1 "general_operand" "m,r,i,r")
	)]
	""
	"@
	lw \\t%0, %m1
	sh \\t%1, %m0
	li \\t%0, %c1
        move \\t%0,%1"
)
(define_insn "movsi"
	[(set (match_operand:SI 0 "nonimmediate_operand" "=r,m,r,r")
		(match_operand:SI 1 "general_operand" "m,r,i,r")
	)]
	""
	"@
	lw \\t%0, %m1
	sh \\t%1, %m0
	li \\t%0, %c1
        move \\t%0,%1"
)

;; Here z is the constraint character defined in REG_CLASS_FROM_LETTER_P
;; The register $zero is used here. 
(define_insn "IITB_move_zero"
	[(set (match_operand:SI 0 "nonimmediate_operand" "=r,m")
	      (match_operand:SI 1 "zero_register_operand" "z,z")
	)]
	""
	"@
	move \\t%0,%1
	sw \\t%1, %m0"
)

(define_insn "addsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
              (plus:SI (match_operand:SI 1 "register_operand" "r,r")
                       (match_operand:SI 2 "nonmemory_operand" "r,K"))
         )]
        ""
        "@
         add \\t%0, %1, %2
         addi \\t%0, %1, %c2"

)

(define_expand "prologue"
	[(clobber (const_int 0))]
	""
	{
		spim_prologue();
		DONE;
	}
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Arithmatic and logical operations
;;===================================

(define_insn "abssi2"
	[(set (match_operand:SI 0 "register_operand" "=r")
	      (abs:SI (match_operand:SI 1 "register_operand" "r")))]
	""
	"abs \\t%0, %1"
)

(define_insn "andsi3"
        [(set (match_operand:SI 0 "register_operand" "=r,r")
              (and:SI (match_operand:SI 1 "register_operand" "r,r")
                      (match_operand:SI 2 "nonmemory_operand" "r,K"))
         )]
        ""
        "@
         and \\t%0, %1, %2
         andi \\t%0, %1, %c2"
)

(define_insn "divsi3"
        [(set (match_operand:SI 0 "register_operand" "=r")
              (div:SI (match_operand:SI 1 "register_operand" "r")
                       (match_operand:SI 2 "register_operand" "r"))
         )]
        ""
        "div \\t%1, %2\\n\\tmflo \\t%0"
)

(define_insn "udivsi3"
        [(set (match_operand:SI 0 "register_operand" "=r,r")
              (udiv:SI (match_operand:SI 1 "register_operand" "r,r")
                       (match_operand:SI 2 "general_operand" "r,im"))
         )]
        ""
        "@
	divu \\t%1, %2\\n\\tmflo \\t%0
	divu \\t%0, %1, %2"
)

(define_insn "modsi3"
        [(set (match_operand:SI 0 "register_operand" "=r")
              (mod:SI (match_operand:SI 1 "register_operand" "r")
                       (match_operand:SI 2 "register_operand" "r"))
         )]
        ""
        "rem \\t%0, %1, %2"
)
                                                                                                    
(define_insn "umodsi3"
        [(set (match_operand:SI 0 "register_operand" "=r")
              (umod:SI (match_operand:SI 1 "register_operand" "r")
                       (match_operand:SI 2 "register_operand" "r"))
         )]
        ""
        "remu \\t%0, %1, %2"
)

(define_insn "negsi2"
        [(set (match_operand:SI 0 "register_operand" "=r")
              (neg:SI (match_operand:SI 1 "register_operand" "r"))
         )]
        ""
        "neg \\t%0, %1"
)

;;There is no standard pattern for NOR instruction, so currently omitting the pattern.

(define_insn "one_cmplsi2"
	[(set (match_operand:SI 0 "register_operand" "=r")
	      (not:SI (match_operand:SI 1 "register_operand" "r")))]
	""
	"not \\t%0, %1"
)

(define_insn "iorsi3"
        [(set (match_operand:SI 0 "register_operand" "=r,r")
              (ior:SI (match_operand:SI 1 "register_operand" "r,r")
                       (match_operand:SI 2 "nonmemory_operand" "r,K"))
         )]
        ""
        "@
	  or \\t%0, %1, %2
	  ori \\t%0, %1, %c2"
)

(define_insn "xorsi3"
        [(set (match_operand:SI 0 "register_operand" "=r,r")
              (xor:SI (match_operand:SI 1 "register_operand" "r,r")
                       (match_operand:SI 2 "nonmemory_operand" "r,K"))
         )]
        ""
        "@
          xor \\t%0, %1, %2
          xori \\t%0, %1, %c2"
)

(define_insn "subsi3"
        [(set (match_operand:SI 0 "register_operand" "=r")
              (minus:SI (match_operand:SI 1 "register_operand" "r")
                       (match_operand:SI 2 "register_operand" "r"))
         )]
        ""
        "sub \\t%0, %1, %2"
)

(define_insn "ashlsi3"
        [(set (match_operand:SI 0 "register_operand" "=r,r")
              (ashift:SI (match_operand:SI 1 "register_operand" "r,r")
                       (match_operand:SI 2 "nonmemory_operand" "r,J"))
         )]
        ""
       "@
	 sllv \\t%0, %1, %2
	 sll \\t%0, %1, %c2"
)

(define_insn "ashrsi3"
        [(set (match_operand:SI 0 "register_operand" "=r,r")
              (ashiftrt:SI (match_operand:SI 1 "register_operand" "r,r")
                       (match_operand:SI 2 "nonmemory_operand" "r,J"))
         )]
        ""
        "@
         srav \\t%0, %1, %2
         sra \\t%0, %1, %c2"
)

(define_insn "lshrsi3"
        [(set (match_operand:SI 0 "register_operand" "=r,r")
              (lshiftrt:SI (match_operand:SI 1 "register_operand" "r,r")
                       (match_operand:SI 2 "nonmemory_operand" "r,J"))
         )]
        ""
        "@
         srlv \\t%0, %1, %2
         srl \\t%0, %1, %c2"
)

;;isha added
(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
)

