// UserSampleヘッダ

#include "StdAfx.h"			// 汎用
#include "NURBS_Func.h"		// NURBS幾何計算関連
#include "Describe_BODY.h"	// BODY描画関連
#include "BODY.h" 			// BODYクラス
#include "IGES_Parser.h"  	// IGES読み込み関連

// Function: SmpRegistBody
// Sample1: IGESファイルを読み込み，Bodyとして登録する
int SmpRegistBody(BODYList *,OBJECTList *);							

// Function: SmpTransBody
// Sample2: 指定したBodyに対しアフィン変換を行う
int SmpTransBody(BODYList *,OBJECTList *,int,double []);

// Function: SmpDivCurves
// Sample3: 曲線を指定した分割数で分割した点を生成する
int SmpDivCurves(BODYList *,OBJECTList *,int,double []);

// Function: SmpNearestPt
// Sample4: 任意の点からの曲面上の最近傍点を求める
int SmpNearestPt(BODYList *,OBJECTList *,int,double []);

// Function: SmpUVDivLine
// Sample5: 曲面のUVパラメータで等間隔に分割した点を生成する
int SmpUVDivLine(BODYList *,OBJECTList *,int,double []);

// Function: SmpContourLine
// Sample6: 等高線を生成する
int SmpContourLine(BODYList *,OBJECTList *,int,double []);

// Function: SmpIntersectSurfs
// Sample7: 曲面同士の交線を算出する
int SmpIntersectSurfs(BODYList *,OBJECTList *,int,double []);

// Function: SmpMeanCurvature
// Sample8: 平均曲率法線ベクトルを算出する
int SmpMeanCurvature(BODYList *,OBJECTList *,int,double []);

// Function: Smp3xCAM
// Sample9: スキャンパスを生成するシンプルな3xCAM
void Smp3xCAM(NURBSS *,NURBSC *,double,int,double,int);
