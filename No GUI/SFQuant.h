#ifndef _SFQUANT_H_
#define _SFQUANT_H_

#include "BODY.h"

// 曲面の第一，第二基本量を格納するクラスを定義
class SFQuant
{
public:
	SFQuant();									// コンストラクタ
	SFQuant(NURBSS *S,double u, double v);		// S(u,v)での基本量を得る(コンストラクタ)
	int SetSFQ(NURBSS *S,double u, double v);	// S(u,v)での基本量を得る

public:
	double U,V;			// 曲面パラメータ
	Coord  n;			// 法線ベクトル
	double E,F,G;		// 第一基本量
	double L,M,N;		// 第二基本量
};


#endif