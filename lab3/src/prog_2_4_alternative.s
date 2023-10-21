;; 87 Cycles
;; 39 Instructions
;; 2.231 Cycles Per Instruction (CPI)

;; 60 RAW Stalls
;; 9 Structural Stalls
;; 2 Branch Taken Stalls

;; Performance of this program in not necessarily better
;; than the alternative, because in both programs we have
;; that all the instructions are useful. Therefore, the
;; increased CPI in this version is worse. Even though we
;; get less instructions, the Cycles don't decrease that much,
;; which is most likely caused by the increase in RAW stalls.

        .data
A:      .word   1, 3, 1, 6, 4
        .word   2, 4, 3, 9, 5
mult:   .word   0

        .code
        daddi   $1, $0, A      ; *A[0]
        lw      $9, 0($1)      ; $9 = A[0]  ;; mult
        lw      $12, 8($1)     ; $12 = A[i=1]
        daddi   $6, $0, 72     ; $6 = 72 (limit ignoring $5)

loop:   dmul    $12, $12, $9   ; $12 = A[i]*mult
        lw      $22, 16($1)    ; $22 = A[i+1]
        lw      $23, 24($1)    ; $22 = A[i+2]
        dadd    $9, $9, $12    ; mult += A[i]*mult

        dmul    $22, $22, $9   ; $22 = A[i+1]*mult
        daddi   $1, $1, 24
        dadd    $9, $9, $22    ; mult += A[i+1]*mult

        dmul    $23, $23, $9   ; $12 = A[i+2]*mult
        lw      $12, 8($1)     ; $12 = A[i for new cycle]
        dadd    $9, $9, $23    ; mult += A[i+2]*mult

        bne     $6, $1, loop   ; Exit loop if offset == 72

        sw      $9, mult($0)   ; Store result
        halt                   ; Stop the program execution

;; Expected result: mult = f6180 (hex), 1008000 (dec)
