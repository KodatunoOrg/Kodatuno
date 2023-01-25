#include "QtDescribeView.h"

// �R���X�g���N�^
QtDescribeView::QtDescribeView(QWidget *parent) :
    QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);             // �}�E�X���`��̈�ɓ��邾���ŁC�}�E�X�ʒu���擾�ł���悤�ɂ���
    setMinimumSize(640,480);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);    
}

// OpenGL������
void QtDescribeView::initializeGL()
{
//    qglClearColor( Qt::black );

//    glShadeModel(GL_FLAT);
//    glShadeModel(GL_SMOOTH);

//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);

//    glEnable( GL_DEPTH_TEST );
//    glEnable(GL_LINE_SMOOTH);
//    glEnable(GL_POLYGON_SMOOTH);
//    glEnable( GL_CULL_FACE );
//    glCullFace(GL_BACK);
}

// ���T�C�Y�E�B�W�F�b�g
void QtDescribeView::resizeGL(int w, int h)
{
    Kodatuno.GetResizeWindow(w,h);
}

// OpenGL�`��
void QtDescribeView::paintGL()
{
    Kodatuno.ReDraw(MouseX,MouseY);
    Kodatuno.ReDrawUserFunc();
}

// �}�E�X�������̃C�x���g
void QtDescribeView::mousePressEvent(QMouseEvent *event)
{
    long trig = KDT_MOUSE_PRESS;
    long btn = getMouseBtn(event);

    Kodatuno.MouseEvent(trig, btn, event->x(), event->y());
}

// �}�E�X�����[�X���C�x���g
void QtDescribeView::mouseReleaseEvent(QMouseEvent *event)
{
    long trig = KDT_MOUSE_RELEASE;
    long btn = getMouseBtn(event);

    Kodatuno.MouseEvent(trig, btn, event->x(), event->y());
}

// �}�E�X�ړ����C�x���g
void QtDescribeView::mouseMoveEvent(QMouseEvent *event)
{
    // ���݂̃}�E�X�ʒu�𒀎��o���Ă���
    MouseX = event->x();
    MouseY = event->y();

    long btn = getMouseBtn(event);
    long key = getModifierKey(event);

    Kodatuno.MouseMotionEvent(btn, key, event->x(), event->y());
}

// �}�E�X�z�C�[���C�x���g
void QtDescribeView::wheelEvent(QWheelEvent *event)
{
    Kodatuno.MouseWheelEvent(event->delta());
}

// �L�[�{�[�h�C�x���g
void QtDescribeView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
        case Qt::Key_Up:
            Kodatuno.ChangeViewUp();
            break;
        case Qt::Key_Down:
            Kodatuno.ChangeViewDown();
            break;
        case Qt::Key_Right:
            Kodatuno.ChangeViewRight();
            break;
        case Qt::Key_Left:
            Kodatuno.ChangeViewLeft();
            break;
        default:
            break;
    }
}

//������Ă���}�E�X�{�^���̎擾
long QtDescribeView::getMouseBtn(QMouseEvent *event )
{
    switch(event->buttons()){
        case Qt::LeftButton:
            return KDT_MOUSE_LEFT;
        case Qt::RightButton:
            return KDT_MOUSE_RIGHT;
        case Qt::MiddleButton:
            return KDT_MOUSE_MIDDLE;
        default:
            return KDT_MOUSE_NONE;
    }
}

//������Ă���C���L�[�̎擾
long QtDescribeView::getModifierKey(QMouseEvent *event )
{
    if( event->modifiers() & Qt::ControlModifier ){
        return KDT_MOD_CTRL;
    }
    else if( event->modifiers() & Qt::ShiftModifier){
        return KDT_MOD_SHIFT;
    }
    else if( event->modifiers() & Qt::AltModifier){
        return KDT_MOD_ALT;
    }
    else{
        return KDT_MOD_NONE;
    }
    
}

// �_��`��
void QtDescribeView::drawPoint(Coord p,double scale,double width,double color[3])
{
    glDisable(GL_LIGHTING);
    glPointSize(width);
    glColor3f(color[0],color[1],color[2]);
    glBegin(GL_POINTS);
    glVertex3d(p.x*scale,p.y*scale,p.z*scale);
    glEnd();
    glEnable(GL_LIGHTING);
}

// �_�Q��`��
// ����   *p:�_�Q, n:�_��, scale:�X�P�[��, width:�_�̃T�C�Y, color,�_�̐F
void QtDescribeView::drawPoints(Coord *p,int n,double scale,double width,double color[3])
{
    glDisable(GL_LIGHTING);
    glPointSize(width);
    glColor3f(color[0],color[1],color[2]);
    glBegin(GL_POINTS);
    for(int i=0;i<n;i++){
        glVertex3d(p[i].x*scale,p[i].y*scale,p[i].z*scale);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// �x�N�g����`��
void QtDescribeView::drawVector(Coord s,Coord e,double vec_len,double width,double color[3])
{
    glDisable(GL_LIGHTING);
    glLineWidth(width);
    glColor3f(color[0],color[1],color[2]);
    e = MulCoord(e,vec_len);
    glBegin(GL_LINES);
    glVertex3d(s.x,s.y,s.z);
    glVertex3d(s.x+e.x,s.y+e.y,s.z+e.z);
    glEnd();
    glEnable(GL_LIGHTING);
}

// 2�_�Ԃɐ�����`��
void QtDescribeView::drawLine(Coord s,Coord e,double width,double color[3])
{
    glDisable(GL_LIGHTING);
    glLineWidth(width);
    glColor3f(color[0],color[1],color[2]);
    glBegin(GL_LINES);
    glVertex3d(s.x,s.y,s.z);
    glVertex3d(e.x,e.y,e.z);
    glEnd();
    glEnable(GL_LIGHTING);

}

