#ifndef QTDESCRIBEVIEW_H
#define QTDESCRIBEVIEW_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include "Kodatuno.h"

// Constants: General Difines
// Non

// Class: QtDescribeView
// OpenGL描画領域のイベント処理クラス
class QtDescribeView : public QOpenGLWidget
{
public:
// Constructor: QtDescribeView
// QtDescribeViewクラスのコンストラクタ．
// 暗黙呼び出しを禁止
    explicit QtDescribeView( QWidget *parent = 0);
	
// Function: drawPoint
// 点を描画する
    void drawPoint(Coord,double,double,double []);			
	
// Function: drawPoints
// 点群を描画する
    void drawPoints(Coord *,int,double,double,double []);	
	
// Function: drawVector
// ベクトルを描画する
    void drawVector(Coord,Coord,double,double,double []);	
	
// Function: drawLine
// 2点間に線分を描画する
    void drawLine(Coord,Coord,double,double []);			
    
protected :
// Function: initializeGL
// OpenGLを初期化する
    void initializeGL();                            
	
// Function: realizeGL
// リザイズウィジェット
    void resizeGL( int w, int h );                  
	
// Function: paintGL
// OpenGL関数はここで使う
    void paintGL();                                 
	
// Function: mousePressEvent
// マウス押下時のイベント
    void mousePressEvent( QMouseEvent *event );     
	
// Function: mouseReleaseEvent
// マウスリリース時のイベント
    void mouseReleaseEvent( QMouseEvent *event );   
	
// Function: mouseMoveEvent
// マウス移動時のイベント
    void mouseMoveEvent( QMouseEvent *event );      

// Function: wheelEvent
// マウスホイールのイベント
    void wheelEvent( QWheelEvent *event );          
	
// Function: keyPressEvent
// キーボードのイベント
    void keyPressEvent( QKeyEvent *event );         
    
private :
// Function: getMouseBtn
// (private)押されているマウスボタンの取得
	long getMouseBtn(QMouseEvent *event );		

// Function: getModidierKey
// (private)押されている修飾キーの取得
	long getModifierKey(QMouseEvent *event );	

private:
// Variable: MouseX,MouseY
// 現在のマウス位置
    int MouseX,MouseY;      // 現在のマウス位置
	
};

#endif // QTDESCRIBEVIEW_H
