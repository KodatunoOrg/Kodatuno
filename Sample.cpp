// KODATUNO�T���v�����[�N

#include"Sample.h"

// ����  
// *Body:BODY���X�g�̐擪�A�h���X  
// *Obj:�Z���N�V�������ꂽ�G���e�B�e�B�̏��  
// pickcount:�Z���N�V�������ꂽ�G���e�B�e�B��
void SampleMain(BODY *Body,OBJECT *Obj,int pickcount)
{
	BODY *curr;		// ��ƗpBODY
	int i;

	// �Z���N�V�������ꂽ���������[�v
	for(i=0;i<pickcount;i++){
		curr = Body;
		curr = SearchBodyList(curr,Obj[i].Body);		// �Z���N�V�������ꂽBODY��������܂�BODY���X�g�����ǂ�

		if(Obj[i].Type == _TRIMMED_SURFACE){			// �Z���N�V�������ꂽ�G���e�B�e�B���g������
			ContourLine(&curr->NurbsS[Obj[i].Num]);		// ���������Z�o
		}
		
		else if(Obj[i].Type == _NURBSC){				// �Z���N�V�������ꂽ�G���e�B�e�B��NURBS�Ȑ�
			// ������ǉ�
		}
	}
}

// �T���v���Ƃ��ĕ��ʂ�NURBS�Ȑ��̌��(��_�Q)���Z�o����v���O����������
// ���ʂ�Z�����ɃV�t�g�����Ă����A�eZ�ʒu�ł̌���𒀎����߂�
void ContourLine(NURBSS *NurbsS)
{
	NURBS_Func NFunc;				// NURBS�֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Coord t[5000];					// ��
	int   num;						// ���̐�
	int   v_divnum=10;				// NURBS�Ȗʂ̌�_�Q�����߂�ۂ�v�p�����[�^������
	Coord pt=SetCoord(0,0,10);		// ���ʏ��1�_���w��
	Coord nvec=SetCoord(0,0,1);		// ���ʂ̖@���x�N�g�����w��
	double pcolor[3] = {0,1,0};		// �\���̐F
	double tcolor[3] = {1,0,0};

	for(int i=-10;i<20;i++){
		if(i==0) continue;
		pt = SetCoord(0,0,3*(double)i);		// ���ʂ�Z�����ɏ��X�ɃV�t�g���Ă���
		fprintf(stderr,"%.3lf�v�Z��...",pt.z);
		num = NFunc.CalcIntersecPtsPlaneSearch3(NurbsS,pt,nvec,0.5,t,5000);		// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
		fprintf(stderr,"END\n");

		for(int i=0;i<num;i++){		// ��_�̐��������[�v
			if(i%5 == 0){
				Coord p = NFunc.CalcNurbsSCoord(NurbsS,t[i].x,t[i].y);			// ��_���p�����[�^�l������W�l�֕ϊ�
				Coord nt = NFunc.CalcNormVecOnNurbsS(NurbsS,t[i].x,t[i].y);		// ��_��̖@���x�N�g�����v�Z
				nt = MulCoord(nt,-1);											// �O�����@���x�N�g���֕ϊ�
				DrawPoint(p,1,3,pcolor);			// ��_��`��
				DrawVector(p,nt,3,1,tcolor);		// �@���x�N�g����`��
			}
		}
	}

}