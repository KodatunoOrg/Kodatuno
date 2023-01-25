/*********************************************************
* VRML1.0ファイルを読み込み、ハーフエッジ構造へ格納する  *
* 2012/4 K.Takasugi                                      *
**********************************************************/

#include "VRML_Parser.h"

int VRML_PARSER::Vrml_Parser_Main(BODY *body, const char *fname)
{
	FILE *fp;
	char mes[BUFSIZEMAX];		// 出力用メッセージ格納バッファ
	char buf[BUFSIZEMAX];		// 文字列一時格納用バッファ
	int tag;					// タグ判別用
	int flag;

	// VRMLファイルオープン
	if((fp = fopen(fname,"r")) == NULL){
		sprintf(mes,"KOD_ERROR: Cannot open %s",fname);
        GuiIF.SetMessage(mes);
		return(KOD_ERR);
	}

	// ヘッダ読み込み
	fgets(buf,sizeof(buf),fp);
	if(strcmp(buf,"#VRML V1.0 ascii\n")){
		sprintf(mes,"KOD_ERROR: Kodatuno supports only #VRML V1.0 ascii.     Please check VRML file version.");
        GuiIF.SetMessage(mes);
		fclose(fp);
		return(KOD_ERR);
	}

	body->Mesh = new MESH;		// MESHクラスのメモリー確保

	// 1行ずつ読み込みながら、タグ解析していく
	while(fgets(buf,sizeof(buf),fp) != NULL){
		if((tag = CheckTags(buf)) == KOD_ERR){	// タグ解析
			continue;							// 登録されたタグが書かれていない行は読み飛ばす
		}

		if(tag == Coordinate3){					// 三次元座標用タグを発見
			flag = GetCoords(fp,body->Mesh);	// 三次元座標値をMESHに格納
			if(flag == KOD_ERR){
				body->Mesh->clear();
				return KOD_ERR;
			}

		}
		else if(tag == IndexedFaceSet){			// ファセット定義用タグを発見
			flag = GetFacets(fp,body->Mesh);	// ファセットをMESHに格納
			if(flag == KOD_ERR){
				body->Mesh->clear();
				return KOD_ERR;
			}
		}
	}

	fclose(fp);

	// ハーフエッジ、頂点、面それぞれの総数を得る
	body->Mesh->EdgeNum = body->Mesh->Edge.getNum();
	body->Mesh->VertNum = body->Mesh->Vert.getNum();
	body->Mesh->FaceNum = body->Mesh->Face.getNum();

	GetHalfEdgePair(body->Mesh);	// 対となる逆方向ハーフエッジへのポインタを得る

	SetFaceParam(body->Mesh);		// 各面に面積と単位法線ベクトルの情報を付加する

	body->MaxCoord = 10;			// 倍率はとりあえず10にしておく

	body->TypeNum[_MESH] = 1;		// TypeNumは1

	return KOD_TRUE;
}

// 各面に面積の情報を付加する
void VRML_PARSER::SetFaceParam(MESH *mesh)
{
	for(int i=0;i<mesh->FaceNum;i++){
		HEface *f = (HEface *)mesh->Face.getData(i);
		mesh->CalcFaceArea(f);		// 面積の情報を付加する
		mesh->CalcFaceNorm(f);		// 単位法線ベクトルの情報を付加する
	}
}

// 対となる逆方向ハーフエッジへのポインタを得る
int VRML_PARSER::GetHalfEdgePair(MESH *mesh)
{
	bool flag = false;

	// 全ハーフエッジに対して
	for(int i=0;i<mesh->EdgeNum;i++){
		HEedge *e = (HEedge *)mesh->Edge.getData(i);	// i番のハーフエッジを取り出す
		mesh->Edge.setSentinel(0);						// 旗を最初のハーフエッジに立てる
		flag = false;									// 見つけたフラグＯＦＦ
		for(int j=0;j<mesh->EdgeNum;j++){				// i番のハーフエッジの対を探す
			if(j==i){									// 同じハーフエッジを見ている場合は
				mesh->Edge.shiftSentinel(1);			// 旗を1つずらす
				continue;
			}
			HEedge *e_ = (HEedge *)mesh->Edge.getSentinelData();	// 旗の立っているハーフエッジのデータを得る
			if(e_->ne->vert->index == e->vert->index &&				// j番のハーフエッジの次のハーフエッジのインデックスがi番のハーフエッジのインデックスと等しいかつ
				e_->vert->index == e->ne->vert->index){				// i番のハーフエッジの次のハーフエッジのインデックスがj番のハーフエッジのインデックスと等しいなら
				e->pair = e_;										// 対を見つけた
				flag = true;							// 見つけたフラグＯＮ
				break;									// 次のハーフエッジに移動
			}
			mesh->Edge.shiftSentinel(1);				// 対を見つけていない場合は、旗を1つずらす
		}
		if(flag == false){								// 見つからなかったら(面の境界エッジ)
			e->pair = NULL;								// 対はNULL
		}
	}

	return KOD_TRUE;
}

// ファセットをMESHに格納
int VRML_PARSER::GetFacets(FILE *fp,MESH *mesh)
{
	char *p,*q;
	char buf[BUFSIZEMAX],word[BUFSIZEMAX];
	int index[MAXVERTNUMINFACE]={-1,-1,-1,-1};	// 頂点のインデックスセット
	int edgecount = 0;							// エッジの総数

	// 三次元座標値を全て読み込むまで1行づつ読み込む
	while(!feof(fp)){
		GetLine(fp,buf);						// 1行取得
		if(strstr(buf,"coordIndex") == NULL)	// "coordIndex"タグが見つからなかったら
			continue;							// 見つかるまで読み続ける

		//  "coordIndex"タグが見つかっていたら頂点座標取得
		while(GetLine(fp,buf)){							// 1行取得
			if(strchr(buf,']') != NULL) break;			// データ終わりを示すトークン']'だけの行だったら読み込み終了
			int i=0;
			p = strtok(buf,",");						// カンマ区切りで文字列を抽出していく
			sscanf(p,"%d",&index[i]);					// 抽出した文字列を整数に変換
			i++;
			while(p != NULL){
				if((p = strtok(NULL,",")) != NULL){
					sscanf(p,"%d",&index[i]);
					if(index[i] == -1){					// ファセットの終わりを示す-1を見つけたら
						edgecount += i;					// エッジ総数をインクリメント
						SetMesh(mesh,index,edgecount);	// それまでのインデックスセットでメッシュを生成
						i = 0;							// インデックス番号をリセット
					}
					else i++;
				}
			}
		}

		return KOD_TRUE;	// 正常に読み込み終了
	}

	return KOD_ERR;			// 三次元座標値を取得中にファイルが終了:エラー
}

// 頂点のインデックスセットから、メッシュデータを生成する
void VRML_PARSER::SetMesh(MESH *mesh,int vindex[],int edgecount)
{
	int vcount=MAXVERTNUMINFACE;
	HEface *f;
	HEvert *v;

	// 頂点数を認識
	for(int i=0;i<MAXVERTNUMINFACE;i++){
		if(vindex[i] == -1)
			vcount = i;
	}

	int startnum = edgecount - vcount;

	// ハーフエッジを登録
	for(int i=0;i<vcount;i++){
		HEedge *e = new HEedge();		// ハーフエッジを1つメモリ確保
		v = (HEvert *)mesh->Vert.getData(vindex[i]);
		e->SetVert(v);					// 先に登録しておいた頂点リストから同じインデックスのものを探し出しエッジと関連付け
		v->SetEdge(e);
		e->SetIndex(startnum + i);		// エッジのインデックスを登録
		e->mom = mesh->Edge.add(e);				// ハーフエッジをメッシュのエッジリストに登録(eを入れた箱のアドレスも保持)
		// 面を登録
		if(!i){
			f = new HEface();					// 面を1つメモリ確保
			f->SetEdge(e);						// 面に族するハーフエッジを1つ持つ
			f->SetIndex(int(edgecount/vcount)-1);	// 面のインデックスを登録
			f->SetVertNum(vcount);				// 面の頂点数を登録
			SetColorStat(&f->Dstat,0.2,0.2,1);	// 色指示
			f->mom = mesh->Face.add(f);			// 面をメッシュの面リストに登録(fを入れた箱のアドレスも保持)
		}
		e->SetFace(f);							// ハーフエッジが属する面を登録
	}

	// ハーフエッジの次へのポインタと、それが属する面を登録
	HEedge *e_ = (HEedge *)mesh->Edge.setSentinel(startnum);		// 1本目のエッジに旗を立てる
	for(int i=0;i<vcount;i++){
		if(i==vcount-1){
			e_->ne = (HEedge *)mesh->Edge.getSentinelData();		// 最後のエッジのnextは最初のエッジ
		}
		else{
			e_->ne = (HEedge *)mesh->Edge.getDataFromSentinel(i+1);	// エッジをリスト化
			e_ = e_->ne;
		}
	}

}

// 三次元座標値をMESHに格納
int VRML_PARSER::GetCoords(FILE *fp,MESH *mesh)
{
	char *p,*q;
	char buf[BUFSIZEMAX],words[BUFSIZEMAX];
	int index=0;	// 頂点のインデックス
	Coord c;

	// 三次元座標値を全て読み込むまで1行づつ読み込む
	while(!feof(fp)){
		GetLine(fp,buf);				// 1行取得
		if(strstr(buf,"point") == NULL)	// "point"タグが見つからなかったら
			continue;					// 見つかるまで読み続ける

		//  "point"タグが見つかっていたら頂点座標取得
		while(GetLine(fp,buf)){							// 1行取得
			if(strchr(buf,']') != NULL) break;			// データ終わりを示すトークン']'だけの行だったら読み込み終了
			HEvert *v = new HEvert();
			p = strtok(buf,",");						// カンマ区切りで文字列を抽出していく
			sscanf(p,"%lf %lf %lf",&c.x,&c.y,&c.z);					// データ格納
			v->SetCod(c);
			v->SetIndex(index);
			v->mom = mesh->Vert.add(v);		// vをリストに登録(vを入れた箱のアドレスも保持)
			//fprintf(stderr,"%p (%lf,%lf,%lf)\n",v,v->cod.x,v->cod.y,v->cod.z);	// debug
			index++;
			while(p != NULL){
				if((p = strtok(NULL,",")) != NULL){
					HEvert *v = new HEvert();
					sscanf(p,"%lf %lf %lf",&v->cod.x,&v->cod.y,&v->cod.z);
					v->index = index;
					v->mom = mesh->Vert.add(v);
					//fprintf(stderr,"%p (%lf,%lf,%lf)\n",v,v->cod.x,v->cod.y,v->cod.z);	// debug
					index++;
				}
			}
		}
		return KOD_TRUE;	// 正常に読み込み終了
	}

	return KOD_ERR;		// 三次元座標値を取得中にファイルが終了:エラー
}

// ファイルから1行取得し、改行文字を終端文字に変更する
int VRML_PARSER::GetLine(FILE *fp,char buf[])
{
	fgets(buf,BUFSIZEMAX,fp);			// 1行取得
	int len = strlen(buf);				// 1行の長さを調べる
	if(len > 0 && buf[len-1] == '\n')	// 最後の改行文字\nを終端文字\0に変更
		buf[len-1] = '\0';

	return len-1;
}

// タグ解析
int VRML_PARSER::CheckTags(char buf[])
{
	if(strstr(buf,"AsciiText") != NULL)			return AsciiText;		// 1
	if(strstr(buf,"Cone") != NULL)				return Cone;			// 2
	if(strstr(buf,"Coordinate3") != NULL)		return Coordinate3;		// 3:三次元座標
	if(strstr(buf,"Cube") != NULL)				return Cube;			// 4
	if(strstr(buf,"Cylinder") != NULL)			return Cylinder;		// 5
	if(strstr(buf,"DirectionalLight") != NULL)	return DirectionalLight;// 6
	if(strstr(buf,"FontStyle") != NULL)			return FontStyle;		// 7
	if(strstr(buf,"Group") != NULL)				return Group;			// 8
	if(strstr(buf,"IndexedFaceSet") != NULL)	return IndexedFaceSet;	// 9:ファセット頂点のインデックス
	if(strstr(buf,"IndexedLineSet") != NULL)	return IndexedLineSet;	// 10
	if(strstr(buf,"Info") != NULL)				return Info;			// 11
	if(strstr(buf,"LOD") != NULL)				return LOD;				// 12
	if(strstr(buf,"MaterialBinding") != NULL)	return MaterialBinding;	// 14
	if(strstr(buf,"Material") != NULL)			return Material;		// 13
	if(strstr(buf,"MatrixTransform") != NULL)	return MatrixTransform;	// 15
	if(strstr(buf,"NormalBinding") != NULL)		return NormalBinding;	// 17
	if(strstr(buf,"Normal") != NULL)			return Normal;			// 16
	if(strstr(buf,"OrthographicCamera") != NULL)return OrthographicCamera;	// 18
	if(strstr(buf,"PerspectiveCamera") != NULL)	return PerspectiveCamera;	// 19
	if(strstr(buf,"PointLight") != NULL)		return PointLight;		// 20
	if(strstr(buf,"PointSet") != NULL)			return PointSet;		// 21
	if(strstr(buf,"Rotation") != NULL)			return Rotation;		// 22
	if(strstr(buf,"Scale") != NULL)				return Scale;			// 23
	if(strstr(buf,"Separator") != NULL)			return Separator;		// 24
	if(strstr(buf,"ShapeHints") != NULL)		return ShapeHints;		// 25
	if(strstr(buf,"Sphere") != NULL)			return Sphere;			// 26
	if(strstr(buf,"SpotLight") != NULL)			return SpotLight;		// 27
	if(strstr(buf,"Switch") != NULL)			return Switch;			// 28
	if(strstr(buf,"Texture2Transform") != NULL)	return Texture2Transform;	// 30
	if(strstr(buf,"Texture2") != NULL)			return Texture2;		// 29
	if(strstr(buf,"TextureCoordinate2") != NULL)return TextureCoordinate2;	// 31
	if(strstr(buf,"TextureCoordinateBinding") != NULL)	return TextureCoordinateBinding;// 32
	if(strstr(buf,"TransformSeparator") != NULL)		return TransformSeparator;		// 34
	if(strstr(buf,"Transform") != NULL)			return Transform;		// 33
	if(strstr(buf,"Translation") != NULL)		return Translation;		// 35
	if(strstr(buf,"WWWAnchor") != NULL)			return WWWAnchor;		// 36
	if(strstr(buf,"WWWInline") != NULL)			return WWWInline;		// 37

	return KOD_ERR;	// TAGが見つからなかった
}
