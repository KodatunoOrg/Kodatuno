/*************************
* STL�t�@�C����ǂݍ���  *
**************************/

#include "stdafx.h"
#include "STL_Parser.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

// Function: STL_Parser_Main
// STL�p�[�T���C��
//
// Parameter:
// *body - ���̂��\������G���e�B�e�B�̏W���I�u�W�F�N�g�ւ̃|�C���^
// TypeNum[] - �e�G���e�B�e�B�̐����i�[�����
//
// Return:
// �����FKOD_TRUE, �t�@�C���I�[�v���G���[�FKOD_ERR
int STL_PARSER::STL_Parser_Main(BODY *body, const char *STL_fname)
{
	FILE *fp;
	NURBS_Func nfunc;
	char buf[BUFSIZEMAX_STL];		// ������ꎞ�i�[�p�o�b�t�@
	char label[LABELSIZEMAX_STL];	// ���x��������ꎞ�i�[�p�o�b�t�@
	char mes[BUFSIZEMAX_STL];		// �o�͗p���b�Z�[�W�i�[�o�b�t�@
	int facet_num=0;			// �t�@�Z�b�g�̑���
	double maxval = -1.0E+6;	// ���W�l�̍ő�l���i�[

	// �t�@�Z�b�g��NURBS�ŕ\�����邽�߂̊e��p�����[�^���Z�b�g
	int K[2] = {2,2};							// �R���g���[���|�C���g�̐���2
	int N[2] = {4,4};							// �m�b�g�x�N�g���̐���4
	int M[2] = {2,2};							// �K����2
	double U[2] = {0,1};						// u�����̃m�b�g�x�N�g���̊J�n�l�ƏI���l
	double V[2] = {0,1};						// v�����m�b�g�x�N�g���̊J�n�l�ƏI���l
	Vector S = NewVector(N[0]);					// u�����̃m�b�g�x�N�g��
	Vector T = NewVector(N[1]);					// v�����m�b�g�x�N�g��
	Matrix W = NewMatrix(K[0],K[1]);			// �E�G�C�g
	Coord **facet = NewCoord2(2,2);				// �R���g���[���|�C���g
	S[0] = S[1] = T[0] = T[1] = 0;				// u�����̃m�b�g�x�N�g���͕��ʂȂ̂Ŋ��Ɍ��܂��Ă���
	S[2] = S[3] = T[2] = T[3] = 1;				// v�����m�b�g�x�N�g���͕��ʂȂ̂Ŋ��Ɍ��܂��Ă���
	W[0][0] = W[0][1] = W[1][0] = W[1][1] = 1;	// �E�G�C�g�͕��ʂȂ̂Ŋ��Ɍ��܂��Ă���

	// STL�t�@�C���I�[�v��
	if((fp = fopen(STL_fname,"r")) == NULL){
		sprintf(mes,"KOD_ERROR: Cannot open %s",STL_fname);
        GuiIF.SetMessage(mes);
		return(KOD_ERR);
	}
//	sprintf(mes,"Open %s",STL_fname);
//	GuiIF.SetMessage(mes);

	// �܂��t�@�Z�b�g�̑����𐔂���
	while(fgets(buf,sizeof(buf),fp) != NULL){
		sscanf(buf,"%s",label);							// ���x�����o
		if(!strncmp(label,"facet",LABEL_FASET_SIZE))	// ���x������"facet"�Ȃ�
			facet_num++;								// �t�@�Z�b�g�����C���N�������g
	}
//	sprintf(mes,"Total facet number is %d.",facet_num);
//	GuiIF.SetMessage(mes);

	fseek(fp,0L,SEEK_SET);		// �t�@�C���擪�ɖ߂�

//	body->NurbsS = (NURBSS *)malloc(sizeof(NURBSS) * facet_num);	// �t�@�Z�b�g����NURBS�Ȗʂ��������[�m��
	body->NurbsS = new NURBSS[facet_num];	// �t�@�Z�b�g����NURBS�Ȗʂ��������[�m��
	body->TypeNum[_NURBSS] = facet_num;

	// ���W�l�ǂݍ���
	int j=0;
	while(fgets(buf,sizeof(buf),fp) != NULL){
		sscanf(buf,"%s",label);
		if(!strncmp(label,"outer",LABEL_OUTER_SIZE)){
			int m=0,n=0;
			for(int i=0;i<VERTEXNUM;i++){	// �t�@�Z�b�g���_���W�ǂݍ���
				fgets(buf,sizeof(buf),fp);
				sscanf(buf,"%s %lf %lf %lf",label,&facet[m][n].x,&facet[m][n].y,&facet[m][n].z);
				double d = CalcEuclid(facet[m][n]);
				if(maxval < d) maxval = d;			// �\���p�ɍ��W�̍ő�l�𒲂ׂ�
				// Add by K.Magara
				body->minmaxCoord[0].SetMinCoord(facet[m][n]);
				body->minmaxCoord[1].SetMaxCoord(facet[m][n]);
				// --
				n++;
				if(n == UVCPNUM){
					n=0;
					m++;
				}
			}
			// NURBS�Ȗʂŕ��ʂ�\������ꍇ�A�_��4�K�v�����A�O�p�p�b�`�̏ꍇ��3�_�����Ȃ����߁A������_�ǉ����Ȃ���΂Ȃ�Ȃ��B
			// �Ƃ肠�����O�p�p�b�`��3�_�ڂƓ������W�l��4�_�ڂƂ��Ă݂��B--> �\���������B
			facet[UVCPNUM-1][UVCPNUM-1] = SetCoord(facet[UVCPNUM-1][0]);
			nfunc.GenNurbsS(&body->NurbsS[j],M[0],M[1],K[0],K[1],S,T,W,facet,U[0],U[1],V[0],V[1]);			// NURBS�t�@�Z�b�g����
			body->NurbsS[j].TrmdSurfFlag = KOD_FALSE;							// �g�����̂Ȃ��P����NURBS�Ȗʂł��邱�Ƃ𖾎�
			body->ChangeStatColor(body->NurbsS[j].Dstat.Color,0.2,0.2,1,0.5);		// �����F��ɃZ�b�g
			j++;
		}
	}

	body->MaxCoord = maxval;	// �ő���W�l��o�^
			
	FreeVector(S);
	FreeVector(T);
	FreeMatrix(W,K[0]);
	FreeCoord2(facet,UVCPNUM);

	fclose(fp);

	return KOD_TRUE;
}

// Function: ***
// ***