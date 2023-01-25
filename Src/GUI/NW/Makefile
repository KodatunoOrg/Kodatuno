##################################################################################################################
# 
# Makefile for N/W版 Kodatuno R3.0
# 
# GL関係のインクルード、ライブラリフォルダの位置は使用しているOS等の環境に依存します。使用している環境に合わせて書き換えてください。
# Src/Sys/のファイルはKutil.hのみ使用します。
# 
##################################################################################################################

CC = g++ $(INC)
CCGL = g++ $(INCGL) $(LIBGL)

OBJS = ../../GE/BODY.o ../../GE/Describe_BODY.o ../../GE/DXF_Parser.o ../../GE/IGES_Parser.o ../../GE/KodListFunc.o ../../GE/MESH.o ../../GE/NURBS_Func.o ../../GE/Quaternion.o  ../../GE/SFQuant.o ../../GE/StdAfx.o ../../GE/STL_Parser.o ../../GE/VRML_Parser.o ./Describe1.o ./Describe2.o ./KODATUNO.o ./StdAfx__.o ../../UA/SampleFunc1.o ../../UA/SampleFunc2.o ../../UA/UserFunc.o

TARGET = exe
INC =  -I ../../GE -I ../../Sys -I ../../UA -I ./


INCGL  = -I /usr/X11R6/include/ -lglut -lGL -lGLU -lXext -lX11
LIBGL  = -L /usr/X11R6/lib -lm

.cpp.o :
	$(CC) -c $*.cpp -g -Wall -O -I /usr/X11R6/include/ 


$(TARGET) : $(OBJS)
	$(CCGL) *.o -Wall -O -o $(TARGET)


${OBJS}: ../../GE/BODY.h ../../GE/Describe_BODY.h ../../GE/DXF_Parser.h ../../GE/IGES_Parser.h ../../GE/KodListFunc.h ../../GE/MESH.h ../../GE/NURBS_Func.h ../../GE/Quaternion.h ../../GE/SFQuant.h ../../GE/StdAfx.h ../../GE/STL_Parser.h ../../GE/VRML_Parser.h ./Describe.h ./KODATUNO.h ./StdAfx__.h ../../Sys/Kutil.h ../../UA/SampleFunc.h ../../UA/UserFunc.h

clean:
	rm -f $(OBJS) $(TARGET) *~ *.o *.core
