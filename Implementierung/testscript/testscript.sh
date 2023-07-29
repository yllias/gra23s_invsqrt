#executes main with various edge cases
cd ..
echo
./main 198123 4172398 -d -V1 

echo
./main "testscript/sample_small_flt.txt"

echo
./main 198123 4172398.0 -d -V1 -V0

echo
./main 0 4172398 -d -V1 -V0 

echo
./main "testscript/sample_big_flt.txt"

echo
./main "testscript/sample_small_flt.txt" -B 1000000 -V1

echo
#./main "testscript/sample_small_flt.txt" -B 1000000 -V1 -m

echo
#./main "testscript/sample_small_flt.txt" 4 1 23 5123 -B 1000000 -V1 -m

echo
./main  4 1 4 1 23 5123 -m 

echo
#./main  -V1 "testscript/sample_small_flt.txt" 4 1 23 5123 -m
