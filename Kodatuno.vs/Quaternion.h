#ifndef _QUATERNION_H_
#define _QUATERNION_H_

//#include "StdAfx.h"

// Constants: General Defines
// TMATELEMNUM - 回転行列配列の要素数(16)
#define TMATELEMNUM 16

// Structure: Quat
//
// Variables:
// t -		実部
// x,y,z -	虚部
typedef struct{
	double t;
	double x,y,z;
}Quat;

// Class: QUATERNION
// クォータニオン演算用モジュール
class QUATERNION
{
public:
	// Function: QInit
	// クォータニオンの初期化
	Quat QInit(double,double,double,double);	
	
	// Function: QGenRot
	// 回転クォータニオン生成
	Quat QGenRot(double,double,double,double);	

	// Function: QRot
	// クォータニオンによるベクトルの回転
	Quat QRot(Quat,Quat,Quat);					

	// Function: QConjugation
	// 共役クォータニオンを生成
	Quat QConjugation(Quat);					

	// Function: QMult
	// クォータニオンの掛け算
	Quat QMult(Quat,Quat);						

	// Function: QtoR
	// クォータニオンqを回転行列r(OpenGL対応のため、1次元16要素配列)へ変換 (必要であれば、2次元配列Verをオーバーロードしてください)
	void QtoR(double [],Quat);					

	// Function: QtoR
	// オーバーロード(引数：r,t,x,y,z)
	void QtoR(double [],double,double,double,double);	

	// Function: QCopy
	// クォータニオンのコピー p <- q
	Quat QCopy(Quat);							

	// Function: QtoC
	// クォータニオンの虚数部をCoordとして返す
	Coord QtoC(Quat);							

	// Function: CtoQ
	// クォータニオン実部を0、虚部をCoordとしてクォータニオンを生成
	Quat CtoQ(Coord);							

	// Function: QSlerp
	// クォータニオンによる球面線形補間
	Quat QSlerp(Quat,Quat,double);				
};

#endif
