// �e�L�X�g�t�H�[���̃v���V�[�W��

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void Console_Expose(WSCbase *);
void Console_KeyHook(WSCbase *);

void Console_Proc(WSCbase* object)
{
	WSCprocedure *pwproc = object->getExecutedProcedure();	// ���s���ꂽ�v���V�[�W�����擾

	switch(pwproc->getTrigger()){		// �擾�����v���V�[�W����
		case WSEV_KEY_HOOK:				// �L�[�t�b�N�̏ꍇ
			Console_KeyHook(object);
			break;
		case WSEV_EXPOSE:				// �ĕ`��w�߂̏ꍇ
			Console_Expose(object);
			break;
		default:
			break;
	}
}

// �ĕ`�掞�̏���
void Console_Expose(WSCbase *object)
{
	WSCstring mes = Console_Form->getString();
	int len = mes.getChars();					// �R���\�[�����̕��������擾
	Console_Form->setProperty(WSNcursorPos,len);// �J�[�\�����Ō�ɂ����Ă���
	Kodatuno.SetConsoleCharNum(Console_Form->getProperty(WSNuserValue));			// ���������X�V
}

// ���͂��ꂽ�L�[���t�b�N���A�L�[���͂𐧌�����(�R���\�[�����ȃe�L�X�g�t�H�[���ɂ���)
void Console_KeyHook(WSCbase *object)
{
	int oldlen,newlen;		// �����񒷂��i�[
	int pos;
	WSCstring mes,com;

	Console_Form->setProperty(WSNdisplayOnly,0);// ���͉\��ԂɈڍs
	WSDkeyboard *key=WSGIappKeyboard();			// 
	long keycode = key->getKey();				// ���͂��ꂽ�L�[���擾

	std::string com_string;
	std::string s; 
	switch(keycode){
		// ���^�[���L�[�������ꂽ��A���͂��ꂽ�R�}���h�𒊏o���A�R�}���h��͂���
		case WSK_Return:
			Console_Form->setProperty(WSNdisplayOnly,1);	// ���͕s�\��ԂɈڍs(�������Ȃ��ƂȂ����J�[�\�����Ō�Ɏ����Ă����Ȃ�)
			oldlen = Kodatuno.GetConsoleCharNum();			// �Ō�ɓo�^�����R���\�[�������񒷂��擾
			Console_Form->addString("\n>");					// �R���\�[�����ɖ��t��
			mes = Console_Form->getString();				// ����܂ł̕������S�Ď擾
			newlen = mes.getChars();						// ���������J�E���g
			if(newlen == oldlen)	break;					// �������͂���Ă��Ȃ�������A�������Ȃ�
			else{											// �R�}���h�����͂���Ă�����
				Console_Form->setProperty(WSNuserValue,newlen);	// ���݂܂ł̕�������ێ�
				mes.deleteChars(0,oldlen);					// �R�}���h���̂ݒ��o
				Kodatuno.ExecCommand((char *)mes);			// �R�}���h���s
				mes.deleteChars(mes.getChars()-2,2);
				s = std::string(mes);
				Kodatuno.SetConsoleHistory(s);				// �R�}���h�𗚗��ɓo�^
				//Kodatuno.SetConsoleCharNum(newlen);		// �R���\�[�������񒷂��X�V
			}
			break;
		// �㉺�L�[�ɑ΂��Ă̓R�}���h������\������
		case WSK_KP_Up:
			if(Kodatuno.GetUpperConsoleHistory(&com_string) == KOD_TRUE){	// �V���ȗ����R�}���h���擾
				com = com_string.c_str();
				newlen = Console_Form->getString().getChars();		// �R���\�[���̕��������擾
				oldlen = Kodatuno.GetConsoleCharNum();				// �Ō�ɓo�^�����R���\�[�������񒷂��擾
				Console_Form->setSelect(oldlen,newlen-oldlen);		// �����R�}���h���\���ς݂̏ꍇ�A
				Console_Form->deleteSelectedString();				// ����
				GuiIF.GUI_Interface::SetCommand((char *)com);							// �V���������R�}���h���R���\�[���ɏo��
			}
			key->setKey(0);
			break;
		case WSK_KP_Down:
			if(Kodatuno.GetLowerConsoleHistory(&com_string) == KOD_TRUE){
				com = com_string.c_str();
				newlen = Console_Form->getString().getChars();
				oldlen = Kodatuno.GetConsoleCharNum();				// �Ō�ɓo�^�����R���\�[�������񒷂��擾
				Console_Form->setSelect(oldlen,newlen-oldlen);		// �����R�}���h���\���ς݂̏ꍇ�A
				Console_Form->deleteSelectedString();				// ����
				GuiIF.SetCommand((char *)com);							// �V���������R�}���h���R���\�[���ɏo��
			}
			key->setKey(0);
			break;

		//�@���E�L�[�ɑ΂��Ă͉������Ȃ�
		case WSK_KP_Left:
		case WSK_KP_Right:
			break;
		// �J�[�\���ʒu�����݂̕�������菬�����ʒu�ɂ���ꍇ��
		// BackSpace��Delete�L�[�͖����ɂ��āA�����������Ȃ��悤�ɂ���
		case WSK_BackSpace:
			pos = Console_Form->getSelectedPos();		// �J�[�\���ʒu���擾
			oldlen = Kodatuno.GetConsoleCharNum();
			if(pos <= oldlen)
				key->setKey(0);
			break;
		case WSK_Delete:
			pos = Console_Form->getSelectedPos();		// �J�[�\���ʒu���擾
			oldlen = Kodatuno.GetConsoleCharNum();
			if(pos < oldlen)
				key->setKey(0);
			break;
		default:
			pos = Console_Form->getSelectedPos();		// �J�[�\���ʒu���擾
			oldlen = Kodatuno.GetConsoleCharNum();
			if(pos < oldlen)									// �J�[�\���ʒu�����݂̕�������菬�����ʒu�ɂ���ꍇ��
				Console_Form->setProperty(WSNcursorPos,Kodatuno.GetConsoleCharNum());	// �J�[�\�����Ō�ɂ����Ă���
			break;
	}

}

static WSCfunctionRegister  op("Console_Proc",(void*)Console_Proc);


