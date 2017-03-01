#!/bin/sh
echo "Attivazione Server Sede.."
# 
gcc serverS.c -o sede
./sede 127.0.0.2 centrodirezionale