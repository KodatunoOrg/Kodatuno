// UserSample�w�b�_

#include "StdAfx.h"
#include "NURBS_Func.h"		// NURBS�􉽌v�Z�֘A
#include "Describe_BODY.h"	// BODY�`��֘A
#include "BODY_Handler.h"
#include "BODY.h"

#define PROPNUM	15			// �v���p�e�B�[��

int ExecSampleFunc1(BODYList *,OBJECTList *,int,double []);		// �T���v��1
int ExecSampleFunc2(BODYList *,OBJECTList *,int,double []);		// �T���v��2

int ContourLine(NURBSS *,double,double,double,double);			// ����������
