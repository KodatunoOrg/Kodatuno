// �C���v�b�g�t�@�C���Ǎ��{�^���̃v���V�[�W��
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void OpenInpFile_Proc(WSCbase* object)
{
	Kodatuno.OpenInpFile();
}
static WSCfunctionRegister  op("OpenInpFile_Proc",(void*)OpenInpFile_Proc);
