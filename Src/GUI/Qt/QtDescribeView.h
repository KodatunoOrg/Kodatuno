#ifndef QTDESCRIBEVIEW_H
#define QTDESCRIBEVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include "Kodatuno.h"

// OpenGL�`��̈�̃C�x���g�����N���X
class QtDescribeView : public QGLWidget
{
public:
    explicit QtDescribeView( QWidget *parent = 0);
    void drawPoint(Coord,double,double,double []);			// �_��`��
    void drawPoints(Coord *,int,double,double,double []);	// �_�Q��`��
    void drawVector(Coord,Coord,double,double,double []);	// �x�N�g����`��
    void drawLine(Coord,Coord,double,double []);			// 2�_�Ԃɐ�����`��
    
protected :
    void initializeGL();                            // OpenGL������
    void resizeGL( int w, int h );                  // ���T�C�Y
    void paintGL();                                 // OpenGL�֐��͂����Ŏg��
    void mousePressEvent( QMouseEvent *event );     // �}�E�X�������̃C�x���g
    void mouseReleaseEvent( QMouseEvent *event );   // �}�E�X�����[�X���̃C�x���g
    void mouseMoveEvent( QMouseEvent *event );      // �}�E�X�ړ����̃C�x���g
    void wheelEvent( QWheelEvent *event );          // �}�E�X�z�C�[���̃C�x���g
    void keyPressEvent( QKeyEvent *event );         // �L�[�{�[�h�̃C�x���g
    
private :
	long getMouseBtn(QMouseEvent *event );		//������Ă���}�E�X�{�^���̎擾
	long getModifierKey(QMouseEvent *event );	//������Ă���C���L�[�̎擾

private:
    int MouseX,MouseY;      // ���݂̃}�E�X�ʒu
	
};

#endif // QTDESCRIBEVIEW_H
