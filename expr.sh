#!/bin/sh

#This script serves as an wrapper around running expr;
#It will break up an expression into separate arguments as expected by expr

exprbin=./expr

#places spaces around +,-,*,^,/,(,)
$exprbin `echo "$@" | sed 's#\(+\|-\|*\|^\|\/\|(\|)\)# \1 #g'`

