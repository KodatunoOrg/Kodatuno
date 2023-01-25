#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "StdAfx.h"

#define TMATELEMNUM 16		// 回転行列配列の要素数

typedef struct{
	double t;		// 実部
	double x,y,z;	// 虚部
}Quat;

// クォータニオン演算用モジュール
class QUATERNION
{
public:
	Quat QInit(double,double,double,double);	// クォータニオンの初期化
	Quat QGenRot(double,double,double,double);	// 回転クォータニオン生成
	Quat QRot(Quat,Quat,Quat);					// クォータニオンによるベクトルの回転
	Quat QConjugation(Quat);					// 共役クォータニオンを生成
	Quat QMult(Quat,Quat);						// クォータニオンの掛け算
	void QtoR(double [],Quat);					// クォータニオンqを回転行列r(OpenGL対応のため、1次元16要素配列)へ変換 (必要であれば、2次元配列Verをオーバーロードしてください)
	void QtoR(double [],double,double,double,double);	// オーバーロード(引数：r,t,x,y,z)
	Quat QCopy(Quat);							// クォータニオンのコピー p <- q
	Coord QtoC(Quat);							// クォータニオンの虚数部をCoordとして返す
};

#endif
