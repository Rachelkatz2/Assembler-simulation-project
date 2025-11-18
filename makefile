project: parsing.o parsing2.o write.o betweenParsing.o functions.o main.o pre_assembler.o
	gcc -g -Wall -ansi -pedantic parsing.o parsing2.o write.o betweenParsing.o functions.o main.o pre_assembler.o -o assembler
 
parsing.o: parsing.c parsings.h global.h functions.h main.h
	gcc -g -Wall -ansi -pedantic parsing.c -c

parsing2.o: parsing2.c parsings.h global.h functions.h main.h
	gcc -g -Wall -ansi -pedantic parsing2.c -c 

write.o: write.c write.h global.h main.h parsings.h
	gcc -g -Wall -ansi -pedantic write.c -c 

betweenParsing.o: betweenParsing.c parsings.h global.h main.h
	gcc -g -Wall -ansi -pedantic betweenParsing.c -c

functions.o: functions.c global.h main.h parsings.h
	gcc -g -Wall -ansi -pedantic functions.c -c 

main.o: main.c main.h global.h pre_assembler.h functions.h parsings.h write.h
	gcc -g -Wall -ansi -pedantic main.c -c 

pre_assembler.o: pre_assembler.c main.h pre_assembler.h functions.h
	gcc -g -Wall -ansi -pedantic pre_assembler.c -c 

