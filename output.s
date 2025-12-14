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
  li $t0, 10
  sw $t0, -4($fp)
  lw $t0, -4($fp)
  sw $t0, -8($fp)
  li $t1, 20
  sw $t1, -12($fp)
  lw $t1, -12($fp)
  sw $t1, -16($fp)
  li $t2, 0
  sw $t2, -20($fp)
  lw $t2, -20($fp)
  sw $t2, -24($fp)
  lw $t3, -28($fp)
  sw $t3, -32($fp)
  lw $t4, -36($fp)
  sw $t4, -40($fp)
  lw $t5, -32($fp)
  lw $t6, -40($fp)
  slt $t7, $t5, $t6
  bnez $t7, L1
  j L2
L1:
  li $t7, 1
  sw $t7, -44($fp)
  lw $t7, -44($fp)
  sw $t7, -48($fp)
  j L3
L2:
  li $s0, 2
  sw $s0, -52($fp)
  lw $s0, -52($fp)
  sw $s0, -56($fp)
L3:
  li $s1, 0
  sw $s1, -60($fp)
  lw $s1, -60($fp)
  sw $s1, -64($fp)
L4:
  lw $s2, -68($fp)
  sw $s2, -72($fp)
  li $s3, 10
  sw $s3, -76($fp)
  lw $s3, -72($fp)
  lw $s4, -76($fp)
  slt $s5, $s3, $s4
  bnez $s5, L5
  j L6
L5:
  lw $s5, -80($fp)
  sw $s5, -84($fp)
  lw $s6, -84($fp)
  sw $s6, -88($fp)
  li $s7, 1
  sw $s7, -92($fp)
  lw $s7, -92($fp)
  addu $t8, $s6, $s7
  j L4
L6:
  li $t9, 0
  sw $t9, -96($fp)
  lw $t9, -96($fp)
  sw $t9, -100($fp)
  j L7
L7:
L8:
L9:
  j L7
L10:
  j L11
L11:
L12:
  j L16
L15:
  j L14
L16:
  j L18
L17:
  j L13
L18:
L13:
  j L11
L14:
  move $sp, $fp
  lw $ra, 56($sp)
  lw $fp, 60($sp)
  addiu $sp, $sp, 64
  jr $ra
