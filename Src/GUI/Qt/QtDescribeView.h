#ifndef QTDESCRIBEVIEW_H
#define QTDESCRIBEVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include "Kodatuno.h"

// OpenGL描画領域のイベント処理クラス
class QtDescribeView : public QGLWidget
{
public:
    explicit QtDescribeView( QWidget *parent = 0);
    void drawPoint(Coord,double,double,double []);			// 点を描画
    void drawPoints(Coord *,int,double,double,double []);	// 点群を描画
    void drawVector(Coord,Coord,double,double,double []);	// ベクトルを描画
    void drawLine(Coord,Coord,double,double []);			// 2点間に線分を描画
    
protected :
    void initializeGL();                            // OpenGL初期化
    void resizeGL( int w, int h );                  // リサイズ
    void paintGL();                                 // OpenGL関数はここで使う
    void mousePressEvent( QMouseEvent *event );     // マウス押下時のイベント
    void mouseReleaseEvent( QMouseEvent *event );   // マウスリリース時のイベント
    void mouseMoveEvent( QMouseEvent *event );      // マウス移動時のイベント
    void wheelEvent( QWheelEvent *event );          // マウスホイールのイベント
    void keyPressEvent( QKeyEvent *event );         // キーボードのイベント
    
private :
	long getMouseBtn(QMouseEvent *event );		//押されているマウスボタンの取得
	long getModifierKey(QMouseEvent *event );	//押されている修飾キーの取得

private:
    int MouseX,MouseY;      // 現在のマウス位置
	
};

#endif // QTDESCRIBEVIEW_H
