#include "UserFunc.h"

// User�p�֐���5�܂œo�^���邱�Ƃ��ł��܂��B
// �J���҂͂����ɓƎ��̏�����ǉ����Ă�������
// �e�֐��͈ȉ��̈����������܂�  

// �R���X�g���N�^
USER::USER()
{
	// ������UserFunc�֐��|�C���^�̎w���֐����w�肷��
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(i==0)
			UserFunc[i] = &USER::ExecSampleFunc0;		// �T���v��
		else if(i==1)
			UserFunc[i] = &USER::ExecSampleFunc1;		// �T���v��
		else if(i==2)
			UserFunc[i] = &USER::ExecSampleFunc2;		// �T���v��
		else if(i==3)
            UserFunc[i] = &USER::ExecSampleFunc3;		// �T���v��
		else if(i==4)
            UserFunc[i] = &USER::MasterUserFunc;		// ���g�p��UserFunc[i]�ɂ�MasterUserFunc()��o�^���Ă���
        else if(i==5)
            UserFunc[i] = &USER::MasterUserFunc;		// ���g�p��UserFunc[i]�ɂ�MasterUserFunc()��o�^���Ă���
        else if(i==6)
            UserFunc[i] = &USER::MasterUserFunc;		// ���g�p��UserFunc[i]�ɂ�MasterUserFunc()��o�^���Ă���
        else if(i==7)
            UserFunc[i] = &USER::MasterUserFunc;		// ���g�p��UserFunc[i]�ɂ�MasterUserFunc()��o�^���Ă���
        else
            UserFunc[i] = &USER::MasterUserFunc;		// ���g�p��UserFunc[i]�ɂ�MasterUserFunc()��o�^���Ă���
	}

	// User�X�e�[�^�X�̏�����
	UserStat.Mode = 0;
	for(int i=0;i<USERPROPNUM;i++)
		UserStat.Prop[i] = 0;
}

// User�{�^���o�^�֐�0�̎��̂��L�q
// *BodyList:BODY�̎��̂��o�^���Ƀ��X�g����Ă���
// *ObjList:�Z���N�V�������ꂽ�G���e�B�e�B���
// PickCount:�Z���N�V�������ꂽ�G���e�B�e�B�̐�
int USER::ExecSampleFunc0(BODYList *BodyList,OBJECTList *ObjList,int PickCount)
{
	char mes[256];

	// ���[�U�[�X�e�[�^�X�ł̑I�����ꂽ���[�h�ԍ����R���\�[���o��
	sprintf(mes,"Selected Mode : %d",UserStat.Mode);
    GuiIF.SetMessage(mes);

	// ���[�U�[�X�e�[�^�X�ł̃v���p�e�B�l(�ŏ���2��)���R���\�[���o��
	sprintf(mes,"User Property 1: %lf",UserStat.Prop[0]);
    GuiIF.SetMessage(mes);
	sprintf(mes,"User Property 2: %lf",UserStat.Prop[1]);
    GuiIF.SetMessage(mes);

    GuiIF.SetMessage("....");

	// �Z���N�V���������R���\�[���o��
	sprintf(mes,"All Pick Count : %d",PickCount);
    GuiIF.SetMessage(mes);

	for(int i=0;i<PickCount;i++){
		OBJECT *obj = (OBJECT *)ObjList->getData(i);			// i�ԖڂɃZ���N�V�������ꂽ�G���e�B�e�B�̏��𓾂�
		sprintf(mes,"#%d",i+1);
        GuiIF.SetMessage(mes);
		sprintf(mes,"Body No. : %d",obj->Body);
        GuiIF.SetMessage(mes);
		sprintf(mes,"Selected Object Type : %d",obj->Type);
        GuiIF.SetMessage(mes);
		sprintf(mes,"Selected Object No. : %d",obj->Num);
        GuiIF.SetMessage(mes);
	}

	return KOD_TRUE;
}
	

// User�{�^���o�^�֐�1�̎��̂��L�q
// *BodyList:BODY�̎��̂��o�^���Ƀ��X�g����Ă���
// *ObjList:�Z���N�V�������ꂽ�G���e�B�e�B���
// PickCount:�Z���N�V�������ꂽ�G���e�B�e�B�̐�
int USER::ExecSampleFunc1(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
	// User Status��Mode�ɂ���ď����𕪂���
	if(UserStat.Mode == 0){
		SmpRegistBody(BodyList,ObjList);	// IGES�t�@�C����ǂݍ��݁CBody�Ƃ��ēo�^����
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 1){
		SmpTransBody(BodyList,ObjList,pickcount,UserStat.Prop);		// �w�肵��Body�ɑ΂��A�t�B���ϊ����s��
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 2){
		SmpDivCurves(BodyList,ObjList,pickcount,UserStat.Prop);		// �Ȑ����w�肵���������ŕ��������_�𐶐�����
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 3){
		SmpNearestPt(BodyList,ObjList,pickcount,UserStat.Prop);		// �C�ӂ̓_����̋Ȗʏ�̍ŋߖT�_�����߂�
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 4){
		SmpUVDivLine(BodyList,ObjList,pickcount,UserStat.Prop);		// �Ȗʂ�UV�p�����[�^�œ��Ԋu�ɕ��������_�𐶐�����
		return KOD_TRUE;
	}
	else{
		// void
		return KOD_FALSE;
	}
}

// User�{�^���o�^�֐�2�̎��̂��L�q
int USER::ExecSampleFunc2(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
	// User Status��Mode�ɂ���ď����𕪂���
	if(UserStat.Mode == 0){
		SmpContourLine(BodyList,ObjList,pickcount,UserStat.Prop);		// �������𐶐�����
        return KOD_TRUE;
	}
	else if(UserStat.Mode == 1){
		SmpIntersectSurfs(BodyList,ObjList,pickcount,UserStat.Prop);	// �Ȗʓ��m�̌�����Z�o����
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 2){
		SmpMeanCurvature(BodyList,ObjList,pickcount,UserStat.Prop);		// ���ϋȗ��@���x�N�g�����Z�o����
		return KOD_TRUE;
	}
	else{
		// void
		return KOD_FALSE;
	}
}

// User�{�^���o�^�֐�3�̎��̂��L�q
int USER::ExecSampleFunc3(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
    GuiIF.SetMessage("aaaaaaa");
    return KOD_TRUE;
}


// User��Widget�̋@�\���g���V����Dialog���|�b�v�A�b�v���������ꍇ�́A�����ɂ���WS�������L�q
// ����   val:User�{�^���̃V���{��(0�`5)
int USER::ExecWidgetFunc(int val)
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
    else if(val == 5){
        // ������ǉ��ł��܂�
    }

    else if(val == 6){
        // ������ǉ��ł��܂�
    }

    else if(val == 7){
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
	strcpy(MenuLabel1,"Sample Func 0");
	strcpy(MenuLabel2,"Sample Func 1");
	strcpy(MenuLabel3,"Sample Func 2");
	strcpy(MenuLabel4,"Sample Func 3");
	strcpy(MenuLabel5,"User Func 5");
    strcpy(MenuLabel6,"User Func 6");
    strcpy(MenuLabel7,"User Func 7");
    strcpy(MenuLabel8,"User Func 8");
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

// User�X�e�[�^�X��Ԃ�
UserStat_ *USER::GetUserStatus()
{
    return &UserStat;
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
        case M_Label6:
            return MenuLabel6;
        case M_Label7:
            return MenuLabel7;
        case M_Label8:
            return MenuLabel8;
	}

	return NULL;
}

// UserFunc�֐��ւ̃|�C���^�̏����l�Ƃ��Ďg�p(�����ɂ͉����L�q���Ȃ�)
int USER::MasterUserFunc(BODYList *BodyList,OBJECTList *Obj,int PickCount)
{
    GuiIF.SetMessage("User Func is not registered.");
	return KOD_TRUE;
}
