// 3D�f�[�^�t�@�C���̓ǂݍ��݃{�^���̃v���V�[�W��

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void OpenFile_Proc(WSCbase* object)
{
	Kodatuno.OpenFile();			// �t�@�C���I�[�v��
	Kodatuno.SetModelScale();		// ���f���X�P�[����ݒ�
	Describe_Form->redraw();		// �`��̈�ɍĕ`����w��
}
static WSCfunctionRegister  op("OpenFile_Proc",(void*)OpenFile_Proc);
