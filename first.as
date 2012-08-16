   .entry MAIN
   .extern STR
K: cmp A, r3
   add K, r0
   bne STR
   not r4
   prn [COUNT]A[r3]
   jsr MAIN
STR2: stop
MAIN: mov K[%STR2], A
R: .data 7, 5, 2
COUNT: .data 0
A: .string "test"
