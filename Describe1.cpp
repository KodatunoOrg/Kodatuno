// OpenGL UserInterface

#include "Describe.h"

// DESCRIBE�N���X�ŗp����X�^�e�B�b�N�ϐ�
BODY *DESCRIBE::glbody;					// IGES�f�[�^���X�g�̐擪�A�h���X
BODY *DESCRIBE::Curr_body;				// IGES�f�[�^���X�g�̒��ڒ��̃A�h���X���i�[
IGES_PARSER DESCRIBE::Parser;			// IGES�p�[�T�[�p�I�u�W�F�N�g�𐶐�
QUATERNION DESCRIBE::QFunc;				// �N�H�[�^�j�I�����Z���C�u�����N���X
NURBS_Func DESCRIBE::NFunc;				// NURBS_Func�N���X�̃I�u�W�F�N�g�𐶐�
USER DESCRIBE::User;					// User�N���X�̃I�u�W�F�N�g�𐶐�
int  DESCRIBE::ScreenHeight;			// �`��E�B���h�E�̍���
int  DESCRIBE::ScreenWidth;				// �`��E�B���h�E�̕�
int  DESCRIBE::StartX;					// �h���b�O�J�n�ʒuX
int  DESCRIBE::StartY;					// �h���b�O�J�n�ʒuY
int  DESCRIBE::OldPosX;					// �E�B���h�E�ĕ`�撼�O��X���W
int  DESCRIBE::OldPosY;					// �E�B���h�E�ĕ`�撼�O��Y���W
int  DESCRIBE::CurrentX;				// �h���b�O���̈ʒuX
int  DESCRIBE::CurrentY;				// �h���b�O���̈ʒuY
int  DESCRIBE::SweepSelectFlag;			// �X�C�[�v�Z���N�V�������J�n�������Ƃ������t���O
int  DESCRIBE::Vp[4];					// �r���[�|�C���g
GLuint  DESCRIBE::SelectBuf[MAXSELECT]; // �Z���N�g�o�b�t�@
int  DESCRIBE::LBtnFlag;				// ���N���b�N�������Ƃ������t���O
int  DESCRIBE::RBtnFlag;				// �E�N���b�N�������Ƃ������t���O
int  DESCRIBE::MBtnFlag;				// ���N���b�N�������Ƃ������t���O
int  DESCRIBE::ShiftKeyFlag;			// Shift�L�[�����������Ƃ������t���O
int  DESCRIBE::CtrlKeyFlag;				// Ctrl�L�[�����������Ƃ������t���O
int  DESCRIBE::AltKeyFlag;				// Alt�L�[�����������Ƃ������t���O
Quat DESCRIBE::StartQ;					// �E�N���b�N�����u�Ԃ̉�]�p���i�[����N�H�[�^�j�I��
Quat DESCRIBE::TargetQ;					// ���肮�����Ă���r���̃N�H�[�^�j�I��
double DESCRIBE::RotMx[TMATELEMNUM];	// �N�H�[�^�j�I���������ϊ��}�g���b�N�X
double DESCRIBE::Trl[3];				// ���s�ړ���
double DESCRIBE::AxisScale;				// ���̃X�P�[��
double DESCRIBE::ModelScale;			// ���f���̃X�P�[��
double DESCRIBE::ModelScale1st;			// ���f���X�P�[���̏����l
double DESCRIBE::IdentX;				// �}�E�X�̐�ΈʒuX���E�B���h�E���ł̑��Έʒu�̊��Z�W��X
double DESCRIBE::IdentY;				// �}�E�X�̐�ΈʒuY���E�B���h�E���ł̑��Έʒu�̊��Z�W��Y
int DESCRIBE::DrawSurfFlagCount;		// Surface�`��t���O���j���[������I�����ꂽ���𐔂���J�E���^
int DESCRIBE::DrawBODYFlagCount;		// BODY�`��t���O���j���[������I�����ꂽ���𐔂���J�E���^
int DESCRIBE::DrawUserFuncFlag;			// PDT�`��t���O
OBJECT DESCRIBE::SelectedEntList[MAXSELECT];// �Z���N�V�������ꂽ�G���e�B�e�B�𒀎��i�[���Ă���
int DESCRIBE::PickCount;				// �s�b�N�񐔂��J�E���g
int DESCRIBE::SPickCount;				// NURBS�Ȗʂ��s�b�N���ꂽ�����J�E���g
int DESCRIBE::CPickCount;				// NURBS�Ȑ����s�b�N���ꂽ�����J�E���g
int DESCRIBE::BodyCount;				// body�̐����J�E���g


// DESCRIBE�N���X�̃R���X�g���N�^
DESCRIBE::DESCRIBE()
{
	// ������
	LBtnFlag = GL_FALSE;
	RBtnFlag = GL_FALSE;
	MBtnFlag = GL_FALSE;
	ShiftKeyFlag = GL_FALSE;
	CtrlKeyFlag = GL_FALSE;
	AltKeyFlag = GL_FALSE;
	SweepSelectFlag = GL_FALSE;
	StartX = 0;
	StartY = 0;
	OldPosX = 0;
	OldPosY = 0;
	CurrentX = 0;
	CurrentY = 0;
	ScreenHeight = 240;
	ScreenWidth = 320;
	AxisScale = 0.2;
	ModelScale = 1;
	StartQ = QFunc.QInit(1.0,0.0,0.0,0.0);
	TargetQ = QFunc.QInit(1.0,0.0,0.0,0.0);
	Vp[0] = Vp[1] = Vp[2] = Vp[3] = 0;
	Trl[0] = Trl[1] = Trl[2] = 0.0;
	DrawBODYFlag = TRUE;
	DrawSurfFlag = TRUE;
	DrawBODY1st = FALSE;
	DrawSurfFlagCount = 0;
	DrawBODYFlagCount = 0;
	DrawUserFuncFlag = FALSE;
	PickCount = 0;
	CPickCount = 0;
	SPickCount = 0;
	BodyCount = 0;
}

// DESCRIBE�N���X�̃f�X�g���N�^�iglut�C�x���g���[�v�𔲂���ƕK���Ă΂��j
DESCRIBE::~DESCRIBE()
{
	Curr_body = glbody;
	for(int i=0;i<BodyCount;i++){
		DelBodyList(&glbody,i);
	}
}

// OpenGL�`��
int DESCRIBE::BODY_Describe_Main(int argc,char *argv[])
{
	int sub_menu_ID1,sub_menu_ID2;
	char YorN = 'y';
	char buf[256];

	// IGES�t�@�C����ǂݍ���
	glbody = NULL;
	while(YorN=='y' || YorN=='Y'){
		if(GenBodyList() == ERR)		// IGES��ǂݍ��݁ABODY�N���X�̃��X�g���`��
			return ERR;
		fprintf(stderr,"Inport new IGES file? ('y' or 'n'):");
		fgets(buf,sizeof(buf),stdin);
		sscanf(buf,"%c",&YorN);
	}
	fprintf(stderr,"IGES�t�@�C���ǂݍ��ݏI��\n");

	//fprintf(stderr,"MaxCoord = %lf\n",glbody->MaxCoord); 
	ModelScale = ModelScale/glbody->MaxCoord;			// ������ʂɃI�u�W�F�N�g�S�̂��ʂ�悤�ɒ���
	ModelScale1st = ModelScale;

	glutInit(&argc, argv);								// OpenGL������
	Init("KODATUNO");									// �O���t�B�b�N�����ݒ�(�E�B���h�E����n��)
	glutKeyboardFunc(Keyboard);							// �L�[�C�x���g�̐ݒ�
	glutSpecialFunc(Special);							// ����L�[�C�x���g�̐ݒ�
	glutMouseFunc(Mouse);								// �}�E�X�C�x���g�̐ݒ�
	glutMotionFunc(Motion);								// �}�E�X���[�V�����C�x���g�̐ݒ�
	glutReshapeFunc(Reshape);							// �ĕ`��C�x���g�̐ݒ�
	glutDisplayFunc(Display);							// �`��R�A(������BODY���`�悳���)

	sub_menu_ID1 = glutCreateMenu(Sub_Menu_View);		// ���_�؊��p���j���[��o�^
	glutAddMenuEntry("View on x-axis",S_M_XView);		// ���_�؊��p���j���[�T�u���j���[
	glutAddMenuEntry("View on y-axis",S_M_YView);		// ���_�؊��p���j���[�T�u���j���[
	glutAddMenuEntry("View on z-axis",S_M_ZView);		// ���_�؊��p���j���[�T�u���j���[
	sub_menu_ID2 = glutCreateMenu(Sub_Menu_User);		// User����֐���o�^(���C��1��+�T�u5��)
	glutAddMenuEntry(User.MenuLabelMain,M_Label_Main);	// User���C���֐�
	glutAddMenuEntry(User.MenuLabel1,M_Label1);			// User�T�u�֐�1
	glutAddMenuEntry(User.MenuLabel2,M_Label2);			// User�T�u�֐�2
	glutAddMenuEntry(User.MenuLabel3,M_Label3);			// User�T�u�֐�3
	glutAddMenuEntry(User.MenuLabel4,M_Label4);			// User�T�u�֐�4
	glutAddMenuEntry(User.MenuLabel5,M_Label5);			// User�T�u�֐�5
	glutCreateMenu(Menu);								// ���C�����j���[�C�x���g�̐ݒ�
	glutAddSubMenu("Execute User Function",sub_menu_ID2);	// ���j���[�o�^(User�֐�)
	glutAddMenuEntry("Selection Cancel",M_UnSelect);		// ���j���[�o�^(PDT�`���������)
	glutAddMenuEntry("Draw BODY ON/OFF",M_Draw_Body);		// ���j���[�o�^(BODY�`�� ON/OFF�؊�)
	glutAddMenuEntry("Draw Surface ON/OFF",M_Draw_Surf);	// ���j���[�o�^(Surface�`�� ON/OFF�؊�)
	glutAddSubMenu("View Angle",sub_menu_ID1);				// ���j���[�o�^(���_�؊�)
	glutAddMenuEntry("Version info",M_Version);				// ���j���[�o�^(�o�[�W�������)
	glutAttachMenu(GLUT_RIGHT_BUTTON);						// �E�{�^���Ƀ��j���[��z�u

	glutMainLoop();											// �C�x���g�҂����[�v

	// �C�x���g���[�v�𔲂���ƁAKODatuno::~KODatUNO���Ă΂��

	return TRUE;
}

// ���C�����j���[�̃C�x���g��o�^
void DESCRIBE::Menu(int val)
{
	switch(val){
		// �o�[�W�������
		case M_Version:
			VersionInfo();
			break;

		// �Z���N�V��������
		case M_UnSelect:
			for(int i=0;i<PickCount;i++){		// �F�����ɖ߂�
				Curr_body = glbody;							
				Curr_body = SearchBodyList(Curr_body,SelectedEntList[i].Body);						// �ǂ�BODY���I������Ă��邩���ׂ�
				if(SelectedEntList[i].Type == _NURBSC){
					ChangeStatColor(Curr_body->NurbsC[SelectedEntList[i].Num].Dstat.Color,1,1,1,1);	// �I���ς݂��������ߐF��ύX
				}
				else if(SelectedEntList[i].Type == _TRIMMED_SURFACE){
					ChangeStatColor(Curr_body->NurbsS[SelectedEntList[i].Num].Dstat.Color,0.2,0.2,0.2,1);	// �I���ς݂��������ߐF��ύX
				}
			}
			PickCount = 0;				// �Z���N�V��������0�ɏ�����
			DrawUserFuncFlag = FALSE;	// User�֐����s�ς݃t���OOFF
			UpDateDisplayList(TRUE);	// �`��I�u�W�F�N�g�̕ύX���X�V
			break;

		// Surface�`���ON/OFF
		case M_Draw_Surf:
			DrawSurfFlagCount = 1 - DrawSurfFlagCount;
			if(DrawSurfFlagCount)
				gluNurbsProperty(NurbsSurf, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);
			else
				gluNurbsProperty(NurbsSurf, GLU_DISPLAY_MODE, GLU_FILL);
			UpDateDisplayList(FALSE);
			break;

		// BODY�`���ON/OFF	
		case M_Draw_Body:
			DrawBODYFlagCount = 1 - DrawBODYFlagCount;
			if(DrawBODYFlagCount)
				DrawBODYFlag = FALSE;
			else
				DrawBODYFlag = TRUE;
			break;
	}

	glutPostRedisplay();
}


// User�֐���menu�ɓo�^
void DESCRIBE::Sub_Menu_User(int val)
{
	switch(val){
		// User���C���֐�
		case M_Label_Main:
			if(PickCount){
				AddEntSymbolToObj();								// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
				glNewList(COMMAND_DRAW_USER,GL_COMPILE);			// User�֐��ł�User���C���֐��ł̏����̂ݕ`�捂�����������{��
				User.UserFuncMain(glbody,SelectedEntList,PickCount);// User���C���֐������s
				glEndList();
				DrawUserFuncFlag = TRUE;							// User���C���֐����s�ς݃t���OON
			}
			break;

		// User�T�u�֐�1
		case M_Label1:
			AddEntSymbolToObj();								// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
			User.UserFunc1(glbody,SelectedEntList,PickCount);	// User�T�u�֐�1�����s
			break;

		// User�T�u�֐�2
		case M_Label2:
			AddEntSymbolToObj();								// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
			User.UserFunc2(glbody,SelectedEntList,PickCount);	// User�T�u�֐�1�����s
			break;

		// User�T�u�֐�3
		case M_Label3:
			AddEntSymbolToObj();								// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
			User.UserFunc3(glbody,SelectedEntList,PickCount);	// User�T�u�֐�1�����s
			break;

		// User�T�u�֐�4
		case M_Label4:
			AddEntSymbolToObj();								// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
			User.UserFunc4(glbody,SelectedEntList,PickCount);	// User�T�u�֐�1�����s
			break;

		// User�T�u�֐�5
		case M_Label5:
			AddEntSymbolToObj();								// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
			User.UserFunc5(glbody,SelectedEntList,PickCount);	// User�T�u�֐�1�����s
			break;
	}
	glutPostRedisplay();
}

// ���_�؊�
void DESCRIBE::Sub_Menu_View(int val)
{
	switch(val){
		// X-View
		case S_M_XView:
			StartQ = QFunc.QInit(1,0,0,0);		// ���̉�]�p����������
			QFunc.QtoR(RotMx,StartQ);			// ���̉�]�p����������
			TargetQ = QFunc.QMult(QFunc.QGenRot(PI/2,0,0,1), StartQ);	// �N�H�[�^�j�I����ŏ����ʒu�ɑ΂���Z������90����]
			QFunc.QtoR(RotMx,TargetQ);			// ��]�s��ɑ��
			StartQ = QFunc.QCopy(TargetQ);
			ModelScale = ModelScale1st;			// �X�P�[��������
			Trl[0] = Trl[1] = Trl[2] = 0;		// ���s�ړ��ʏ�����
			break;
		// Y-View
		case S_M_YView:
			StartQ = QFunc.QInit(1,0,0,0);
			QFunc.QtoR(RotMx,StartQ);
			ModelScale = ModelScale1st;
			Trl[0] = Trl[1] = Trl[2] = 0;
			break;
		// Z-View
		case S_M_ZView:
			StartQ = QFunc.QInit(1,0,0,0);
			QFunc.QtoR(RotMx,StartQ);
			TargetQ = QFunc.QMult(QFunc.QGenRot(PI/2,1,0,0), StartQ);	// �N�H�[�^�j�I����ŏ����ʒu�ɑ΂���X������90����]
			QFunc.QtoR(RotMx,TargetQ);	
			StartQ = QFunc.QCopy(TargetQ);
			ModelScale = ModelScale1st;
			Trl[0] = Trl[1] = Trl[2] = 0;
			break;
	}
	glutPostRedisplay();
}

// IGES�`��R�A
void DESCRIBE::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);						// ���f���r���[�s��

	glLoadIdentity();								// ���f���r���[�s��̐擪�ɒP�ʍs���}��

	gluLookAt(0,1,0.0,0.0,0.0,0.0,0.0,0.0,1.0);		// ���_��ݒ�

	glScaled(ModelScale,ModelScale,ModelScale);		// ���f���X�P�[����ݒ�

	glTranslatef(Trl[0],Trl[1],Trl[2]);				// ���W�n�𕽍s�ړ�����
	
	glMultMatrixd(RotMx);							// ���f���r���[�s��ɕύX���ꂽ��]�s��������Ă���

	glDrawAxis(AxisScale,ModelScale);				// ���W����`��

	// BODY�`��t���O��ON�Ȃ�`��
	if(DrawBODYFlag == TRUE){
		if(DrawBODY1st == FALSE){									// ��ԏ���̂݃f�B�X�v���C���X�g��BODY��o�^
			glNewList(COMMAND_DRAW_BODY,GL_COMPILE_AND_EXECUTE);	// glDrawBODY()���R�}���h�Ƃ��ă������[�ɕۑ�(�Z���N�V���������̍�����)
			glDrawBODY();											// IGES�f�[�^��`��
			glEndList();
			DrawBODY1st = TRUE;
		}
		else{
			glCallList(COMMAND_DRAW_BODY);			// 2��ڈȍ~�̕`��Ăяo���̓f�B�X�v���C���X�g���Ăяo��
		}
	}

	// UserFunc�ɂ���Đ������ꂽ�`��w�߂����s
	if(DrawUserFuncFlag == TRUE)
		glCallList(COMMAND_DRAW_USER);

	// �X�C�[�v�Z���N�V�����t���O��ON�Ȃ�`��
	if(SweepSelectFlag == GL_TRUE){
		glMatrixMode(GL_PROJECTION);				// ���e�ϊ��s��X�^�b�N��I��
		glPushMatrix();								// ���݂̍��W�n��ۑ�
		glLoadIdentity();							// ���e�ϊ��s��X�^�b�N�̐擪�ɒP�ʍs���}��
		gluOrtho2D(0,ScreenWidth,ScreenHeight,0); 	// ���_��ݒ�(���o�[�o���h�𐳂����ʒu�ɕ\�������邽��)

		glMatrixMode(GL_MODELVIEW);					// ���f���r���[�s��
		glPushMatrix();								// ���݂̍��W�n��ۑ�
		glLoadIdentity();							// ���f���r���[�s��̐擪�ɒP�ʍs���}��
		
		glDrawRubberband(StartX,StartY,CurrentX,CurrentY);		// ���o�[�o���h��`��

		// ���W�n�����ɖ߂�
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	glutSwapBuffers();
}

// �E�C���h�E�̍č쐬
void DESCRIBE::Reshape(int width, int height)
{
	double ratio;
	double a=2;

	ScreenWidth = width;			// �ύX���ꂽ�X�N���[���̕��𓾂�
	ScreenHeight = height;			// �ύX���ꂽ�X�N���[���̍����𓾂�

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);	// ���e�ϊ��s��X�^�b�N��I��
	glLoadIdentity();				// ���e�ϊ��s��X�^�b�N�̐擪�ɒP�ʍs���}��

	// ���s���e��ݒ�(�A�X�y�N�g��ɂ���ă��f���̏c���䂪�ς��Ȃ��悤�ɏ����𕪂���)
	if(width <= height){
		ratio = (double)height/(double)width;
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}
	else{
		ratio = (double)width/(double)height;
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}

	glMatrixMode(GL_MODELVIEW);		// ���f���r���[�s��X�^�b�N��I��
}

// �O���t�B�b�N�̏����ݒ�
void DESCRIBE::Init(char *progname)
{
	GLfloat diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(ScreenWidth, ScreenHeight);		// �E�B���h�E�T�C�Y
	glutInitWindowPosition(100, 100);					// �E�B���h�E�ʒu
	glutCreateWindow(progname);							// �E�B���h�E����
	glClearColor( 0.0, 0.0, 0.0, 1.0 );					// �w�i�F

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

	SetNurbsStat();

	QFunc.QtoR(RotMx,StartQ);		// ��]�s��̏�����
}

// �L�[�C�x���g�̐ݒ�
void DESCRIBE::Keyboard(unsigned char key, int x, int y)
{
	if(key == 27){		// ESC�L�[�ŏI��
		exit(0);
	}
}

// �Z���N�V����(�}�E�X�s�b�L���O)�̐ݒ�
void DESCRIBE::DoSelect(int x,int y)
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
	if(DrawBODYFlag == TRUE)					// BODY�`��t���O��ON�Ȃ�
		glCallList(COMMAND_DRAW_BODY);			// �e�G���e�B�e�B�ŊK�w�����ꂽ�Z���N�V�����p�ԍ��𓾂�(���炩���ߓo�^���Ă������R�}���h�����s)
	glMatrixMode(GL_PROJECTION);				// �ˉe�ϊ����[�h�ɖ߂�
	glPopMatrix();								// �ˉe�ϊ��}�g���N�X�����ɖ߂�
	hits = glRenderMode(GL_RENDER);				// �Z���N�V�������[�h�I���A�����_�����O���[�h�ɖ߂�

	if(StartX == x && StartY == y)				// �N���b�N�őI�������ꍇ
		ClickPicking(SelectBuf,hits);
	else
		DragPicking(SelectBuf,hits);			// �h���b�O�őI�������ꍇ

	glutPostRedisplay();
}

// �N���b�N�ɂ��}�E�X�s�b�L���O
void DESCRIBE::ClickPicking(GLuint SelectBuf[],int hits)
{
	int i;
	int objnum=0;

	objnum = ObjSelect(SelectBuf,hits);			// �q�b�g�����I�u�W�F�N�g�̒��Ńf�v�X�l�̈�ԏ������I�u�W�F�N�g��I������

	// �ȑO�ɃZ���N�g���ꂽ���̂��ăZ���N�g�����ꍇ�̓J�E���g���Ȃ�
	for(i=0;i<PickCount;i++){
		if(SelectedEntList[i].Body == SelectBuf[objnum*(3+SelectBuf[0])+3] &&
			SelectedEntList[i].Type == SelectBuf[objnum*(3+SelectBuf[0])+4] &&
			SelectedEntList[i].Num == SelectBuf[objnum*(3+SelectBuf[0])+5]){
				hits = 0;
		}
	}
	
	// �V�����I�u�W�F�N�g�Ƀq�b�g�����ꍇ
	if(hits >= 1){
		SelectedEntList[PickCount].Body = SelectBuf[objnum*(3+SelectBuf[0])+3];	// �Z���N�g���ꂽBODY
		SelectedEntList[PickCount].Type = SelectBuf[objnum*(3+SelectBuf[0])+4];	// �Z���N�g���ꂽ�G���e�B�e�B�^�C�v
		SelectedEntList[PickCount].Num = SelectBuf[objnum*(3+SelectBuf[0])+5];	// �Z���N�g���ꂽ�G���e�B�e�B�^�C�v�ɂ����鎯�ʔԍ�
		BODY *Curr = glbody;
		while(Curr != NULL){
			if(SelectedEntList[PickCount].Body == Curr->key && SelectedEntList[PickCount].Type == _NURBSC){
				ChangeStatColor(Curr->NurbsC[SelectedEntList[PickCount].Num].Dstat.Color,1,0.2,1,1);	// �I���ς݂��������ߐF��ύX
			}
			else if(SelectedEntList[PickCount].Body == Curr->key && SelectedEntList[PickCount].Type == _TRIMMED_SURFACE){
				ChangeStatColor(Curr->NurbsS[SelectedEntList[PickCount].Num].Dstat.Color,1,1,0.2,1);	// �I���ς݂��������ߐF��ύX
			}
			Curr = ShiftBodyList(Curr);
		}
		PickCount++;
		UpDateDisplayList(FALSE);
	}
}

// �h���b�O�ɂ��}�E�X�s�b�L���O
void DESCRIBE::DragPicking(GLuint SelectBuf[],int hits)
{
	int i,j;
	int temp=0;	// �ăZ���N�g���ʗp�J�E���^

	// �͈͑I���ɂ���ăI�u�W�F�N�g�Ƀq�b�g�����ꍇ
	for(i=0;i<hits;i++){
		for(j=0;j<PickCount;j++){
			if(SelectedEntList[i].Body == SelectBuf[j*(3+SelectBuf[0])+3] &&
				SelectedEntList[i].Type == SelectBuf[j*(3+SelectBuf[0])+4] &&
				SelectedEntList[i].Num == SelectBuf[j*(3+SelectBuf[0])+5]){
					temp++;
			}
		}
		// �I�u�W�F�N�g���V�����q�b�g�����ꍇ
		if(temp == 0){
			SelectedEntList[PickCount].Body = SelectBuf[i*(3+SelectBuf[0])+3];	// �Z���N�g���ꂽBODY
			SelectedEntList[PickCount].Type = SelectBuf[i*(3+SelectBuf[0])+4];	// �Z���N�g���ꂽ�G���e�B�e�B�^�C�v
			SelectedEntList[PickCount].Num = SelectBuf[i*(3+SelectBuf[0])+5];	// �Z���N�g���ꂽ�G���e�B�e�B�^�C�v�ɂ����鎯�ʔԍ�
			BODY *Curr = glbody;
			while(Curr != NULL){
				if(SelectedEntList[PickCount].Body == Curr->key && SelectedEntList[PickCount].Type == _NURBSC){
					ChangeStatColor(Curr->NurbsC[SelectedEntList[PickCount].Num].Dstat.Color,1,0.2,1,1);	// �I���ς݂��������ߐF��ύX
				}
				else if(SelectedEntList[PickCount].Body == Curr->key && SelectedEntList[PickCount].Type == _TRIMMED_SURFACE){
					ChangeStatColor(Curr->NurbsS[SelectedEntList[PickCount].Num].Dstat.Color,1,1,0.2,1);	// �I���ς݂��������ߐF��ύX
				}
				Curr = ShiftBodyList(Curr);
			}
			PickCount++;
		}
		temp = 0;
	}
	UpDateDisplayList(FALSE);
}

// �I�u�W�F�N�g�̑I�𔻕�
int DESCRIBE::ObjSelect(GLuint SelectBuf[],int hits)
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

// ����L�[�C�x���g�̐ݒ�
void DESCRIBE::Special(int key,int x,int y)
{
	switch(key){
		case GLUT_KEY_UP:
			ModelScale += 0.2*ModelScale;	// �g��
			break;
		case GLUT_KEY_DOWN:
			ModelScale -= 0.2*ModelScale;	// �k��
			break;
		default:
			break;
	}
	StartX = x;
	StartY = y;
	glutPostRedisplay();
}

// �}�E�X�N���b�N�C�x���g�̐ݒ�
void DESCRIBE::Mouse(int button,int state,int x,int y)
{
	int GetModif = glutGetModifiers();		// Shift,Ctrl,Alt�L�[�̎擾

	// ���N���b�N
	if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
		StartX = x;
		StartY = y;
		MBtnFlag = GL_TRUE;
		if(GetModif == GLUT_ACTIVE_SHIFT){		// Sift
			OldPosX = x;
			OldPosY = y;
			ShiftKeyFlag = GL_TRUE;
		}
		if(GetModif == GLUT_ACTIVE_CTRL){		// Ctrl
			OldPosX = x;
			OldPosY = y;
			CtrlKeyFlag = GL_TRUE;
		}
	}
	// ���N���b�N
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		StartX = x;
		StartY = y;
		LBtnFlag = GL_TRUE;
	}
	// �{�^���𗣂���
	else{
		if(MBtnFlag == GL_TRUE){
			StartQ = QFunc.QCopy(TargetQ);
		}
		if(LBtnFlag == GL_TRUE){
			DoSelect(x,y);		// �I�u�W�F�N�g�I�𔻕�
		}
		SweepSelectFlag = GL_FALSE;
		LBtnFlag = GL_FALSE;
		MBtnFlag = GL_FALSE;
		ShiftKeyFlag = GL_FALSE;
		CtrlKeyFlag = GL_FALSE;
	}

	glutPostRedisplay();
}

// �}�E�X���ړ������Ƃ��ɌĂяo�����R�[���o�b�N�̐ݒ�
void DESCRIBE::Motion(int x,int y)
{
	double dx,dy;				// �P�ʉ����ꂽ�}�E�X�ړ���
	double dox,doy;				// �P�ʉ����ꂽ�E�B���h�E�ĕ`�撼�O����̃}�E�X�ړ���
	double a;					// �}�E�X�ړ�����
	double ar;					// ��]�p
	Quat dq;					// ��]�N�H�[�^�j�I��
	Coord axis;					// ��]��

	// ���h���b�O��
	if(LBtnFlag == GL_TRUE){
		SweepSelectFlag = GL_TRUE;		// �X�B�[�v�Z���N�V����ON
		CurrentX = x;
		CurrentY = y;
	}

	// �^�񒆃h���b�O��
	if(MBtnFlag == GL_TRUE){
		// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̕ψ�
		dx = (double)(x-StartX)/(double)ScreenWidth;
		dy = (double)(y-StartY)/(double)ScreenHeight;
		// �}�E�X�|�C���^�̈ʒu�̃E�B���h�E�ĕ`�撼�O����̕ψ�
		dox = (double)(x-OldPosX)/(double)ScreenWidth;
		doy = (double)(y-OldPosY)/(double)ScreenHeight;

		// Shift,Ctrl�L�[�������Ă��Ȃ�-->���f����]
		if(ShiftKeyFlag == GL_FALSE && CtrlKeyFlag == GL_FALSE){
			a = sqrt(dx*dx + dy*dy);	// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̋���
			if(a != 0.0){
				ar = 2*PI*a;			// ��]�p���Z�o
				// ��]��axis�����߂�
				axis.x = dx/a;
				axis.y = 0.0;
				axis.z = -dy/a;
				dq = QFunc.QGenRot(ar,axis.z,axis.y,axis.x);	// ��]�N�H�[�^�j�I������
				TargetQ = QFunc.QMult(dq, StartQ);				// ��]�̏����lStartQ��dq���|���ĉ�]������
				QFunc.QtoR(RotMx, TargetQ);						// �N�H�[�^�j�I�������]�̕ϊ��s������߂�
			}
		}

		// Shift�L�[�������Ă���-->���s�ړ�
		else if(ShiftKeyFlag == GL_TRUE){
			Trl[0] -= 4*dox/ModelScale;							// �}�E�X�����E�֓��������獶�E�֕��s�ړ�
			Trl[2] -= 4*doy/ModelScale;							// �}�E�X���㉺�֓���������㉺�֕��s�ړ�
		}

		// Ctrl�L�[�������Ă���-->�g��k��
		else if(CtrlKeyFlag == GL_TRUE){
			a = sqrt(dox*dox + doy*doy);
			if(x-OldPosX > 0)	ModelScale += a*ModelScale;		// �}�E�X���E�֓���������g��
			else				ModelScale -= a*ModelScale;		// �}�E�X�����֓���������k��
			if(ModelScale < 0.0001) ModelScale = 0.0001;		// �k���X�P�[���̍ŏ��l
		}			

		OldPosX = x;
		OldPosY = y;
	}

	glutPostRedisplay();
}

// body��ǂݍ��݃��X�g�ɓo�^
int DESCRIBE::GenBodyList()
{
	BODY *new_body;

	new_body = AllocBodyList();					// body��V����1�������[�m��

	if(Parser.IGES_Parser_Main(new_body) == ERR){		// IGES�f�[�^�̓ǂݍ���
		return ERR;
	}

	CheckDegenracy(new_body);					// �k��(2D�p�����g���b�N�Ȑ��̎n�_�ƏI�_����v���Ă��邩)�̃`�F�b�N

	new_body->key = BodyCount;					// ���݂�body�ԍ��𓾂�
	
	AddBodyList(&glbody,BodyCount-1,new_body);	// �ǂݍ���body�����X�g�̍Ō���Ɍq��

	BodyCount++;

	return TRUE;
}



// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
void DESCRIBE::AddEntSymbolToObj()
{
	int Cnum = 0;
	int Snum = 0;

	for(int i=0;i<PickCount;i++){
		if(SelectedEntList[i].Type == _NURBSC){
			SelectedEntList[i].CCount = Cnum;
			Cnum++;
		}
		else if(SelectedEntList[i].Type == _TRIMMED_SURFACE){
			SelectedEntList[i].SCount = Snum;
			Snum++;
		}
		else{
			fprintf(stderr,"DESCRIBE ERROR:�T�|�[�g�O�̃G���e�B�e�B�����o���܂���\n");
		}
	}
}

// �A�C�h�����O���̏���
void DESCRIBE::Idle()
{
	glutPostRedisplay();
}

// �f�B�X�v���C���X�g�̍X�V
void DESCRIBE::UpDateDisplayList(int Val)
{
	glNewList(COMMAND_DRAW_BODY,GL_COMPILE);	// glDrawBODY()���R�}���h�Ƃ��ă������[�ɕۑ�(�Z���N�V���������̍�����)
	glDrawBODY();								// IGES�f�[�^��`��
	glEndList();

	if(DrawUserFuncFlag == TRUE && Val == TRUE){	// UserFunc�����s�ς݂Ȃ�UserFunc�����X�g������(UserFucn�����s�ς݂ł��A���X�g���������Ȃ��ꍇ��Val��FALSE��^����)
		glNewList(COMMAND_DRAW_USER,GL_COMPILE);
		User.UserFuncMain(glbody,SelectedEntList,PickCount);
		glEndList();
	}
}

// ���`��
void DESCRIBE::glDrawAxis(double Ascale,double Mscale)
{
	double AxisPos = Ascale/Mscale;
	double CharPos = (Ascale+0.1)/Mscale;

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
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4.0,1.0);
	glPopMatrix();

	/* Draw Ycone */
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, AxisPos, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4.0,1.0);
	glPopMatrix();

	/* Draw Zcone */
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, AxisPos);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4.0,1.0);
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
void DESCRIBE::glDrawRubberband(double sx,double sy,double ex,double ey)
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

// �o�[�W���������o��
void DESCRIBE::VersionInfo()
{
	fprintf(stderr,"KODATUNO R1.0\n");
}
