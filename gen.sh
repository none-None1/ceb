#!/bin/bash
echo "#include <stdlib.h>"
echo "#include <assert.h>"
echo "#include <ir/ir.h>"
echo "#include <target/util.h>"
~/elvm/out/8cc -E -DPREP chicken.c
