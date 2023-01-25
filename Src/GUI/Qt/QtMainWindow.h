#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

// Kodatuno���C���E�B���h�E

#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include "Kodatuno.h"
#include "QtUserStatDialog.h"

namespace Ui {
    class QtMainWindow;
}

// Kodatuno���C���E�B���h�E�N���X
class QtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();

private:
    Ui::QtMainWindow *ui;
    QSettings *setting;

    void initGUI();			// Initializing GUI

// �X���b�g�֐�
private slots:
    // �{�^������C�x���g
    // BODY�̕\���֘A
    void on_actionViewOnX_triggered();          // X�����r���[
    void on_actionViewOnY_triggered();          // Y�����r���[
    void on_actionViewOnZ_triggered();          // Z�����r���[
    void on_actionViewOnXYZ_triggered();        // ���Ճr���[
    void on_actionViewFit_triggered();          // �{��������
    void on_actionViewSolid_triggered();        // �\���b�h�\��
    void on_actionViewNoEdge_triggered();       // �G�b�W��\��
    void on_actionViewWireFlame_triggered();    // ���C���[�t���[��
    void on_actionViewVoid_triggered();         // �\�����Ȃ�
    void on_actionSelectAll_triggered();        // ���ׂđI��
    void on_actionSelect_Cancel_triggered();    // �I������
    void on_actionViewErase_triggered();        // Userfunc()�ɂ���ĕ`�悳�ꂽ���̂�����
    void on_actionViewBlend_triggered();        // BODY�𔼓���

    // BODY�̑���
    void on_bodyList_itemClicked(QListWidgetItem *item);    // BODY���X�g��BODY��I��
    void on_actionDeleteBody_triggered();       // BODY�̏���
    void on_actionOpenBody_triggered();         // BODY�t�@�C���̃I�[�v��

    // ���f���쐬�֘A
    void on_actionNURBSSarface_triggered();     // NURBS�Ȗʐ���
    void on_actionNURBSCurve_triggered();       // NURBS�Ȑ�����
    void on_actionSweepSurface_triggered();     // �X�C�[�v�T�[�t�F�X����
    void on_actionRotationSurface_triggered();  // ��]�T�[�t�F�X����

    // DOCK�̕\��ON
    void on_actionShowBodyList_triggered();     // BODY List��DOCK�\��
    void on_actionShowConsole_triggered();      // Console��DOCK�\��

    // UserFunc�֘A
    void on_actionUserStatus_triggered();       // ���[�U�[�X�e�[�^�X�_�C�A���O�̕\��
    void on_actionUserFunc1_triggered();        // ���[�U�[�֐�1�̎��s
    void on_actionUserFunc2_triggered();        // ���[�U�[�֐�2�̎��s
    void on_actionUserFunc3_triggered();        // ���[�U�[�֐�3�̎��s
    void on_actionUserFunc4_triggered();        // ���[�U�[�֐�4�̎��s
    void on_actionUserFunc5_triggered();        // ���[�U�[�֐�5�̎��s
    void on_actionUserFunc6_triggered();        // ���[�U�[�֐�6�̎��s
    void on_actionUserFunc7_triggered();        // ���[�U�[�֐�7�̎��s
    void on_actionUserFunc8_triggered();        // ���[�U�[�֐�8�̎��s

    void on_actionAboutKodatuno_triggered();    // AboutBOX�̕\��

protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // QTMAINWINDOW_H
