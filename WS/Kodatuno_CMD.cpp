#include"Kodatuno.h"


//////////////////////////////////////////////////////
// Kodatunoコマンド実行関数は以降に記述してください //
//////////////////////////////////////////////////////

// Kodatunoコマンド(バージョン情報出力)
void CmdVerInf(int argc,char *argv[])
{
	char *p;

	// オプションなし
	if(!argc){
		SetMessage("Kodatuno R1.1");
		return;
	}

	// オプションあり
	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'f':		// "-f"オプションでフルネーム出力
				SetMessage("Kanazawa univ's Open Developed Alternative Trajectory Utility Nucleus Obuject");
				break;
			case 'F':		// "-F (数値)"でその数値分文字列を出力(意味なし)
				if(argc==1) return;
				argc--; argv++;
				for(int i=0;i<atoi(argv[0]);i++){
					char mes[256];
					sprintf(mes,"%d:Kodatuno 1.1",i+1);
					SetMessage(mes);
				}
				break;
			}
		}while(*++p);
		argv++; argc--;
	}

}

// Kodatunoコマンド(ファイルオープン)
void CmdFileOpen(int argc,char *argv[])
{
	char *p;

	// オプションなし
	if(!argc){
		Kodatuno.OpenFile();
	}

	// オプションあり
	else{
		argv++;
		while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
			do{
				switch(*p){
			case 'N':		// "-N (ファイル名)"でそのファイルをオープン
				if(argc==1) return;
				argc--; argv++;
				Kodatuno.OpenFile(argv[0]);
				break;
				}
			}while(*++p);
			argv++; argc--;
		}
	}

	Describe_Form->redraw();
}

// UVパラメータで分割されたワイヤーフレームを表示
void CmdUVWire(int argc,char *argv[])
{
	Kodatuno.UVWireFlameViewFlag = KOD_TRUE;
}

// スケール変更
void CmdChScale(int argc,char *argv[])
{
	char *p;

	// オプションなし
	if(!argc)
		Kodatuno.GetModelScale();

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (数値)"で、モデルスケールをセット
			case 'r':
				if(argc==1) return;
				argc--; argv++;
				Kodatuno.SetModelScale(atof(argv[0]));
				break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// トレランス変更
void CmdChTolerance(int argc, char *argv[])
{
	char *p;

	if(!argc)
		Kodatuno.GetTolerance();

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (数値)"で、トレランスをセット
			case 'r':
				if(argc==1) return;
				argc--; argv++;
				Kodatuno.SetTolerance(atof(argv[0]));
				break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// 回転サーフェス生成コマンド
void CmdGenRotSurf(int argc,char *argv[])
{
	RotSurf_Dlg->setVisible(True);		// 回転サーフェス生成ダイアログを表示
}

// スイープサーフェス生成コマンド
void CmdGenSweepSurf(int argc,char *argv[])
{
	SweepSurf_Dlg->setVisible(True);	// スイープサーフェス生成ダイアログを表示
}

// Nurbs曲線生成コマンド
void CmdGenNurbsCurve(int argc,char *argv[])
{
	NurbsCurve_Dlg->setVisible(True);
}

// Nurbs曲面生成コマンド
void CmdGenNurbsSurface(int argc,char *argv[])
{
	NurbsSurface_Dlg->setVisible(True);
}
