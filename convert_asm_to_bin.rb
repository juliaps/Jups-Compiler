##################################################################################################
#######                                 File Description                                 #########
##################################################################################################
#
# filename:      convert_asm_to_bin.rb
# created:       25/06/2019
# company:       Federal University of São Paulo
# developers:    Júlia P.S.
# utility:       Used to convert assembly code to binary code.
#
##################################################################################################
#######                               End File Description                               #########
##################################################################################################

def dec_to_bin(num)
  n = num.to_i
  return "%b" % n
end

def zeros_complete(num, qt)

  while num.length < qt
    num = "0" + num
  end
  return num
end

def inst_complete(num)

  while num.length < 32
    num = num + "0"
  end
  return num
end

labels = {}
count_line = 0
File.open("assembly.txt").each do |line|
  text = line.split(" ")
  if(((text).length == 1 )and (text != "halt"))
    labels.merge!("#{text[0]}"=> count_line)
  elsif(text[0] == "label")
    labels.merge!("#{text[1]}"=> count_line)
  else
    count_line = count_line + 1
  end
end

count_line = 0
f_print = 0

regs = {
  "r0" =>0,
  "t0" =>1,
  "t1" =>2,
  "t2" =>3,
  "t3" =>4,
  "t4" =>5,
  "t5" =>6,
  "t6" =>7,
  "t7" =>8,
  "t8" =>9,
  "t9" =>10,
  "r1" =>11,
  "r2" =>12,
  "r3" =>13,
  "r4" =>14,
  "r5" =>15,
  "r6" =>16,
  "r7" =>17,
  "r8" =>18,
  "r9" =>19,
  "r10" =>20,
  "r11" =>21,
  "r12" =>22,
  "r13" =>23,
  "r14" =>24,
  "r15" =>25,
  "r16" =>26,
  "r27" =>27,
  "r28" =>28,
  "r29" =>29,
  "r30" =>30,
  "r31" =>31
}

File.open("assembly.txt").each do |line|
  text = line.split(" ")
  op_inst = text[0]
  op1 = text[1]
  op2 = text[2]
  op3 = text[3]
  case op_inst
  when "add"
      begin
        opcode = "000010"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        sb = zeros_complete(dec_to_bin(regs[op3]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "sub"
      begin
        opcode = "000011"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        sb = zeros_complete(dec_to_bin(regs[op3]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "mult"
      begin
        opcode = "000100"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        sb = zeros_complete(dec_to_bin(regs[op3]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "div"
      begin
        opcode = "000101"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        sb = zeros_complete(dec_to_bin(regs[op3]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "addi"
      begin
        opcode = "010100"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "loadi"
      begin
        opcode = "010110"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(0), 5)
        im = zeros_complete(dec_to_bin(op2),16)
        type_inst = "i"
        f_print = 1
      end
    when "subi"
      begin
        opcode = "010101"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "j"
      begin
        opcode = "001110"
        endr = labels[op1].to_i + 1
        dr = zeros_complete(dec_to_bin(0), 5)
        sa = zeros_complete(dec_to_bin(0), 5)
        im = zeros_complete(dec_to_bin(endr),16)
        type_inst = "i"
        f_print = 1
      end
    when "load" # r1 r2 0 => r[r1] = m[r[r2]+ 0]
      begin
        opcode = "000111"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "store" # r1 r2 0 => m[r[r2]+ 0] = r[r1] 
      begin
        opcode = "001000"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "jr"
      begin
        opcode = "001101"
        dr = zeros_complete(dec_to_bin(0), 5)
        sa = zeros_complete(dec_to_bin(0), 5)
        sb = zeros_complete(dec_to_bin(regs[op1]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "jal"
      begin
        opcode = "001111"
        endr = labels[op1].to_i + 1
        dr = zeros_complete(dec_to_bin(0), 5)
        sa = zeros_complete(dec_to_bin(0), 5)
        im = zeros_complete(dec_to_bin(endr),16)
        type_inst = "i"
        f_print = 1
      end
    when "beq"
      begin
        opcode = "010000"
        endr = labels[op3].to_i + 1
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        sb = zeros_complete(dec_to_bin(endr),16)
        type_inst = "r"
        f_print = 1
      end
    when "slt"
      begin
        opcode = "010010"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        sb = zeros_complete(dec_to_bin(regs[op3]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "sgt"
      begin
        opcode = "010011"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        sb = zeros_complete(dec_to_bin(regs[op3]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "bneq"
      begin
        opcode = "010001"
        endr = labels[op3].to_i + 1
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        sb = zeros_complete(dec_to_bin(endr),16)
        type_inst = "r"
        f_print = 1
      end
    when "in"
      begin
        opcode = "001011"#{ok}
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(0), 5)
        im = zeros_complete(dec_to_bin(0), 16)
        type_inst = "i"
        f_print = 1
      end
    when "out"
      begin
        opcode = "001100"
        dr = zeros_complete(dec_to_bin(0), 5)
        sa = zeros_complete(dec_to_bin(regs[op1]), 5)
        sb = zeros_complete(dec_to_bin(0), 5)
        type_inst = "r"
        f_print = 1
      end
    when "pop"
      begin
        opcode = "001010"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "push"
      begin
        opcode = "001001"
        dr = zeros_complete(dec_to_bin(regs[op1]), 5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "halt"
      begin
        opcode = "000001"
        dr = zeros_complete(dec_to_bin(0),5)
        sa = zeros_complete(dec_to_bin(0),5)
        sb = zeros_complete(dec_to_bin(0),5)
        type_inst = "r"
        f_print = 1
      end
    when "nop"
      begin
        opcode = "000000"
        dr = zeros_complete(dec_to_bin(0),5)
        sa = zeros_complete(dec_to_bin(0),5)
        sb = zeros_complete(dec_to_bin(0),5)
        type_inst = "r"
        f_print = 1
      end
    when "mov"
      begin
        opcode = "000110"
        dr = zeros_complete(dec_to_bin(regs[op1]),5)
        sa = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(0),16)
        type_inst = "i"
        f_print = 1
      end
    else
      puts "//#{op_inst} #{op1}" #unless op_inst == "label"
      f_print = 0
    end
     if(type_inst == "r")
       bin = "#{opcode}#{sa}#{sb}#{dr}" # ok
     elsif (type_inst == "i")
       bin = "#{opcode}#{sa}#{dr}#{im}" # ok
     end

     unless f_print == 0
       i_bin = inst_complete(bin)
       instruction = "InstMem[#{count_line}] <= 32'b#{i_bin}; //#{text.join(" ")}"
       puts instruction
       count_line = count_line +1
     end
end
