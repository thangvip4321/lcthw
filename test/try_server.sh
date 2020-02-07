if $VALGRIND ./test/server_tests 2>>./test/server_tests.log &  nc 127.0.0.1 1234 < ./test/server_file.txt
then
echo "gay"
else
echo "bullshit"
fi