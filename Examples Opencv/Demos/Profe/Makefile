CC=g++
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`

APP=Ultimo
SRCS=$(APP)1.c $(APP)2.c
OBJS=$(APP)1.o $(APP)2.o 

$(APP): $(OBJS)
	   $(CC) -o $(APP) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

all: $(APP)

clean:
	rm -f $(OBJS) $(APP)
