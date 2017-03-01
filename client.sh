#!/bin/sh
echo "Attivazione Client.."
# 
gcc client.c -o client
echo "**** CLIENT ****"
./client 127.0.0.1
