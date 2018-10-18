#!/bin/bash

echo "Test summary:"
echo "============="

echo "Test started at " `date`>testResult.log

# Number of failed tests.
testFailedCnt=0

# Incremental test counter.
testNumber=0

# Run all the tests
for test in ./bin/tests/*
do
	echo "" >>testResult.log
	echo "Running test number $testNumber :" $test >>testResult.log 
	echo "---------------------------------------------------------" >>testResult.log
	$test 2>>testResult.log 1>/dev/null
	
	# Check for result.
	res=$?
	if [ "$res" -eq "0" ]; then
		echo "Test $testNumber passed."
		echo "Test $testNumber passed." >>testResult.log
	else
		echo "Test $testNumber : $test FAILED! Return code = $res"
		echo "Test $testNumber : $test FAILED! Return code = $res" >>testResult.log
		testFailedCnt=$((testFailedCnt+1))
	fi

	testNumber=$((testNumber+1))
done

# Print results.
echo "============================================================" >>testResult.log

if [ "$testFailedCnt" -eq "0" ]; then
	echo "All tests PASSED!"
	echo "All tests PASSED!" >>testResult.log
else
	echo "$testFailedCnt tests FAILED!"
	echo "$testFailedCnt tests FAILED!" >>testResult.log
fi

echo "See testResult.log file for detailed output."
