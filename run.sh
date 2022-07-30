#!/bin/bash

echo -e -n "\e[43m Compilazione"
for i in {1..3} 
do
    echo -n "."
    sleep .35
done
echo -e "\e[0m"

#rm airportFIFO 2>/dev/null
#mkfifo airportFIFO

make

if test $? -ne 0
then
	echo -e "\e[31m Errore durante la compilazione \e[0m"
	exit 1
fi


echo -e "\n \e[32m 🗸 Puoi eseguire il programma con ./torre \e[0m"

read -p "Eseguire (Y/n)? " avvio 
case $avvio in 
	[Nn] ) exit;;
esac 

echo -e -n "\e[35m Avvio"
for i in {1..3} 
do
    echo -n "."
    sleep .35
done
echo -e "\e[0m"

clear
./torre