/*************************************************
* KODatUNO Ver2.0 β  Takasugi 2009/6            
* KODatUNO Juza       Takasugi Kumasaka 2009/9   
* KODatUNO Mamiya	  Takasugi Kumasaka 2009/11
* Takasugi Last Cording 2010/03/04
* Kumasaka Last Cording 2010/??/??
**************************************************/

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
