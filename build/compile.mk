all: setup compile link clear

FILE_NAME := main

setup:
	del /Q environment\$(FILE_NAME).exe
compile:
	g++ -Isrc/import -Isrc/include -Iplugins -Icoding -c coding/$(FILE_NAME).cpp
link:
	g++ *.o -o environment/$(FILE_NAME) -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lsfml-audio
clear:
	del *.o
