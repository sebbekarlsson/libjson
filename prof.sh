# this is a profiling tool.

rm *callgrind* ; valgrind --tool=callgrind --dump-instr=yes --simulate-cache=no --collect-jumps=yes ./build/json_e $1 ; kcachegrind ./*callgrind*
