main
   loadi r2 0
   store r2 r0 89 
   loadi r3 1
   store r3 r0 88 
   loadi r4 2
   store r4 r0 86 
   loadi r5 8
   store r5 r0 87 
label L1
   load r5 r0 89 
   load r6 r0 87 
   slt t0 r5 r6 
   beq t0 r0 L2
   load r7 r0 86 
   load r8 r0 88 
   mult t1 r7 r8 
   store t1 r0 88 
   load r10 r0 89 
   loadi r11 1
   add t2 r10 r11 
   store t2 r0 89 
   j L1
label L2
   load r28 r0 88
   out r28
   halt_proc 
