#include"UserFunc.h"

// User�p���C���֐�1�A�T�u�֐�5�܂œo�^���邱�Ƃ��ł��܂��B
// �J���҂͂����ɓƎ��̏�����ǉ����Ă�������
// �e�֐��͈ȉ��̈����������܂�  
// *Body:BODY���X�g�̐擪�|�C���^
// *Obj:�Z���N�V�������ꂽ�G���e�B�e�B���
// PickCount:�Z���N�V�������ꂽ�G���e�B�e�B�̐�

// User���C���֐�
void USER::UserFuncMain(BODY *Body,OBJECT *Obj,int PickCount)
{
	SampleMain(Body,Obj,PickCount);		// �T���v��
}

// User�T�u�֐�1
void USER::UserFunc1(BODY *Body,OBJECT *Obj,int PickCount)
{
	// �����ɐV���ȏ�����ǉ��ł��܂�
}

// User�T�u�֐�2
void USER::UserFunc2(BODY *Body,OBJECT *Obj,int PickCount)
{
	// �����ɐV���ȏ�����ǉ��ł��܂�
}

// User�T�u�֐�3
void USER::UserFunc3(BODY *Body,OBJECT *Obj,int PickCount)
{
	// �����ɐV���ȏ�����ǉ��ł��܂�
}

// User�T�u�֐�4
void USER::UserFunc4(BODY *Body,OBJECT *Obj,int PickCount)
{
	// �����ɐV���ȏ�����ǉ��ł��܂�
}

// User�T�u�֐�5
void USER::UserFunc5(BODY *Body,OBJECT *Obj,int PickCount)
{
	// �����ɐV���ȏ�����ǉ��ł��܂�
}

// User���j���[����o�^
void USER::SetMenuLabelName()
{
	// �Ǝ��̃��j���[���������ɋL�q���Ă�������
	strcpy(MenuLabelMain,"Exec Sample Func");
	strcpy(MenuLabel1,"Exec Func1");
	strcpy(MenuLabel2,"Exec Func2");
	strcpy(MenuLabel3,"Exec Func3");
	strcpy(MenuLabel4,"Exec Func4");
	strcpy(MenuLabel5,"Exec Func5");
}

// �R���X�g���N�^
USER::USER()
{
	SetMenuLabelName();		// User���j���[����o�^
}