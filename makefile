ALL:
	cc -g -c rbs.c -o rbs.o
	cc -g stem_bn.c rbs.o -o stem_bn
