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
  addiu $sp, $sp, -64
  sw $fp, 60($sp)
  sw $ra, 56($sp)
  move $fp, $sp
  li $t0, 5
  sw $t0, -4($fp)
  lw $t0, -4($fp)
  sw $t0, -8($fp)
  li $t1, 10
  sw $t1, -12($fp)
  lw $t1, -12($fp)
  sw $t1, -16($fp)
  lw $t2, -4($fp)
  sw $t2, -20($fp)
  lw $t3, -4($fp)
  sw $t3, -24($fp)
  lw $t4, -20($fp)
  lw $t5, -24($fp)
  addu $t6, $t4, $t5
  sw $t6, -28($fp)
  lw $t7, -4($fp)
  sw $t7, -32($fp)
  li $s0, 10
  sw $s0, -36($fp)
  lw $s0, -32($fp)
  lw $s1, -36($fp)
  slt $s2, $s1, $s0
  bnez $s2, L1
  j L2
L1:
  lw $s2, -4($fp)
  sw $s2, -40($fp)
  lw $s3, -40($fp)
  sw $s3, -44($fp)
  li $s4, 5
  sw $s4, -48($fp)
  lw $s4, -48($fp)
  subu $s5, $s3, $s4
L2:
  lw $s6, -4($fp)
  sw $s6, -52($fp)
  lw $s7, -52($fp)
  move $v0, $s7
  move $sp, $fp
  lw $ra, 56($sp)
  lw $fp, 60($sp)
  addiu $sp, $sp, 64
  jr $ra
