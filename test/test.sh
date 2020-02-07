echo "running unit test"
for i in ./test/*_tests
do
	if test -f $i 
	then
	    if [ "$i" = "./test/server_tests" ]
	    then
		echo "hmmmmmmmmm again"
	    	if $VALGRIND ./$i 2>./test/server_tests.log & nc 127.0.0.1 1234 < ./test/server_file.txt
			then
	    		echo $i PASS
	    	else
	    		echo "ERROR IN TEST $i , check test log"
				echo "------"
				tail ./test/tests.log
				exit 1
	        fi
	    else
		echo "seems right where the fuck is it"
		if $VALGRIND ./$i 2>>  ./test/tests.log  
		then
			echo $i PASS
		else
			echo "ERROR IN TEST $i , check test log"
			echo "------"
			tail ./test/tests.log
			exit 1
		fi
	    fi
	fi
done


echo ""
