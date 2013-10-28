CC=g++-mp-4.8
CFLAGS=-std=c++11 -fPIC -Wall -I. -c
LDFLAGS=-std=c++11 -shared 
LDFLAGSEXTRA=-install_name @rpath/Services/lib/
OBJDIR=build
LIBDIR=lib

all: $(OBJDIR) $(LIBDIR) python/_IncidentService.so

$(OBJDIR):
	mkdir $(OBJDIR)

$(LIBDIR):
	mkdir $(LIBDIR) 

python/_IncidentService.so: lib/libServices.so python/IncidentService_wrap.o
	$(CC) $(LDFLAGS) python/IncidentService_wrap.o -lpython2.7 -Llib/ -lServices -Xlinker -rpath -Xlinker `dirname \`pwd\`` -o python/_IncidentService.so

python/IncidentService_wrap.o: python/IncidentService_wrap.cxx
	$(CC) $(CFLAGS) python/IncidentService_wrap.cxx -I/usr/include/python2.7/ -o python/IncidentService_wrap.o

python/IncidentService_wrap.cxx:
	swig -Wall -c++ -python python/IncidentService.i

lib/libServices.so: build/Messaging.o build/IncidentService.o
	$(CC) $(LDFLAGS) $(LDFLAGSEXTRA)libServices.so build/Messaging.o build/IncidentService.o -o lib/libServices.so

build/Messaging.o: src/Messaging.cxx Services/MessageService.h Services/Messaging.h
	$(CC) $(CFLAGS) src/Messaging.cxx -o build/Messaging.o

build/IncidentService.o: src/IncidentService.cxx Services/IncidentService.h 
	$(CC) $(CFLAGS) src/IncidentService.cxx -o build/IncidentService.o 


clean:
	rm -r build/
	rm -r lib/
	rm python/IncidentService_wrap*
	rm python/IncidentService.py*
	rm python/_IncidentService.so
