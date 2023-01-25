// Kodatuno���C���N���X
// BODY�`��A�C���^�[�t�F�[�X���`

#include "Kodatuno.h"

// Kodatuno�t�H�[���̏���������
void KODatUNO::InitializeWin(int width, int height)
{

    GuiIF.SetMessage("Welcome to Kodatuno!");			// �R���\�[��������
    std::string mes = GuiIF.GetConsoleText();
    SetConsoleCharNum(mes.size());			// ���݂̃R���\�[����������ۑ�

    ScreenWidth = width;		// �`��E�B���h�E�̕����i�[
    ScreenHeight = height;	// �`��E�B���h�E�̍������i�[

	// �R���\�[���R�}���h��o�^
	SetCmdList(CmdMap,"verinf","fF",0,CmdVerInf);			// Command �o�[�W���������o��
	SetCmdList(CmdMap,"open","N",1,CmdFileOpen);			// Command �t�@�C���I�[�v���_�C�A���O���I�[�v��
	SetCmdList(CmdMap,"wire","",2,CmdUVWire);				// Command Body�����C���[�t���[���\���ɂ���
	SetCmdList(CmdMap,"scale","r",3,CmdChScale);			// Command �X�P�[���̊m�F�y�ѕύX
	SetCmdList(CmdMap,"rots","",4,CmdGenRotSurf);			// Command ��]�T�[�t�F�X�𐶐�����
	SetCmdList(CmdMap,"sweeps","",5,CmdGenSweepSurf);		// Command �X�C�[�v�T�[�t�F�X�𐶐�����
	SetCmdList(CmdMap,"nurbsc","",6,CmdGenNurbsCurve);		// Command �^����ꂽ�_�Q����NURBS�Ȑ��𐶐�����
	SetCmdList(CmdMap,"nurbss","",7,CmdGenNurbsSurface);	// Command �^����ꂽ�_�Q����NURBS�Ȗʂ𐶐�����
	SetCmdList(CmdMap,"tol","r",8,CmdChTolerance);			// Commnad �ʕ`��̃g�������X�l�̊m�F�y�ѕύX
	SetCmdList(CmdMap,"move","r",9,CmdMoveBody);			// Commnad �I������Ă���Body���w�肵���ړ��ʂ������s�ړ�����
	SetCmdList(CmdMap,"rot","r",10,CmdRotateBody);			// Command �I������Ă���Body���w�肵���p�x������]����
	SetCmdList(CmdMap,"cpview","",11,CmdCPView);			// Command �R���g���[���|�C���g��`��
	SetCmdList(CmdMap,"surfinfo","",12,CmdSurfInfo);		// Command �Ȗʏ����o��
	SetCmdList(CmdMap,"exp","r",13,CmdExpand);				// Command Body���g�傷��
	SetCmdList(CmdMap,"chrank","r",14,CmdChRank);			// Command �w�肵��NURBS�Ȗʂ�Rank��ύX����
	SetCmdList(CmdMap,"chbkcol","r",15,CmdChBkCol);			// Command �w�i�̐F��ύX����
	SetCmdList(CmdMap,"meshinf","",16,CmdMeshInf);			// Command �w�肵��Mesh�̏����o��
	SetCmdList(CmdMap,"uvdir","",17,CmdUVdir);				// Command U,V�̕�����\������

	// User�R�}���h�̓o�^
	User.RegistUserCommand(CmdMap);

	User.SetMenuLabelName();	// User���j���[����o�^
    GuiIF.SetUserFunc1Label(User.GetMenuLabelName(0));
    GuiIF.SetUserFunc2Label(User.GetMenuLabelName(1));
    GuiIF.SetUserFunc3Label(User.GetMenuLabelName(2));
    GuiIF.SetUserFunc4Label(User.GetMenuLabelName(3));
	GuiIF.SetUserFunc5Label(User.GetMenuLabelName(4));
	GuiIF.SetUserFunc6Label(User.GetMenuLabelName(5));
	GuiIF.SetUserFunc7Label(User.GetMenuLabelName(6));
	GuiIF.SetUserFunc8Label(User.GetMenuLabelName(7));

	// ���̑�������
	Focus_Body = KOD_ERR;
	OpenGLInitFlag = KOD_FALSE;
	LBtnFlag = KOD_FALSE;
	RBtnFlag = KOD_FALSE;
	MBtnFlag = KOD_FALSE;
	ShiftKeyFlag = KOD_FALSE;
	CtrlKeyFlag = KOD_FALSE;
	AltKeyFlag = KOD_FALSE;
	SweepSelectFlag = KOD_FALSE;
	SolidViewFlag = KOD_TRUE;
	EdgeViewFlag = KOD_TRUE;
	WireFlameViewFlag = KOD_FALSE;
	BlendViewFlag = KOD_FALSE;
	CPViewFlag = KOD_FALSE;
	UVDirFlag = KOD_FALSE;
	InitCmdFlags();
	CmdStackNum = 0;
	CmdStackNow = 0;
	StartX = 0;
	StartY = 0;
	OldPosX = 0;
	OldPosY = 0;
	CurrentX = 0;
	CurrentY = 0;
	AxisScale = 0.2;
	ModelScale = 1;
	ModelScale1st = 1;
	StartQ = QFunc.QInit(1.0,0,0,0);
	TargetQ = QFunc.QInit(1.0,0,0,0);
	QFunc.QtoR(RotMx,StartQ);
	Vp[0] = Vp[1] = Vp[2] = Vp[3] = 0;
	Trl[0] = Trl[1] = Trl[2] = 0.0;
	DrawBODYFlag = KOD_FALSE;
	DrawSurfFlag = KOD_TRUE;
	ReDrawBODYFlag = KOD_FALSE;
	for(int i=0;i<USERFUNCNUMMAX;i++)
		ExecUserFuncFlag[i] = KOD_FALSE;
	for(int i=0;i<USERCOMMANDNUM;i++)
		ExecUserCommandFlag[i] = KOD_FALSE;
	DoSelectFlag = KOD_FALSE;
	Tolerance = TOLERANCE;
	BkCol[0] = BkCol[1] = BkCol[2] = 0;

	glGenLists(USERFUNCNUMMAX+1);

}

// Kodatuno�t�H�[���̏I������
void KODatUNO::DeleteWin()
{
	int bodynum = BodyList.getNum();				// �ǂݍ���BODY�̐��𓾂�

	for(int i=0;i<bodynum;i++){
		BODY *body = (BODY *)BodyList.getData(i);	// i�Ԗڂ�BODY��I��
		body->DelBodyElem();						// BODY���Ŋm�ۂ��Ă��郁�����[�̉��
		delete (BODY *)body;						// BODY���g�̃������[���
	}
	BodyList.clear();		// BODY���X�g���g������

	exit(0);
}

// OpenGL�`��̈�̍ĕ`�掞�Ɏ��s�����(OpenGL���p�ӂ���API�͂��̊֐��ɂԂ牺�����Ă��Ȃ���΂Ȃ�Ȃ�)
void KODatUNO::ReDraw(int x,int y)
{
	SetOpenGLStat();				// �`��X�e�[�^�X�̐ݒ�

	// �Z���N�V�����g���K
	if(DoSelectFlag == KOD_TRUE){
		DoSelect(x,y);				// �Z���N�V�����������s
		DoSelectFlag = KOD_FALSE;	// �Z���N�V�����g���KOFF
		ReDrawBODYFlag = KOD_FALSE;	// �ĕ`��
	}

	// �������\���ؑ�
	if(BlendViewFlag == KOD_TRUE){
		glDepthMask(GL_FALSE);
		glDisable(GL_BLEND); 
		Describe_Main();				// BODY�`��
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);  
		Describe_Main();				// BODY�`��
	}
	else{
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND); 
		Describe_Main();				// BODY�`��
	}

	// �X�C�[�v�Z���N�V�����t���O��ON�Ȃ�`��
	if(SweepSelectFlag == KOD_TRUE){
		glMatrixMode(GL_PROJECTION);				// ���e�ϊ��s��X�^�b�N��I��
		glPushMatrix();								// ���݂̍��W�n��ۑ�
		glLoadIdentity();							// ���e�ϊ��s��X�^�b�N�̐擪�ɒP�ʍs���}��
		gluOrtho2D(0,ScreenWidth,ScreenHeight,0); 	// ���_��ݒ�(���o�[�o���h�𐳂����ʒu�ɕ\�������邽��)

		glMatrixMode(GL_MODELVIEW);					// ���f���r���[�s��
		glPushMatrix();								// ���݂̍��W�n��ۑ�
		glLoadIdentity();							// ���f���r���[�s��̐擪�ɒP�ʍs���}��
		
		DrawRubberband(StartX,StartY,CurrentX,CurrentY);		// ���o�[�o���h��`��

		// ���W�n�����ɖ߂�
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	// �R�}���h���s�ɂ��`��
	if(UVWireFlameViewFlag == KOD_TRUE)				// ���C���[�t���[���̏ڍו`��
		UVWireView();
	if(CPViewFlag == KOD_TRUE)						// �R���g���[���|�C���g�̕`��
		DescribeCP();
	if(UVDirFlag == KOD_TRUE)						// U,V�����̕`��
		DispUVdirection();

}

// BODY�`�惁�C��
void KODatUNO::Describe_Main()
{
	if(DrawBODYFlag != KOD_TRUE) return;

	// BODY�`��t���O��ON�Ȃ�`��
	if(ReDrawBODYFlag == KOD_FALSE){							// ��ԏ���̂݃f�B�X�v���C���X�g��BODY��o�^
		SetNurbsSTolerance(Tolerance);							// �ĕ`�扉�Z�w�߂��󂯂��ꍇ�A���݂̃��f���X�P�[���ɍ�����Nurbs�g�������X���w��
		glNewList(COMMAND_DRAW_BODY,GL_COMPILE_AND_EXECUTE);	// glDrawBODY()���R�}���h�Ƃ��ă������[�ɕۑ�(�Z���N�V���������̍�����)
		DrawBODY();												// BODY�f�[�^��`��
		glEndList();
		ReDrawBODYFlag = KOD_TRUE;
	}
	else{
		glCallList(COMMAND_DRAW_BODY);			// 2��ڈȍ~�̕`��Ăяo���̓f�B�X�v���C���X�g���Ăяo��
	}
}

// BODY�f�[�^�̕`��
void KODatUNO::DrawBODY()
{
	int i;
	BODY *body;

	// body�`��
	for(int k=0;k<BodyList.getNum();k++){					// ���݃��X�g�ɓo�^����Ă���BODY�̐��������[�v
		if((body = (BODY *)BodyList.getData(k)) != NULL){	// k�Ԗڂ̃��X�g�ɓo�^����Ă���BODY�̃f�[�^���擾
			glPushName(k);									// �Z���N�V�����o�b�t�@���K�w��
			// �e�^�C�v���Ƃɕ`������s���Ă���(�����\����NURBS�Ɍ��肵���̂ŁA������~�Ȃǂ͕`�悵�Ȃ�)
			// 2012/4  MESH��ǉ�
			for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){

				if(i == _CIRCLE_ARC){						// �~�E�~��
					// �~�E�~�ʂ�NRBS�Ȑ��ɕϊ������
					//glPushName(_CIRCLE_ARC);				// �Z���N�V�����o�b�t�@���K�w��
					//Draw_CircleArcs(body);
					//glPopName();
				}
				else if(i == _CONIC_ARC){					// �~���Ȑ�
					//glPushName(_CONIC_ARC);				// �Z���N�V�����o�b�t�@���K�w��
					//Draw_ConicArc();
					//glPopName();
				}
				else if(i == _LINE){						// ����
					// ������NURBS�Ȑ��ɕϊ������
					//glPushName(_LINE);					// �Z���N�V�����o�b�t�@���K�w��
					//Draw_Lines(body);
					//glPopName();
				}
				else if(i == _NURBSC){						// NURBS�Ȑ�
					if(EdgeViewFlag == KOD_TRUE){			// Edge�\��/��\���؂�ւ�
						glPushName(_NURBSC);				// �Z���N�V�����o�b�t�@���K�w��
						Draw_NurbsCurve(body);
						glPopName();
					}
				}
				else if(i == _NURBSS){
					glPushName(_NURBSS);
					Draw_NurbsSurface(body);
					glPopName();
				}
				else if(i == _TRIMMED_SURFACE){				// �g������(NURBS�Ȗ�)
					glPushName(_TRIMMED_SURFACE);
					Draw_TrimSurfe(body);
					glPopName();
				}
				else if(i == _MESH){						// ���b�V��
					glPushName(_MESH);
					Draw_Mesh(body);
					glPopName();
				}
			}
			glPopName();
		}
	}
}


// NURBS�Ȑ��̕`��
void KODatUNO::Draw_NurbsCurve(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSC];i++){
		glPushName(i);		// �l�[���X�^�b�N�̐擪��i��}��
		glColor3f(Curr_body->NurbsC[i].Dstat.Color[0],Curr_body->NurbsC[i].Dstat.Color[1],Curr_body->NurbsC[i].Dstat.Color[2]);
		if(Curr_body->NurbsC[i].EntUseFlag == GEOMTRYELEM){				// IGES�f�B���N�g������"Entity Use Flag"��0�̏ꍇ�͎��ۂ̃��f���v�f�Ƃ��ĕ`�悷��
			DrawNurbsCurve(Curr_body->NurbsC[i]);						// �`��
		}
		glPopName();		// �l�[���X�^�b�N�̐擪���폜
	}
}

// NURBS�Ȗʂ̕`��
void KODatUNO::Draw_NurbsSurface(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSS];i++){
		if(Curr_body->NurbsS[i].TrmdSurfFlag == KOD_TRUE)	// �g�����ʂƂ���NURBS�Ȗʂ��o�^����Ă���Ȃ�
			continue;		// �`�悵�Ȃ�
		else{
			glPushName(i);
			DrawNurbsSurfe(Curr_body->NurbsS[i]);	// NURBS�Ȗʕ`��
			glPopName();
		}
	}
}


// �g�����ʂ̕`��
void KODatUNO::Draw_TrimSurfe(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_TRIMMED_SURFACE];i++){
		glPushName(i);			// �l�[���X�^�b�N�̐擪��i��}��
		DrawTrimdSurf(Curr_body->TrmS[i]);
		glPopName();			// �l�[���X�^�b�N�̐擪���폜
	}
}

// ���b�V���̕`��
void KODatUNO::Draw_Mesh(BODY *body)
{
	for(int i=0;i<body->TypeNum[_MESH];i++){
		DrawMesh(body->Mesh,WireFlameViewFlag);
	}
}

// ���`��
void KODatUNO::DrawAxis(double Ascale,double Mscale)
{
	GLdouble AxisPos = Ascale/Mscale;
	GLdouble CharPos = (Ascale+0.01)/Mscale;

	glDisable(GL_LIGHTING);

	glLineWidth(1);
	glBegin(GL_LINES);
	//draw Xaxis�@��
	glColor3f(1.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(AxisPos, 0.0, 0.0);

	//draw Yaxis�@��
	glColor3f(0.0, 1.0, 0.0 );
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, AxisPos, 0.0);

	//draw Zaxis�@��
	glColor3f(0.0, 0.0, 1.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, AxisPos);
	glEnd();

	/* Draw Xcone */
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(AxisPos, 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4,1);
	glPopMatrix();

	/* Draw Ycone */
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, AxisPos, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4,1);
	glPopMatrix();

	/* Draw Zcone */
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, AxisPos);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4,1);
	glPopMatrix();

	// ���� X
	glColor3d(1.0,0.0,0.0);
	glRasterPos3d(CharPos,0.0,0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,'X');

	// ���� Y
	glColor3d(0.0,1.0,0.0);
	glRasterPos3d(0.0,CharPos,0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,'Y');

	// ���� Z
	glColor3d(0.0,0.0,1.0);
	glRasterPos3d(0.0,0.0,CharPos);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,'Z');

	glEnable(GL_LIGHTING);
}

// ���o�[�o���h�`��
void KODatUNO::DrawRubberband(double sx,double sy,double ex,double ey)
{
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glLineStipple(1, 0xE0E0);
	glColor3f(0.2, 0.9, 0.9);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINE_STRIP);
	glVertex2d(sx,sy);
	glVertex2d(ex,sy);
	glVertex2d(ex,ey);
	glVertex2d(sx,ey);
	glVertex2d(sx,sy);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glEnable(GL_LIGHTING);
}

// �R���\�[����������ۑ�
void KODatUNO::SetConsoleCharNum(int len)
{
	ConsoleCharNum = len;
}

// �R���\�[�����������o��
int KODatUNO::GetConsoleCharNum()
{
	return ConsoleCharNum;
}

// 3D�f�[�^�t�@�C���̓ǂݍ���
int KODatUNO::OpenFile()
{
    GuiIF.SetMessage("Reading...");
    std::string full_name = GuiIF.OpenBodyFileDlg();		// �_�C�A���O�I�[�v���C�t�@�C���̃t���p�X�擾
	if(full_name != ""){
	    std::string fname = GuiIF.GetFileName(full_name);	// �t�@�C�����擾
		std::string ext = GuiIF.GetFileExt(full_name);		// �g���q���擾

	    BODY *body = new BODY;								// BODY��1�������[�m��

		std::string ext_igs("igs");
		std::string ext_IGS("IGS");
		std::string ext_stl("stl");
		std::string ext_STL("STL");
		std::string ext_vrml("wrl");
		std::string ext_VRML("WRL");
		std::string ext_dxf("dxf");
		std::string ext_DXF("DXF");

		int flag;
		if(ext == ext_igs || ext == ext_IGS){		// �g���q��"igs" or "IGS"
			IGES_PARSER Iges;
			flag = Iges.IGES_Parser_Main(body,full_name.c_str());	// IGES�f�[�^��ǂݍ���ŁAbody�Ɋi�[
			if(flag == KOD_TRUE){
				Iges.Optimize4OpenGL(body);					// �ǂݍ���IGES�t�@�C����OpenGL�p�ɍœK������
			}
		}
		else if(ext == ext_stl || ext == ext_STL){	// �g���q��"stl" or "STL"
			STL_PARSER Stl;
			flag = Stl.STL_Parser_Main(body,full_name.c_str());
		}
		else if(ext == ext_vrml || ext == ext_VRML){		// �g���q��"wrl" or �g���q��"WRL"
			VRML_PARSER Vrml;
			flag = Vrml.Vrml_Parser_Main(body,full_name.c_str());
		}
		else if(ext == ext_dxf || ext == ext_DXF){
			DXF_PARSER Dxf;
			flag = Dxf.DXF_Parser_Main(body,full_name.c_str());
		}
		else{
			char mes[256];
			sprintf(mes,"Wrong filename : \"%s\" " ,full_name.c_str());
			GuiIF.SetMessage(mes);
			flag = KOD_ERR;
		}

		if(flag == KOD_ERR){
			delete body;					// �G���[�̏ꍇ��BODY�̃������[���
			GuiIF.SetMessage("This file was not able to be read normaly.");
			return KOD_ERR;
		}

		strcpy(body->Name,fname.c_str());       // �t�@�C������body���Ƃ��ēo�^
		body->Mom = BodyList.add(body);         // ���X�g�ɓǂݍ���body��o�^
		GuiIF.AddBodyNameToWin(fname.c_str());	// Body���X�g�E�B���h�E�ɐV���ɓǂݍ���BODY����t��
		GuiIF.SetMessage("Finished");
	}

	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾

	SetModelScale();						// ���f���X�P�[�����œK��

	return KOD_TRUE;
}

// 3D�f�[�^�t�@�C���̓ǂݍ���(�I�[�o�[���[�h)
int KODatUNO::OpenFile(char *Fname)
{/*
    std::string fname(Fname);
	fname.to_lower();							// �t�@�C������S�ď������ɕϊ�
	Body_List_Win->addItem(fname);				// Body���X�g�E�B���h�E�ɐV���ɓǂݍ���BODY����t��
	int priodnum = fname.getWords(".");				// �t�@�C�����Ɋ܂܂�Ă���s���I�h�̐��𒲂ׂ�
    std::string ext = fname.getWord(priodnum-1,".");	// �g���q���擾

	BODY *body = new BODY;						// BODY��1�������[�m��

    std::string ext_igs("igs");
    std::string ext_stl("stl");

	int flag;
	if(ext == ext_igs){				// �g���q��"igs"
		IGES_PARSER Iges;
		flag = Iges.IGES_Parser_Main(body,fname);		// IGES�f�[�^��ǂݍ���ŁAbody�Ɋi�[
		if(flag == KOD_TRUE){
			Iges.Optimize4OpenGL(body);					// �ǂݍ���IGES�t�@�C����OpenGL�p�ɍœK������
		}
	}
	else if(ext == ext_stl){		// �g���q��"stl"
		STL_PARSER Stl;
		flag = Stl.STL_Parser_Main(body,fname);
	}
	if(flag == KOD_ERR){
		delete body;					// �G���[�̏ꍇ��BODY�̃������[���
        GuiIF.SetMessage("This file was not able to be read normaly.");
		return KOD_ERR;
	}
	strcpy(body->Name,fname);				// �t�@�C������body���Ƃ��ēo�^
	body->Mom = BodyList.add(body);			// ���X�g�ɓǂݍ���body��o�^
	Body_List_Win->addItem(fname);			// Body���X�g�E�B���h�E�ɐV���ɓǂݍ���BODY����t��
    GuiIF.SetMessage("Finished");

	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾

	SetModelScale();						// ���f���X�P�[�����œK��

	OpenDelBtn();
*/
    return KOD_TRUE;
}

// �C���v�b�g�t�@�C���̓ǂݍ���
std::string KODatUNO::OpenInpFile()
{
    std::string full_name = GuiIF.OpenInpFileDlg();	// �t�@�C���̃t���p�X�擾
	return full_name;	
}


// �I������Ă���BODY������
void KODatUNO::DeleteBody()
{
	if(!SeldEntList.getNum() && Focus_Body == KOD_ERR)	return;			// �Z���N�V����,���X�g�E�B���h�E�I������Ă��Ȃ��ꍇ�͉������Ȃ�

	int buf=KOD_ERR;
	int delnum[BODYLISTMAX];
	int delcount=0;
	BODY *body;
	OBJECT *obj;


	// �Z���N�V��������Ă��Ȃ����A���X�g�E�B���h�E��ł͑I������Ă���ꍇ
	if(!SeldEntList.getNum()){
		body = (BODY *)BodyList.getData(Focus_Body);		// ���X�g�E�B���h�E��őI������Ă���BODY�ԍ������������Ă���
		DeleteBodySub(body,Focus_Body);						// BODY�폜
		Focus_Body = KOD_ERR;								// ���X�g�E�B���h�E�łȂɂ��t�H�[�J�X����Ă��Ȃ���Ԃɖ߂�
	}

	// �Z���N�V��������Ă���ꍇ,
	// �Z���N�V��������Ă���G���e�B�e�B����A�ǂ�BODY���I������Ă��邩���o
	for(int i=0;i<SeldEntList.getNum();i++){
		obj = (OBJECT *)SeldEntList.getData(i);
		if(buf != obj->Body){
			buf = obj->Body;
			delnum[delcount] = obj->Body;
			delcount++;
		}
	}
	BubbleSort(delnum,delcount);

	for(int i=delcount-1;i>=0;i--){
		body = (BODY *)BodyList.getData(delnum[i]);
		DeleteBodySub(body,delnum[i]);
	}

	ClearSeldEntList();

	ReDrawBODYFlag = KOD_FALSE;		// �ĕ`��w��
}

// �I������Ă���BODY������Sub
void KODatUNO::DeleteBodySub(BODY *body,int n)
{
	if (body == NULL) return;

	char mes[256];
	sprintf(mes,"%s was deleted.",body->Name);
    GuiIF.SetMessage(mes);

    BodyList.delData(n);			// ���X�g����͂���
    GuiIF.DelBodyFromWin(n);		// ���X�g�E�B���h�E����͂���
	body->DelBodyElem();			// BODY���Ŋm�ۂ��Ă��郁�����[�̉��
	delete (BODY *)body;			// BODY���g�̃������[���
}

// Open�CDelete�{�^���̕\�������̕ύX
void KODatUNO::OpenDelBtn()
{
    // �J���{�^���̐ݒ�
    if(BodyList.getNum() == BODYLISTMAX)	//�t�@�C����8�J���Ă�����
        GuiIF.SetEnableOpenFile(false);		//�W�F�\���i�����Ȃ��j
    else
        GuiIF.SetEnableOpenFile(true);		//�ʏ�\���i������j

    // �폜�{�^���̐ݒ�
    if(BodyList.getNum() == 0)				//�t�@�C�����Ȃ�������
        GuiIF.SetEnableDelBody(false);		//�W�F�\���i�����Ȃ��j
    else
        GuiIF.SetEnableDelBody(true);		//�ʏ�\���i������j
}

// �{�f�B���X�g�ł̑I��
int KODatUNO::BodyListSelect()
{
    Focus_Body = GuiIF.GetBodyListSelectedPos();	// �I�����ڂ̏o��

	return KOD_TRUE;
}

// �œK���f���X�P�[�����擾
void KODatUNO::SetModelScale()
{
	BODY *body;
	double max=0;

	if(!BodyList.getNum()) return;

	for(int i=0;i<BodyList.getNum();i++){
		body = (BODY *)BodyList.getData(i);
		if(max < body->MaxCoord)
			max = body->MaxCoord;
	}
	ModelScale = 1/max;						// ������ʂɃI�u�W�F�N�g�S�̂��ʂ�悤�ɕ`��X�P�[���𒲐�
	ModelScale1st = ModelScale;				// ���f���X�P�[���̏����l���o���Ă���
}

// BODY�̍ő咷��ݒ肷��
void KODatUNO::SetMaxCoord()
{
	BODY *body;

	if(!BodyList.getNum()) return;

	for(int i=0;i<BodyList.getNum();i++){
		body = (BODY *)BodyList.getData(i);
		double max = 0;
		for(int j=0;j<body->TypeNum[_NURBSS];j++){
			for(int u=0;u<body->NurbsS[j].K[0];u++){
				for(int v=0;v<body->NurbsS[j].K[1];v++){
					double d = CalcEuclid(body->NurbsS[j].cp[u][v]);
					if(d > max)	max = d;
				}
			}
		}
		body->MaxCoord = max;
	}
}


// �E�B���h�E�̃T�C�Y���擾
void KODatUNO::GetResizeWindow(int width, int height)
{
    ScreenWidth = width;	// �����擾
    ScreenHeight =height;	// �������擾

	glViewport(0,0,ScreenWidth,ScreenHeight);
}

// OpenGL�̏�����
void KODatUNO::SetOpenGLStat()
{
	if(OpenGLInitFlag == KOD_FALSE){
		GLfloat diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
		GLfloat specular[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
		GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat mat_shininess[] = { 100.0 };
		GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

        glClearColor(BkCol[0],BkCol[1],BkCol[2],0);	// �w�i�F

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ���̐ݒ�
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

		// �}�e���A���̐ݒ�
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

		glEnable(GL_LIGHTING);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glEnable(GL_LIGHT0);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_NORMALIZE);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		if(SolidViewFlag == KOD_TRUE)
			SetNurbsSProperty(GLU_DISPLAY_MODE, GLU_FILL);
		else if(WireFlameViewFlag == KOD_TRUE)
			SetNurbsSProperty(GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);

		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE); 



		OpenGLInitFlag = KOD_TRUE;
	}

	double a=2;
	double ratio;

	glViewport(0,0,ScreenWidth,ScreenHeight);
	glMatrixMode(GL_PROJECTION);	// ���e�ϊ��s��X�^�b�N��I��
	glLoadIdentity();				// ���e�ϊ��s��X�^�b�N�̐擪�ɒP�ʍs���}��

	// ���s���e��ݒ�(�A�X�y�N�g��ɂ���ă��f���̏c���䂪�ς��Ȃ��悤�ɏ����𕪂���)
	if(ScreenWidth <= ScreenHeight){
		ratio = (double)ScreenHeight/(double)ScreenWidth;
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}
	else{
		ratio = (double)ScreenWidth/(double)ScreenHeight;
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}

	glMatrixMode(GL_MODELVIEW);						// ���f���r���[�s��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();								// ���f���r���[�s��̐擪�ɒP�ʍs���}��
	gluLookAt(0,1,0,0.0,0.0,0.0,0.0,0.0,1.0);		// ���_��ݒ�
	glScaled(ModelScale,ModelScale,ModelScale);		// ���f���X�P�[����ݒ�
	glTranslatef(Trl[0],Trl[1],Trl[2]);				// ���W�n�𕽍s�ړ�����
	glMultMatrixd(RotMx);							// ���f���r���[�s��ɕύX���ꂽ��]�s��������Ă���
	DrawAxis(AxisScale,ModelScale);					// ���W����`��

}

// �}�E�X�C�x���g�̏���
// ����  Trig:�g���K�[�̎��	Btn:�ǂ̃{�^���������ꂽ��		x,y:�}�E�X�̈ʒu
void KODatUNO::MouseEvent(long Trig,long Btn,short x,short y)
{
    if((Trig == KDT_MOUSE_PRESS) ){
        // �E�N���b�N
        if(Btn == KDT_MOUSE_RIGHT){
            StartX = x;
            StartY = y;
            RBtnFlag = KOD_TRUE;				// �E�{�^���h���b�O���t���OON
        }

        // �^�񒆃N���b�N
        else if(Btn == KDT_MOUSE_MIDDLE){
            MBtnFlag = KOD_TRUE;
        }

        // ���N���b�N
        else if(Btn == KDT_MOUSE_LEFT){
            StartX = x;
            StartY = y;
            LBtnFlag = KOD_TRUE;
        }
    }

    // �{�^���𗣂���
    else if(Trig == KDT_MOUSE_RELEASE){
        if(LBtnFlag == KOD_TRUE){			// ���N���b�N�������ꍇ
            DoSelectFlag = KOD_TRUE;		// �I�u�W�F�N�g�I�𔻕�
        }
        StartQ = QFunc.QCopy(TargetQ);		// ���݂̃N�H�[�^�j�I���l�����̏����l�Ƃ���
        MBtnFlag = KOD_FALSE;				// �^�񒆃{�^���h���b�O���t���OOFF
        RBtnFlag = KOD_FALSE;				// �E�{�^���h���b�O���t���OOFF
        LBtnFlag = KOD_FALSE;				// ���{�^���h���b�O���t���OOFF
        SweepSelectFlag = KOD_FALSE;		// �X�C�[�v�Z���N�V�����t���OOFF

        GuiIF.UpdateDescribeWidget();
    }
}

// �}�E�X�ړ����̃C�x���g����
void KODatUNO::MouseMotionEvent(long Btn, long Key, short x,short y)
{           
	// �E�{�^���h���b�O��-->��]
    if(Btn & KDT_MOUSE_RIGHT && RBtnFlag == KOD_TRUE){
		double dx = (double)(x-StartX)/(double)ScreenWidth;
		double dy = (double)(y-StartY)/(double)ScreenHeight;
		double a = sqrt(dx*dx + dy*dy);						// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̋���
		Quat dq = QFunc.QGenRot(2*PI*a,-dy/a,0,dx/a);		// ��]�N�H�[�^�j�I������
		TargetQ = QFunc.QMult(dq, StartQ);					// ��]�̏����lStartQ��dq���|���ĉ�]������
		QFunc.QtoR(RotMx, TargetQ);							// �N�H�[�^�j�I�������]�̕ϊ��s������߂�
        GuiIF.UpdateDescribeWidget();                       // �ĕ`��w��
    }

	// ���{�^���h���b�O��-->���o�[�o���h
    if(Btn & KDT_MOUSE_LEFT && LBtnFlag == KOD_TRUE){
		SweepSelectFlag = KOD_TRUE;
		CurrentX = x;
		CurrentY = y;
        GuiIF.UpdateDescribeWidget();                       // �ĕ`��w��
    }

	// �^�񒆃{�^���h���b�O��-->���s�ړ� or Ctrl+�^�񒆃{�^���h���b�O��-->�g��k��
    else if(Btn & KDT_MOUSE_MIDDLE && MBtnFlag == KOD_TRUE){
		double dx = (double)(x-OldPosX)/(double)ScreenWidth;	// �ړ������擾
		double dy = (double)(y-OldPosY)/(double)ScreenHeight;

        // Ctrl�L�[�������Ă�-->�g��k��
        if(Key == KDT_MOD_CTRL){
            double a = 2*sqrt(dx*dx + dy*dy);
            if(x-OldPosX > 0)	ModelScale += a*ModelScale;		// �}�E�X���E�֓���������g��
            else				ModelScale -= a*ModelScale;		// �}�E�X�����֓���������k��
            if(ModelScale < 0.0001) ModelScale = 0.0001;		// �k���X�P�[���̍ŏ��l
        }
		// Ctrl�L�[�������Ă��Ȃ�-->���s�ړ�
        else{


			Trl[0] -= 5*dx/ModelScale;
			Trl[2] -= 5*dy/ModelScale;
		}
        GuiIF.UpdateDescribeWidget();                       // �ĕ`��w��
        ReDrawBODYFlag = KOD_FALSE;							// �ĕ`��w��
	}

	OldPosX = x;
	OldPosY = y;
}

// �}�E�X�z�C�[���̃C�x���g����
void KODatUNO::MouseWheelEvent(long delta)
{
    double a = (double)delta / 1000;					//�z�C�[���̕���\�ɍ��킹�ĕύX
    ModelScale += a*ModelScale;					// �g��k��
    if(ModelScale < 0.0001) ModelScale = 0.0001;	// �k���X�P�[���̍ŏ��l
    GuiIF.UpdateDescribeWidget();
    ReDrawBODYFlag = KOD_FALSE;							// �ĕ`��w��
}


// �Z���N�V����(�}�E�X�s�b�L���O)�̐ݒ�
void KODatUNO::DoSelect(int x,int y)
{
	int hits;
	double ratio;
	double a=2;

    glGetIntegerv(GL_VIEWPORT,Vp);
	glSelectBuffer(MAXSELECT, SelectBuf);	// �Z���N�V�������ꂽ�I�u�W�F�N�g�̏������Ă����o�b�t�@��p��
	glRenderMode(GL_SELECT);				// �Z���N�V�������[�h�Ɉڍs
	glInitNames();							// �I�u�W�F�N�g�ɂ��閼�O(�����l)��o�^���Ă����l�[���X�^�b�N��������

	glMatrixMode(GL_PROJECTION);			// �ˉe�ϊ����[�h
	glPushMatrix();							// ����܂ł̕ϊ��}�g���b�N�X��ۑ�
	glLoadIdentity();						// �ˉe�ϊ��}�g���b�N�X��������

	// �ʏ�̃Z���N�V�����̏ꍇ
	if(StartX == x && StartY == y){
		gluPickMatrix((GLdouble)x,(GLdouble)(Vp[3] - y),5.0,5.0,Vp);	// �\���̈悪�}�E�X�|�C���^�̎��͂����ɂȂ�悤�Ɏˉe�ϊ��s���ݒ�
	}
	// �X�C�[�v�Z���N�V�����̏ꍇ
	else{
		gluPickMatrix((GLdouble)((StartX+x)/2),(GLdouble)((Vp[3]*2-y-StartY)/2),fabs((double)(x-StartX)),fabs((double)(StartY-y)),Vp);	// �\���̈悪���o�[�o���h���ɂȂ�悤�Ɏˉe�ϊ��s���ݒ�
	}

	if(Vp[2] <= Vp[3]){
		ratio = (double)Vp[3]/(double)Vp[2];
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}
	else{
		ratio = (double)Vp[2]/(double)Vp[3];
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}

	glMatrixMode(GL_MODELVIEW);					// ���f���ϊ����[�h�Ɉڍs
	glLoadIdentity();							// ���f���r���[�s��̐擪�ɒP�ʍs���}��
	gluLookAt(0,1,0.0,0.0,0.0,0.0,0.0,0.0,1.0);	// ���_��ݒ�
	glScaled(ModelScale,ModelScale,ModelScale);	// ���f���X�P�[����ݒ�
	glTranslatef(Trl[0],Trl[1],Trl[2]);			// ���W�n�𕽍s�ړ�����
	glMultMatrixd(RotMx);						// ���f���r���[�s��ɕύX���ꂽ��]�s��������Ă���
	if(DrawBODYFlag == KOD_TRUE)				// BODY�`��t���O��ON�Ȃ�
		glCallList(COMMAND_DRAW_BODY);			// �e�G���e�B�e�B�ŊK�w�����ꂽ�Z���N�V�����p�ԍ��𓾂�(���炩���ߓo�^���Ă������R�}���h�����s)
	glMatrixMode(GL_PROJECTION);				// �ˉe�ϊ����[�h�ɖ߂�
	glPopMatrix();								// �ˉe�ϊ��}�g���N�X�����ɖ߂�
	hits = glRenderMode(GL_RENDER);				// �Z���N�V�������[�h�I���A�����_�����O���[�h�ɖ߂�

	if(StartX == x && StartY == y)				// �N���b�N�őI�������ꍇ
		ClickPicking(SelectBuf,hits);
	else
		DragPicking(SelectBuf,hits);			// �h���b�O�őI�������ꍇ
}

// �N���b�N�ɂ��}�E�X�s�b�L���O
void KODatUNO::ClickPicking(GLuint SelectBuf[],int hits)
{
	int objnum=0;

	objnum = ObjSelect(SelectBuf,hits);			// �q�b�g�����I�u�W�F�N�g�̒��Ńf�v�X�l�̈�ԏ������I�u�W�F�N�g��I������


	// �ȑO�ɃZ���N�g���ꂽ���̂��ăZ���N�g�����ꍇ�̓J�E���g���Ȃ�
	for(int i=0;i<SeldEntList.getNum();i++){
		OBJECT *obj;
		obj = (OBJECT *)SeldEntList.getData(i);
		if(obj->Body == SelectBuf[objnum*(3+SelectBuf[0])+3] &&
			obj->Type == SelectBuf[objnum*(3+SelectBuf[0])+4] &&
			obj->Num == SelectBuf[objnum*(3+SelectBuf[0])+5]){
			hits = 0;
		}
	}
	
	// �V�����I�u�W�F�N�g�Ƀq�b�g�����ꍇ
	if(hits >= 1)
		SetNewObject(SelectBuf[objnum*(3+SelectBuf[0])+3],SelectBuf[objnum*(3+SelectBuf[0])+4],SelectBuf[objnum*(3+SelectBuf[0])+5]);
}

// �h���b�O�ɂ��}�E�X�s�b�L���O
void KODatUNO::DragPicking(GLuint SelectBuf[],int hits)
{
	int temp=0;	// �ăZ���N�g���ʗp�J�E���^

	// �͈͑I���ɂ���ăI�u�W�F�N�g�Ƀq�b�g�����ꍇ
	for(int i=0;i<hits;i++){
		for(int j=0;j<SeldEntList.getNum();j++){
			OBJECT *obj = (OBJECT *)SeldEntList.getData(j);
			if(obj != NULL && obj->Body == SelectBuf[i*(3+SelectBuf[0])+3] &&
				obj->Type == SelectBuf[i*(3+SelectBuf[0])+4] &&
				obj->Num == SelectBuf[i*(3+SelectBuf[0])+5]){
					temp++;
			}
		}
		// �I�u�W�F�N�g���V�����q�b�g�����ꍇ
		if(temp == 0)
			SetNewObject(SelectBuf[i*(3+SelectBuf[0])+3],SelectBuf[i*(3+SelectBuf[0])+4],SelectBuf[i*(3+SelectBuf[0])+5]);

		temp = 0;
	}
}

// �s�b�N���ꂽ�I�u�W�F�N�g��OBJECT���X�g�ɓo�^
void KODatUNO::SetNewObject(int BodyNum,int TypeNum,int NumNum)
{
	
	OBJECT *obj = new OBJECT;
	obj->Body = BodyNum;
	obj->Type = TypeNum;
	obj->Num = NumNum;
	SeldEntList.add(obj);							// �Z���N�g���ꂽ�G���e�B�e�B�������X�g�ɓo�^
	BODY *body;
	body = (BODY *)BodyList.getData(obj->Body);
	if(obj->Type == _NURBSC){
		body->ChangeStatColor(body->NurbsC[obj->Num].Dstat.Color,1,0.2,1,0.5);	// �I���ς݂��������ߐF��ύX
	}
	else if(obj->Type == _TRIMMED_SURFACE || obj->Type == _NURBSS){
		body->ChangeStatColor(body->NurbsS[obj->Num].Dstat.Color,0.3,0.1,0.2,0.5);	// �I���ς݂��������ߐF��ύX
	}
	else if(obj->Type == _MESH){
		HEface *f = body->Mesh->getIndexedFace(NumNum);
		SetColorStat(&f->Dstat,1,0.2,1,0.5);
	}

}

// �S�ăZ���N�g��Ԃɂ���
void KODatUNO::SelectAll()
{
	BODY *body;
	OBJECT *obj_;

	for(int i=0;i<BodyList.getNum();i++){					// ���݃��X�g�ɓo�^����Ă���BODY�̐��������[�v
		if((body = (BODY *)BodyList.getData(i)) != NULL){	// i�Ԗڂ̃��X�g�ɓo�^����Ă���BODY�̃f�[�^���擾
			// NURBSC��I���ς݂ɂ��鑀��
			for(int j=0;j<body->TypeNum[_NURBSC];j++){
				// ���ݒ��ڒ��̗v�f�����ɑI���ς݂��𔻕�
				bool flag = false;
				for(int k=0;k<SeldEntList.getNum();k++){
					obj_ = (OBJECT *)SeldEntList.getData(k);
					if(obj_ != NULL && obj_->Body == i && obj_->Type == _NURBSC && obj_->Num == j){
						flag = true;
						break;
					}
				}
				// �܂��I������Ă��Ȃ�������
				if(flag == false){
					SetNewObject(i,_NURBSC,j);
				}
			}

			// _TRIMMED_SURFACE��I���ς݂ɂ��鑀��
			for(int j=0;j<body->TypeNum[_TRIMMED_SURFACE];j++){
				// ���ݒ��ڒ��̗v�f�����ɑI���ς݂��𔻕�
				bool flag = false;
				for(int k=0;k<SeldEntList.getNum();k++){
					obj_ = (OBJECT *)SeldEntList.getData(k);
					if(obj_ != NULL && obj_->Body == i && obj_->Type == _TRIMMED_SURFACE && obj_->Num == j){
						flag = true;
						break;
					}
				}
				// �܂��I������Ă��Ȃ�������
				if(flag == false)
					SetNewObject(i,_TRIMMED_SURFACE,j);
			}
			// _NURBSS��I���ς݂ɂ��鑀��
			for(int j=0;j<body->TypeNum[_NURBSS];j++){
				bool flag = false;
				// �g�����h�ȖʂƂ��đI���ς݂̏ꍇ�͉������Ȃ�
				for(int k=0;k<body->TypeNum[_TRIMMED_SURFACE];k++){
					if(body->TrmS[j].pts == &body->NurbsS[k]){
						flag = true;
						break;
					}
				}
				// �g�����h�ȖʂłȂ��ꍇ�́A���ɑI���ς݂��𒲂ׂ�
				if(flag == false){
					for(int k=0;k<SeldEntList.getNum();k++){
						obj_ = (OBJECT *)SeldEntList.getData(k);
						if(obj_ != NULL && obj_->Body == i && obj_->Type == _NURBSS && obj_->Num == j){
							flag = true;
							break;
						}
					}
				}
				// �܂��I������Ă��Ȃ�������
				if(flag == false)
					SetNewObject(i,_NURBSS,j);
			}
			// _MESH��I���ς݂ɂ��鑀��
			if(body->TypeNum[_MESH]){
				for(int j=0;j<body->Mesh->FaceNum;j++){
					bool flag = false;
					int index = ((HEface *)body->Mesh->Face.getData(j))->index;
					for(int k=0;k<SeldEntList.getNum();k++){
						obj_ = (OBJECT *)SeldEntList.getData(k);
						if(obj_ != NULL && obj_->Body == i && obj_->Type == _MESH && obj_->Num == index){
							flag = true;
							break;
						}
					}
					// �܂��I������Ă��Ȃ�������
					if(flag == false){
						SetNewObject(i,_MESH,index);
					}
				}
			}
		}
	}

	ReDrawBODYFlag = KOD_FALSE;		// �`�惁�������X�g���Đݒ�
}

// �Z���N�V�����L�����Z��
void KODatUNO::SelectionCancel()
{
	BODY *body;
	OBJECT *obj;

	for(int i=0;i<SeldEntList.getNum();i++){
		obj = (OBJECT *)SeldEntList.getData(i);
		body = SearchBodyList(&BodyList,obj->Body);			// �Z���N�V��������Ă���G���e�B�e�B��������BODY�ԍ��𒲂ׂ�
		if (body == NULL) continue;							// DeleteBody�ŏ�������Ă����ꍇ
		if(obj->Type == _NURBSC){
			body->InitCurveColor(body->NurbsC[obj->Num].Dstat.Color);		// �I���������������ߐF�����ɖ߂�
		}
		else if(obj->Type == _TRIMMED_SURFACE || obj->Type == _NURBSS){
			body->InitSurfaceColor(body->NurbsS[obj->Num].Dstat.Color);		// �I���������������ߐF�����ɖ߂�
		}
		else if(obj->Type == _MESH){
			HEface *f = body->Mesh->getIndexedFace(obj->Num);
			//HEface *f = (HEface *)body->Mesh->Face.getData(obj->Num);
			if(f == NULL) continue;
			body->InitSurfaceColor(f->Dstat.Color);	// �I���������������ߐF�����ɖ߂�
		}
	}
	ClearSeldEntList();				// �Z���N�V�������X�g���N���A
	InitCmdFlags();					// Cmd�p�t���O��S��KOD_FALSE�ɏ�����
	ReDrawBODYFlag = KOD_FALSE;		// �`�惁�������X�g���Đݒ�
}

// �I�u�W�F�N�g�̑I�𔻕�
int KODatUNO::ObjSelect(GLuint SelectBuf[],int hits)
{
	int i;
	int ZdepthMin=0;
	double Zdepth[MAXSELECT];
	double depthbuf[MAXSELECT];
	double temp=0;

	// �q�b�g�����S�ẴI�u�W�F�N�g�̃f�v�X�l�𓾂�
	for(i=0;i<hits;i++){
		Zdepth[i] = ((((double)SelectBuf[i*(3+SelectBuf[0])+1]/0xFFFFFFFF)+((double)SelectBuf[i*(3+SelectBuf[0])+2]/0xFFFFFFFF))/2.0);	// �f�v�X�o�b�t�@�̍ő�l�ƍŏ��l�̒��Ԃ̒l
		depthbuf[i] = Zdepth[i];				// �f�v�X�l�̃\�[�g�����p�̃o�b�t�@��p��
	}
	
	// �q�b�g�����I�u�W�F�N�g�̒��Ńf�v�X�l���ŏ��̂��̂����߂�
	if(hits == 1)
		ZdepthMin = 0;
	else if(hits > 1)
		BubbleSort(depthbuf,hits);		// �o�u���\�[�g�ɂ��f�v�X�l���ŏ��̂��̂𓾂�
	
	// �f�v�X�l���ŏ��̂��̂������������ꍇ�A�ʂ�����D��I�ɑI������
	for(i=0;i<hits;i++){
		if(Zdepth[i] == depthbuf[0] && SelectBuf[i*(3+SelectBuf[0])+4] == _NURBSC){
			ZdepthMin = i;
			temp++;
			break;
		}
	}
	
	// �q�b�g�������ɐ����Ȃ��ꍇ
	if(temp == 0){
		for(i=0;i<hits;i++){
			if(Zdepth[i] == depthbuf[0])
				ZdepthMin = i;
		}
	}

	return ZdepthMin;
}

// X���ォ��̎��_�ɐ؂�ւ���
void KODatUNO::ChangeViewX()
{
	StartQ = QFunc.QInit(1,0,0,0);		// ���̉�]�p����������
	QFunc.QtoR(RotMx,StartQ);			// ���̉�]�p����������
	TargetQ = QFunc.QMult(QFunc.QGenRot(PI/2,0,0,1), StartQ);	// �N�H�[�^�j�I����ŏ����ʒu�ɑ΂���Z������90����]
	QFunc.QtoR(RotMx,TargetQ);			// ��]�s��ɑ��
	StartQ = QFunc.QCopy(TargetQ);
	Trl[0] = Trl[1] = Trl[2] = 0;		// ���s�ړ��̓��Z�b�g
}

// Y���ォ��̎��_�ɐ؂�ւ���
void KODatUNO::ChangeViewY()
{
	StartQ = QFunc.QInit(1,0,0,0);
	QFunc.QtoR(RotMx,StartQ);
	Trl[0] = Trl[1] = Trl[2] = 0;
}

// Z���ォ��̎��_�ɐ؂�ւ���
void KODatUNO::ChangeViewZ()
{
	StartQ = QFunc.QInit(1,0,0,0);
	QFunc.QtoR(RotMx,StartQ);
	TargetQ = QFunc.QMult(QFunc.QGenRot(PI/2,1,0,0), StartQ);	// �N�H�[�^�j�I����ŏ����ʒu�ɑ΂���X������90����]
	QFunc.QtoR(RotMx,TargetQ);	
	StartQ = QFunc.QCopy(TargetQ);
	Trl[0] = Trl[1] = Trl[2] = 0;		// ���s�ړ��̓��Z�b�g
}

// ���Վ��_�ɐ؂�ւ���
void KODatUNO::ChangeViewXYZ()
{
	StartQ = QFunc.QInit(1,0,0,0);
	QFunc.QtoR(RotMx,StartQ);
	TargetQ = QFunc.QMult(QFunc.QGenRot(PI/6,0,0,1), StartQ);		// Z������30��
	TargetQ = QFunc.QMult(QFunc.QGenRot(-PI/10,1,0,0), TargetQ);	// X������18��
	QFunc.QtoR(RotMx,TargetQ);	
	StartQ = QFunc.QCopy(TargetQ);
	Trl[0] = Trl[1] = Trl[2] = 0;		// ���s�ړ��̓��Z�b�g
}

// BODY���]
bool KODatUNO::ChangeViewUp()
{
	Quat dq = QFunc.QGenRot(PI/12,1,0,0);				// ��]�N�H�[�^�j�I������
	TargetQ = QFunc.QMult(dq, StartQ);					// ��]�̏����lStartQ��dq���|���ĉ�]������
	QFunc.QtoR(RotMx, TargetQ);							// �N�H�[�^�j�I�������]�̕ϊ��s������߂�
	StartQ = QFunc.QCopy(TargetQ);						// �X�V
    GuiIF.UpdateDescribeWidget();		// �ĕ`��w��

	return KOD_FALSE;
}

// BODY����]
bool KODatUNO::ChangeViewDown()
{
	Quat dq = QFunc.QGenRot(PI/12,-1,0,0);				// ��]�N�H�[�^�j�I������
	TargetQ = QFunc.QMult(dq, StartQ);					// ��]�̏����lStartQ��dq���|���ĉ�]������
	QFunc.QtoR(RotMx, TargetQ);							// �N�H�[�^�j�I�������]�̕ϊ��s������߂�
	StartQ = QFunc.QCopy(TargetQ);						// �X�V
    GuiIF.UpdateDescribeWidget();		// �ĕ`��w��

	return KOD_FALSE;
}


// BODY����]
bool KODatUNO::ChangeViewLeft()
{
	Quat dq = QFunc.QGenRot(PI/12,0,0,-1);				// ��]�N�H�[�^�j�I������
	TargetQ = QFunc.QMult(dq, StartQ);					// ��]�̏����lStartQ��dq���|���ĉ�]������
	QFunc.QtoR(RotMx, TargetQ);							// �N�H�[�^�j�I�������]�̕ϊ��s������߂�
	StartQ = QFunc.QCopy(TargetQ);						// �X�V
    GuiIF.UpdateDescribeWidget();		// �ĕ`��w��

	return KOD_FALSE;
}


// BODY�E��]
bool KODatUNO::ChangeViewRight()
{
	Quat dq = QFunc.QGenRot(PI/12,0,0,1);				// ��]�N�H�[�^�j�I������
	TargetQ = QFunc.QMult(dq, StartQ);					// ��]�̏����lStartQ��dq���|���ĉ�]������
	QFunc.QtoR(RotMx, TargetQ);							// �N�H�[�^�j�I�������]�̕ϊ��s������߂�
	StartQ = QFunc.QCopy(TargetQ);						// �X�V
    GuiIF.UpdateDescribeWidget();		// �ĕ`��w��

	return KOD_FALSE;
}


// �{�����f�t�H���g�ɖ߂�
void KODatUNO::FitView()
{
	ModelScale = ModelScale1st;
}

// BODY�`��`����Solid�\���ɕύX
void KODatUNO::SolidView()
{
	DrawBODYFlag = KOD_TRUE;		// BODY�`��t���OON
	EdgeViewFlag = KOD_TRUE;		// �G�b�W�`��t���OON
	SolidViewFlag = KOD_TRUE;		// �\���b�h�`��t���OON
	WireFlameViewFlag = KOD_FALSE;	// ���C���[�t���[���`��t���OOFF

	ReDrawBODYFlag = KOD_FALSE;		// BODY�`�惁�������X�g�Đݒ�t���OON
	OpenGLInitFlag = KOD_FALSE;		// OpenGL�X�e�[�^�X�Đݒ�t���OON
}

// Edge���\���ɂ���
void KODatUNO::NoEdgeView()
{
	DrawBODYFlag = KOD_TRUE;		// BODY�`��t���OON
	EdgeViewFlag = KOD_FALSE;		// �G�b�W�`��t���OOFF
	SolidViewFlag = KOD_TRUE;		// �\���b�h�`��t���OON
	WireFlameViewFlag = KOD_FALSE;	// ���C���[�t���[���`��t���OOFF

	ReDrawBODYFlag = KOD_FALSE;		// BODY�`�惁�������X�g�Đݒ�t���OON
	OpenGLInitFlag = KOD_FALSE;		// OpenGL�X�e�[�^�X�Đݒ�t���OON
}

// BODY�`��`����WireFlame�\���ɕύX
void KODatUNO::WireFlameView()
{
	DrawBODYFlag = KOD_TRUE;		// BODY�`��t���OON
	EdgeViewFlag = KOD_TRUE;		// �G�b�W�`��t���OON
	WireFlameViewFlag = KOD_TRUE;	// ���C���[�t���[���`��t���OON
	SolidViewFlag = KOD_FALSE;		// �\���b�h�`��t���OOFF

	ReDrawBODYFlag = KOD_FALSE;		// BODY�`�惁�������X�g�Đݒ�t���OON
	OpenGLInitFlag = KOD_FALSE;		// OpenGL�X�e�[�^�X�Đݒ�
}

// BODY��\����
void KODatUNO::VoidView()
{
	DrawBODYFlag = KOD_FALSE;		// BODY�`��t���OOFF
	EdgeViewFlag = KOD_FALSE;		// �G�b�W�`��t���OON
	WireFlameViewFlag = KOD_FALSE;	// ���C���[�t���[���`��t���OOFF
	SolidViewFlag = KOD_FALSE;		// �\���b�h�`��t���OOFF
}

// BODY�𔼓����ɂ���
void KODatUNO::ViewBlending()
{
	BlendViewFlag = KOD_TRUE - BlendViewFlag;
}

// �Z���N�V�������X�g�y��OBJECT�̏�����
void KODatUNO::ClearSeldEntList()
{
	OBJECT *obj;
	int objnum = SeldEntList.getNum();

	for(int i=0;i<objnum;i++){
		obj = (OBJECT *)SeldEntList.getData(i);
		delete obj;
	}
	SeldEntList.clear();
}


// Cmd�p�t���O��S��KOD_FALSE�ɏ�����
void KODatUNO::InitCmdFlags()
{
	UVWireFlameViewFlag = KOD_FALSE;
	CPViewFlag = KOD_FALSE;
	UVDirFlag = KOD_FALSE;
}

// UserFunc�����s
void KODatUNO::ExecuteUserFuncMain(int val)
{
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(i == val){
			AddEntSymbolToObj();				// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
			User.ExecWidgetFunc(val);			// User�֐�����Dialog������s�������ꍇ�͂��̊֐���ʂ��čs����
			ExecUserFuncFlag[i] = KOD_TRUE;
			return;
		}
	}

}

// UserCommand���s���̍ĕ`��Ăяo��(OpenGL�`�惋�[�v��)
void KODatUNO::ReDrawUserCommand()
{
	for(int i=0;i<USERCOMMANDNUM;i++){
		if(ExecUserCommandFlag[i] == KOD_TRUE){
			ExecUserCommandFlag[i] = KOD_DONE;
			glNewList(COMMAND_DRAW_USER_COMMAND+i,GL_COMPILE_AND_EXECUTE);		// User�֐�i���������[���X�g�ɓo�^
			User.Command(&BodyList,&SeldEntList,SeldEntList.getNum(),i+SYSCOMMANDNUM,argc,argv);	// User�R�}���h�����s
			glEndList();												// �������[���X�g�o�^�I��
			ReDrawBODYFlag = KOD_FALSE;		// BODY�`�惁�������X�g�Đݒ�t���OON
			DrawBODYFlag = KOD_TRUE;		// BODY�`�悵�Ă�OK�t���OON
           // GuiIF.RedrawDescribeForm();		// �`��̈�ɍĕ`����w��
			GuiIF.UpdateDescribeWidget();
            OpenDelBtn();					// Del�{�^�������̕ύX
			return;
		}
	}

	// �쐬����User�֐��̃������[���X�g���Ăяo��
	for(int i=0;i<USERCOMMANDNUM;i++){
		if(ExecUserCommandFlag[i] == KOD_DONE){			// User�֐�i���s�ς݃t���O��ON�Ȃ�
			glCallList(COMMAND_DRAW_USER_COMMAND+i);			// User�֐�i�̃������[���X�g���Ăяo��(User�֐�i���ŌĂяo���ꂽOpenGL�`������s)
		}
	}

}

// UserFunc���s���̍ĕ`��Ăяo��(OpenGL�`�惋�[�v��)
void KODatUNO::ReDrawUserFunc()
{
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(ExecUserFuncFlag[i] == KOD_TRUE){
			ExecUserFuncFlag[i]  = KOD_DONE;
			glNewList(COMMAND_DRAW_USER+i,GL_COMPILE_AND_EXECUTE);		// User�֐�i���������[���X�g�ɓo�^
			int (USER::*userfn)(BODYList *,OBJECTList *,int);	
			userfn = User.UserFunc[i];
			ExecUserFuncFlag[i] = (User.*userfn)(&BodyList,&SeldEntList,SeldEntList.getNum());		// User�֐�i�����s()
			if(ExecUserFuncFlag[i]  != KOD_FALSE && ExecUserFuncFlag[i]  != KOD_ERR){
				ExecUserFuncFlag[i]  = KOD_DONE;
			}
			glEndList();												// �������[���X�g�o�^�I��
			ReDrawBODYFlag = KOD_FALSE;		// BODY�`�惁�������X�g�Đݒ�t���OON
			DrawBODYFlag = KOD_TRUE;		// BODY�`�悵�Ă�OK�t���OON
         //   GuiIF.RedrawDescribeForm();		// �`��̈�ɍĕ`����w��
			GuiIF.UpdateDescribeWidget();
            OpenDelBtn();					// Del�{�^�������̕ύX
			return;
		}
	}

	// �쐬����User�֐��̃������[���X�g���Ăяo��
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(ExecUserFuncFlag[i] == KOD_DONE){			// User�֐�i���s�ς݃t���O��ON�Ȃ�
			glCallList(COMMAND_DRAW_USER+i);			// User�֐�i�̃������[���X�g���Ăяo��(User�֐�i���ŌĂяo���ꂽOpenGL�`������s)
		}
	}

}

// UserStatus�̐ݒ�
void KODatUNO::AcceptUserStat(int mode,double prop[])
{
	User.SetUserStatus(mode,prop);	// �_�C�A���O����̒l��User�N���X��UserStat�\���̂Ɋi�[
}

// UserStatus���擾
void KODatUNO::GetUserStat(int *mode,double prop[])
{
    UserStat_ *us = User.GetUserStatus();
    *mode = us->Mode;
    for(int i=0;i<USERPROPNUM;i++)
        prop[i] = us->Prop[i];
}

// UserFunc�ɂ���ĕ`�悳�ꂽ�摜������
void KODatUNO::UserViewCanncel()
{
	for(int i=0;i<USERFUNCNUMMAX;i++)
		ExecUserFuncFlag[i] = KOD_FALSE;
	for(int i=0;i<USERCOMMANDNUM;i++)
		ExecUserCommandFlag[i] = KOD_FALSE;

	User.ExecdClear();	// User�N���X��ExecdClear()���Ăяo��

	InitCmdFlags();
}


// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
int KODatUNO::AddEntSymbolToObj()
{
	OBJECT *obj;
	int Cnum = 0;
	int Snum = 0;
	int Mnum = 0;

	for(int i=0;i<SeldEntList.getNum();i++){
		obj = (OBJECT *)SeldEntList.getData(i);
		if(obj->Type == _NURBSC){
			obj->CCount = Cnum;
			Cnum++;
		}
		else if(obj->Type == _TRIMMED_SURFACE || obj->Type == _NURBSS){
			obj->SCount = Snum;
			Snum++;
		}
		else if(obj->Type == _MESH){
			obj->SCount = Mnum;
			Mnum++;
		}
		else{
            GuiIF.SetMessage("ERROR:Unsupported entity was detected.");
			return KOD_ERR;
		}
	}

	return KOD_TRUE;
}


// �R���\�[������̃R�}���h�����s
void KODatUNO::ExecCommand(char *allcmdline)
{
	fprintf(stderr,"%s\n",allcmdline);
	argc = AnalCommandLine(allcmdline,argv);		// �R�}���h���C������͂��Aargc,argv�𓾂�
	if(argc == KOD_ERR)								// �������͂���Ă��Ȃ�������
		return;
	fprintf(stderr,"%s\n",allcmdline);

	int cmdno = SearchRegdCmdNum(CmdMap,argv[0]);	// �R�}���h�̓o�^�ԍ��𒲂ׂ�
	if(cmdno == KOD_ERR){
        GuiIF.SetMessage("No such command name!");
		return;
	}

	if(cmdno<SYSCOMMANDNUM)
		CmdMap[cmdno].Command(argc,argv);			// �o�^�ԍ��𔻕ʂ��A�V�X�e���R�}���h�����s

	else if(cmdno >= SYSCOMMANDNUM && cmdno < COMMANDNUMMAX)
		ExecUserCommandFlag[cmdno-SYSCOMMANDNUM] = KOD_TRUE;
		//User.Command(&BodyList,&SeldEntList,SeldEntList.getNum(),cmdno,argc,argv);

    GuiIF.UpdateDescribeWidget();		// �`��̈�ɍĕ`����w��
}

// UV�p�����[�^�ŕ������ꂽ���C���[�t���[����\��
void KODatUNO::UVWireView()
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;
	NURBSS *NurbsS;
	NURBS_Func NFunc;
	Coord p;
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glColor3f(0.5,0.5,0.5);

	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);
		if(obj->Type != _NURBSS && obj->Type != _TRIMMED_SURFACE)	continue;
		int num;
		if(body->NurbsS[0].TrmdSurfFlag == KOD_TRUE)
			num = body->TypeNum[_TRIMMED_SURFACE];
		else
			num = body->TypeNum[_NURBSS];
		for(int i=0;i<num;i++){
			if(body->NurbsS[0].TrmdSurfFlag == KOD_TRUE)
				NurbsS = body->TrmS[i].pts;
			else
				NurbsS = &body->NurbsS[i];
			double du = NurbsS->U[1] - NurbsS->U[0];
			double dv = NurbsS->V[1] - NurbsS->V[0];
			for(int j=0;j<11;j++){
				glBegin(GL_LINE_STRIP);
				for(int k=0;k<51;k++){		// v�����p�����[�^���C���`��
					p = NFunc.CalcNurbsSCoord(NurbsS,0.1*(double)j*du,0.02*(double)k*dv);
					glVertex3d(p.x,p.y,p.z);
				}
				glEnd();
				glBegin(GL_LINE_STRIP);
				for(int k=0;k<51;k++){		// u�����p�����[�^���C���`��
					p = NFunc.CalcNurbsSCoord(NurbsS,0.02*(double)k*du,0.1*(double)j*dv);
					glVertex3d(p.x,p.y,p.z);
				}
				glEnd();
			}
		}
	}
	glEnable(GL_LIGHTING);
}

// ���f���X�P�[�����Z�b�g
void KODatUNO::SetModelScale(double scale)
{
	if(scale == 0) return;
	ModelScale = scale;
}

// ���f���X�P�[�����擾
void KODatUNO::GetModelScale()
{
	char mes[256];
	sprintf(mes,"%.6lf",ModelScale);
    SetConsoleCharNum(GuiIF.SetMessage(mes));
}

// �g�������X�l���Z�b�g
void KODatUNO::SetTolerance(double val)
{
	if (val == 0) return;

	Tolerance = val;
	ReDrawBODYFlag = KOD_FALSE;		// �ĕ`��w��
    GuiIF.UpdateDescribeWidget();		// �`��̈�ɍĕ`����w��
}

// �g�������X�l���擾
void KODatUNO::GetTolerance()
{
	char mes[256];
	sprintf(mes,"%.6lf",Tolerance);
    SetConsoleCharNum(GuiIF.SetMessage(mes));	// �g�������X�l��\���A�R���\�[���������X�V
}

// BODY�𕽍s�ړ�������
void KODatUNO::GetShiftBody(Coord d)
{
	if(!BodyList.getNum()){
        GuiIF.SetMessage("Command ERROR: No BODY");
		return;
	}
	if(!SeldEntList.getNum()){
        GuiIF.SetMessage("Command ERROR: No selected entity");
		return ;
	}

	BODY *body;
	OBJECT *obj;	
	NURBS_Func NFunc;

	obj = (OBJECT *)SeldEntList.getData(0);
	body = (BODY *)BodyList.getData(obj->Body);
	body->ShiftBody(d);
	
	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾
}

// BODY����]������
void KODatUNO::GetRotateBody(Coord ax,double d)
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;	
	NURBS_Func NFunc;

	obj = (OBJECT *)SeldEntList.getData(0);
	body = (BODY *)BodyList.getData(obj->Body);
	body->RotBody(ax,d);

	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾
}

// BODY���g��
void KODatUNO::ExpandBody(Coord r)
{
	NURBS_Func NFunc;
	OBJECT *obj;
	BODY *body;

	obj = (OBJECT *)SeldEntList.getData(0);			// ���̃Z���N�V��������Ă���G���e�B�e�B���𓾂�
	body = SearchBodyList(&BodyList,obj->Body);		// BODY�̎��̂𓾂�
	body->ExpandBody(r);

	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾
}


// �e��T�[�t�F�X�𐶐�
int KODatUNO::GenSurface(Coord Axis,double Prop,int Flag)
{
	NURBS_Func NFunc;
	OBJECT *obj;
	BODY *body,*newbody;

	for(int i=0;i<SeldEntList.getNum();i++){										// �Z���N�V��������Ă���S�ẴG���e�B�e�B�ɑ΂���
		obj = (OBJECT *)SeldEntList.getData(i);										// ���̃Z���N�V��������Ă���G���e�B�e�B���𓾂�
		if(obj->Type == _NURBSC){													// �G���e�B�e�B��NURBS�Ȑ��Ȃ�
			body = SearchBodyList(&BodyList,obj->Body);								// ����NURBS�Ȑ��������Ă���BODY�̎��̂𓾂�
			newbody = new BODY;														// �V�������������]�T�[�t�F�X�p��BODY���������[�m��
			newbody->NurbsS = (NURBSS *)malloc(sizeof(NURBSS));						// NURBS�Ȗʂ�1�������[�m��
			if(Flag == ROTSURF){
				if(NFunc.GenRotNurbsS(newbody->NurbsS,body->NurbsC[obj->Num],Axis,Prop) == KOD_ERR)	// ��]�T�[�t�F�X����
					goto EXIT;
			}
			else if(Flag == SWEEPSURF){
				if(NFunc.GenSweepNurbsS(newbody->NurbsS,body->NurbsC[obj->Num],Axis,Prop) == KOD_ERR)	// �X�C�[�v�T�[�t�F�X����
					goto EXIT;
			}

			newbody->NurbsS[0].TrmdSurfFlag = KOD_FALSE;							// �g�����̂Ȃ��P����NURBS�Ȗʂł��邱�Ƃ𖾎�
			newbody->TypeNum[_NURBSS] = 1;											// NURBS�Ȗʂ̐�1�ɂ���
			newbody->ChangeStatColor(newbody->NurbsS[0].Dstat.Color,0.2,0.2,1.0,0.5);		// �F
			newbody->Mom = BodyList.add(newbody);									// ���X�g��newbody��o�^
            GuiIF.AddBodyNameToWin("NewBody");										// Body���X�g�E�B���h�E�ɂ�newbody��o�^
            strcpy(newbody->Name,"NewBody");										// �Ƃ肠����newbody�̖��O��"NewBody"�Ƃ��Ă���
		}
	}
	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾

	return KOD_TRUE;

EXIT:
	free(newbody->NurbsS);
	delete newbody;
	return KOD_ERR;
}

// NURBS�Ȑ��𐶐�
int KODatUNO::GenNurbsCurve(int Val,char *Fname,int M)
{
	if(M < 2){
        GuiIF.SetMessage("NURBS ERROR: Few Rank. You should set rank more than 2.");
		return KOD_ERR;
	}

	NURBS_Func NFunc;
	FILE *fp;
	char buf[256];
	Coord pt[CTLPNUMMAX];
	int ptnum=0;
	BODY *newbody;

	// �t�@�C���Ǎ�
	if((fp = fopen(Fname,"r")) == NULL){
        GuiIF.SetMessage("KODATUNO ERROR: fail to open file");
		return KOD_ERR;
	}
	while(fgets(buf,sizeof(buf),fp) != NULL){
		if(buf[0] == '#' || buf[0] == '\n')	continue;
		sscanf(buf,"%lf,%lf,%lf",&pt[ptnum].x,&pt[ptnum].y,&pt[ptnum].z);
		ptnum++;
		if(ptnum == CTLPNUMMAX){
			sprintf(buf,"KODATUNO ERROR: Too many points.  Reduce point number to %d or fewer.",CTLPNUMMAX);
            GuiIF.SetMessage(buf);
			return KOD_ERR;
		}
	}
	fclose(fp);

	// NURBS�Ȑ�����
	NURBSC nurb;
	switch(Val){
		case 1:
			NFunc.GenPolygonalLine(&nurb,pt,ptnum);	// �܂��
			break;
		case 2:
			if(NFunc.GenInterpolatedNurbsC1(&nurb,pt,ptnum,M) != KOD_TRUE)	// �_����NURBS�Ȑ�
				goto EXIT;
			break;
		case 3:
			if(NFunc.GenApproximationNurbsC(&nurb,pt,ptnum,M) != KOD_TRUE)	// �_��ߎ�NURBS�Ȑ�
				goto EXIT;
			break;
		case 4:
			if(NFunc.GenInterpolatedNurbsC2(&nurb,pt,ptnum,M) != KOD_TRUE)	// �_����NURBS�Ȑ�
				goto EXIT;
			break;			
		case 5:
			if(NFunc.GenNurbsCfromCP(&nurb,pt,ptnum,M) != KOD_TRUE)				// �R���g���[���|�C���g����NURBS�Ȑ��𐶐�
				goto EXIT;
			break;
		default:
			goto EXIT;
	}

	// �o�^
	newbody = new BODY;
	newbody->RegistNurbsCtoBody(&BodyList,nurb,"NewBody_C");	// �V����NURBS�Ȑ���V����BODY�Ƃ��ēo�^
	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾
	OpenDelBtn();							// Del�{�^�������̕ύX

	return KOD_TRUE;

EXIT:
	return KOD_ERR;
}

// NURBS�Ȗʂ𐶐�
// ����
// Val:�Ȗʂ̎��(1:�܂�� 2:�_����NURBS�Ȗ�)
// *Fname:�t�@�C����   M:�K��
int KODatUNO::GenNurbsSurface(int Val,char *Fname,int M)
{
	if(M < 2 && Val != 1){
        GuiIF.SetMessage("NURBS ERROR: Few Rank. You should set Rank more than 2.");
		return KOD_ERR;
	}

	NURBS_Func NFunc;
	FILE *fp;
	char buf[256];
	Coord **pt;
	int row,col;
	int flag=KOD_FALSE;
	int ptnum=0;
	int i=0,j=0;
	BODY *newbody;

	// �t�@�C���Ǎ�
	if((fp = fopen(Fname,"r")) == NULL){
        GuiIF.SetMessage("KODATUNO ERROR: fail to open file");
		return KOD_ERR;
	}

	// �t�@�C������_��̍��W�𓾂�
	while(fgets(buf,sizeof(buf),fp) != NULL){
		if(buf[0] == '#' || buf[0] == '\n')	continue;
		if(!flag){
			sscanf(buf,"%d,%d",&row,&col);
			if((pt = NewCoord2(row,col)) == NULL){		// �_���i�[����̈�̃������[�m��
                GuiIF.SetMessage("KODATUNO ERROR: fail to allocate memory");
				return KOD_ERR;
			}
			flag = KOD_TRUE;
		}
		else{
			sscanf(buf,"%lf,%lf,%lf",&pt[i][j].x,&pt[i][j].y,&pt[i][j].z);
			j++;
			if(j == col){
				i++;
				j = 0;
			}
			ptnum++;
			if(ptnum > PTNUMMAX){
				sprintf(buf,"KODATUNO ERROR: Too many points (%d).  Reduce point number to %d or fewer.",ptnum,PTNUMMAX);
                GuiIF.SetMessage(buf);
				FreeCoord2(pt,col);
				return KOD_ERR;
			}
		}
	}
	fclose(fp);

	// NURBS�Ȗʐ���
	NURBSS nurb;
	switch(Val){
		case 1:
			NFunc.GenPolygonalSurface(&nurb,pt,row,col);	// �܂��
			break;
		case 2:
			if(NFunc.GenInterpolatedNurbsS1(&nurb,pt,row,col,M,M) == KOD_ERR)	// �_����NURBS�Ȗ�
				goto EXIT;
			break;
		case 3:
			if(NFunc.GenApproximationNurbsS(&nurb,pt,row,col,M,M) == KOD_ERR)	// �_����NURBS�Ȗ�
				goto EXIT;
			break;
		case 5:
			if(NFunc.GenNurbsSfromCP(&nurb,pt,row,col,M,M) == KOD_ERR)	// �R���g���[���|�C���g����NURBS�Ȗʂ𐶐�
				goto EXIT;
			break;
		default:
			goto EXIT;
	}

	// �o�^
	newbody = new BODY;
	newbody->RegistNurbsStoBody(&BodyList,nurb,"NewBody_S");	// �V����NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^
	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾
	OpenDelBtn();							// Del�{�^�������̕ύX

	SetMaxCoord();			// �쐬����BODY�̍ő�Coord�l�𒲂ׂ�
	SetModelScale();		// ���f���X�P�[�����œK��

	FreeCoord2(pt,row);
	return KOD_TRUE;

EXIT:
	FreeCoord2(pt,row);
	return KOD_ERR;
}

// �R�}���h�o�^
void KODatUNO::SetConsoleHistory(std::string His)
{
    if(His == (std::string)"")	return;		// �R�}���h�Ȃ�

	CmdStackNow = 0;		// ���ݒ��ڒ��̗����R�}���h�ԍ���0�ɏ�����

	// CmdStack[0]���󂯂�
	if(CmdStackNum){
		for(int i=CmdStackNum-1;i>=0;i--){
			if(CmdStackNum == HISTORYNUMMAX && i == CmdStackNum-1)	continue;
			sprintf(CmdStack[i+1],"%s",CmdStack[i]);
		}
	}

	// CmdStack[0]�ɐV���ȃR�}���h���L�^
    sprintf(CmdStack[0],"%s",(char *)His.data());

	if(CmdStackNum != HISTORYNUMMAX)
		CmdStackNum++;

}

// �R�}���h���𐔂��擾
int KODatUNO::GetConsoleHistoryNum()
{
    return CmdStackNum;
}

// �R�}���h�擾(����L�[)
int KODatUNO::GetUpperConsoleHistory(std::string *cmd)
{
	if(!CmdStackNum)
		return KOD_FALSE;		// �����R�}���h�Ȃ�

	if(CmdStackNow == CmdStackNum)		// ����L�[�𗚗��R�}���h���ȏ�ɉ������ꍇ�͍ŏ��ɖ߂�
		CmdStackNow = 0;

    *cmd = (std::string)CmdStack[CmdStackNow];	// �R�}���h���o��
	CmdStackNow++;				// �����R�}���h�ԍ����C���N�������g
	return KOD_TRUE;
}

// �R�}���h�擾(�����L�[)
int KODatUNO::GetLowerConsoleHistory(std::string *cmd)
{
	if(!CmdStackNum)
		return KOD_FALSE;

	CmdStackNow--;

	if(CmdStackNow <= 0)
		CmdStackNow = CmdStackNum;

    *cmd = (std::string)CmdStack[CmdStackNow-1];
	return KOD_TRUE;
}

// �I������Ă���Ȗʂ̃R���g���[���|�C���g��`��
void KODatUNO::DescribeCP()
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;
	double col[3] = {0,1,0};
	char buf[256];

	glDisable(GL_LIGHTING);

	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);
		//if(obj->Type == _NURBSC){
		//	DrawPoints(body->NurbsC[obj->Num].cp,body->NurbsC[obj->Num].K,1,3,col);
		//}
		if(obj->Type == _NURBSS){
			for(int i=0;i<body->NurbsS[obj->Num].K[0];i++){
				for(int j=0;j<body->NurbsS[obj->Num].K[1];j++){
					DrawPoint(body->NurbsS[obj->Num].cp[i][j],1,3,col);
				}
			}
		}
		else if(obj->Type == _TRIMMED_SURFACE){
			for(int i=0;i<body->TrmS[obj->Num].pts->K[0];i++){
				for(int j=0;j<body->TrmS[obj->Num].pts->K[1];j++){
					DrawPoint(body->TrmS[obj->Num].pts->cp[i][j],1,3,col);
				}
			}
		}
	}

	glEnable(GL_LIGHTING);
}

// �I������Ă���Ȗʂ̏����R���\�[���o�͂���
void KODatUNO::GetSurfInfo()
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;
	NURBSS *ns;
	char buf[256];

	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);
		sprintf(buf,"Surface No.: %d",obj->Num);
		GuiIF.SetMessage(buf);
		if(obj->Type == _NURBSS){
			ns=&body->NurbsS[obj->Num];
			sprintf(buf,"NURBS Surface");
            GuiIF.SetMessage(buf);
		}
		else if(obj->Type == _TRIMMED_SURFACE){
			ns=body->TrmS[obj->Num].pts;
			sprintf(buf,"TRIMED Surface");
            GuiIF.SetMessage(buf);
		}
		sprintf(buf,"No. of CP: %d x %d = %d",ns->K[0],ns->K[1],ns->K[0]*ns->K[1]);
        GuiIF.SetMessage(buf);
		sprintf(buf,"Rank: %d, %d",ns->M[0],ns->M[1]);
        GuiIF.SetMessage(buf);
		sprintf(buf,"No. of Knot [Range]: %d[%.3lf - %.3lf], %d[%.3lf - %.3lf]",ns->N[0],ns->U[0],ns->U[1],ns->N[1],ns->V[0],ns->V[1]);
        GuiIF.SetMessage(buf);
		GuiIF.SetMessage("U Knot Value");
		for(int i=0;i<ns->N[0];i++){
			sprintf(buf,"%lf",ns->S[i]);
			GuiIF.SetMessage(buf);
		}
		GuiIF.SetMessage("V Knot Value");
		for(int i=0;i<ns->N[1];i++){
			sprintf(buf,"%lf",ns->T[i]);
			GuiIF.SetMessage(buf);
		}
	}
}

// �I������Ă���Mesh�̏����R���\�[���o�͂���
void KODatUNO::GetMeshInfo()
{
	if(!BodyList.getNum())	return;		// �Z���N�V��������Ă��Ȃ�������A�������Ȃ�

	BODY *body;
	OBJECT *obj;
	MESH *m;
	char buf[256];


	// �Z���N�V�������ꂽ���������[�v
	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);

		// �Z���N�V�������ꂽ�G���e�B�e�B�����b�V��
		if(obj->Type == _MESH){
			m = body->Mesh;
			sprintf(buf,"Face Num:%d\n",m->FaceNum);
            GuiIF.SetMessage(buf);
			sprintf(buf,"Edge Num:%d\n",m->EdgeNum);
            GuiIF.SetMessage(buf);
			sprintf(buf,"Vert Num:%d\n",m->VertNum);
            GuiIF.SetMessage(buf);

			HEface *f = (HEface *)m->Face.getData(obj->Num);
			int vnum = f->GetVetexNum();
			sprintf(buf,"Face index:%d\n",f->GetIndex());
            GuiIF.SetMessage(buf);
			sprintf(buf,"Vert Num:%d\n",vnum);
            GuiIF.SetMessage(buf);
			Coord n = f->GetNormVec();
			sprintf(buf,"Norm Vec:(%.3lf,%.3lf,%.3lf)\n",n.x,n.y,n.z);
            GuiIF.SetMessage(buf);
            GuiIF.SetMessage("Vert Coord:");
			HEedge *e = f->GetStartHE();
			for(int i=0;i<vnum;i++){
				Coord p = e->GetStartVcoord();
				sprintf(buf,"	e%d-v%d:(%.3lf,%.3lf,%.3lf)",e->GetIndex(),e->GetStartVindex(),p.x,p.y,p.z);
                GuiIF.SetMessage(buf);
				e = e->GetNextHE();
			}
		}
	}
}

// �I������Ă���Ȗʂ�U,V������\������
// U�����͉��F�CV�����͐��F�ŕ\��, ���_�͔��_
void KODatUNO::DispUVdirection()
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;
	NURBSS *ns;
	NURBS_Func NFunc;
	Coord p;

	glDisable(GL_LIGHTING);
	glLineWidth(2);

	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);
		if(obj->Type == _NURBSS)
			ns = &body->NurbsS[obj->Num];
		else if(obj->Type == _TRIMMED_SURFACE)
			ns = body->TrmS[obj->Num].pts;
		double du = ns->U[1] - ns->U[0];
		double dv = ns->V[1] - ns->V[0];
		glColor3f(1,1,0);
		glBegin(GL_LINE_STRIP);
		for(int i=0;i<11;i++){		// U�����p�����[�^���C���`��
			p = NFunc.CalcNurbsSCoord(ns,0.1*(double)i*du,0);
			glVertex3d(p.x,p.y,p.z);
		}
		glEnd();

		glColor3f(0,0.749,1);
		glBegin(GL_LINE_STRIP);
		for(int i=0;i<11;i++){		// V�����p�����[�^���C���`��
			p = NFunc.CalcNurbsSCoord(ns,0,0.1*(double)i*dv);
			glVertex3d(p.x,p.y,p.z);
		}
		glEnd();

		glPointSize(8);
		glColor3f(1,1,1);
		glBegin(GL_POINTS);
		p = NFunc.CalcNurbsSCoord(ns,0,0);
		glVertex3d(p.x,p.y,p.z);
		glEnd();
	}

	glEnable(GL_LIGHTING);
}



// �I������Ă���Ȗʂ�Rank��ύX����
void KODatUNO::ChangeRank(int Newrank[2])
{
	if(!BodyList.getNum())	return;

	char buf[256];

	if(Newrank[0] < 2 || Newrank[1] < 2){
		sprintf(buf,"ERROR:Specified few rank");
        GuiIF.SetMessage(buf);
		return;
	}

	NURBS_Func NFunc;
	BODY *body;
	OBJECT *obj;
	NURBSS *ns;

	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);
		if(obj->Type == _NURBSS){
			ns=&body->NurbsS[obj->Num];
		}
		else if(obj->Type == _TRIMMED_SURFACE){
			ns=body->TrmS[obj->Num].pts;
		}
		else
			continue;

		// rank���R���g���[���|�C���g�����傫���ꍇ�̓G���[
		if(Newrank[0] > ns->K[0] || Newrank[1] > ns->K[1]){
			sprintf(buf,"ERROR:Specified rank exceeds number of control points");
            GuiIF.SetMessage(buf);
			return;
		}
		int oldrank[2];
		oldrank[0] = ns->M[0];		// ���܂ł�rank��ۑ����Ă���
		oldrank[1] = ns->M[1];
		ns->M[0] = Newrank[0];		// �V����rank��o�^
		ns->M[1] = Newrank[1];
		ns->N[0] = ns->K[0] + ns->M[0];	// �m�b�g�̐����Ďw��
		ns->N[1] = ns->K[1] + ns->M[1];

		// Newrank��oldrank�����傫���ꍇ�̓m�b�g�x�N�g���z����Ċm��
		double *tmp1,*tmp2;
		bool flag = false;
		if(Newrank[0] > oldrank[0]){
			if((tmp1 = (double *)realloc(ns->S,sizeof(double)*ns->N[0])) == NULL){
				goto EXIT;
			}
			ns->S = tmp1;
			flag = true;
		}
		if(Newrank[1] > oldrank[1]){
			if((tmp2 = (double *)realloc(ns->T,sizeof(double)*ns->N[1])) == NULL){
				goto EXIT;
			}
			ns->T = tmp2;
			flag = true;
		}

		NFunc.GetEqIntervalKont(ns->K[0],ns->M[0],ns->S);	// �m�b�g�x�N�g�����Đݒ�
		NFunc.GetEqIntervalKont(ns->K[1],ns->M[1],ns->T);
		ns->U[0] = ns->V[0] = 0;		// �m�b�g�x�N�g���͈̔͂��w��
		ns->U[1] = ns->V[1] = NORM_KNOT_VAL;		
	}

	DrawBODYFlag = KOD_TRUE;				// BODY�`�悵�Ă�OK�t���OON
	ReDrawBODYFlag = KOD_FALSE;				// BODY�̃��������X�g���Ď擾
	return;

EXIT:			
	sprintf(buf,"ERROR:Fail to malloc");
    GuiIF.SetMessage(buf);
}

// �Z���N�V��������Ă���BODY�ԍ��𓾂�
BODY *KODatUNO::SearchBodyList(BODYList *BodyList,int key)
{
	return (BODY *)BodyList->getData(key);
}

// �w�i�̐F��ύX����
void KODatUNO::ChangeBackColor(double rgb[3])
{
	BkCol[0] = rgb[0];
	BkCol[1] = rgb[1];
	BkCol[2] = rgb[2];
	OpenGLInitFlag = KOD_FALSE;		// OpenGL�X�e�[�^�X�Đݒ�t���OON

}
