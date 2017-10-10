all:
	gcc -g httpflood.c -o httpflood
	gcc -g tcpflood.c -o tcpflood
clean:
	rm -rf httpflood.dSYM tcpflood.dSYM httpflood tcpflood
