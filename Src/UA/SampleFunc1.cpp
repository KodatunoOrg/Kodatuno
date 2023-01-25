// UserSample���L�q
#include "SampleFunc.h"


// Sample1: IGES�t�@�C����ǂݍ��݁CBody�Ƃ��ēo�^����
int SmpRegistBody(BODYList *BodyList,OBJECTList *ObjList)
{
	char fname[256] = "../../../Model/IGES/FreeSurf.igs";	// �����ł̓t�@�C�����͌��ߑł��Ƃ���
	IGES_PARSER Iges;					// IGES�t�@�C����ǂݍ��ނ��߂̃N���X���Ăяo��
	BODY *body = new BODY;				// �ǂݍ���IGES�f�[�^���i�[���邽�߂̔�(BODY)��p��

	int flag = Iges.IGES_Parser_Main(body,fname);	// IGES�f�[�^��ǂݍ���ŁAbody�Ɋi�[
	if(flag == KOD_TRUE){							// �ǂݍ��ݐ����Ȃ�
		Iges.Optimize4OpenGL(body);					// �ǂݍ���IGES�t�@�C����OpenGL�p�ɍœK������(�K�����̊֐������s���Ă�������)
        GuiIF.SetMessage("Succeeded to read IGES file");
	}
	else{											// �ǂݍ��݂Ɏ��s������
		delete body;								// �m�ۂ����������[�������
        GuiIF.SetMessage("Error: fail to read IGES file");
		return KOD_ERR;								// �I��
	}

	body->RegistBody(BodyList,fname);		// Body�����X�g�ɓo�^

	// �ǂݍ���BODY����������ꍇ�͈ȉ������s
	//body->DeleteBody(BodyList);			// Body�����X�g����͂���
	//delete body;							// Body���g�̃������[���

	return KOD_TRUE;
}

// Sample2: �w�肵��Body�ɑ΂��A�t�B���ϊ����s��
int SmpTransBody(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	OBJECT *obj = (OBJECT *)ObjList->getData(0);			// ��ԍŏ��ɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
	BODY *body = (BODY *)BodyList->getData(obj->Body);		// ��ԍŏ��ɃZ���N�V�������ꂽBODY�̎��̂𓾂�

	Coord rot_ax = SetCoord(0,0,1);		// ��]��
	double deg = 90;					// ��]�p�x
	Coord shift = SetCoord(0,0,10);		// �V�t�g��
	Coord expand = SetCoord(2,2,2);		// �g���

	body->RotBody(rot_ax,deg);			// Z�������90�x��]
	body->ShiftBody(shift);				// Z������10�V�t�g
	body->ExpandBody(expand);			// BODY��2�{�Ɋg��

	return KOD_TRUE;
}

// Sample3: �Ȑ����w�肵���������ŕ��������_�𐶐�����
int SmpDivCurves(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	NURBS_Func	nfunc;					// NURBS�������֐��W���Ăяo��
	Coord div_pt[102];					// �����_�̍��W�l���i�[����
	double green[3] = {0,1,0};			// �����_�\���̐F(��)

	int divnum = (int)Prop[0];			// ���[�U�[�X�e�[�^�X��prop1�𕪊����Ƃ��ēǂݍ���

	// �Z���N�V�����������������[�v
	for(int i=0;i<PickCount;i++){
		OBJECT *obj = (OBJECT *)ObjList->getData(i);			// i�ԖڂɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
		BODY *body = (BODY *)BodyList->getData(obj->Body);		// i�ԖڂɃZ���N�V�������ꂽBODY�̎��̂𓾂�
		if(obj->Type == _NURBSC){								// i�ԖڂɃZ���N�V�������ꂽ�G���e�B�e�B��NURBS�Ȑ��Ȃ�
			int ptnum = nfunc.CalcDeltaPtsOnNurbsC(&body->NurbsC[obj->Num],divnum,div_pt);		// �����_�����߂�
			for(int j=0;j<ptnum;j++){
				DrawPoint(div_pt[j],1,3,green);					// �����_��\��
			}
		}
	}

	return KOD_TRUE;
}

// Sample4: �C�ӂ̓_����̋Ȗʏ�̍ŋߖT�_�����߂�
int SmpNearestPt(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	NURBS_Func	nfunc;					// NURBS�������֐��W���Ăяo��
	NURBSS *S;							// �Z���N�V�������ꂽ�Ȗʂւ̃|�C���^
	Coord Q,Q_;							// �ŋߖT�_�i�[�p
	char mes[256];						// ���b�Z�[�W�o�͗p
	double blue[3] = {0,0,1};			// �ŋߖT�_�\���̐F(��)
	double red[3] = {1,0,0};			// �R���g���[���|�C���g�\���̐F(��)
	double green[3] = {0,1,0};			// �ŋߖT�_�ƃR���g���[���|�C���g���Ȃ����̐F(��)

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// ��ԍŏ��ɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// ��ԍŏ��ɃZ���N�V�������ꂽBODY�̎��̂𓾂�

	// �Z���N�V�������ꂽ�ʂ��g�������ꂽNURBS�Ȗʂ��C������NURBS�Ȗʂ����ׁCNURBS�Ȗʂ݂̂����o��
	// KODATUNO�ł́CIGES�t�@�C���ǂݍ��݂̒i�K�őS�Ă̖�/����NURBS�Ȗ�/�Ȑ��ւƂ���邽�߁C
	// ���̂悤��3�̏�������ɂ���ė~�����ʂ����o�����Ƃ��ł���
	if(obj->Type == _TRIMMED_SURFACE){
		S = body->TrmS[obj->Num].pts;	// �g�����ʂ̏ꍇ�́C�g�����O�̌���NURBS�Ȗʂ����o��
	}
	else if(obj->Type == _NURBSS){
		S = &body->NurbsS[obj->Num];	// ������NURBS�Ȗʂ̏ꍇ�͂��̂܂܂��̋Ȗʂւ̃|�C���^�𓾂�
	}
	else{
		return KOD_ERR;					// �Z���N�V�������ꂽ�Ȗʂ��g�����ʂł�NURBS�Ȗʂł��łȂ��ꍇ�͏I��
	}

	// �C�ӂ̓_��3�p��
	Coord P[3];
	P[0] = SetCoord(0,0,100);
	P[1] = SetCoord(100,50,50);
	P[2] = SetCoord(0,100,-50);

	// �ߖT�_�𓾂�
	for(int i=0;i<3;i++){
		DrawPoint(P[i],1,3,red);									// �C�ӓ_��`��
		int flag = nfunc.CalcIntersecPtNurbsPt(S,P[i],3,0.2,&Q);	// �ŋߖT�_�Z�o
		if(flag == KOD_TRUE){										// �ŋߖT�_������������
			Q_ = nfunc.CalcNurbsSCoord(S,Q.x,Q.y);					// ����ԍ��W�֕ϊ����ۑ�
			DrawPoint(Q_,1,3,blue);									// �ŋߖT�_��`��
			DrawLine(Q_,P[i],1,green);								// �����`��
			sprintf(mes,"%d:True(%lf,%lf,%lf)",i,Q_.x,Q_.y,Q_.z);
            GuiIF.SetMessage(mes);
		}
		else{									// �ŋߖT�_��������Ȃ�������
			sprintf(mes,"%d:False\n",i);		// �G���[���b�Z�[�W���R���\�[���ɏo��	
            GuiIF.SetMessage(mes);
		}
	}

	return KOD_TRUE;
}

// Sample5: �Ȗʂ�UV�p�����[�^�œ��Ԋu�ɕ��������_�𐶐�����
int SmpUVDivLine(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	NURBS_Func	nfunc;					// NURBS�������֐��W���Ăяo��
	double green[3] = {0,1,0};			// �_�\���̐F(��)

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// ��ԍŏ��ɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// ��ԍŏ��ɃZ���N�V�������ꂽBODY�̎��̂𓾂�

	// �{���ł���΁CSample4�Ɏ������Ȗʂ̃^�C�v�ɂ��������򂪕K�v�ł��邪�C����ȍ~��Sample�ł̓g�����ʂŌ��ߑł�����
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODY����NURBS�Ȗʂ����o��

	int u_divnum = (int)Prop[0];						// ���[�U�[�X�e�[�^�X��prop1��u�����������Ƃ��ēǂݍ���
	int v_divnum = (int)Prop[1];						// ���[�U�[�X�e�[�^�X��prop2��v�����������Ƃ��ēǂݍ���
	double u_val = (S->U[1] - S->U[0])/u_divnum;		// �p�����g���b�N��ԓ��ł�u�����������𓾂�
	double v_val = (S->V[1] - S->V[0])/v_divnum;		// �p�����g���b�N��ԓ��ł�v�����������𓾂�

	// u�����Cv�����̊e�����_�ɂ�������W�l�����߂�
	for(int i=0;i<=u_divnum;i++){
		for(int j=0;j<=v_divnum;j++){
			Coord P = nfunc.CalcNurbsSCoord(S,S->U[0]+u_val*i,S->V[0]+v_val*j);	// �w�肵��(u,v)�̍��W�l�����߂�
			DrawPoint(P,1,3,green);						// �`��
		}
	}

	return KOD_TRUE;
}
