#include "UserFunc.h"

// User�p�֐���5�܂œo�^���邱�Ƃ��ł��܂��B
// �J���҂͂����ɓƎ��̏�����ǉ����Ă�������
// �e�֐��͈ȉ��̈����������܂�  

// �R���X�g���N�^
USER::USER()
{
	// ������UserFunc�֐��|�C���^�̎w���֐����w�肷��
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(i==0)
			UserFunc[i] = &USER::ExecSampleFunc0;		// �T���v��
		else if(i==1)
			UserFunc[i] = &USER::ExecSampleFunc1;		// �T���v��
		else if(i==2)
			UserFunc[i] = &USER::ExecSampleFunc2;		// �T���v��
		else if(i==3)
			UserFunc[i] = &USER::MasterUserFunc;		
		else if(i==4)
			UserFunc[i] = &USER::MasterUserFunc;					
		else
			UserFunc[i] = &USER::MasterUserFunc;		// ���g�p��UserFunc[i]�ɂ�MasterUserFunc()��o�^���Ă���
	}

}


// User�{�^���o�^�֐�0�̎��̂��L�q
// *BodyList:BODY�̎��̂��o�^���Ƀ��X�g����Ă���
// *ObjList:�Z���N�V�������ꂽ�G���e�B�e�B���
// PickCount:�Z���N�V�������ꂽ�G���e�B�e�B�̐�
int USER::ExecSampleFunc0(BODYList *BodyList,OBJECTList *ObjList,int PickCount)
{
	char mes[256];

	// �Z���N�V���������R���\�[���o��
	sprintf(mes,"All Pick Count : %d",PickCount);
	SetMessage(mes);

	for(int i=0;i<PickCount;i++){
		OBJECT *obj = (OBJECT *)ObjList->getData(i);			// i�ԖڂɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
		sprintf(mes,"#%d",i+1);
		SetMessage(mes);
		sprintf(mes,"Body No. : %d",obj->Body);
		SetMessage(mes);
		sprintf(mes,"Selected Object Type : %d",obj->Type);
		SetMessage(mes);
		sprintf(mes,"Selected Object No. : %d",obj->Num);
		SetMessage(mes);
	}

	return KOD_TRUE;
}
	

// User�{�^���o�^�֐�1�̎��̂��L�q
// *BodyList:BODY�̎��̂��o�^���Ƀ��X�g����Ă���
// *ObjList:�Z���N�V�������ꂽ�G���e�B�e�B���
// PickCount:�Z���N�V�������ꂽ�G���e�B�e�B�̐�
int USER::ExecSampleFunc1(BODYList *BodyList,OBJECTList *ObjList,int PickCount)
{
	if(!PickCount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	NURBS_Func	nfunc;					// NURBS�������֐��W���Ăяo��
	double green[3] = {0,1,0};			// �_�\���̐F(��)

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// ��ԍŏ��ɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// ��ԍŏ��ɃZ���N�V�������ꂽBODY�̎��̂𓾂�

	// �{���ł���΁CSample4�Ɏ������Ȗʂ̃^�C�v�ɂ��������򂪕K�v�ł��邪�C����ȍ~��Sample�ł̓g�����ʂŌ��ߑł�����
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODY����NURBS�Ȗʂ����o��

	int u_divnum = 50;						// ���[�U�[�X�e�[�^�X��prop1��u�����������Ƃ��ēǂݍ���
	int v_divnum = 50;						// ���[�U�[�X�e�[�^�X��prop2��v�����������Ƃ��ēǂݍ���
	double u_val = (S->U[1] - S->U[0])/u_divnum;		// �p�����g���b�N��ԓ��ł�u�����������𓾂�
	double v_val = (S->V[1] - S->V[0])/v_divnum;		// �p�����g���b�N��ԓ��ł�v�����������𓾂�

	// u�����Cv�����̊e�����_�ɂ�������W�l�����߂�
	for(int i=0;i<=u_divnum;i++){
		for(int j=0;j<=v_divnum;j++){
			Coord P = nfunc.CalcNurbsSCoord(S,S->U[0]+u_val*i,S->V[0]+v_val*j);	// �w�肵��(u,v)�̍��W�l�����߂�
			DrawPoint(P,1,3,green);						// �`��
		}
	}

	return KOD_TRUE;}

// User�{�^���o�^�֐�2�̎��̂��L�q
int USER::ExecSampleFunc2(BODYList *BodyList,OBJECTList *ObjList,int PickCount)
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

	double feed = 1;			// ��_�Q�̓_�Ԋu
	double under = -20;			// ��������Z_min
	double upper = 40;			// ��������Z_max
	double delta = 5;			// ������������Z�Ԋu
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


// KODATUNO��View���j���[��Canncel�{�^��(�����S���}�[�N)���������Ƃ��̊֐������s�����
// ������Canncel�{�^����������User�������R�[�h���L�q
void USER::ExecdClear()
{
	// ������ǉ��ł��܂�
}

// User���j���[����o�^(User�{�^���̕\����)
void USER::SetMenuLabelName()
{
	// �Ǝ��̃��j���[���������ɋL�q���Ă�������
	strcpy(MenuLabel1,"Sample Func 0");
	strcpy(MenuLabel2,"Sample Func 1");
	strcpy(MenuLabel3,"Sample Func 2");
	strcpy(MenuLabel4,"User Func 4");
	strcpy(MenuLabel5,"User Func 5");
}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ȍ~��Kodatuno�J���҈ȊO�͕ύX���Ȃ�����
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// User�{�^�����x�������擾(�����ɂ͉����L�q���Ȃ�)
char *USER::GetMenuLabelName(int val)
{
	switch(val){
		case M_Label1:
			return MenuLabel1;
		case M_Label2:
			return MenuLabel2;
		case M_Label3:
			return MenuLabel3;
		case M_Label4:
			return MenuLabel4;
		case M_Label5:
			return MenuLabel5;
	}

	return NULL;
}

// UserFunc�֐��ւ̃|�C���^�̏����l�Ƃ��Ďg�p(�����ɂ͉����L�q���Ȃ�)
int USER::MasterUserFunc(BODYList *BodyList,OBJECTList *Obj,int PickCount)
{
	// void
	return KOD_TRUE;
}