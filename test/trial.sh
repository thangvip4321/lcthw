#!/bin/sh
echo "what is your name?"
read PERSON
echo "Hello, $PERSON"
test -f list_testd.c || echo "its not here"
NAME="wow"
echo $NAME
test -f ./list_tests  && echo Yes || echo No