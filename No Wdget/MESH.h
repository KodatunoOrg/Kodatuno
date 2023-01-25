// Half-Edgeデータ構造の定義
// 2012/4時点でのメッシュデータのルール
// 三角パッチか四角パッチとする．
// 三角パッチと四角パッチの混在は許さない．

#ifndef _HALF_EDGE_H_
#define _HALF_EDGE_H_

#include "StdAfx.h"
#include "KodListFunc.h"

#define MAXVERTNUMINFACE	4		// 1つのメッシュが持つ頂点数の上限(四角形まで)
#define TRIMESHVERTNUM		3		// 三角メッシュの頂点数
#define QUADMESHVERTNUM		4		// 四角形メッシュの頂点数

class HEedge;
class HEvert;
class HEface;

// 頂点クラスの定義
class HEvert{
public:
	HEvert() {cod=InitCoord(); edge=NULL; index=-1; mom = NULL; qemQ=NULL; NewQEM(); flag = false;}	// コンストラクタ
	Coord	GetCoord()	{return cod;}			// この頂点の座標値を返す
	HEedge	*GetHE()	{return edge;}			// この頂点から伸びるハーフエッジの1つを返す
	int		GetIndex()	{return index;}			// この頂点のインデックスを返す
	int    GetFlag()	{return flag;}			// フラグを返す

	void NewQEM();				// qemQのメモリ確保(4x4)
	void DelQEM();				// qemQのメモリ解放
	void InitQEM();				// qemQの初期化
	int SetQEM(Matrix);			// qemパラメータをセットする
	int AddQEM(Matrix);			// qemパラメータを加える
	double **GetQEM();			// qemパラメータを得る

public:
	void SetEdge(HEedge *e)	{edge = e;}			// この頂点から伸びるハーフエッジをセット
	void SetCod(Coord c)	{cod = c;}			// この頂点の座標値をセット
	void SetIndex(int n)	{index = n;}		// この頂点のインデックスをセット
	void SetFlag(int f)		{flag = f;}			// フラグをセット


public:
	Coord  cod;		// この頂点の三次元座標
	HEedge *edge;	// この頂点から伸びるハーフエッジへのポインタ(注：頂点から伸びるHEは1本とは限らない。どのHEと繋がっているか分からない)
	int    index;	// インデックス
	Data   *mom;	// 自分が属する親のアドレス
	double **qemQ;	// QEM法用係数行列
	int   flag;		// 汎用フラグ
};

// ハーフエッジクラスの定義
class HEedge{
public:
	HEedge() {vert=NULL; pair=NULL; face=NULL; ne=NULL; index=-1; mom = NULL; InitQEMP(); InitQEMD();}	// コンストラクタ
	HEvert	*GetStartVert()		{return vert;}			// このハーフエッジの始点を返す
	Coord	GetStartVcoord()	{return vert->cod;}		// このハーフエッジの始点座標を返す
	HEedge	*GetPairHE()		{return pair;}			// このハーフエッジの対となるハーフエッジを返す
	HEface	*GetFace()	{return face;}					// このハーフエッジが属する面を返す
	HEedge	*GetNextHE()		{return ne;}			// このハーフエッジの次のハーフエッジを返す
	HEedge  *GetPrevHE();								// このハーフエッジの(共有する面における)1つ前のハーフエッジを返す
	int		GetIndex()			{return index;}			// このハーフエッジのインデックスを返す
	int		GetStartVindex()	{return vert->index;}	// このハーフエッジの始点のインデックスを返す
	int		GetFlag()			{return flag;}			// フラグを返す
	
	void InitQEMP()				{qemP = InitCoord();}	// qemPの初期化
	void InitQEMD()				{qemD = 0;}				// qemDの初期化
	void SetQEMP(Coord p)		{qemP = p;}				// qemPをセット
	void SetQEMD(double d)		{qemD = d;}				// qemDをセット
	Coord GetQEMP()				{return qemP;}			// qemPを返す
	double GetQEMD()			{return qemD;}			// qemDを返す

public:
	void SetVert(HEvert *v)		{vert = v;}				// このハーフエッジの始点をセット
	void SetPair(HEedge *e)		{pair = e;}				// このハーフエッジの対となるハーフエッジをセット
	void SetFace(HEface *f)		{face = f;}				// このハーフエッジが属する面をセット
	void SetNextHE(HEedge *e)	{ne = e;}				// このハーフエッジの次のハーフエッジをセット
	void SetIndex(int n)		{index = n;}			// このハーフエッジのインデックスをセット
	void SetFlag(int f)			{flag = f;}				// フラグをセット

public:
	HEvert *vert;	// このハーフエッジの始点へのポインタ
	HEedge *pair;	// 対となる逆方向ハーフエッジへのポインタ
	HEface *face;	// このハーフエッジに接している面へのポインタ
	HEedge *ne;		// 面周りの次のハーフエッジへのポインタ
	int    index;	// インデックス
	Data   *mom;	// 自分が属する親のアドレス

	Coord  qemP;		// QEM法用(Edge Collapse適用後の頂点位置)
	double qemD;		// QEM法用(Edge Collapse適用のコスト)
	int   flag;			// 汎用フラグ
};

// 面クラスの定義
class HEface{
public:
	HEface() {edge=NULL; index=-1; vertnum=0; mom = NULL;}	// コンストラクタ
	HEedge	*GetStartHE()	{return edge;}		// この面が持つハーフエッジの1つを返す
	Coord	GetNormVec()	{return norm;}		// この面の法線ベクトルを返す
	double	GetArea()		{return area;}		// この面の面積を返す
	int		GetIndex()		{return index;}		// この面のインデックスを返す
	int		GetVetexNum()	{return vertnum;}	// この面の持つ頂点数を返す
	DispStat GetDispStat()	{return Dstat;}		// この面の表示属性を返す

public:
	void SetNormVec(Coord n)	{norm = n;}		// この面の法線ベクトルをセット
	void SetFaceArea(double a)	{area = a;}		// この面の面積をセット
	void SetEdge(HEedge *e)		{edge = e;}		// この面が持つハーフエッジの1つをセット
	void SetVertNum(int n)		{vertnum = n;}	// この面の持つ頂点数をセット
	void SetIndex(int n)		{index = n;}	// この面のインデックスをセット

public:
	HEedge *edge;	// この面に接しているハーフエッジの最初
	Coord  norm;	// 法線ベクトル
	double area;	// 面積
	int    vertnum;	// 頂点数
	int    index;	// インデックス
	DispStat Dstat;	// 表示属性（色r,g,b）
	Data   *mom;	// 自分が属する親のアドレス
};

// メッシュクラスの定義
class MESH
{
public:
	MESH();										// コンストラクタ
	int clear();								// メッシュ全消去
	void DelVert(HEvert *);						// 指定した頂点を消去
	void DelEdge(HEedge *);						// 指定したハーフエッジを消去
	void DelFace(HEface *);						// 指定した面を消去
	HEvert *getIndexedVert(int);				// 指定したインデックスと同じインデックスを持つ頂点を探し出す
	HEedge *getIndexedEdge(int);				// 指定したインデックスと同じインデックスを持つハーフエッジを探し出す
	HEface *getIndexedFace(int);				// 指定したインデックスと同じインデックスを持つ面を探し出す
	HEedge *setSentinelOnIndexedEdge(int);		// 指定したインデックスのエッジデータに旗を立てる
	HEedge *getIndexedEdgeFromSentinel(int);	// 旗からn番目後or前のエッジデータを得る
	int CalcFaceNorm(HEface *);					// 指定した面fの法線ベクトルを求め、f->normにセットする
	int CalcFaceArea(HEface *);					// 指定した面fの面積を求め、f->areaにセットする
	void EdgeSortbyQEMD();						// EdgeをQEMコスト(Edge.qemD)で昇順ソートする
	void InsertQEMD(HEedge *);					//	昇順ソートされたEdgeリストに、変更されたQEMコストを持つHEedgeを挿入


private:
	Data *MergeSortForQEMD(Data *);				// EdgeをQEMコスト(Edge.qemD)で昇順ソートするSub1
	Data *MergeListForQEMD(Data *,Data *);		// EdgeをQEMコスト(Edge.qemD)で昇順ソートするSub2


public:
	KODlistData Face;		// 面リストのルート
	KODlistData Edge;		// ハーフエッジリストのルート
	KODlistData Vert;		// 頂点リストのルート
	int VertNum;			// 面の総数
	int EdgeNum;			// ハーフエッジの総数
	int FaceNum;			// 頂点の総数
};

#endif