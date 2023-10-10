#!/bin/bash

LD_LIBRARY_PATH="v1/" ./main_use_v1
LD_LIBRARY_PATH="v2/" ./main_use_v1
LD_LIBRARY_PATH="v2/" ./main_use_v2
LD_LIBRARY_PATH="v1/" ./main_use_v2
