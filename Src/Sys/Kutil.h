/*************************************************************************
 * Kutil.h 
 * 最上位ヘッダ　迷ったらこれをインクルードせよ
 * V1.0 2013/09/19 K.Takasugi
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <ctype.h>

//#define USE_WIDE_STUDIO
#define USE_QT

// GLUTヘッダファイルのインクルード
// 各OSに合わせて適切にGLUT.hのファイル位置を指定してください
#ifndef MacOS
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#ifdef USE_WIDE_STUDIO
#include "StdAfxWS.h"
#elif defined(USE_QT)
#include "../GUI/Qt/StdAfxQt.h"
#else
#include "StdAfx__.h"
#endif
