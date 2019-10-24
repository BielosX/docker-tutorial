
prog: func main
	gcc -o app obj/main.o obj/func.o

main: outdir
	gcc -c -o obj/main.o src/main.c

func: outdir
	gcc -c -o obj/func.o src/func.c

outdir:
	mkdir -p obj

clean:
	rm obj -r -f
	rm app -f
