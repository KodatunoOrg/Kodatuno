#include "Command.h"

// �R�}���h�ǉ�
// ���� *CmdMap:�R�}���h�����i�[����\���̂ւ̃|�C���^,  *name:�R�}���h��,  *op:�R�}���h�I�v�V������
//      key:�R�}���h���\����l(0�`99:Kodatuno���g�p�A100�`199:User�Ɋ��蓖��),  (*Cmd)(int,char *[]):���s�����R�}���h�̊֐��ւ̃|�C���^
void SetCmdList(CommandMap *CmdMap,char *name,char *op,int key,void (*Cmd)(int,char *[]))
{
	if(key >= COMMANDNUMMAX){
        GuiIF.SetMessage("ERROR:your selected key number is out of the region!");
        GuiIF.SetMessage(" select range from (int)100 to (int)199.");
		return ;
	}

	strcpy(CmdMap[key].Name,name);	// �R�}���h���擾
	strcpy(CmdMap[key].Op,op);		// �R�}���h�I�v�V�����擾
	CmdMap[key].CmdNo = key;		// �R�}���h��\�l�擾
	CmdMap[key].Command = Cmd;		// �R�}���h���s�֐��擾
}

// �R�}���h���
// Qt�łł̓R�}���h���������o�����������n��
int AnalCommandLine(char *str,char *argv[])
{
	int argc=0;
	int len;
	char *p;

	len = strlen(str);						// �R�}���h���C���̕��������J�E���g
	len -= 2;
	str[len] = '\0';						// �������"\n>"�����Ă���̂ŁA����

	// ���͕�����Ȃ�
	if(!strcmp(str,"")){
		return KOD_ERR;
	}

	// �R�}���h���C�����o
	else{
		// str�𕪉����Aargc,argv�𓾂�
		p = str;
			while(((p = strchr(str,' ')) != NULL) || ((p = strchr(str,'>')) != NULL)){
				str[p-str] = '\0';
				argv[argc] = str;
				str = p;
				str++;
				argc++;
			}
			argv[argc] = str;
	}

	return argc;
}

// �����Ŏw�肵���R�}���h�̓o�^No�𒲂ׂ�
int SearchRegdCmdNum(CommandMap *CmdMap,char *cmd)
{
	for(int i=0;i<COMMANDNUMMAX;i++){
		if(!strcmp(cmd,CmdMap[i].Name))
			return CmdMap[i].CmdNo;
	}

	return KOD_ERR;
}


