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

	// User�X�e�[�^�X�̏�����
	UserStat.Mode = 0;
	for(int i=0;i<USERPROPNUM;i++)
		UserStat.Prop[i] = 0;
}

// User�{�^���o�^�֐�4�̎��̂��L�q
int USER::CalcIntersecPts(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
	if(UserStat.Mode == 0){
		ExecSampleFunc1(BodyList,ObjList,pickcount,UserStat.Prop);		// �T���v���֐�1�����s����
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 1){
		ExecSampleFunc2(BodyList,ObjList,pickcount,UserStat.Prop);		// �T���v���֐�2�����s����
		return KOD_TRUE;
	}
	else{
		// void
		return KOD_FALSE;
	}
}

// User��WS�̋@�\���g���V����Dialog���|�b�v�A�b�v���������ꍇ�́A�����ɂ���WS�������L�q
// ����   val:User�{�^���̃V���{��(0�`5)
int USER::ExecWSFunc(int val)
{
	// User�{�^��1�������ꂽ
	if(val == 0){
		// ������ǉ��ł��܂�
	}

	// User�{�^��2�������ꂽ
	else if(val == 1){
		// ������ǉ��ł��܂�
	}

	// User�{�^��3�������ꂽ
	else if(val == 2){
		// ������ǉ��ł��܂�
	}

	// User�{�^��4�������ꂽ
	else if(val == 3){
		// ������ǉ��ł��܂�
	}

	// User�{�^��5�������ꂽ
	else if(val == 4){
		// ������ǉ��ł��܂�
	}

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

// User�R�}���h��o�^
void USER::RegistUserCommand(CommandMap *CmdMap)
{
	// �Ǝ��̃R�}���h���������ɋL�q���Ă�������
	// ��5������NULL���w��

}

// User�R�}���h�����s
// ����   *BodList:BODY���X�g�̐擪�A�h���X    *ObjList:�Z���N�V�������ꂽ�G���e�B�e�B�̏��(�Z���N�V�����̐��������X�g��)
//        pickcount:�Z���N�V������    CmdNo:���s���ꂽ�R�}���h�ԍ�      argc,*argv[]:�����̃A��
void USER::Command(BODYList *BodyList,OBJECTList *ObjList,int pickcount,int CmdNo,int argc,char *argv[])
{
	// �o�^�����R�}���h���Ƃɏ������L�q���Ă�������

}











///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ȍ~��Kodatuno�J���҈ȊO�͕ύX���Ȃ�����
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// User�X�e�[�^�X��UserStat�\���̂Ɋi�[(User Status�_�C�A���OOK�{�^���������Ɏ��s�����)(�����ɂ͉����L�q���Ȃ�)
void USER::SetUserStatus(int mode,double prop[])
{
	UserStat.Mode = mode;
	for(int i=0;i<USERPROPNUM;i++){
		UserStat.Prop[i] = prop[i];
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