#include"Kodatuno.h"


//////////////////////////////////////////////////////
// Kodatuno�R�}���h���s�֐��͈ȍ~�ɋL�q���Ă������� //
//////////////////////////////////////////////////////

// Kodatuno�R�}���h(�o�[�W�������o��)
void CmdVerInf(int argc,char *argv[])
{
	char *p;

	// �I�v�V�����Ȃ�
	if(!argc){
		SetMessage("Kodatuno R1.2");
		return;
	}

	// �I�v�V��������
	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'f':		// "-f"�I�v�V�����Ńt���l�[���o��
				SetMessage("Kodatuno is Open Developed Alternative Trajectory Utility Nucleus Obuject");
				break;
			case 'F':		// "-F (���l)"�ł��̐��l����������o��(�Ӗ��Ȃ�)
				if(argc==1) return;
				argc--; argv++;
				for(int i=0;i<atoi(argv[0]);i++){
					char mes[256];
					sprintf(mes,"%d:Kodatuno 2.0",i+1);
					SetMessage(mes);
				}
				break;
			}
		}while(*++p);
		argv++; argc--;
	}

}

// Kodatuno�R�}���h(�t�@�C���I�[�v��)
void CmdFileOpen(int argc,char *argv[])
{
	char *p;

	// �I�v�V�����Ȃ�
	if(!argc){
		Kodatuno.OpenFile();
	}

	// �I�v�V��������
	else{
		argv++;
		while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
			do{
				switch(*p){
			case 'N':		// "-N (�t�@�C����)"�ł��̃t�@�C�����I�[�v��
				if(argc==1) return;
				argc--; argv++;
				Kodatuno.OpenFile(argv[0]);
				break;
				}
			}while(*++p);
			argv++; argc--;
		}
	}

	Describe_Form->redraw();
}

// UV�p�����[�^�ŕ������ꂽ���C���[�t���[����\��
void CmdUVWire(int argc,char *argv[])
{
	Kodatuno.UVWireFlameViewFlag = KOD_TRUE;
}

// �X�P�[���ύX
void CmdChScale(int argc,char *argv[])
{
	char *p;

	// �I�v�V�����Ȃ�
	if(!argc)
		Kodatuno.GetModelScale();

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (���l)"�ŁA���f���X�P�[�����Z�b�g
			case 'r':
				if(argc==1) return;
				argc--; argv++;
				Kodatuno.SetModelScale(atof(argv[0]));
				break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// �g�������X�ύX
void CmdChTolerance(int argc, char *argv[])
{
	char *p;

	if(!argc)
		Kodatuno.GetTolerance();

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (���l)"�ŁA�g�������X���Z�b�g
			case 'r':
				if(argc==1) return;
				argc--; argv++;
				Kodatuno.SetTolerance(atof(argv[0]));
				break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// Body�̕��s�ړ�
void CmdMoveBody(int argc,char *argv[])
{
	char *p;

	// �I�v�V�����Ȃ�
	if(!argc){
		SetMessage("Command Error: Set -r dx dy dz options");
		return;
	}

	double d[3] = {0,0,0};	// �ړ��ʊi�[�p

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (���l)"�ŁA�ړ��ʂ��Z�b�g
			case 'r':
				if(argc==1) return;
				for(int i=0;i<3;i++){
					argc--; argv++;
					d[i] = atof(argv[0]);
				}
				Kodatuno.GetShiftBody(SetCoord(d[0],d[1],d[2]));
			break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// Body�̉�]
void CmdRotateBody(int argc,char *argv[])
{
	char *p;

	// �I�v�V�����Ȃ�
	if(!argc){
		SetMessage("Command Error: Set -r x y z deg options");
		return;
	}

	double axd[4] = {0,0,0,0};	// ��]��,��]�p

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (���l)"�ŁA�ړ��ʂ��Z�b�g
			case 'r':
				if(argc==1) return;
				for(int i=0;i<4;i++){
					argc--; argv++;
					axd[i] = atof(argv[0]);
				}
				Kodatuno.GetRotateBody(SetCoord(axd[0],axd[1],axd[2]),axd[3]);
			break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// �R���g���[���|�C���g��`��
void CmdCPView(int argc,char *argv[])
{
	Kodatuno.CPViewFlag = KOD_TRUE;
}

// �Ȗʏ����o��
void CmdSurfInfo(int argc,char *argv[])
{
	Kodatuno.GetSurfInfo();
}

// ��]�T�[�t�F�X�����R�}���h
void CmdGenRotSurf(int argc,char *argv[])
{
	RotSurf_Dlg->setVisible(True);		// ��]�T�[�t�F�X�����_�C�A���O��\��
}

// �X�C�[�v�T�[�t�F�X�����R�}���h
void CmdGenSweepSurf(int argc,char *argv[])
{
	SweepSurf_Dlg->setVisible(True);	// �X�C�[�v�T�[�t�F�X�����_�C�A���O��\��
}

// Nurbs�Ȑ������R�}���h
void CmdGenNurbsCurve(int argc,char *argv[])
{
	NurbsCurve_Dlg->setVisible(True);
}

// Nurbs�Ȗʐ����R�}���h
void CmdGenNurbsSurface(int argc,char *argv[])
{
	NurbsSurface_Dlg->setVisible(True);
}

// Body�̊g��
void CmdExpand(int argc,char *argv[])
{
	char *p;

	// �I�v�V�����Ȃ�
	if(!argc){
		SetMessage("Command Error: Set -r x y z deg options");
		return;
	}

	double axd[3] = {0,0,0};	// �e�������g�嗦

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (���l)"�ŁA�ړ��ʂ��Z�b�g
			case 'r':
				if(argc==1) return;
				for(int i=0;i<3;i++){
					argc--; argv++;
					axd[i] = atof(argv[0]);
				}
				Kodatuno.ExpandBody(SetCoord(axd[0],axd[1],axd[2]));
			break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// NURBS Rnak�̕ύX
void CmdChRank(int argc, char *argv[])
{
	char *p;

	// �I�v�V�����Ȃ�
	if(!argc){
		SetMessage("Command Error: Set -r (u-val) (v-val)");
		return;
	}

	int r[2];

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (���l)"�ŁA�g�������X���Z�b�g
			case 'r':
				if(argc==1) return;
				for(int i=0;i<2;i++){
					argc--; argv++;
					r[i] = atoi(argv[0]);
				}
				Kodatuno.ChangeRank(r);
				break;
			}
		}while(*++p);
		argv++; argc--;
	}
}