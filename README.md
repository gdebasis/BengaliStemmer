This is a very simple light-weight rule based stemmer for Bengali.
To build on a Linux system, type make.
You can then invoke the stemmer (the executable name is rbs) by
./stem_bn <input file> <output file>

The input file is a new line separated list of Bengali words and the output is also a new line separated file, the first word being the original word and the consecutive word being its stemmed form.

No corpus preprocessing is required to run this stemmer. I have provided a sample input file.
Just type in ./rbs sample.txt sample.stem to see the output.

NOTE: You can provide an optional third argument for the aggressiveness of the stemmer.
By default, the aggressive mode is turned off. To turn it on, please append a "1" at the end of the argument list.

