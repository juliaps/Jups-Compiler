main
   loadi r2 1
   store r2 r0 48 
   loadi r3 5
   store r3 r0 49  
   loadi r4 1
   store r4 r0 50 
label L1
   load r4 r0 50 
   load r5 r0 49 
   slt t0 r4 r5 
   beq t0 r0 L2
   load r6 r0 50 
   loadi r7 1
   add t1 r6 r7 
   store t1 r0 50 
   load r8 r0 48 
   load r9 r0 50 
   mult t2 r8 r9 
   store t2 r0 48 
   j L1
label L2
   load r28 r0 48
   out r28
   halt_proc 
