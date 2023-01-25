/*******************************
* KODATUNO R1.0		  2010/3
********************************/

#include "KODATUNO.h"

int main(int argc,char *argv[])
{
	KODatUNO Kakuma;					// KODatUNOクラスのオブジェクトを生成

	Kakuma.Kodatuno_Main(argc,argv);	// KODatUNOアプリケーションスタート
}

// KODatUNOメイン
void KODatUNO::Kodatuno_Main(int argc,char *argv[])
{
	// OpenGLによる描画クラス
	static DESCRIBE Describe;			// IGES描画用オブジェクトを生成
	Describe.BODY_Describe_Main(argc,argv);
}
