// Half-Edge�f�[�^�\���̒�`
// 2012/4���_�ł̃��b�V���f�[�^�̃��[��
// �O�p�p�b�`���l�p�p�b�`�Ƃ���D
// �O�p�p�b�`�Ǝl�p�p�b�`�̍��݂͋����Ȃ��D

#ifndef _HALF_EDGE_H_
#define _HALF_EDGE_H_

//#include "StdAfx.h"
#include "KodListFunc.h"

// Constants: General Defines
// MAXVERTNUMINFACE -	1�̃��b�V���������_���̏��(�l�p�`�܂�)(4)
// TRIMESHVERTNUM -		�O�p���b�V���̒��_��(3)
// QUADMESHVERTNUM -	�l�p�`���b�V���̒��_��(4)
#define MAXVERTNUMINFACE	4
#define TRIMESHVERTNUM		3
#define QUADMESHVERTNUM		4

class HEedge;
class HEvert;
class HEface;

// Class: HEvert
// ���_�N���X
class HEvert{
public:
	// Constructor: HEvert
	// �R���X�g���N�^
	HEvert() {cod=InitCoord(); edge=NULL; index=-1; mom = NULL; qemQ=NULL; NewQEM(); flag = false;}

	// Function: GetCoord
	// ���̒��_�̍��W�l��Ԃ�
	//
	// Return:
	// cod
	Coord	GetCoord()	{return cod;}

	// Function: GetHE
	// ���̒��_����L�т�n�[�t�G�b�W��1��Ԃ�
	//
	// Return:
	// edge
	HEedge	*GetHE()	{return edge;}

	// Function: GetIndex
	// ���̒��_�̃C���f�b�N�X��Ԃ�
	//
	// Return:
	// index
	int		GetIndex()	{return index;}

	// Function: GetFlag
	// �t���O��Ԃ�
	//
	// Return:
	// flag
	int    GetFlag()	{return flag;}			

	// Function: NewQEM
	// QEM�@�p�W���s��qemQ�̃������m��(4x4)
	void NewQEM();

	// Function: DelQEM
	// QEM�@�p�W���s��qemQ�̃��������
	void DelQEM();

	// Function: InitQEM
	// QEM�@�p�W���s��qemQ�̏�����
	void InitQEM();

	// Function: SetQEM
	// QEM�@�p�W���s��qemQ��qem�p�����[�^���Z�b�g����
	int SetQEM(Matrix);

	// Function: AddQEM
	// QEM�@�p�W���s��qemQ��qem�p�����[�^��������
	int AddQEM(Matrix);	

	// Function: GetQEM
	// QEM�@�p�W���s��qemQ��qem�p�����[�^�𓾂�
	double **GetQEM();

	// Function: SetEdge
	// ���̒��_����L�т�n�[�t�G�b�W���Z�b�g
	//
	// Parameter:
	// *e - �n�[�t�G�b�W�ւ̃|�C���^
	void SetEdge(HEedge *e)	{edge = e;}

	// Function: SetCod
	// ���̒��_�̍��W�l���Z�b�g
	//
	// Parameter:
	// c - ���W�l
	void SetCod(Coord c)	{cod = c;}

	// Function: SetIndex
	// ���̒��_�̃C���f�b�N�X���Z�b�g
	//
	// Parameter:
	// n - �C���f�b�N�X�l
	void SetIndex(int n)	{index = n;}

	// Function: SetFlag
	// �t���O���Z�b�g
	// 
	// Parameter:
	// f - �t���O
	void SetFlag(int f)	{flag = f;}			


public:
	// Variable: cod
	// ���̒��_�̎O�������W
	Coord  cod;		

	// Variable: *edge
	// ���̒��_����L�т�n�[�t�G�b�W�ւ̃|�C���^(���F���_����L�т�HE��1�{�Ƃ͌���Ȃ��B�ǂ�HE�ƌq�����Ă��邩������Ȃ�)
	HEedge *edge;

	// Variable: index
	// �C���f�b�N�X
	int    index;

	// Variable: *mom
	// ������������e(Data�N���X)�̃A�h���X
	Data   *mom;

	// Variable: **qemQ
	// QEM�@�p�W���s��
	double **qemQ;	

	// Variable: flag
	// �ėp�t���O
	int   flag;		
};

// Class: HEedge
// �n�[�t�G�b�W�N���X
class HEedge{
public:
	// Fucntion: HEedge
	// �R���X�g���N�^
	HEedge() {vert=NULL; pair=NULL; face=NULL; ne=NULL; index=-1; mom = NULL; InitQEMP(); InitQEMD();}

	// Function: GetStartVert
	// ���̃n�[�t�G�b�W�̎n�_��Ԃ�
	//
	// Return:
	// vert
	HEvert	*GetStartVert()	{return vert;}

	// Function: GetStartVcoord
	// ���̃n�[�t�G�b�W�̎n�_���W��Ԃ�
	//
	// Return:
	// vert->cod
	Coord	GetStartVcoord()	{return vert->cod;}

	// Function: GetPairHE
	// ���̃n�[�t�G�b�W�̑΂ƂȂ�n�[�t�G�b�W��Ԃ�
	//
	// Return:
	// pair
	HEedge	*GetPairHE()	{return pair;}

	// Function: GetFace
	// ���̃n�[�t�G�b�W��������ʂ�Ԃ�
	//
	// Return:
	// face
	HEface	*GetFace()	{return face;}

	// Function: GetNextHE
	// ���̃n�[�t�G�b�W�̎��̃n�[�t�G�b�W��Ԃ�
	//
	// Return:
	// ne
	HEedge	*GetNextHE()	{return ne;}

	// Function: GetNextHE
	// ���̃n�[�t�G�b�W��(���L����ʂɂ�����)1�O�̃n�[�t�G�b�W��Ԃ�
	HEedge  *GetPrevHE();

	// Function: GetIndex
	// ���̃n�[�t�G�b�W�̃C���f�b�N�X��Ԃ�
	//
	// Return:
	// index
	int	GetIndex()	{return index;}

	// Function: GetStartVindex
	// ���̃n�[�t�G�b�W�̎n�_�̃C���f�b�N�X��Ԃ�
	//
	// Return:
	// vert->index
	int	GetStartVindex()	{return vert->index;}

	// Function: GetStartVindex
	// �t���O��Ԃ�
	//
	// Return:
	// vert->index
	int	GetFlag()	{return flag;}			
	
	// Function: InitQEMP
	// QEM�@�p�O�������W�l(Edge Collapse�K�p��̒��_�ʒu)qemP�̏�����
	void InitQEMP()	{qemP = InitCoord();}

	// Function: InitQEMD
	// QEM�@�p�p�����[�^(Edge Collapse�K�p�̃R�X�g)qemD�̏����� 
	void InitQEMD()	{qemD = 0;}

	// Function: SetQEMP
	// QEM�@�p�O�������W�l(Edge Collapse�K�p��̒��_�ʒu)qemP���Z�b�g
	// 
	// Parameter:
	// p - ���W�l
	void SetQEMP(Coord p)	{qemP = p;}

	// Function: SetQEMD
	// QEM�@�p�p�����[�^(Edge Collapse�K�p�̃R�X�g)qemD���Z�b�g
	// 
	// Parameter:
	// d - �K�p�R�X�g�p�����[�^
	void SetQEMD(double d)	{qemD = d;}

	// Function: GetQEMP
	// QEM�@�p�O�������W�l(Edge Collapse�K�p��̒��_�ʒu)qemP��Ԃ�
	//
	// Return
	// qemP
	Coord GetQEMP()	{return qemP;}

	// Function: GetQEMD
	// QEM�@�p�p�����[�^(Edge Collapse�K�p�̃R�X�g)qemD��Ԃ�
	// 
	// Return
	// qemD
	double GetQEMD()	{return qemD;}

	// Function: SetVert
	// ���̃n�[�t�G�b�W�̎n�_���Z�b�g
	//
	// Parameter:
	// *v - ���̃n�[�t�G�b�W�̎n�_�ƂȂ�HEvert�ւ̃|�C���^
	void SetVert(HEvert *v)		{vert = v;}

	// Function: SetPair
	// ���̃n�[�t�G�b�W�̑΂ƂȂ�n�[�t�G�b�W���Z�b�g
	//
	// Parameter:
	// *e - ���̃n�[�t�G�b�W�̑΂ƂȂ�HEedge�ւ̃|�C���^
	void SetPair(HEedge *e)		{pair = e;}

	// Function: SetFace
	// ���̃n�[�t�G�b�W��������ʂ��Z�b�g
	//
	// Parameter:
	// *f - ���̃n�[�t�G�b�W���������HEface�ւ̃|�C���^
	void SetFace(HEface *f)		{face = f;}

	// Function: SetNextHE
	// ���̃n�[�t�G�b�W�̎��̃n�[�t�G�b�W���Z�b�g
	//
	// Parameter:
	// *e - ���̃n�[�t�G�b�W�̎���HEedge�ւ̃|�C���^
	void SetNextHE(HEedge *e)	{ne = e;}

	// Function: SetIndex
	// ���̃n�[�t�G�b�W�̃C���f�b�N�X���Z�b�g
	//
	// Parameter:
	// n - �C���f�b�N�X�l
	void SetIndex(int n)		{index = n;}

	// Function: SetFlag
	// ���̃n�[�t�G�b�W�̃t���O���Z�b�g
	//
	// Parameter:
	// n - �t���O
	void SetFlag(int f)			{flag = f;}

public:

	// Variable: *vert
	// ���̃n�[�t�G�b�W�̎n�_�ւ̃|�C���^
	HEvert *vert;

	// Variable: *pair
	// �΂ƂȂ�t�����n�[�t�G�b�W�ւ̃|�C���^
	HEedge *pair;

	// Variable: *face
	// ���̃n�[�t�G�b�W�ɐڂ��Ă���ʂւ̃|�C���^
	HEface *face;

	// Variable: *ne
	// �ʎ���̎��̃n�[�t�G�b�W�ւ̃|�C���^
	HEedge *ne;		

	// Variable: index
	// �C���f�b�N�X
	int    index;

	// Variable: *mom
	// ������������e�̃A�h���X
	Data   *mom;	

	// Variable: qemP
	// QEM�@�p�O�������W�l(Edge Collapse�K�p��̒��_�ʒu)
	Coord  qemP;

	// Variable: qemD
	// QEM�@�p�p�����[�^(Edge Collapse�K�p�̃R�X�g)
	double qemD;

	// Variable: flag
	// �ėp�t���O
	int   flag;
};

// Class: HEface
// �ʃN���X
class HEface{
public:
	// Function: HEface
	// �R���X�g���N�^
	HEface() {edge=NULL; index=-1; vertnum=0; mom = NULL;}

	// Function: GetStartHE
	// ���̖ʂ����n�[�t�G�b�W��1��Ԃ�
	//
	// Return:
	// edge
	HEedge	*GetStartHE()	{return edge;}

	// Function: GetNormVec
	// ���̖ʂ̖@���x�N�g����Ԃ�
	//
	// Return:
	// norm
	Coord	GetNormVec()	{return norm;}	

	// Function: GetArea
	// ���̖ʂ̖ʐς�Ԃ�
	//
	// Return:
	// area
	double	GetArea()		{return area;}	

	// Function: GetIndex
	// ���̖ʂ̃C���f�b�N�X��Ԃ�
	//
	// Return:
	// index
	int		GetIndex()		{return index;}	

	// Function: GetVetexNum
	// ���̖ʂ̎����_����Ԃ�
	//
	// Return:
	// vertnum
	int		GetVetexNum()	{return vertnum;}

	// Function: GetDispStat
	// ���̖ʂ̕\��������Ԃ�
	//
	// Return:
	// Dstat
	DispStat GetDispStat()	{return Dstat;}		

	// Function: SetNormVec
	// ���̖ʂ̖@���x�N�g�����Z�b�g
	//
	// Parameter:
	// 
	void SetNormVec(Coord n)	{norm = n;}		

	// Function: SetFaceArea
	// ���̖ʂ̖ʐς��Z�b�g
	//
	// Parameter:
	// a - �ʐ�
	void SetFaceArea(double a)	{area = a;}		

	// Function: SetEdge
	// ���̖ʂ����n�[�t�G�b�W��1���Z�b�g
	//
	// Parameter:
	// *e - ���̖ʂ����n�[�t�G�b�W��1�ւ̃|�C���^
	void SetEdge(HEedge *e)		{edge = e;}		

	// Function: SetVertNum
	// ���̖ʂ̎����_�����Z�b�g
	//
	// Parameter:
	// n - ���_��
	void SetVertNum(int n)		{vertnum = n;}	

	// Function: SetIndex
	// ���̖ʂ̃C���f�b�N�X���Z�b�g
	//
	// Parameter:
	// n - �C���f�b�N�X
	void SetIndex(int n)		{index = n;}	

public:
	// Variable: *edge
	// ���̖ʂɐڂ��Ă���n�[�t�G�b�W�̍ŏ�
	HEedge *edge;	

	// Variable: norm
	// �@���x�N�g��
	Coord  norm;

	// Variable: area
	// �ʐ�
	double area;	

	// Variable: vertnum
	// ���_��
	int    vertnum;	

	// Variable: index
	// �C���f�b�N�X
	int    index;	

	// Variable: Dstat
	// �\�������i�Fr,g,b�j
	DispStat Dstat;	

	// Variable: *mom
	// ������������e�̃A�h���X
	Data   *mom;	
};

// Class: MESH
// ���b�V���N���X�̒�`
class MESH
{
public:
	// Function: MESH
	// �R���X�g���N�^
	MESH();

	// Function: clear
	// ���b�V���S����
	int clear();

	// Function: DelVert
	// �w�肵�����_������
	void DelVert(HEvert *);	
	
	// Function: DelEdge
	// �w�肵���n�[�t�G�b�W������
	void DelEdge(HEedge *);
	
	// Function:DelFace 
	// �w�肵���ʂ�����
	void DelFace(HEface *);
	
	// Function: getIndexedVert
	// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�������_��T���o��
	HEvert *getIndexedVert(int);
	
	// Function: getIndexedEdge
	// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�����n�[�t�G�b�W��T���o��
	HEedge *getIndexedEdge(int);
	
	// Function:getIndexedFace
	// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�����ʂ�T���o��
	HEface *getIndexedFace(int);
	
	// Function: setSentinelOnIndexedEdge
	// �w�肵���C���f�b�N�X�̃G�b�W�f�[�^�Ɋ��𗧂Ă�
	HEedge *setSentinelOnIndexedEdge(int);
	
	// Function: getIndexedEdgeFromSentinel
	// ������n�Ԗڌ�or�O�̃G�b�W�f�[�^�𓾂�
	HEedge *getIndexedEdgeFromSentinel(int);
	
	// Function: CalcFaceNorm
	// �w�肵����f�̖@���x�N�g�������߁Af->norm�ɃZ�b�g����
	int CalcFaceNorm(HEface *);
	
	// Function: CalcFaceArea
	// �w�肵����f�̖ʐς����߁Af->area�ɃZ�b�g����
	int CalcFaceArea(HEface *);
	
	// Function: EdgeSortbyQEMD
	// Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����
	void EdgeSortbyQEMD();
	
	// Function: InsertQEMD
	//	�����\�[�g���ꂽEdge���X�g�ɁA�ύX���ꂽQEM�R�X�g������HEedge��}��
	void InsertQEMD(HEedge *);


private:
	// Function: MergeSortForQEMD
	// (private)Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����Sub1
	Data *MergeSortForQEMD(Data *);

	// Function: MergeListForQEMD
	// (private)Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����Sub2
	Data *MergeListForQEMD(Data *,Data *);


public:
	// Variable: 
	// �ʃ��X�g�̃��[�g
	KODlistData Face;

	// Variable: Edge
	// �n�[�t�G�b�W���X�g�̃��[�g
	KODlistData Edge;

	// Variable: Vert
	// ���_���X�g�̃��[�g
	KODlistData Vert;

	// Variable: VertNum
	// �ʂ̑���
	int VertNum;

	// Variable: EdgeNum
	// �n�[�t�G�b�W�̑���
	int EdgeNum;

	// Variable: FaceNum
	// ���_�̑���
	int FaceNum;
};

#endif