.extern FF
.extern SEC

FF2: rts
LOOP: sub #10, r4
SEC2: lea LOOP[%GOOD], r0
GOOD: inc r5
      mov LOOP, r2
      stop
GOOD2: .string "second"
LOOP2: .data 6, 4, 78, 10
