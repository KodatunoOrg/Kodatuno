#include "SampleFunc.h"

// Function: Smp3xCAM
// �X�L�����p�X�𐶐�����V���v����3xCAM
//
// Parameter:
// *S - ���H�ʁi��̖ʂ̂݁j
// *C - ��������X�L�����p�X�̃K�C�h�J�[�u
// R - �{�[���G���h�~�����a
// N - �X�L���j���O���C��������(N < 100)
// H - �f�ޏ�ʂ�Z���W
// D - Z�����������i�e���H�p�j
void Smp3xCAM(NURBSS *S,NURBSC *C,double R,int N,double H,int D)
{
    NURBS_Func	nf;                 // NURBS_Func�ւ̃C���X�^���X
    double green[3] = {0,1,0};      // �`�悷��p�X�̐F�i�΁j
    Coord plane_pt;                 // �������镽�ʏ��1�_
    Coord plane_n;                  // �������镽�ʂ̖@���x�N�g��
    Coord path_[2000];              // �ꎞ�i�[�p�o�b�t�@
    Coord ***path = NewCoord3(D+1,N+1,2000);    // �������ꂽ�p�X���i�[
    int ptnum[100];                 // �X�L�������C��1�{���Ƃ̉��H�_�����i�[
    int flag = 0;                   // �W�O�U�O�p�X�������̕����]���p�t���O

    // �K�C�h�J�[�u�ɉ����Đ������ʂ��V�t�g���Ă����C���H�ʂƂ̌�_�Q�����߂Ă���
	for(int i=0;i<=N;i++){
        double t = (double)i/(double)N;
		if(i==0) t += 0.0001;		// ���ٓ_���
		else if(i==N) t-= 0.0001;	// ���ٓ_���
        plane_pt = nf.CalcNurbsCCoord(C,t);     // ���ڒ��̐������ʏ��1�_
        plane_n = nf.CalcTanVecOnNurbsC(C,t);   // ���ڒ��̐������ʂ̖@���x�N�g��
        ptnum[i] = nf.CalcIntersecPtsPlaneSearch(S,plane_pt,plane_n,0.5,3,path_,2000,RUNGE_KUTTA);  // ��_�Q�Z�o
        // ����ꂽ��_�Q���C���H�ʖ@�������ɍH��a���I�t�Z�b�g�������_�𓾂�
		for(int j=0;j<ptnum[i];j++){
            Coord pt = nf.CalcNurbsSCoord(S,path_[j].x,path_[j].y);     // �H��R���^�N�g�_
            Coord n = nf.CalcNormVecOnNurbsS(S,path_[j].x,path_[j].y);  // �@���x�N�g��
            if(n.z < 0)	n = n*(-1);         // �@���x�N�g���̌�������
			path[D][i][j] = pt + n*R;		// �H��a�I�t�Z�b�g
		}
	}

	// �e���H�p�X����
	for(int i=0;i<D;i++){
		for(int j=0;j<N+1;j++){
			for(int k=0;k<ptnum[j];k++){
				double del = (H - path[D][j][k].z)/(double)D;
				double Z = H - del*(double)i;
				path[i][j][k] = SetCoord(path[D][j][k].x,path[D][j][k].y,Z);
			}
		}
	}

	// path�`��
	for(int i=0;i<D+1;i++){
		for(int j=0;j<N+1;j++){
			for(int k=0;k<ptnum[j];k++){
				DrawPoint(path[i][j][k],1,1,green);
			}
		}
	}

    // NC�R�[�h����(�e�����͊e���ŕύX��������)
	double ret = H + 10;		// ���g���N�gZ
	FILE *fp = fopen("demo.nc","w");
	fprintf(fp,"G92X0.0Y0.0Z0.0\n");
	fprintf(fp,"S4000M03\n");
	fprintf(fp,"F500\n");
	for(int i=0;i<D+1;i++){
		fprintf(fp,"G00X%.2lfY%.2lfZ%.2lf\n",path[i][0][0].x,path[i][0][0].y,ret);
		for(int j=0;j<N+1;j++){
			if(flag == 0){
				for(int k=0;k<ptnum[j];k++){
					fprintf(fp,"G01X%.2lfY%.2lfZ%.2lf\n",path[i][j][k].x,path[i][j][k].y,path[i][j][k].z);
				}
			}
			else{
				for(int k=ptnum[j]-1;k>=0;k--){
					fprintf(fp,"G01X%.2lfY%.2lfZ%.2lf\n",path[i][j][k].x,path[i][j][k].y,path[i][j][k].z);
				}
			}
			flag = 1 - flag;
		}
		fprintf(fp,"G01Z%.2lf\n",ret);
	}
	fprintf(fp,"G01Z%.2lf\n",ret);
	fprintf(fp,"M05\n");
	fprintf(fp,"M30\n");

	fclose(fp);
}
