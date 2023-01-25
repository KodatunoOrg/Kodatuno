#ifndef _SFQUANT_H_
#define _SFQUANT_H_

#include "BODY.h"

// Constants: General Defines
// Non

// Class: SFQuant
// 曲面の第一，第二基本量を格納するクラスを定義
class SFQuant
{
public:
	// Constructor: SFQuant
	// 変数初期化
	SFQuant();

	// Constructor: SFQuant
	// S(u,v)での基本量を得る
	SFQuant(NURBSS *S,double u, double v);

	// Function: SetSFQ
	// S(u,v)での基本量を得る
	int SetSFQ(NURBSS *S,double u, double v);

    // Function: SetSFQ1
    // S(u,v)での第一基本量を得る
    int SetSFQ1(NURBSS *S,double u, double v);

public:

	// Variables: U,V
	// 曲面パラメータ
	double U,V;			

	// Variables: n
	// 法線ベクトル
	Coord  n;		

	// Variables:  E,F,G
	// 第一基本量
	double E,F,G;		

	// Variables: L,M,N
	// 第二基本量
	double L,M,N;		
};


#endif
