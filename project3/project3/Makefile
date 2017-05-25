# $Id: Makefile,v 1.2 2015/11/08 19:00:59 korpe Exp korpe $ 

#type make to compile with this Makefile

#you may change the path below to your compile directory
#MYPATH = /home/korpe/projec
 MYPATH = /home/Desktop/project3

all: libdp app1  libhash  app2

libdp:  dp.c
#below  we use TAB before gcc as the space character
	gcc -c -Wall -fpic dp.c
	gcc -shared -o libdp.so dp.o
	sudo cp -f libdp.so /usr/local/lib


app1:   app1.c
	gcc -Wall  -Wl,-rpath=${MYPATH} -L${MYPATH}  -o app1 app1.c -lpthread -ldp


libhash: hash.c
	gcc -c -Wall -fpic hash.c
	gcc -shared -o libhash.so hash.o
	sudo cp -f libhash.so /usr/local/lib


app2: app2.c
	gcc -Wall -Wl,-rpath=${MYPATH} -L${MYPATH} -o app2 app2.c -lpthread -lhash


#type "make clean" to clean your directory for recompile
clean: 
	rm -fr app1 app2 dp.o *.so *.o *~
