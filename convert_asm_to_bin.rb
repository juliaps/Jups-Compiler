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
        opcode = "000000"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(regs[op3]), 5)
        funct = "000000"
        type_inst = "r"
        f_print = 1
      end
    when "sub"
      begin
        opcode = "000000"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(regs[op3]), 5)
        funct = "000001"
        type_inst = "r"
        f_print = 1
      end
    when "mult"
      begin
        opcode = "000000"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(regs[op3]), 5)
        funct = "000010"
        type_inst = "r"
        f_print = 1
      end
    when "div"
      begin
        opcode = "000000"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(regs[op3]), 5)
        funct = "000011"
        type_inst = "r"
        f_print = 1
      end
    when "inc"
      begin
        opcode = "000000"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(1), 5)
        funct = "000100"
        type_inst = "r"
        f_print = 1
      end
    when "dec"
      begin
        opcode = "000000"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(1), 5)
        funct = "000101"
        type_inst = "r"
        f_print = 1
      end
    when "addi"
      begin
        opcode = "000010"
        rt = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "loadi" # rever se tbm o imediato pra somar com o endr
      begin
        opcode = "010110"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(0), 5)
        im = zeros_complete(dec_to_bin(op2),16)
        type_inst = "i"
        f_print = 1
      end
    when "subi"
      begin
        opcode = "010100"
        rt = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "j"
      begin
        opcode = "000101"
        endr = labels[op1].to_i
        rd = zeros_complete(dec_to_bin(0), 10)
        rs = zeros_complete(dec_to_bin(endr),16)
        type_inst = "j"
        f_print = 1
      end
    when "lw"
      begin
        opcode = "000110"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "sw"
      begin # r1 r2 5
        opcode = "000111"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "jr"
      begin
        opcode = "010011"
        rd = zeros_complete(dec_to_bin(0), 5)
        rs = zeros_complete(dec_to_bin(regs[op1]), 5)
        rt = zeros_complete(dec_to_bin(0), 5)
        type_inst = "r"
        f_print = 1
      end
    when "jal"
      begin
        opcode = "001111"
        endr = labels[op1].to_i + 1
        rd = zeros_complete(dec_to_bin(0), 10)
        rs = zeros_complete(dec_to_bin(endr),16)
        type_inst = "j"
        f_print = 1
      end
    when "beq"
      begin
        opcode = "010000"
        endr = labels[op3].to_i + 1
        rt = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(op3), 16)
        type_inst = "i"
        f_print = 1
      end
    when "slt"
      begin
        opcode = "010010"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(regs[op3]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "sgt"
      begin
        opcode = "010011"
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(regs[op3]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "bneq"
      begin
        opcode = "010001"
        endr = labels[op3].to_i + 1
        rd = zeros_complete(dec_to_bin(regs[op1]), 5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        im = zeros_complete(dec_to_bin(endr),16)
        type_inst = "i"
        f_print = 1
      end
    when "in"
      begin
        opcode = "001011"#{ok}
        rd = zeros_complete(dec_to_bin(0),5)
        rs = zeros_complete(dec_to_bin(0),5)
        rt = zeros_complete(dec_to_bin(regs[op1]), 5)
        type_inst = "r"
        f_print = 1
      end
    when "out"
      begin
        opcode = "001100"
        rd = zeros_complete(dec_to_bin(regs[op1]),5)
        rs = zeros_complete(dec_to_bin(0), 5)
        rt = zeros_complete(dec_to_bin(0),5)
        type_inst = "r"
        f_print = 1
      end
    when "pop"
      begin # pop r2 sp 0
        opcode = "001010"
        rd = zeros_complete(dec_to_bin(regs[op1]),5)
        rs = zeros_complete(dec_to_bin(regs[op2]),5)
        im = zeros_complete(dec_to_bin(0),16)
        type_inst = "r"
        f_print = 1
      end
    when "push"
      begin # push r1 sp 0
        opcode = "001001"
        rd = zeros_complete(dec_to_bin(regs[op1]),5)
        rs = zeros_complete(dec_to_bin(regs[op2]),5)
        im = zeros_complete(dec_to_bin(0),16)
        type_inst = "i"
        f_print = 1
      end
    when "halt"
      begin
        opcode = "000001"
        rd = zeros_complete(dec_to_bin(0),5)
        rs = zeros_complete(dec_to_bin(0),5)
        rt = zeros_complete(dec_to_bin(0),5)
        type_inst = "r"
        f_print = 1
      end
    when "nop"
      begin
        opcode = "000000"
        rd = zeros_complete(dec_to_bin(0),5)
        rs = zeros_complete(dec_to_bin(0),5)
        rt = zeros_complete(dec_to_bin(0),5)
        type_inst = "r"
        f_print = 1
      end
    when "mov"
      begin
        opcode = "000110"
        rd = zeros_complete(dec_to_bin(regs[op1]),5)
        rs = zeros_complete(dec_to_bin(regs[op2]), 5)
        rt = zeros_complete(dec_to_bin(0),5)
        type_inst = "r"
        f_print = 1
      end
    else
      puts "//#{op_inst} #{op1}" #unless op_inst == "label"
      f_print = 0
    end
     if(type_inst == "r")
       bin = "#{opcode}#{rs}#{rt}#{rd}"
     elsif (type_inst == "i")
       bin = "#{opcode}#{rs}#{rt}#{im}"
     elsif (type_inst == "j")
       bin = "#{opcode}#{im}"
     end

     unless f_print == 0
       i_bin = inst_complete(bin)
       instruction = "InstMem[#{count_line}] = 32'b#{i_bin}; //#{text.join(" ")}"
       puts instruction
       count_line = count_line +1
     end
end
