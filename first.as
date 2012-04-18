   .entry MAIN
   .extern STR
K: cmp A, r3
   add K, r0
   bne STR
   not r4
   prn [COUNT]A[%K]
   jsr MAIN
STR: stop
MAIN: mov K[%STR], A
K: .data 7, 5, 2
COUNT: .data 0
A: .string "test"
