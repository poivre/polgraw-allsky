@                       Yeppp! library implementation
@                   This file is auto-generated by Peach-Py,
@        Portable Efficient Assembly Code-generator in Higher-level Python,
@                  part of the Yeppp! library infrastructure
@ This file is part of Yeppp! library and licensed under the New BSD license.
@ See LICENSE.txt for the full text of the license.

.macro BEGIN_ARM_FUNCTION name
	.arm
	.globl \name
	.align 2
	.func \name
	.internal \name
	\name:
.endm

.macro END_ARM_FUNCTION name
	.endfunc
	.type \name, %function
	.size \name, .-\name
.endm