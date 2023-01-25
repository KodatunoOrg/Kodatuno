// Half-Edgeデータ構造の定義
// 2012/4時点でのメッシュデータのルール
// 三角パッチか四角パッチとする．
// 三角パッチと四角パッチの混在は許さない．

#ifndef _HALF_EDGE_H_
#define _HALF_EDGE_H_

#include "StdAfx.h"
#include "KodListFunc.h"

// Constants: General Defines
// MAXVERTNUMINFACE -	1つのメッシュが持つ頂点数の上限(四角形まで)(4)
// TRIMESHVERTNUM -		三角メッシュの頂点数(3)
// QUADMESHVERTNUM -	四角形メッシュの頂点数(4)
#define MAXVERTNUMINFACE	4
#define TRIMESHVERTNUM		3
#define QUADMESHVERTNUM		4

class HEedge;
class HEvert;
class HEface;

// Class: HEvert
// 頂点クラス
class HEvert{
public:
	// Constructor: HEvert
	// コンストラクタ
	HEvert() {cod=InitCoord(); edge=NULL; index=-1; mom = NULL; qemQ=NULL; NewQEM(); flag = false;}

	// Function: GetCoord
	// この頂点の座標値を返す
	//
	// Return:
	// cod
	Coord	GetCoord()	{return cod;}

	// Function: GetHE
	// この頂点から伸びるハーフエッジの1つを返す
	//
	// Return:
	// edge
	HEedge	*GetHE()	{return edge;}

	// Function: GetIndex
	// この頂点のインデックスを返す
	//
	// Return:
	// index
	int		GetIndex()	{return index;}

	// Function: GetFlag
	// フラグを返す
	//
	// Return:
	// flag
	int    GetFlag()	{return flag;}			

	// Function: NewQEM
	// QEM法用係数行列qemQのメモリ確保(4x4)
	void NewQEM();

	// Function: DelQEM
	// QEM法用係数行列qemQのメモリ解放
	void DelQEM();

	// Function: InitQEM
	// QEM法用係数行列qemQの初期化
	void InitQEM();

	// Function: SetQEM
	// QEM法用係数行列qemQのqemパラメータをセットする
	int SetQEM(Matrix);

	// Function: AddQEM
	// QEM法用係数行列qemQのqemパラメータを加える
	int AddQEM(Matrix);	

	// Function: GetQEM
	// QEM法用係数行列qemQのqemパラメータを得る
	double **GetQEM();

	// Function: SetEdge
	// この頂点から伸びるハーフエッジをセット
	//
	// Parameter:
	// *e - ハーフエッジへのポインタ
	void SetEdge(HEedge *e)	{edge = e;}

	// Function: SetCod
	// この頂点の座標値をセット
	//
	// Parameter:
	// c - 座標値
	void SetCod(Coord c)	{cod = c;}

	// Function: SetIndex
	// この頂点のインデックスをセット
	//
	// Parameter:
	// n - インデックス値
	void SetIndex(int n)	{index = n;}

	// Function: SetFlag
	// フラグをセット
	// 
	// Parameter:
	// f - フラグ
	void SetFlag(int f)	{flag = f;}			


public:
	// Variable: cod
	// この頂点の三次元座標
	Coord  cod;		

	// Variable: *edge
	// この頂点から伸びるハーフエッジへのポインタ(注：頂点から伸びるHEは1本とは限らない。どのHEと繋がっているか分からない)
	HEedge *edge;

	// Variable: index
	// インデックス
	int    index;

	// Variable: *mom
	// 自分が属する親(Dataクラス)のアドレス
	Data   *mom;

	// Variable: **qemQ
	// QEM法用係数行列
	double **qemQ;	

	// Variable: flag
	// 汎用フラグ
	int   flag;		
};

// Class: HEedge
// ハーフエッジクラス
class HEedge{
public:
	// Fucntion: HEedge
	// コンストラクタ
	HEedge() {vert=NULL; pair=NULL; face=NULL; ne=NULL; index=-1; mom = NULL; InitQEMP(); InitQEMD();}

	// Function: GetStartVert
	// このハーフエッジの始点を返す
	//
	// Return:
	// vert
	HEvert	*GetStartVert()	{return vert;}

	// Function: GetStartVcoord
	// このハーフエッジの始点座標を返す
	//
	// Return:
	// vert->cod
	Coord	GetStartVcoord()	{return vert->cod;}

	// Function: GetPairHE
	// このハーフエッジの対となるハーフエッジを返す
	//
	// Return:
	// pair
	HEedge	*GetPairHE()	{return pair;}

	// Function: GetFace
	// このハーフエッジが属する面を返す
	//
	// Return:
	// face
	HEface	*GetFace()	{return face;}

	// Function: GetNextHE
	// このハーフエッジの次のハーフエッジを返す
	//
	// Return:
	// ne
	HEedge	*GetNextHE()	{return ne;}

	// Function: GetNextHE
	// このハーフエッジの(共有する面における)1つ前のハーフエッジを返す
	HEedge  *GetPrevHE();

	// Function: GetIndex
	// このハーフエッジのインデックスを返す
	//
	// Return:
	// index
	int	GetIndex()	{return index;}

	// Function: GetStartVindex
	// このハーフエッジの始点のインデックスを返す
	//
	// Return:
	// vert->index
	int	GetStartVindex()	{return vert->index;}

	// Function: GetStartVindex
	// フラグを返す
	//
	// Return:
	// vert->index
	int	GetFlag()	{return flag;}			
	
	// Function: InitQEMP
	// QEM法用三次元座標値(Edge Collapse適用後の頂点位置)qemPの初期化
	void InitQEMP()	{qemP = InitCoord();}

	// Function: InitQEMD
	// QEM法用パラメータ(Edge Collapse適用のコスト)qemDの初期化 
	void InitQEMD()	{qemD = 0;}

	// Function: SetQEMP
	// QEM法用三次元座標値(Edge Collapse適用後の頂点位置)qemPをセット
	// 
	// Parameter:
	// p - 座標値
	void SetQEMP(Coord p)	{qemP = p;}

	// Function: SetQEMD
	// QEM法用パラメータ(Edge Collapse適用のコスト)qemDをセット
	// 
	// Parameter:
	// d - 適用コストパラメータ
	void SetQEMD(double d)	{qemD = d;}

	// Function: GetQEMP
	// QEM法用三次元座標値(Edge Collapse適用後の頂点位置)qemPを返す
	//
	// Return
	// qemP
	Coord GetQEMP()	{return qemP;}

	// Function: GetQEMD
	// QEM法用パラメータ(Edge Collapse適用のコスト)qemDを返す
	// 
	// Return
	// qemD
	double GetQEMD()	{return qemD;}

	// Function: SetVert
	// このハーフエッジの始点をセット
	//
	// Parameter:
	// *v - このハーフエッジの始点となるHEvertへのポインタ
	void SetVert(HEvert *v)		{vert = v;}

	// Function: SetPair
	// このハーフエッジの対となるハーフエッジをセット
	//
	// Parameter:
	// *e - このハーフエッジの対となるHEedgeへのポインタ
	void SetPair(HEedge *e)		{pair = e;}

	// Function: SetFace
	// このハーフエッジが属する面をセット
	//
	// Parameter:
	// *f - このハーフエッジが属する面HEfaceへのポインタ
	void SetFace(HEface *f)		{face = f;}

	// Function: SetNextHE
	// このハーフエッジの次のハーフエッジをセット
	//
	// Parameter:
	// *e - このハーフエッジの次のHEedgeへのポインタ
	void SetNextHE(HEedge *e)	{ne = e;}

	// Function: SetIndex
	// このハーフエッジのインデックスをセット
	//
	// Parameter:
	// n - インデックス値
	void SetIndex(int n)		{index = n;}

	// Function: SetFlag
	// このハーフエッジのフラグをセット
	//
	// Parameter:
	// n - フラグ
	void SetFlag(int f)			{flag = f;}

public:

	// Variable: *vert
	// このハーフエッジの始点へのポインタ
	HEvert *vert;

	// Variable: *pair
	// 対となる逆方向ハーフエッジへのポインタ
	HEedge *pair;

	// Variable: *face
	// このハーフエッジに接している面へのポインタ
	HEface *face;

	// Variable: *ne
	// 面周りの次のハーフエッジへのポインタ
	HEedge *ne;		

	// Variable: index
	// インデックス
	int    index;

	// Variable: *mom
	// 自分が属する親のアドレス
	Data   *mom;	

	// Variable: qemP
	// QEM法用三次元座標値(Edge Collapse適用後の頂点位置)
	Coord  qemP;

	// Variable: qemD
	// QEM法用パラメータ(Edge Collapse適用のコスト)
	double qemD;

	// Variable: flag
	// 汎用フラグ
	int   flag;
};

// Class: HEface
// 面クラス
class HEface{
public:
	// Function: HEface
	// コンストラクタ
	HEface() {edge=NULL; index=-1; vertnum=0; mom = NULL;}

	// Function: GetStartHE
	// この面が持つハーフエッジの1つを返す
	//
	// Return:
	// edge
	HEedge	*GetStartHE()	{return edge;}

	// Function: GetNormVec
	// この面の法線ベクトルを返す
	//
	// Return:
	// norm
	Coord	GetNormVec()	{return norm;}	

	// Function: GetArea
	// この面の面積を返す
	//
	// Return:
	// area
	double	GetArea()		{return area;}	

	// Function: GetIndex
	// この面のインデックスを返す
	//
	// Return:
	// index
	int		GetIndex()		{return index;}	

	// Function: GetVetexNum
	// この面の持つ頂点数を返す
	//
	// Return:
	// vertnum
	int		GetVetexNum()	{return vertnum;}

	// Function: GetDispStat
	// この面の表示属性を返す
	//
	// Return:
	// Dstat
	DispStat GetDispStat()	{return Dstat;}		

	// Function: SetNormVec
	// この面の法線ベクトルをセット
	//
	// Parameter:
	// 
	void SetNormVec(Coord n)	{norm = n;}		

	// Function: SetFaceArea
	// この面の面積をセット
	//
	// Parameter:
	// a - 面積
	void SetFaceArea(double a)	{area = a;}		

	// Function: SetEdge
	// この面が持つハーフエッジの1つをセット
	//
	// Parameter:
	// *e - この面が持つハーフエッジの1つへのポインタ
	void SetEdge(HEedge *e)		{edge = e;}		

	// Function: SetVertNum
	// この面の持つ頂点数をセット
	//
	// Parameter:
	// n - 頂点数
	void SetVertNum(int n)		{vertnum = n;}	

	// Function: SetIndex
	// この面のインデックスをセット
	//
	// Parameter:
	// n - インデックス
	void SetIndex(int n)		{index = n;}	

public:
	// Variable: *edge
	// この面に接しているハーフエッジの最初
	HEedge *edge;	

	// Variable: norm
	// 法線ベクトル
	Coord  norm;

	// Variable: area
	// 面積
	double area;	

	// Variable: vertnum
	// 頂点数
	int    vertnum;	

	// Variable: index
	// インデックス
	int    index;	

	// Variable: Dstat
	// 表示属性（色r,g,b）
	DispStat Dstat;	

	// Variable: *mom
	// 自分が属する親のアドレス
	Data   *mom;	
};

// Class: MESH
// メッシュクラスの定義
class MESH
{
public:
	// Function: MESH
	// コンストラクタ
	MESH();

	// Function: clear
	// メッシュ全消去
	int clear();

	// Function: DelVert
	// 指定した頂点を消去
	void DelVert(HEvert *);	
	
	// Function: DelEdge
	// 指定したハーフエッジを消去
	void DelEdge(HEedge *);
	
	// Function:DelFace 
	// 指定した面を消去
	void DelFace(HEface *);
	
	// Function: getIndexedVert
	// 指定したインデックスと同じインデックスを持つ頂点を探し出す
	HEvert *getIndexedVert(int);
	
	// Function: getIndexedEdge
	// 指定したインデックスと同じインデックスを持つハーフエッジを探し出す
	HEedge *getIndexedEdge(int);
	
	// Function:getIndexedFace
	// 指定したインデックスと同じインデックスを持つ面を探し出す
	HEface *getIndexedFace(int);
	
	// Function: setSentinelOnIndexedEdge
	// 指定したインデックスのエッジデータに旗を立てる
	HEedge *setSentinelOnIndexedEdge(int);
	
	// Function: getIndexedEdgeFromSentinel
	// 旗からn番目後or前のエッジデータを得る
	HEedge *getIndexedEdgeFromSentinel(int);
	
	// Function: CalcFaceNorm
	// 指定した面fの法線ベクトルを求め、f->normにセットする
	int CalcFaceNorm(HEface *);
	
	// Function: CalcFaceArea
	// 指定した面fの面積を求め、f->areaにセットする
	int CalcFaceArea(HEface *);
	
	// Function: EdgeSortbyQEMD
	// EdgeをQEMコスト(Edge.qemD)で昇順ソートする
	void EdgeSortbyQEMD();
	
	// Function: InsertQEMD
	//	昇順ソートされたEdgeリストに、変更されたQEMコストを持つHEedgeを挿入
	void InsertQEMD(HEedge *);


private:
	// Function: MergeSortForQEMD
	// (private)EdgeをQEMコスト(Edge.qemD)で昇順ソートするSub1
	Data *MergeSortForQEMD(Data *);

	// Function: MergeListForQEMD
	// (private)EdgeをQEMコスト(Edge.qemD)で昇順ソートするSub2
	Data *MergeListForQEMD(Data *,Data *);


public:
	// Variable: 
	// 面リストのルート
	KODlistData Face;

	// Variable: Edge
	// ハーフエッジリストのルート
	KODlistData Edge;

	// Variable: Vert
	// 頂点リストのルート
	KODlistData Vert;

	// Variable: VertNum
	// 面の総数
	int VertNum;

	// Variable: EdgeNum
	// ハーフエッジの総数
	int EdgeNum;

	// Variable: FaceNum
	// 頂点の総数
	int FaceNum;
};

#endif