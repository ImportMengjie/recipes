#!/bin/bash

echo "my pid is $$"
for i in $(seq 1 10);do
  echo "$i my pid is $$"
  sleep 1
done
# while true;do 
#   echo "running $$"
#   sleep 10s
# done

./test_print_pid

# top -b >/dev/null&
#
