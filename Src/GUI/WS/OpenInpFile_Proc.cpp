// �C���v�b�g�t�@�C���Ǎ��{�^���̃v���V�[�W��
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void OpenInpFile_Proc(WSCbase* object)
{
	std::string full_name = Kodatuno.OpenInpFile();
	InpFileName->setProperty(WSNlabelString,full_name.c_str());	// �t�@�C���̃t���p�X�����[�U�[�ݒ蕶����ɃZ�b�g

}
static WSCfunctionRegister  op("OpenInpFile_Proc",(void*)OpenInpFile_Proc);
