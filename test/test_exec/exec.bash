#!/bin/bash

exec  2>&1 >(tee -a log.txt) 
# exec > log.txt  2>&1 
# exec &> log.txt  

echo "stdout"
echo "stderr" >&2

