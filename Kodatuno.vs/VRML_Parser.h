// VRML V1.0 パーサー

#ifndef _VRML_PARSER_MAIN_H_
#define _VRML_PARSER_MAIN_H_

#include "BODY.h"

// Constants: General Defines
// BUFSIZEMAX -		文字列一時格納用バッファのサイズ(256)
// ALLTAGNUM -		VRMLタグの総数(37)
#define BUFSIZEMAX 256
#define ALLTAGNUM  37

// Enum: Enum Symbol of VRML Tag
// AsciiText -			1:文字
// Cone -				2:円錐
// Coordinate3 -		3:三次元座標
// Cube -				4:立方体
// Cylinder -			5:円柱
// DirectionalLight -	6:指向性光源
// FontStyle -			7:書体
// Group -				8:グループ 
// IndexedFaceSet -		9:面を頂点のインデックスで指定
// IndexedLineSet -		10:線を頂点のインデックスで指定
// Info -				11:インフォメーション
// LOD -				12:詳細指定
// Material -			13:材質
// MaterialBinding -	14:材質（結合）
// MatrixTransform -	15:三次元座標変換
// Normal -				16:法線
// NormalBinding -		17:法線（結合）
// OrthographicCamera - 18:平行投影カメラ
// PerspectiveCamera -	19:透視カメラ
// PointLight -			20:点光源
// PointSet -			21:点
// Rotation -			22:回転
// Scale -				23:オブジェクトのスケール
// Separator -			24:空間分離
// ShapeHints -			25:形状情報
// Sphere -				26:球
// SpotLight -			27:スポットライト（指向性）
// Switch -				28:選択
// Texture2 -			29:テクスチャ画像
// Texture2Transform -	30:テクスチャ座標
// TextureCoordinate2 - 31:座標
// TextureCoordinateBinding - 32
// Transform -			33:総合移動
// TransformSeparator - 34:座標分離
// Translation -		35:平行移動
// WWWAnchor -			36:WWWアンカー 
// WWWInline -			37:ファイル読み込み
enum VRMLTags{
	AsciiText=1,
	Cone,
	Coordinate3,
	Cube,
	Cylinder,
	DirectionalLight,
	FontStyle,
	Group,
	IndexedFaceSet,
	IndexedLineSet,
	Info,
	LOD,
	Material,
	MaterialBinding,
	MatrixTransform,
	Normal,
	NormalBinding,
	OrthographicCamera,
	PerspectiveCamera,
	PointLight,
	PointSet,
	Rotation,
	Scale,
	Separator,
	ShapeHints,
	Sphere,
	SpotLight,
	Switch,
	Texture2,
	Texture2Transform,
	TextureCoordinate2,
	TextureCoordinateBinding,
	Transform,
	TransformSeparator,
	Translation,
	WWWAnchor,
	WWWInline,
};

// Class: VRML_PARSER
// VRML V1.0 パーサー用クラスを定義
class VRML_PARSER
{
public:
	// Function: Vrml_Parser_Main
	// VRMLファイルのパーサmain
    int Vrml_Parser_Main(BODY *, const char *);	

private:
	// Function: CheckTags
	// (private)タグ解析
	int CheckTags(char []);					
	
	// Function: CheckTags
	// (private)三次元座標値をMESHに格納する
	int GetCoords(FILE *,MESH *);			
	
	// Function: CheckTags
	// (private)ファセットをMESHに格納する
	int GetFacets(FILE *,MESH *);			
	
	// Function: CheckTags
	// (private)ファイルから1行取得し、改行文字を終端文字に変更する
	int GetLine(FILE *,char []);			
	
	// Function: CheckTags
	// (private)頂点のインデックスセットから、メッシュデータを生成する
	void SetMesh(MESH *,int [],int);		
	
	// Function: CheckTags
	// (private)対となる逆方向ハーフエッジへのポインタを得る
	int GetHalfEdgePair(MESH *);			
	
	// Function: CheckTags
	// (private)各面に面積と単位法線ベクトルの情報を付加する
	void SetFaceParam(MESH *);				
};

#endif
