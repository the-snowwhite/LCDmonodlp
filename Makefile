all:
	gcc ppmmonodlp.c -o ppmmonodlp

clean:
	rm -rf ppmmonodlp *~

install:
	cp ppmmonodlp /usr/bin

uninstall:
	rm /usr/bin/ppmmonodlp
