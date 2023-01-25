// UserSample�w�b�_

#include "StdAfx.h"			// �ėp
#include "NURBS_Func.h"		// NURBS�􉽌v�Z�֘A
#include "Describe_BODY.h"	// BODY�`��֘A
#include "BODY.h"			// BODY�N���X
#include "IGES_Parser.h"	// IGES�ǂݍ��݊֘A

int SmpRegistBody(BODYList *,OBJECTList *);							// Sample1: IGES�t�@�C����ǂݍ��݁CBody�Ƃ��ēo�^����
int SmpTransBody(BODYList *,OBJECTList *,int,double []);			// Sample2: �w�肵��Body�ɑ΂��A�t�B���ϊ����s��
int SmpDivCurves(BODYList *,OBJECTList *,int,double []);			// Sample3: �Ȑ����w�肵���������ŕ��������_�𐶐�����
int SmpNearestPt(BODYList *,OBJECTList *,int,double []);			// Sample4: �C�ӂ̓_����̋Ȗʏ�̍ŋߖT�_�����߂�
int SmpUVDivLine(BODYList *,OBJECTList *,int,double []);			// Sample5: �Ȗʂ�UV�p�����[�^�œ��Ԋu�ɕ��������_�𐶐�����
int SmpContourLine(BODYList *,OBJECTList *,int,double []);			// Sample6: �������𐶐�����
int SmpIntersectSurfs(BODYList *,OBJECTList *,int,double []);		// Sample7: �Ȗʓ��m�̌�����Z�o����
int SmpMeanCurvature(BODYList *,OBJECTList *,int,double []);		// Sample8: ���ϋȗ��@���x�N�g�����Z�o����

