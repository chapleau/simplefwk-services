CC=g++-mp-4.8
CFLAGS=-std=c++11 -fPIC -Wall -I. -c
LDFLAGS=-std=c++11 -shared 
LDFLAGSEXTRA=-install_name @rpath/Services/lib/
OBJDIR=build
LIBDIR=lib

all: $(OBJDIR) $(LIBDIR) python/_IncidentService.so python/_Messaging.so

$(OBJDIR):
	mkdir $(OBJDIR)

$(LIBDIR):
	mkdir $(LIBDIR) 

python/_IncidentService.so: lib/libServices.so python/IncidentService_wrap.o
	$(CC) $(LDFLAGS) python/IncidentService_wrap.o -L/opt/local/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -ldl -lpython2.7 -Llib/ -lServices -Xlinker -rpath -Xlinker `dirname \`pwd\`` -o python/_IncidentService.so

python/IncidentService_wrap.o: python/IncidentService_wrap.cxx
	$(CC) $(CFLAGS) python/IncidentService_wrap.cxx `python2.7-config --cflags` -o python/IncidentService_wrap.o

python/IncidentService_wrap.cxx:
	swig -Wall -c++ -python python/IncidentService.i

python/_Messaging.so: lib/libServices.so python/Messaging_wrap.o
	$(CC) $(LDFLAGS) python/Messaging_wrap.o -L/opt/local/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -ldl -lpython2.7 -Llib/ -lServices -Xlinker -rpath -Xlinker `dirname \`pwd\`` -o python/_Messaging.so

python/Messaging_wrap.o: python/Messaging_wrap.cxx
	$(CC) $(CFLAGS) python/Messaging_wrap.cxx `python2.7-config --cflags` -o python/Messaging_wrap.o

python/Messaging_wrap.cxx:
	swig -Wall -c++ -python python/Messaging.i

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
	rm python/Messaging_wrap* 
	rm python/Messaging.py* 
	rm python/_Messaging.so 

clean_:
	rm python/Messaging_wrap* 
	rm python/Messaging.py* 
	rm python/_Messaging.so
