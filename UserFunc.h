#ifndef _USER_FUNC_H_
#define _USER_FUNC_H_

#include"BODY.h"
#include"Sample.h"

#define LABELNAMEMAX 256	// ���j���[�ɓo�^���郁�j���[���o�b�t�@

enum Sub_Menu_Label{
	M_Label_Main,
	M_Label1,
	M_Label2,
	M_Label3,
	M_Label4,
	M_Label5
};

// User�N���X�F�`��Z�N�V�����ł̃��j���[�I���ɂ�蔭������C�x���g���e���L�q
class USER
{
public:
	USER();										// �R���X�g���N�^
	void UserFuncMain(BODY *,OBJECT *,int);		// User���C���֐�
	void UserFunc1(BODY *,OBJECT *,int);		// User�T�u�֐�1
	void UserFunc2(BODY *,OBJECT *,int);		// User�T�u�֐�2
	void UserFunc3(BODY *,OBJECT *,int);		// User�T�u�֐�3
	void UserFunc4(BODY *,OBJECT *,int);		// User�T�u�֐�4
	void UserFunc5(BODY *,OBJECT *,int);		// User�T�u�֐�5

private:
	void SetMenuLabelName();					// ���j���[�ɓo�^���郁�j���[�����Z�b�g

public:
	char MenuLabelMain[LABELNAMEMAX];			// User���C���֐��̃��j���[��
	char MenuLabel1[LABELNAMEMAX];				// User�T�u�֐�1�̃��j���[��
	char MenuLabel2[LABELNAMEMAX];				// User�T�u�֐�2�̃��j���[��
	char MenuLabel3[LABELNAMEMAX];				// User�T�u�֐�3�̃��j���[��
	char MenuLabel4[LABELNAMEMAX];				// User�T�u�֐�4�̃��j���[��
	char MenuLabel5[LABELNAMEMAX];				// User�T�u�֐�5�̃��j���[��


///////////�V���ɒǉ�����ꍇ�͈ȉ��ɋL�q//////////////////////////////////////////////////////////

};

#endif