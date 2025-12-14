.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text

read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

main:
  addiu $sp, $sp, -256
  sw $fp, 252($sp)
  sw $ra, 248($sp)
  move $fp, $sp
  li $t0, 10
  sw $t0, -4($fp)
  move $t1, $t0
  sw $t1, -8($fp)
  move $t0, $t1
  li $t2, 20
  sw $t2, -12($fp)
  move $t3, $t2
  sw $t3, -16($fp)
  move $t2, $t3
  li $t4, 0
  sw $t4, -20($fp)
  move $t5, $t4
  sw $t5, -24($fp)
  move $t4, $t5
  lw $t6, -4($fp)
  sw $t6, -28($fp)
  move $t7, $t6
  lw $t8, -8($fp)
  sw $t8, -32($fp)
  move $t9, $t8
  slt $t6, $t7, $t9
  sw $t6, -36($fp)
  bnez $t6, L1
  sw $t6, -36($fp)
  j L2
  sw $t0, -8($fp)
  sw $t1, -4($fp)
  sw $t2, -16($fp)
  sw $t3, -12($fp)
  sw $t4, -24($fp)
  sw $t5, -20($fp)
  sw $t7, -28($fp)
  sw $t9, -32($fp)
L1:
  li $t0, 1
  sw $t0, -40($fp)
  move $t1, $t0
  sw $t1, -44($fp)
  move $t0, $t1
  j L3
  sw $t0, -44($fp)
  sw $t1, -40($fp)
L2:
  li $t0, 2
  sw $t0, -48($fp)
  move $t1, $t0
  sw $t1, -52($fp)
  move $t0, $t1
  sw $t0, -52($fp)
  sw $t1, -48($fp)
L3:
  li $t0, 0
  sw $t0, -56($fp)
  move $t1, $t0
  sw $t1, -60($fp)
  move $t0, $t1
  sw $t0, -60($fp)
  sw $t1, -56($fp)
L4:
  lw $t0, -16($fp)
  sw $t0, -64($fp)
  move $t1, $t0
  li $t2, 10
  sw $t2, -68($fp)
  move $t3, $t2
  slt $t2, $t1, $t3
  sw $t2, -72($fp)
  bnez $t2, L5
  sw $t2, -72($fp)
  j L6
  sw $t1, -64($fp)
  sw $t3, -68($fp)
L5:
  lw $t0, -16($fp)
  sw $t0, -76($fp)
  move $t1, $t0
  sw $t1, -80($fp)
  move $t2, $t1
  li $t3, 1
  sw $t3, -84($fp)
  move $t4, $t3
  addu $t3, $t1, $t4
  sw $t3, -88($fp)
  sw $t1, -76($fp)
  sw $t4, -84($fp)
  j L4
  sw $t2, -80($fp)
  sw $t3, -88($fp)
L6:
  li $t0, 0
  sw $t0, -92($fp)
  move $t1, $t0
  sw $t1, -96($fp)
  move $t0, $t1
  j L7
  sw $t0, -96($fp)
  sw $t1, -92($fp)
L7:
  lw $t0, -20($fp)
  sw $t0, -100($fp)
  move $t1, $t0
  li $t2, 10
  sw $t2, -104($fp)
  move $t3, $t2
  slt $t2, $t1, $t3
  sw $t2, -108($fp)
  li $t4, 0
  xor $t5, $t2, $t4
  sltiu $t5, $t5, 1
  sw $t5, -112($fp)
  bnez $t5, L10
  sw $t5, -112($fp)
  sw $t1, -100($fp)
  sw $t2, -108($fp)
  sw $t3, -104($fp)
L8:
  lw $t0, -12($fp)
  sw $t0, -116($fp)
  move $t1, $t0
  sw $t1, -120($fp)
  move $t2, $t1
  lw $t3, -20($fp)
  sw $t3, -124($fp)
  move $t4, $t3
  addu $t5, $t1, $t4
  sw $t5, -128($fp)
  sw $t1, -116($fp)
  sw $t4, -124($fp)
  sw $t2, -120($fp)
  sw $t5, -128($fp)
L9:
  lw $t0, -20($fp)
  sw $t0, -132($fp)
  move $t1, $t0
  sw $t1, -136($fp)
  move $t2, $t1
  li $t3, 1
  sw $t3, -140($fp)
  move $t4, $t3
  addu $t3, $t1, $t4
  sw $t3, -144($fp)
  sw $t1, -132($fp)
  sw $t4, -140($fp)
  j L7
  sw $t2, -136($fp)
  sw $t3, -144($fp)
L10:
  li $t0, 0
  sw $t0, -148($fp)
  move $t1, $t0
  sw $t1, -152($fp)
  move $t0, $t1
  j L11
  sw $t0, -152($fp)
  sw $t1, -148($fp)
L11:
  lw $t0, -24($fp)
  sw $t0, -156($fp)
  move $t1, $t0
  li $t2, 10
  sw $t2, -160($fp)
  move $t3, $t2
  slt $t2, $t1, $t3
  sw $t2, -164($fp)
  li $t4, 0
  xor $t5, $t2, $t4
  sltiu $t5, $t5, 1
  sw $t5, -168($fp)
  bnez $t5, L14
  sw $t5, -168($fp)
  sw $t1, -156($fp)
  sw $t2, -164($fp)
  sw $t3, -160($fp)
L12:
  lw $t0, -24($fp)
  sw $t0, -172($fp)
  move $t1, $t0
  li $t2, 5
  sw $t2, -176($fp)
  move $t3, $t2
  xor $t2, $t1, $t3
  sltiu $t2, $t2, 1
  sw $t2, -180($fp)
  bnez $t2, L15
  sw $t2, -180($fp)
  j L16
  sw $t1, -172($fp)
  sw $t3, -176($fp)
L15:
  j L14
L16:
  lw $t0, -24($fp)
  sw $t0, -184($fp)
  move $t1, $t0
  li $t2, 2
  sw $t2, -188($fp)
  move $t3, $t2
  div $t1, $t3
  mflo $t2
  sw $t2, -192($fp)
  sw $t1, -184($fp)
  sw $t3, -188($fp)
  lw $t1, -188($fp)
  mul $t3, $t2, $t1
  sw $t3, -196($fp)
  sw $t2, -192($fp)
  lw $t1, -184($fp)
  subu $t2, $t1, $t3
  sw $t2, -200($fp)
  sw $t3, -196($fp)
  li $t1, 0
  sw $t1, -204($fp)
  move $t3, $t1
  xor $t1, $t2, $t3
  sltiu $t1, $t1, 1
  sw $t1, -208($fp)
  bnez $t1, L17
  sw $t1, -208($fp)
  j L18
  sw $t2, -200($fp)
  sw $t3, -204($fp)
L17:
  j L13
L18:
  lw $t0, -12($fp)
  sw $t0, -212($fp)
  move $t1, $t0
  sw $t1, -216($fp)
  move $t2, $t1
  lw $t3, -24($fp)
  sw $t3, -220($fp)
  move $t4, $t3
  addu $t5, $t1, $t4
  sw $t5, -224($fp)
  sw $t1, -212($fp)
  sw $t4, -220($fp)
  sw $t2, -216($fp)
  sw $t5, -224($fp)
L13:
  lw $t0, -24($fp)
  sw $t0, -228($fp)
  move $t1, $t0
  sw $t1, -232($fp)
  move $t2, $t1
  li $t3, 1
  sw $t3, -236($fp)
  move $t4, $t3
  addu $t3, $t1, $t4
  sw $t3, -240($fp)
  sw $t1, -228($fp)
  sw $t4, -236($fp)
  j L11
  sw $t2, -232($fp)
  sw $t3, -240($fp)
L14:
  lw $t0, -12($fp)
  sw $t0, -244($fp)
  move $t1, $t0
  move $v0, $t1
  sw $t1, -244($fp)
  move $sp, $fp
  lw $ra, 248($sp)
  lw $fp, 252($sp)
  addiu $sp, $sp, 256
  jr $ra
