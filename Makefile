# Makefile

CC = g++ 
CCGL = g++ $(INC) $(LIBGL)
OBJS   = KODATUNO.o IGES_Parser.o BODY.o Describe1.o Describe2.o Describe_BODY.o StdAfx.o Quaternion.o NURBS_Func.o Sample.o UserFunc.o 
TARGET = KODatUNO
INC  = -I /usr/X11R6/include/ -lglut -lGL -lGLU -lXext -lX11
LIBGL  = -L /usr/X11R6/lib -lm

.cpp.o :
	$(CC) -c $*.cpp -g -Wall -O -I /usr/X11R6/include/ 

$(TARGET) : $(OBJS)
	$(CCGL) $(OBJS) -Wall -O -o $(TARGET) 

${OBJS}: KODATUNO.h IGES_Parser.h BODY.h Describe.h Describe_BODY.h StdAfx.h Quaternion.h NURBS_Func.h Sample.h UserFunc.h

clean:
	rm -f $(OBJS) $(TARGET) *~ *.o *.core
