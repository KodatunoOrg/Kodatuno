// UserSample���L�q
#include "SampleFunc.h"

// Sample6: �������𐶐�����
// ��̓I�ɂ́CNURBS�Ȗʂƕ��ʂƂ̌��(��_�Q)��Z�����ɏ������߂Ă���
// ����ɁC���̌�_�Q�̖@���x�N�g�����Z�o����
int SmpContourLine(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	NURBS_Func	nfunc;					// NURBS�������֐��W���Ăяo��

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// ��ԍŏ��ɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// ��ԍŏ��ɃZ���N�V�������ꂽBODY�̎��̂𓾂�
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	// �Z���N�V�������ꂽ�Ȗʂ��g�����ʂłȂ��ꍇ�͏I��

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODY����NURBS�Ȗʂ����o��

	Coord t[5000];					// ���i�p�[
	double red[3] = {1,0,0};		// �@���x�N�g���\���̐F
	double blue[3] = {0,0,1};		// �_�\���̐F
	char mes[256];					// ���b�Z�[�W�o�͗p

	double feed = Prop[0];			// ��_�Q�̓_�Ԋu
	double under = Prop[1];			// ��������Z_min
	double upper = Prop[2];			// ��������Z_max
	double delta = Prop[3];			// ������������Z�Ԋu
	int step = fabs(upper - under)/delta + 1;	// �������̖{�����Z�o

	Coord nvec = SetCoord(0,0,1);	// ���ʂ̖@���x�N�g�����w��(X-Y���ʂƂ���)

	// ���ʂ�Z�����ɃV�t�g���Ă����Ȃ���C���������Z�o����
	for(int i=0;i<step;i++){
		double z = under + delta*(double)i;	// ���݂̕��ʂ�Z�ʒu
		Coord pt = SetCoord(0,0,z);			// ���݂̕��ʏ��1�_���w��

		sprintf(mes,"z=%.3lf  calculating...",z);
		SetMessage(mes);

		int num = nfunc.CalcIntersecPtsPlaneSearch(S,pt,nvec,feed,10,t,5000,RUNGE_KUTTA);		// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�

		for(int i=0;i<num;i++){		// ��_�̐��������[�v
			Coord p = nfunc.CalcNurbsSCoord(S,t[i].x,t[i].y);			// ��_���p�����[�^�l������W�l�֕ϊ�
			Coord nt = nfunc.CalcNormVecOnNurbsS(S,t[i].x,t[i].y);		// ��_��̖@���x�N�g�����v�Z
			nt = MulCoord(nt,-2);											// �O�����@���x�N�g���֕ϊ����K���Ȓ����ɂ���
			DrawPoint(p,1,3,blue);			// ��_��`��
			DrawVector(p,nt,1,1,red);		// �@���x�N�g����`��
		}
	}
	SetMessage("END");


	return KOD_TRUE;
}


// Sample7: �Ȗʓ��m�̌�����Z�o����
int SmpIntersectSurfs(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	NURBS_Func nfunc;				// NURBS�֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Coord Rt[5000];					// NURBS�Ȗ�R(w,t)�ɂ������
	Coord St[5000];					// NURBS�Ȗ�S(u,v)�ɂ������
	double green[3] = {0,1,0};		// �_�\���̐F

	double feed = Prop[0];			// ��_�Q�̓_�Ԋu

	SetMessage("Start calcu....");

	// 1�ԖڂɃZ���N�V�������ꂽBODY�f�[�^�𓾂�
	OBJECT *obj1 = (OBJECT *)ObjList->getData(0);
	BODY *body1 = (BODY *)BodyList->getData(obj1->Body);

	// 2�ԖڂɃZ���N�V�������ꂽBODY�f�[�^�𓾂�
	OBJECT *obj2 = (OBJECT *)ObjList->getData(1);
	BODY *body2 = (BODY *)BodyList->getData(obj2->Body);

	if(obj1->Type != _TRIMMED_SURFACE || obj2->Type != _TRIMMED_SURFACE)	// �Z���N�V�������ꂽ2�̃G���e�B�e�B���g�����ʂłȂ��ꍇ�͏I��
		return KOD_ERR;

	NURBSS *S1 = body1->TrmS[obj1->Num].pts;	// BODY1����NURBS�Ȗʂ����o��
	NURBSS *S2 = body2->TrmS[obj2->Num].pts;	// BODY2����NURBS�Ȗʂ����o��

	int num = nfunc.CalcIntersecPtsNurbsSSearch(S1,S2,10,feed,Rt,St,5000);	// NURBS�Ȗʓ��m�̌���Z�o

	for(int i=0;i<num;i++){
		Coord p = nfunc.CalcNurbsSCoord(S1,Rt[i].x,Rt[i].y);	// uv���W��̓_��xyz���W�l�ɕϊ�
		DrawPoint(p,1,3,green);									// �`��
	}

	SetMessage("END");
	return KOD_TRUE;
}

// Sample8: NURBS�Ȗʂ̓K����(u,v)�ɂ����镽�ϋȗ��@���x�N�g�����Z�o����
int SmpMeanCurvature(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	NURBS_Func nfunc;
	double blue[3] = {0,0,1};

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// ��ԍŏ��ɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// ��ԍŏ��ɃZ���N�V�������ꂽBODY�̎��̂𓾂�
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	// �Z���N�V�������ꂽ�Ȗʂ��g�����ʂłȂ��ꍇ�͏I��

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODY����NURBS�Ȗʂ����o��

	int udiv = (int)Prop[0];		// u����������
	int vdiv = (int)Prop[1];		// v����������

	for(int i=0;i<=udiv;i++){
		double u = S->U[0]+(double)i*(S->U[1] - S->U[0])/(udiv+1);
		for(int j=0;j<=vdiv;j++){
			double v = S->V[0]+(double)j*(S->V[1] - S->V[0])/(vdiv+1);
			Coord P = nfunc.CalcNurbsSCoord(S,u,v);				// ���݂�(u,v)���(x,y,z)�����߂�
			Coord Hn = nfunc.CalcMeanCurvatureNormVec(S,u,v);	// ���ϋȗ��@���x�N�g�������߂�
			DrawVector(P,Hn,100,1,blue);						// ���ϋȗ��@���x�N�g����\��
		}
	}

	return KOD_TRUE;
}
