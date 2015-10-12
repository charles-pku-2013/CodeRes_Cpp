all:
	cc -o first first.c -g
	cc -o second other.c -g
	cc -o third other.c -g
clean:
	rm -f `ls | xargs file | grep -i 'executable' | cut -d':' -f1`
