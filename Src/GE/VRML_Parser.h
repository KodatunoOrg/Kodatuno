// VRML V1.0 パーサー

#ifndef _VRML_PARSER_MAIN_H_
#define _VRML_PARSER_MAIN_H_

#include "BODY.h"

#define BUFSIZEMAX 256		// 文字列一時格納用バッファのサイズ
#define ALLTAGNUM  37		// VRMLタグの総数

// VRMLファイルに用いられるタグのシンボル定義
enum VRMLTags{
	AsciiText=1,		// 1:文字
	Cone,				// 2:円錐
	Coordinate3,		// 3:三次元座標
	Cube,				// 4:立方体
	Cylinder,			// 5:円柱
	DirectionalLight,	// 6:指向性光源
	FontStyle,			// 7:書体
	Group,				// 8:グループ 
	IndexedFaceSet,		// 9:面を頂点のインデックスで指定
	IndexedLineSet,		// 10:線を頂点のインデックスで指定
	Info,				// 11:インフォメーション
	LOD,				// 12:詳細指定
	Material,			// 13:材質
	MaterialBinding,	// 14:材質（結合）
	MatrixTransform,	// 15:三次元座標変換
	Normal,				// 16:法線
	NormalBinding,		// 17:法線（結合）
	OrthographicCamera,	// 18:平行投影カメラ
	PerspectiveCamera,	// 19:透視カメラ
	PointLight,			// 20:点光源
	PointSet,			// 21:点
	Rotation,			// 22:回転
	Scale,				// 23:オブジェクトのスケール
	Separator,			// 24:空間分離
	ShapeHints,			// 25:形状情報
	Sphere,				// 26:球
	SpotLight,			// 27:スポットライト（指向性）
	Switch,				// 28:選択
	Texture2,			// 29:テクスチャ画像
	Texture2Transform,	// 30:テクスチャ座標
	TextureCoordinate2,	// 31:座標
	TextureCoordinateBinding,	// 32
	Transform,			// 33:総合移動
	TransformSeparator,	// 34:座標分離
	Translation,		// 35:平行移動
	WWWAnchor,			// 36:WWWアンカー 
	WWWInline,			// 37:ファイル読み込み
};

// VRML V1.0 パーサー用クラスを定義
class VRML_PARSER
{
public:
    int Vrml_Parser_Main(BODY *, const char *);	// VRMLファイルのパーサmain

private:
	int CheckTags(char []);					// タグ解析
	int GetCoords(FILE *,MESH *);			// 三次元座標値をMESHに格納する
	int GetFacets(FILE *,MESH *);			// ファセットをMESHに格納する
	int GetLine(FILE *,char []);			// ファイルから1行取得し、改行文字を終端文字に変更する
	void SetMesh(MESH *,int [],int);		// 頂点のインデックスセットから、メッシュデータを生成する
	int GetHalfEdgePair(MESH *);			// 対となる逆方向ハーフエッジへのポインタを得る
	void SetFaceParam(MESH *);				// 各面に面積と単位法線ベクトルの情報を付加する
};

#endif
