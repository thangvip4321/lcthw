#!/bin/sh
for i in ./*_tests
do
		if [ "$i" = "./server_tests" ]
		then
			echo "hmm"
		else
			echo "holyshit wtf"
		fi

done
#FILE ="./server_tests"
#basename "$FILE"
#f = "$(basename -- $FILE)"
#echo f