.SUFFIXES: .o .C .c  .cpp


############################################################

include mkflags
include mk.inf

############################################################

C++FLAGS = $(DEBUG) $(ARCH)

.c.o:
	$(CC) $(ALL_INCLUDES) $(C++FLAGS) -C -E $*.c -o $*.ip
	$(SJISFIX) $*.ip  $*.i
	$(CC) $(ALL_INCLUDES) $(C++FLAGS) -c $*.i -o $*.o
	$(DBGRM) -f $*.ip $*.i

.C.o:
	$(CXX) $(ALL_INCLUDES) $(C++FLAGS) -C -E $*.C -o $*.ip
	$(SJISFIX) $*.ip  $*.i
	$(CXX) $(ALL_INCLUDES) $(C++FLAGS) -c $*.i -o $*.o
	$(DBGRM) -f $*.ip $*.i

.cpp.o:
	$(CXX) $(ALL_INCLUDES) $(C++FLAGS) -C -E $*.cpp -o $*.ip
	$(SJISFIX) $*.ip  $*.i
	$(CXX) $(ALL_INCLUDES) $(C++FLAGS) -c $*.i -o $*.o
	$(DBGRM) -f $*.ip $*.i

all: $(TARGET).exe

$(TARGET).exe: $(OBJS) wsres.o
	$(CXX) $(C++FLAGS) -o $(TARGET) $(OBJS) $(ALL_LIBS) wsres.o

wsres.o:wsres.rc
	$(WINDRES) --preprocessor='$(CXX) -E -xc' --use-temp-file --include-dir ./icons -I rc -i wsres.rc -o wsres.o

clean:
	$(RM) -f $(TARGET).exe $(OBJS) log.txt


