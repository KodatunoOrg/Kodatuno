// BODY���X�g�̃v���V�[�W���[
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

// BODY���X�g�̃A�C�e�����Z���N�g���ꂽ
void BodyListSel_Proc(WSCbase* object)
{
	Kodatuno.BodyListSelect();		// �Z���N�g���ꂽ�A�C�e���̔ԍ��𓾂�
}
static WSCfunctionRegister  op("BodyListSel_Proc",(void*)BodyListSel_Proc);
