# Arpl Test Suite

The test suite can be run from the command prompt as in:
	arpl -w -S -fno-exceptions 00001.a

A for loop is handy to batch compiler:	
	for %f in (*.a) do arpl -w -S -fno-exceptions -finline0 %f

Files can be combined for review using the type command as in:
	for %f in (*.asm) do type %~nf.a type %~nf.asm >>t17.inc
	
The arpl compiler must be located somewhere in the path.
For testing I just copy the compiler to the test directory.
