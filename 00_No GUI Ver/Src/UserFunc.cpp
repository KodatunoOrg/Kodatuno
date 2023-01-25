#include"UserFunc.h"

// User�p�֐���5�܂œo�^���邱�Ƃ��ł��܂��B
// �J���҂͂����ɓƎ��̏�����ǉ����Ă�������
// �e�֐��͈ȉ��̈����������܂�  
// *Body:BODY���X�g�̐擪�|�C���^
// *Obj:�Z���N�V�������ꂽ�G���e�B�e�B���
// PickCount:�Z���N�V�������ꂽ�G���e�B�e�B�̐�

// �R���X�g���N�^
USER::USER()
{
	// ������UserFunc�֐��|�C���^�̎w���֐����w�肷��
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(i==0)
			UserFunc[i] = &USER::MasterUserFunc;		
		else if(i==1)
			UserFunc[i] = &USER::MasterUserFunc;		
		else if(i==2)
			UserFunc[i] = &USER::MasterUserFunc;		
		else if(i==3)
			UserFunc[i] = &USER::CalcIntersecPts;		// �T���v��
		else if(i==4)
			UserFunc[i] = &USER::MasterUserFunc;						
		else
			UserFunc[i] = &USER::MasterUserFunc;		// ���g�p��UserFunc[i]�ɂ�MasterUserFunc()��o�^���Ă���
	}

}

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
	strcpy(MenuLabel1,"User Func 1");
	strcpy(MenuLabel2,"User Func 2");
	strcpy(MenuLabel3,"User Func 3");
	strcpy(MenuLabel4,"CalcIntersecLine");
	strcpy(MenuLabel5,"User Func 5");
}

// User�{�^���o�^�֐�4�̎��̂��L�q
int USER::CalcIntersecPts(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
	if(!pickcount)	return KOD_ERR;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	BODY *body;
	OBJECT *obj;
	int i;

	// �Z���N�V�������ꂽ���������[�v
	for(i=0;i<pickcount;i++){
		obj = (OBJECT *)ObjList->getData(i);				// �Z���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
		body = (BODY *)BodyList->getData(obj->Body);		// �Z���N�V�������ꂽBODY�̎��̂𓾂�

		// �Z���N�V�������ꂽ�G���e�B�e�B���g�����ʂ�NURBS�Ȗ�
		if(obj->Type == _NURBSS){
				ContourLine(&body->NurbsS[obj->Num]);
		}		
		else if(obj->Type == _TRIMMED_SURFACE){
			ContourLine(body->TrmS[obj->Num].pts);
		}


		// �Z���N�V�������ꂽ�G���e�B�e�B��NURBS�Ȑ�
		else if(obj->Type == _NURBSC){						
			// ������ǉ����Ă�������
		}
	}

	return KOD_TRUE;
}


// �T���v���Ƃ��ĕ��ʂ�NURBS�Ȑ��̌��(��_�Q)���Z�o����v���O����������
// ���ʂ�Z�����ɃV�t�g�����Ă����A�eZ�ʒu�ł̌���𒀎����߂�
// ����  *NurbsS�F����Z�o�ΏۂƂȂ�Nurbs�Ȗʂւ̃|�C���^
int USER::ContourLine(NURBSS *NurbsS)
{
	NURBS_Func NFunc;				// NURBS�֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Coord t[5000];					// ��
	int   num;						// ���̐�
	Coord pt=SetCoord(0,0,10);		// ���ʏ��1�_���w��
	Coord nvec=SetCoord(0,0,1);		// ���ʂ̖@���x�N�g�����w��
	double pcolor[3] = {0,1,0};		// �\���̐F
	double tcolor[3] = {1,0,0};
	int allnum = 0;
	double under_z = -20;
	double upper_z = 40;
	double delta = 4;
	int step = fabs(upper_z - under_z)/delta + 1;

	for(int i=0;i<step;i++){
		double z = under_z + delta*(double)i;
		if(z == 0.0) continue;			// z=0�̎��͓��ٓ_�Ȃ̂Ōv�Z���Ȃ�
		pt = SetCoord(0,0,z);			// ���ʂ�Z�����ɏ��X�ɃV�t�g���Ă���
		char mes[256];
		sprintf(mes,"z=%.3lf  calculating...",z);
		SetMessage(mes);
		num = NFunc.CalcIntersecPtsPlaneSearch(NurbsS,pt,nvec,0.5,10,t,5000);		// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
		sprintf(mes,"num = %d",num);
		SetMessage(mes);
		allnum += num;
		for(int i=0;i<num;i++){		// ��_�̐��������[�v
			//if(i%5 == 0){
				Coord p = NFunc.CalcNurbsSCoord(NurbsS,t[i].x,t[i].y);			// ��_���p�����[�^�l������W�l�֕ϊ�
				Coord nt = NFunc.CalcNormVecOnNurbsS(NurbsS,t[i].x,t[i].y);		// ��_��̖@���x�N�g�����v�Z
				nt = MulCoord(nt,-1);											// �O�����@���x�N�g���֕ϊ�
				DrawPoint(p,1,3,pcolor);			// ��_��`��
				//DrawVector(p,nt,3,1,tcolor);		// �@���x�N�g����`��
			//}
		}
	}
	SetMessage("END");

	return KOD_TRUE;
}


