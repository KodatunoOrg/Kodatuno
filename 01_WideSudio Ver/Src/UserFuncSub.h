// UserSampleヘッダ

#include "StdAfx.h"
#include "NURBS_Func.h"		// NURBS幾何計算関連
#include "Describe_BODY.h"	// BODY描画関連
#include "BODY_Handler.h"
#include "BODY.h"

#define PROPNUM	15			// プロパティー数

int ExecSampleFunc1(BODYList *,OBJECTList *,int,double []);		// サンプル1
int ExecSampleFunc2(BODYList *,OBJECTList *,int,double []);		// サンプル2

int ContourLine(NURBSS *,double,double,double,double);			// 等高線生成
