#include "QtConsole.h"
#include <QByteArray>

QtConsole::QtConsole(QWidget *parent):
    QTextEdit(parent)
{
    this->setReadOnly(false);
}

void QtConsole::keyPressEvent(QKeyEvent *e)
{
    int curPos;         // �J�[�\���̌��݈ʒu
    int lastLen;
    QString text, textTemp, cmd;
    std::string cmdTemp;
    QTextCursor cursor;

    if(!e) return;

    switch (e->key()){
    // ���^�[���L�[���͂��ꂽ��C�R�}���h���o�E��́E���s
    case Qt::Key_Return:
    case Qt::Key_Enter:
        text = this->toPlainText();    // console����e�L�X�g���擾
        cmd = text.split(">").last();   // ">"�ŕ��������Ō�̃e�L�X�g���擾 = �R�}���h�̒��o
        this->append(">");               // console���ɖ��t��

        if(cmd == "") break;            // �󔒂Ȃ�
        else{
            // �R�}���h�����s���C�����ɓo�^
            QString temp = cmd + "\n>";
            Kodatuno.ExecCommand(temp.toUtf8().data());
            Kodatuno.SetConsoleHistory(cmd.toStdString());
        }

        break;

    // ��L�[���͂ɑ΂��āC�R�}���h����\��
    case Qt::Key_Up:
        if(Kodatuno.GetUpperConsoleHistory(&cmdTemp) == KOD_TRUE){
            text = this->toPlainText();                 // console����e�L�X�g���擾
            lastLen = text.split(">").last().count();   // ">"�ŕ��������Ō�̃e�L�X�g�̕��������擾
            text.resize(text.count() - lastLen);        // ���͂���Ă����������폜
            text += QString(cmdTemp.c_str());
            this->setText(text);                        // console�ɕ\��

            // �J�[�\���𖖔��Ɉړ�
            cursor = this->textCursor();
            cursor.movePosition(QTextCursor::End);
            this->setTextCursor(cursor);
        }
        break;

    // ���L�[���͂ɑ΂��āC�R�}���h����\��
    case Qt::Key_Down:
        if(Kodatuno.GetLowerConsoleHistory(&cmdTemp) == KOD_TRUE){
            text = this->toPlainText();                 // console����e�L�X�g���擾
            lastLen = text.split(">").last().count();   // ">"�ŕ��������Ō�̃e�L�X�g�̕��������擾
            text.resize(text.count() - lastLen);        // ���͂���Ă����������폜
            text += QString(cmdTemp.c_str());
            this->setText(text);                        // console�ɕ\��

            // �J�[�\���𖖔��Ɉړ�
            cursor = this->textCursor();
            cursor.movePosition(QTextCursor::End);
            this->setTextCursor(cursor);
        }
        break;

    // ">"�Ŏ~�߂�
    case Qt::Key_Left:
        text = this->toPlainText();                 // console����e�L�X�g���擾
        lastLen = text.split(">").last().count();   // ">"�ŕ��������Ō�̃e�L�X�g�̕��������擾

        // �J�[�\���̈ʒu�擾
        cursor = this->textCursor();
        curPos = cursor.position();

        // ">"�Ŏ~�߂�
        if( curPos > (text.count() - lastLen) ){
            QTextEdit::keyPressEvent(e);    // �ʏ�̏���
        }
        break;

    // ">"�Ŏ~�߂�
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        if(CheckCurPos() == false || this->textCursor().hasSelection()==true)
            break;
        text = this->toPlainText();         // console����e�L�X�g���擾
        textTemp = text.split(">").last();  // ">"�ŕ��������Ō�̃e�L�X�g�̕������擾
        if(textTemp != ""){
            QTextEdit::keyPressEvent(e);    // �ʏ�̏���
        }
        break;

    // ����ȊO�̃L�[�͒ʏ�̏���
    default:
        // ���݂̃J�[�\���ʒu���Ō��">"���O�ɂ���ꍇ�́C�R���\�[���̍Ō�ɃJ�[�\���������Ă���
        if(CheckCurPos() == false || this->textCursor().hasSelection()==true){
            cursor = this->textCursor();
            cursor.movePosition(QTextCursor::End);
            this->setTextCursor(cursor);
        }
        QTextEdit::keyPressEvent(e);
        break;
    }
}

// �J�[�\���̌��݈ʒu���Ō��">"�ȍ~�ɂ���̂��𒲂ׂ�
// �Ԓl�@">"�ȍ~�Fture�@�@�@">"�ȑO�Ffalse
bool QtConsole::CheckCurPos()
{
    int curpos = this->textCursor().position();
    QString text = this->toPlainText();
    if(curpos - (text.count()-text.split(">").last().count()) < 0)
        return false;
    else
        return true;
}
