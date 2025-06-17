



.text

.global main
main:
      LDR R3, =array
      ... more code here
done: NOP
      MOV PC,L

.data
array:
      .byte 0x08
      .byte 0x10
      .byte 0xFF
      ... more data here
.end
