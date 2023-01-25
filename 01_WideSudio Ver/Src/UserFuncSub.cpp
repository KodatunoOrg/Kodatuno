// UserSample���L�q
#include "UserFuncSub.h"

// Example 1 : NURBS�ȖʂƁA�w�肵�����ʂ̌�������߂�(���ʂ𒀎��I�t�Z�b�g���Ă����A���������Z�o)
// ����  
// *Body:BODY���X�g�̐擪�A�h���X  
// *Obj:�Z���N�V�������ꂽ�G���e�B�e�B�̏��  
// pickcount:�Z���N�V�������ꂽ�G���e�B�e�B��
int ExecSampleFunc1(BODYList *BodyList,OBJECTList *ObjList,int pickcount,double prop[])
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
		if(obj->Type == _TRIMMED_SURFACE || obj->Type == _NURBSS){
				ContourLine(&body->NurbsS[obj->Num],prop[0],prop[1],prop[2],prop[3]);
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
//       upper_z�FNurbs�Ȗʂƌ�������x-y���ʂ�z�����̍ő�l
//       under_z�FNurbs�Ȗʂƌ�������x-y���ʂ�z�����̍ŏ��l
//       delta�FNurbs�Ȗʂƌ�������x-y���ʂ�z�����s�b�N�t�B�[�h
//       roughness�F����̍r��(0.1�`1����)
int ContourLine(NURBSS *NurbsS,double upper_z,double under_z,double delta,double roughness)
{
	NURBS_Func NFunc;				// NURBS�֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Coord t[5000];					// ��
	int   num;						// ���̐�
	Coord pt=SetCoord(0,0,10);		// ���ʏ��1�_���w��
	Coord nvec=SetCoord(0,0,1);		// ���ʂ̖@���x�N�g�����w��
	double pcolor[3] = {0,1,0};		// �\���̐F
	double tcolor[3] = {1,0,0};

	int step = fabs(upper_z - under_z)/delta + 1;

	for(int i=0;i<step;i++){
		double z = under_z + delta*(double)i;
		pt = SetCoord(0,0,z);			// ���ʂ�Z�����ɏ��X�ɃV�t�g���Ă���
		char mes[256];
		sprintf(mes,"z=%.3lf  calculating...",z);
		SetMessage(mes);
		num = NFunc.CalcIntersecPtsPlaneSearch(NurbsS,pt,nvec,roughness,10,t,5000,BULIRSH_STOER);		// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
		sprintf(mes,"num = %d",num);
		SetMessage(mes);

		for(int i=0;i<num;i++){		// ��_�̐��������[�v
			Coord p = NFunc.CalcNurbsSCoord(NurbsS,t[i].x,t[i].y);			// ��_���p�����[�^�l������W�l�֕ϊ�
			Coord nt = NFunc.CalcNormVecOnNurbsS(NurbsS,t[i].x,t[i].y);		// ��_��̖@���x�N�g�����v�Z
			nt = MulCoord(nt,-1);											// �O�����@���x�N�g���֕ϊ�
			DrawPoint(p,1,3,pcolor);			// ��_��`��
			DrawVector(p,nt,3,1,tcolor);		// �@���x�N�g����`��
		}
	}
	SetMessage("END");

	return KOD_TRUE;
}


// Example 2 : NURBS�Ȗʓ��m�̌�����Z�o����
// ����  
// *Body:BODY���X�g�̐擪�A�h���X  
// *Obj:�Z���N�V�������ꂽ�G���e�B�e�B�̏��  
// pickcount:�Z���N�V�������ꂽ�G���e�B�e�B��
// prop: prop[0]:�����_�T�[�`���̕�����   prop[1]:����ǐՕ�
int ExecSampleFunc2(BODYList *BodyList,OBJECTList *ObjList,int pickcount,double prop[])
{
	if(!pickcount)	return KOD_ERR;

	BODY *body;
	OBJECT *obj;
	TRIMD_NURBSS *tnurbs[100];	// NURBS�Ȗʂւ̃|�C���^���i�[
	int surf_body[100];
	int surf_type[100];			// �Ȗʂ̃^�C�v���i�[
	int surf_objn[100];
	int scount=0;				// �Ȗʐ��J�E���^
	NURBS_Func NFunc;			// NURBS�֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Coord Rt[5000];				// NURBS�Ȗ�R(w,t)�ɂ������
	Coord St[5000];				// NURBS�Ȗ�S(u,v)�ɂ������
	double green[3] = {0,1,0};	// �\���̐F
	int flag_r;					// �g�������ʃt���O
	int flag_s;					// 

	SetMessage("Start calcu....");

	// �Z���N�V�������ꂽ���������[�v
	// �e�Ȗʂ�"�g������"�Ȃ̂�"�g�������Ȃ�������NURBS�Ȗ�"�Ȃ̂����ׂ�
	for(int i=0;i<pickcount;i++){
		obj = (OBJECT *)ObjList->getData(i);
		body = (BODY *)BodyList->getData(obj->Body);		// �Z���N�V�������ꂽBODY�f�[�^�𓾂�

		if(obj->Type != _TRIMMED_SURFACE && obj->Type != _NURBSS)	// �Z���N�V�������ꂽ�G���e�B�e�B���g�����ʂł�NURBS�Ȗʂł��Ȃ��ꍇ
			continue;

		else if(obj->Type == _TRIMMED_SURFACE)				// �Z���N�V�������ꂽ�G���e�B�e�B���g������
			tnurbs[scount] = &body->TrmS[obj->Num];			// �Z���N�V�������ꂽNURBS�Ȗʂ��ꎞ�o�^

		else if(obj->Type == _NURBSS)						// �Z���N�V�������ꂽ�G���e�B�e�B��NURBS�Ȗ�
			tnurbs[scount]->pts = &body->NurbsS[obj->Num];	// �Z���N�V�������ꂽNURBS�Ȗʂ��ꎞ�o�^
		surf_body[scount] = obj->Body;
		surf_type[scount] = obj->Type;						// ���ڒ��̖ʂ̃^�C�v���L��
		surf_objn[scount] = obj->Num;
		scount++;											// �ʂ��J�E���g
		if(scount == 100){
			SetMessage("USER FUNC ERROR: Too Many Surfaces");
			return KOD_ERR;
		}
	}

	// �I�����ꂽ�ʑS�Ă̑g�ݍ��킹�Ō���Z�o
	for(int i=0;i<scount-1;i++){
		for(int j=i+1;j<scount;j++){
			if(surf_body[i] == surf_body[j])	continue;
			fprintf(stderr,"%d-(%d,%d)\n",scount,i,j);
			int num = NFunc.CalcIntersecPtsNurbsSSearch(tnurbs[i]->pts,tnurbs[j]->pts,(int)prop[0],prop[1],Rt,St,5000);	// NURBS�Ȗʓ��m�̌���Z�o
			// �`��
			for(int k=0;k<num;k++){
				flag_r = flag_s = KOD_TRUE;
				if(surf_type[i] == _TRIMMED_SURFACE)								// NurbsR���g������
					flag_r = NFunc.DetermPtOnTRMSurf(tnurbs[i],Rt[k].x,Rt[k].y);	// ���ڒ��̌�_�̃g�����ʓ��O����
				if(surf_type[j] == _TRIMMED_SURFACE)								// NurbsS���g������
					flag_s = NFunc.DetermPtOnTRMSurf(tnurbs[j],St[k].x,St[k].y);	// ���ڒ��̌�_�̃g�����ʓ��O����
				if(flag_r == KOD_TRUE && flag_s == KOD_TRUE){						// NurbsR,NurbsS�����Ƃ�flag==TRUE�̂Ƃ��̂݁A��_�Ƃ��ĕ\��
					Coord p = NFunc.CalcNurbsSCoord(tnurbs[i]->pts,Rt[k].x,Rt[k].y);// uv���W��̓_��xyz���W�l�ɕϊ�
					DrawPoint(p,1,6,green);											// �`��
				}
			}
			fprintf(stderr,"done\n");
		}
	}

	SetMessage("END");

	return KOD_TRUE;
}
