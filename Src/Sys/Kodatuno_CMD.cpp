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
        char mes[256];
        sprintf(mes,"Kodatuno R%d.%d",KODATUNO_MAJOR_VER,KODATUNO_MINOR_VER);
        GuiIF.SetMessage(mes);
        return;
	}

	// オプションあり
	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'f':		// "-f"オプションでフルネーム出力
                GuiIF.SetMessage("Kodatuno is Open Developed Alternative Trajectory Utility Nucleus Object");
				break;
			case 'F':		// "-F (数値)"でその数値分文字列を出力(意味なし)
				if(argc==1) return;
				argc--; argv++;
				for(int i=0;i<atoi(argv[0]);i++){
					char mes[256];
					sprintf(mes,"%d:Kodatuno 2.0",i+1);
                    GuiIF.SetMessage(mes);
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

    GuiIF.UpdateDescribeWidget();
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

// Bodyの平行移動
void CmdMoveBody(int argc,char *argv[])
{
	char *p;

	// オプションなし
	if(!argc){
        GuiIF.SetMessage("Command Error: Set -r dx dy dz options");
		return;
	}

	double d[3] = {0,0,0};	// 移動量格納用

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (数値)"で、移動量をセット
			case 'r':
				if(argc==1) return;
				for(int i=0;i<3;i++){
					argc--; argv++;
					d[i] = atof(argv[0]);
				}
				Kodatuno.GetShiftBody(SetCoord(d[0],d[1],d[2]));
			break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// Bodyの回転
void CmdRotateBody(int argc,char *argv[])
{
	char *p;

	// オプションなし
	if(!argc){
        GuiIF.SetMessage("Command Error: Set -r x y z deg options");
		return;
	}

	double axd[4] = {0,0,0,0};	// 回転軸,回転角

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (数値)"で、移動量をセット
			case 'r':
				if(argc==1) return;
				for(int i=0;i<4;i++){
					argc--; argv++;
					axd[i] = atof(argv[0]);
				}
				Kodatuno.GetRotateBody(SetCoord(axd[0],axd[1],axd[2]),axd[3]);
			break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// コントロールポイントを描画
void CmdCPView(int argc,char *argv[])
{
	Kodatuno.CPViewFlag = KOD_TRUE;
}

// 曲面情報を出力
void CmdSurfInfo(int argc,char *argv[])
{
	Kodatuno.GetSurfInfo();
}

// 回転サーフェス生成コマンド
void CmdGenRotSurf(int argc,char *argv[])
{
	GuiIF.ShowRotSurfDlg();
}

// スイープサーフェス生成コマンド
void CmdGenSweepSurf(int argc,char *argv[])
{
	GuiIF.ShowSweepSurfDlg();
}

// Nurbs曲線生成コマンド
void CmdGenNurbsCurve(int argc,char *argv[])
{
	GuiIF.ShowNurbsCurveDlg();
}

// Nurbs曲面生成コマンド
void CmdGenNurbsSurface(int argc,char *argv[])
{
	GuiIF.ShowNurbsSurfDlg();
}

// Bodyの拡大
void CmdExpand(int argc,char *argv[])
{
	char *p;

	// オプションなし
	if(!argc){
        GuiIF.SetMessage("Command Error: Set -r x y z deg options");
		return;
	}

	double axd[3] = {0,0,0};	// 各軸方向拡大率

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (数値)"で、移動量をセット
			case 'r':
				if(argc==1) return;
				for(int i=0;i<3;i++){
					argc--; argv++;
					axd[i] = atof(argv[0]);
				}
				Kodatuno.ExpandBody(SetCoord(axd[0],axd[1],axd[2]));
			break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// NURBS Rnakの変更
void CmdChRank(int argc, char *argv[])
{
	char *p;

	// オプションなし
	if(!argc){
        GuiIF.SetMessage("Command Error: Set -r (u-val) (v-val)");
		return;
	}

	int r[2];

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (数値)"で、トレランスをセット
			case 'r':
				if(argc==1) return;
				for(int i=0;i<2;i++){
					argc--; argv++;
					r[i] = atoi(argv[0]);
				}
				Kodatuno.ChangeRank(r);
				break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// 背景の色を変更する
void CmdChBkCol(int argc, char *argv[])
{
	char *p;

	// オプションなし
	if(!argc){
        GuiIF.SetMessage("Command Error: Set -r (r-val) (g-val) (b-val)");
		return;
	}

	double r[3]={0,0,0};

	argv++;
	while(argc > 0 && (p=argv[0])[0] == '-' && *++p != '\0'){
		do{
			switch(*p){
			case 'R':		// "-r (数値)"で、トレランスをセット
			case 'r':
				if(argc==1) return;
				for(int i=0;i<3;i++){
					argc--; argv++;
					r[i] = atof(argv[0]);
				}
				Kodatuno.ChangeBackColor(r);
				break;
			}
		}while(*++p);
		argv++; argc--;
	}
}

// Mesh情報を出力
void CmdMeshInf(int argc, char *argv[])
{
	Kodatuno.GetMeshInfo();
}

// U,Vの方向を表示する
void CmdUVdir(int argc,char *argv[])
{
	Kodatuno.UVDirFlag = KOD_TRUE;
}