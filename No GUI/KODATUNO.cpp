/*************************************************
* KODatUNO Ver2.0 ��  Takasugi 2009/6            
* KODatUNO Juza       Takasugi Kumasaka 2009/9   
* KODatUNO Mamiya	  Takasugi Kumasaka 2009/11
* Takasugi Last Cording 2010/03/04
* Kumasaka Last Cording 2010/??/??
**************************************************/

#include "KODATUNO.h"

int main(int argc,char *argv[])
{
	KODatUNO Kakuma;					// KODatUNO�N���X�̃I�u�W�F�N�g�𐶐�

	Kakuma.Kodatuno_Main(argc,argv);	// KODatUNO�A�v���P�[�V�����X�^�[�g
}

// KODatUNO���C��
void KODatUNO::Kodatuno_Main(int argc,char *argv[])
{
	// OpenGL�ɂ��`��N���X
	static DESCRIBE Describe;			// IGES�`��p�I�u�W�F�N�g�𐶐�
	Describe.BODY_Describe_Main(argc,argv);
}
