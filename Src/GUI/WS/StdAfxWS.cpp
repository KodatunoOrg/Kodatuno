#include "StdAfxWS.h"

GUI_Interface GuiIF;		// �N���XGUI_Interface�̃C���X�^���X���O���[�o���Ő錾

// �R���\�[���Ƀ��b�Z�[�W���o��
int GUI_Interface::SetMessage(char *mes)
{
	fprintf(stderr,"%s\n",mes);						// ���b�Z�[�W��W���o��
	Console_Form->addString(mes);					// ���b�Z�[�W���o��
	Console_Form->addString("\n>");					// ���̃R�}���h�ʒu�ֈړ�
	int len = Console_Form->getString().getChars();	// �������J�E���g
	Console_Form->setProperty(WSNuserValue,len);	// ���݂܂ł̕�������ێ�
	return len;
}

// �R���\�[���ɃR�}���h���o��
int GUI_Interface::SetCommand(char *cmd)
{
	Console_Form->addString(cmd);					// ���b�Z�[�W���o��
	int len = Console_Form->getString().getChars();	// �������J�E���g
	Console_Form->setProperty(WSNcursorPos,len);	// �J�[�\�����Ō�ɂ����Ă���
	return len;
}

// �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
void GUI_Interface::AddBodyNameToWin(const char *name)
{
	char* mes = const_cast<char*>(name);
	Body_List_Win->addItem(mes);					// Body���X�g�E�B���h�E��BODY����o�^
}

// �w�肳�ꂽ�ԍ���BODY�����X�g����폜����
void GUI_Interface::DelBodyFromWin(int pos)
{
	Body_List_Win->delPos(pos);
}

// �t�@�C��OPEN�p�I�u�W�F�N�g�̏�Ԑݒ�
void GUI_Interface::SetEnableOpenFile(bool flag)
{
	Open_Body_Btn->setSensitive(flag);
}

// BODY�폜�p�I�u�W�F�N�g�̏�Ԑݒ�
void GUI_Interface::SetEnableDelBody(bool flag)
{
	Del_Body_Btn->setSensitive(flag);
}

// �`��pGUI�̍ĕ`��w��
void GUI_Interface::UpdateDescribeWidget()
{
	Describe_Form->execEventProc(WSEV_EXPOSE,NULL);
}

// �`��̈�̍ĕ`��w��
void GUI_Interface::RedrawDescribeForm()
{
	Describe_Form->redraw();
}

// UserFunc1�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc1Label(const char *label)
{
	User_Func_Btn1->setProperty(WSNlabelString,label);
}

// UserFunc2�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc2Label(const char *label)
{
	User_Func_Btn2->setProperty(WSNlabelString,label);
}

// UserFunc3�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc3Label(const char *label)
{
	User_Func_Btn3->setProperty(WSNlabelString,label);
}

// UserFunc4�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc4Label(const char *label)
{    
	User_Func_Btn4->setProperty(WSNlabelString,label);
}

// UserFunc5�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc5Label(const char *label)
{    
	User_Func_Btn5->setProperty(WSNlabelString,label);
}

// UserFunc6�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc6Label(const char *label)
{    
	User_Func_Btn6->setProperty(WSNlabelString,label);
}

// UserFunc7�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc7Label(const char *label)
{    
	User_Func_Btn7->setProperty(WSNlabelString,label);
}

// UserFunc8�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc8Label(const char *label)
{    
	User_Func_Btn8->setProperty(WSNlabelString,label);
}

// �t�@�C���I�[�v���_�C�A���O�̕\���iBODY�j
// return �t�@�C���̃t���p�X
std::string GUI_Interface::OpenBodyFileDlg()
{
	WSCfileSelect *fs=WSGIappFileSelect();			// �t�@�C���I���_�C�A���O�̃A�h���X���擾

	fs->setProperty(WSNmaskFileName,"IGS,igs,STL,stl,WRL,wrl,dxf,DXF");		// IGS,STL,VRML,DXF�t�@�C���̂ݑI����

	fs->setProperty(WSNx,300);						// �_�C�A���O�\���ʒu
	fs->setProperty(WSNy,320);

	long fchoice_dlg = fs->popup();					// �t�@�C���I���_�C�A���O���J��
	if(fchoice_dlg == WS_DIALOG_OK){
		WSCstring full_name = fs->getFileName();	// �t�@�C���̃t���p�X�擾
		std::string s= std::string(full_name);
		return s;
	}
	
	return "";
}

// �t�@�C���I�[�v���_�C�A���O�̕\���iINP�j
//return �t�@�C���̃t���p�X
std::string GUI_Interface::OpenInpFileDlg()
{
	WSCfileSelect *fs=WSGIappFileSelect();			// �t�@�C���I���_�C�A���O�̃A�h���X���擾

	fs->setProperty(WSNmaskFileName,"INP,inp");		// inp�t�@�C���̂ݑI����

	long fchoice_dlg = fs->popup();					// �t�@�C���I���_�C�A���O���J��

	if(fchoice_dlg == WS_DIALOG_OK){				// �_�C�A���O��OK�{�^���������ꂽ��
		WSCstring full_name = fs->getFileName();	// �t�@�C���̃t���p�X�擾
		InpFileName->setProperty(WSNlabelString,full_name);	// �t�@�C���̃t���p�X�����[�U�[�ݒ蕶����ɃZ�b�g
		std::string s= std::string(full_name);
		return s;
	}
	return "";
}

// �t���p�X����t�@�C�������o
std::string GUI_Interface::GetFileName(const std::string fullPath)
{
	WSCstring full_name = fullPath.c_str();
	int priodnum = full_name.getWords("/");				// �t�@�C�����Ɋ܂܂�Ă���X���b�V���̐��𒲂ׂ�
	WSCstring fname = full_name.getWord(priodnum-1,"/");	// �t�@�C�������擾
	std::string s= std::string(fname);
	return s;
}

// �t���p�X����t�@�C���̊g���q���o
std::string GUI_Interface::GetFileExt(const std::string fullPath)
{
	WSCstring full_name = fullPath.c_str();
	int priodnum = full_name.getWords(".");				// �t�@�C�����Ɋ܂܂�Ă���X���b�V���̐��𒲂ׂ�
	WSCstring ext = full_name.getWord(priodnum-1,".");	// �t�@�C�������擾
	std::string s= std::string(ext);
	return s;
}

// �R���\�[�����當������擾
std::string GUI_Interface::GetConsoleText()
{
	std::string mes = std::string(Console_Form->getString());
    return mes;
}

// BODY���X�g�̑I������Ă���ԍ����擾����
int GUI_Interface::GetBodyListSelectedPos()
{
    return Body_List_Win->getSelectedPos();
}

// Nurbs�Ȑ��_�C�A���O�\��
void GUI_Interface::ShowNurbsCurveDlg()
{
	NurbsCurve_Dlg->setVisible(True);
}

// Nurbs�Ȗʃ_�C�A���O�\��
void GUI_Interface::ShowNurbsSurfDlg()
{ 
	NurbsSurface_Dlg->setVisible(True);
}

// ��]�T�[�t�F�X�_�C�A���O�\��
void GUI_Interface::ShowRotSurfDlg()
{
	RotSurf_Dlg->setVisible(True);		// ��]�T�[�t�F�X�����_�C�A���O��\��
}

// �X�C�[�v�T�[�t�F�X�_�C�A���O�\��
void GUI_Interface::ShowSweepSurfDlg()
{
	SweepSurf_Dlg->setVisible(True);	// �X�C�[�v�T�[�t�F�X�����_�C�A���O��\��
}

