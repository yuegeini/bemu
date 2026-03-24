.section .text
.globl _start

_start:

    li sp, 0x00100000      # stack top
    li gp, 0x00080000      # global pointer (пример)

    call main

1:
    j 1b                   # бесконечный цикл после main