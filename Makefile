all: puissance4.c
	gcc -Wall puissance4.c

clean:
	rm -f *~
	rm -f *#
	rm -f a.out

package: clean
	cd ..; tar -cvzf Dubrunfaut-Vieillefon.tar.gz * ; chmod 750 Dubrunfaut-Vieillefon.tar.gz *
