#include "QtDescribeView.h"

// Constructor: QtDescribeView
// QtDescribeViewクラスのコンストラクタ
//
// Parameters: 
// *parent - 呼び出し元の親ウィジェットを指定
QtDescribeView::QtDescribeView(QWidget *parent) :
    QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);             // マウスが描画領域に入るだけで，マウス位置を取得できるようにする
    setMinimumSize(640,480);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);    
}

// Function: initializeGL
// OpenGLを初期化する
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

// Function: resizeGL
// リサイズウィジェット
// 
// Parameters:
// w - ウィンドウの幅のサイズ
// h - ウィンドウの高さのサイズ
void QtDescribeView::resizeGL(int w, int h)
{
    Kodatuno.GetResizeWindow(w,h);
}

// Function: paintGL
// OpenGLを描画する
void QtDescribeView::paintGL()
{
    Kodatuno.ReDraw(MouseX,MouseY);
    Kodatuno.ReDrawUserFunc();
}

// Function: mousePressEvent
// マウス押下時のイベント
// 
// Parameters:
// *event - マウスからの情報
void QtDescribeView::mousePressEvent(QMouseEvent *event)
{
    long trig = KDT_MOUSE_PRESS;
    long btn = getMouseBtn(event);

    Kodatuno.MouseEvent(trig, btn, event->x(), event->y());
}

// Function: mouseReleaseEvent
// マウスリリース時イベント
//
// Parameters:
// *event - マウスからの情報
void QtDescribeView::mouseReleaseEvent(QMouseEvent *event)
{
    long trig = KDT_MOUSE_RELEASE;
    long btn = getMouseBtn(event);

    Kodatuno.MouseEvent(trig, btn, event->x(), event->y());
}

// Function: mouseMoveEvent
// マウス移動時イベント
// 
// Parameters:
// *event - マウスからの情報
void QtDescribeView::mouseMoveEvent(QMouseEvent *event)
{
    // 現在のマウス位置を逐次覚えておく
    MouseX = event->x();
    MouseY = event->y();

    long btn = getMouseBtn(event);
    long key = getModifierKey(event);

    Kodatuno.MouseMotionEvent(btn, key, event->x(), event->y());
}

// Function: wheelEvent
// マウスホイールイベント
// 
// Parameters:
// *event - ホイールからの情報
void QtDescribeView::wheelEvent(QWheelEvent *event)
{
    Kodatuno.MouseWheelEvent(event->delta());
}

// Function: keyPressEvent
// キーボードイベント
//
// Parameters:
// *event - キーボードからの情報
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

// Function: getMouseBtn
// 押されているマウスボタンの取得
//
// Parameters:
// *event - マウスからの情報
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

// Function: getModifierkey
// 押されている修飾キーを取得する
//
// Parameters:
// *event - マウスからの情報
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

// Function: drawPoint
// 点を描画する
//
// Parameters:
// p - 点の座標値
// scale - pをscale倍する
// width - 点のサイズ
// color[3] - 点の色をRGBで指定(0 <= r,g,b <= 1)
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

// Function: drawPoints
// 点群を描画する
//
// Parameters:
// p - 点の座標値
// scale - pをscale倍する
// width - 点のサイズ
// color[3] - 点の色をRGBで指定(0 <= r,g,b <= 1)
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

// Function: drawVector
// ベクトルを描画する
// 
// Parameters:
// s - 3次元ベクトルの始点座標
// e - 3次元ベクトルの終点座標
// vec_len - 表示するベクトルの長さの倍率
// width - 描画する線分の太さ
// color[3] - 点の色をRGBで指定(0 <= r,g,b <= 1)
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

// Function: drawLine
// 2点間に線分を描画する
//
// Parameters:
// s - 3次元ベクトルの始点座標
// e - 3次元ベクトルの終点座標
// vec_len - 表示するベクトルの長さの倍率
// width - 描画する線分の太さ
// color[3] - 点の色をRGBで指定(0 <= r,g,b <= 1)
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

