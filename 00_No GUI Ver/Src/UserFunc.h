#ifndef _USER_FUNC_H_
#define _USER_FUNC_H_

#include "StdAfx.h"
#include "BODY.h"
#include "NURBS_Func.h"


#define LABELNAMEMAX 256	// ���j���[�ɓo�^���郁�j���[���o�b�t�@
#define USERFUNCNUMMAX	5	// UserFunc�̍ő�o�^��
#define USERPROPNUM	15		// ���p�ł���User�v���p�e�B��

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
	int MasterUserFunc(BODYList *,OBJECTList *,int);			// ���g�pUserFunc�֐��ւ̃|�C���^�͕K������ɂ���
	void ExecdClear();											// Canncel�{�^�����������Ƃ��̊֐������s�����̂ŁA�����ɏ������R�[�h��}�����Ă���

private:
	char MenuLabel1[LABELNAMEMAX];				// User�֐�1�̃��j���[��
	char MenuLabel2[LABELNAMEMAX];				// User�֐�2�̃��j���[��
	char MenuLabel3[LABELNAMEMAX];				// User�֐�3�̃��j���[��
	char MenuLabel4[LABELNAMEMAX];				// User�֐�4�̃��j���[��
	char MenuLabel5[LABELNAMEMAX];				// User�֐�5�̃��j���[��

///////////////////////////////////////////////////////////////////////////////////////////////////////


///////////�V����User�֐�����o�^����ꍇ�͈ȉ���private:�ŋL�q////////////////////////////////////////
private:
	int CalcIntersecPts(BODYList *,OBJECTList *,int);	// �T���v��
	int ContourLine(NURBSS *);			// ����������

};

#endif
