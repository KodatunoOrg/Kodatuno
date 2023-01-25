#ifndef _USER_FUNC_H_
#define _USER_FUNC_H_

#include "StdAfx.h"
#include "Command.h"
#include "BODY.h"
#include "UserFuncSub.h"


#define LABELNAMEMAX 256	// ���j���[�ɓo�^���郁�j���[���o�b�t�@
#define USERFUNCNUMMAX	5	// UserFunc�̍ő�o�^��
#define USERPROPNUM	15		// ���p�ł���User�v���p�e�B��


// �R�}���h�o�^�ԍ�(�o�^�R�}���h�̃V���{����100�`199�̐��l�Ŏw�肷��)

// User�X�e�[�^�X�l�i�[�p�\����
typedef struct{
	int Mode;
	double Prop[USERPROPNUM];
}UserStat_;

enum Sub_Menu_Label{
	M_Label1,
	M_Label2,
	M_Label3,
	M_Label4,
	M_Label5
};

// User�N���X�F�`��Z�N�V�����ł̃��j���[�I���ɂ�蔭������C�x���g���e���L�q
class USER
{
// KODATUNO�V�X�e���Ƃ̃u���b�W�֐�(�J���҈ȊO�͕ύX���Ȃ�����) ///////////////////////////////////////
public:
	USER();														// �R���X�g���N�^
	int (USER::*UserFunc[USERFUNCNUMMAX])(BODYList *,OBJECTList *,int);	// UserFunc�֐��ւ̃|�C���^
	char *GetMenuLabelName(int);								// ���j���[����n��	
	void SetMenuLabelName();									// ���j���[�ɓo�^���郁�j���[�����Z�b�g
	void RegistUserCommand(CommandMap *);						// User�R�}���h�̓o�^
	void Command(BODYList *,OBJECTList *,int,int,int,char *[]);	// User�R�}���h�̎��s
	int MasterUserFunc(BODYList *,OBJECTList *,int);			// ���g�pUserFunc�֐��ւ̃|�C���^�͕K������ɂ���
	void SetUserStatus(int,double []);							// User�X�e�[�^�X��UserStat�\���̂Ɋi�[
	void ExecdClear();											// Canncel�{�^�����������Ƃ��̊֐������s�����̂ŁA�����ɏ������R�[�h��}�����Ă���
	int ExecWSFunc(int);										// �eUser�{�^���ɑ΂���WS�֐����g�p�������ꍇ�͂��̊֐����Ŋ��蓖�Ă邱��

private:
	char MenuLabel1[LABELNAMEMAX];				// User�֐�1�̃��j���[��
	char MenuLabel2[LABELNAMEMAX];				// User�֐�2�̃��j���[��
	char MenuLabel3[LABELNAMEMAX];				// User�֐�3�̃��j���[��
	char MenuLabel4[LABELNAMEMAX];				// User�֐�4�̃��j���[��
	char MenuLabel5[LABELNAMEMAX];				// User�֐�5�̃��j���[��
	UserStat_	UserStat;						// User�X�e�[�^�X

///////////////////////////////////////////////////////////////////////////////////////////////////////


///////////�V����User�֐�����o�^����ꍇ�͈ȉ���private:�ŋL�q////////////////////////////////////////
private:
	int CalcIntersecPts(BODYList *,OBJECTList *,int);	// �T���v��

};

#endif
