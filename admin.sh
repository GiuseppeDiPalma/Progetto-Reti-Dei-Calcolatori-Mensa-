#!/bin/sh
echo "Attivazione Admin"
# 
gcc admin.c -o admin
echo "**** ADMIN ****"
./admin 127.0.0.1
