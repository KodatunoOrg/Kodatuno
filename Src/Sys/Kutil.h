/*************************************************************************
 * Kutil.h 
 * �ŏ�ʃw�b�_�@�������炱����C���N���[�h����
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

// GLUT�w�b�_�t�@�C���̃C���N���[�h
// �eOS�ɍ��킹�ēK�؂�GLUT.h�̃t�@�C���ʒu���w�肵�Ă�������
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
