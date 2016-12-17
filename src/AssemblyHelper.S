#
#    IntelGalileoOS
#    Created by: Jakub Powierza
#

.text
.globl getCR0
.globl getCR3
.globl setCR3
.globl enablePaging

getCR0:
	mov %cr0, %eax
	ret

getCR3:
	mov %cr3, %eax
	ret

setCR3:
	mov 4(%esp), %eax
	mov %eax, %cr3
	ret

enablePaging:
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0
	ret
