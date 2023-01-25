// Half-Edge�f�[�^�\���̒�`
// 2012/4���_�ł̃��b�V���f�[�^�̃��[��
// �O�p�p�b�`���l�p�p�b�`�Ƃ���D
// �O�p�p�b�`�Ǝl�p�p�b�`�̍��݂͋����Ȃ��D

#ifndef _HALF_EDGE_H_
#define _HALF_EDGE_H_

#include "StdAfx.h"
#include "KodListFunc.h"

#define MAXVERTNUMINFACE	4		// 1�̃��b�V���������_���̏��(�l�p�`�܂�)
#define TRIMESHVERTNUM		3		// �O�p���b�V���̒��_��
#define QUADMESHVERTNUM		4		// �l�p�`���b�V���̒��_��

class HEedge;
class HEvert;
class HEface;

// ���_�N���X�̒�`
class HEvert{
public:
	HEvert() {cod=InitCoord(); edge=NULL; index=-1; mom = NULL; qemQ=NULL; NewQEM(); flag = false;}	// �R���X�g���N�^
	Coord	GetCoord()	{return cod;}			// ���̒��_�̍��W�l��Ԃ�
	HEedge	*GetHE()	{return edge;}			// ���̒��_����L�т�n�[�t�G�b�W��1��Ԃ�
	int		GetIndex()	{return index;}			// ���̒��_�̃C���f�b�N�X��Ԃ�
	int    GetFlag()	{return flag;}			// �t���O��Ԃ�

	void NewQEM();				// qemQ�̃������m��(4x4)
	void DelQEM();				// qemQ�̃��������
	void InitQEM();				// qemQ�̏�����
	int SetQEM(Matrix);			// qem�p�����[�^���Z�b�g����
	int AddQEM(Matrix);			// qem�p�����[�^��������
	double **GetQEM();			// qem�p�����[�^�𓾂�

public:
	void SetEdge(HEedge *e)	{edge = e;}			// ���̒��_����L�т�n�[�t�G�b�W���Z�b�g
	void SetCod(Coord c)	{cod = c;}			// ���̒��_�̍��W�l���Z�b�g
	void SetIndex(int n)	{index = n;}		// ���̒��_�̃C���f�b�N�X���Z�b�g
	void SetFlag(int f)		{flag = f;}			// �t���O���Z�b�g


public:
	Coord  cod;		// ���̒��_�̎O�������W
	HEedge *edge;	// ���̒��_����L�т�n�[�t�G�b�W�ւ̃|�C���^(���F���_����L�т�HE��1�{�Ƃ͌���Ȃ��B�ǂ�HE�ƌq�����Ă��邩������Ȃ�)
	int    index;	// �C���f�b�N�X
	Data   *mom;	// ������������e�̃A�h���X
	double **qemQ;	// QEM�@�p�W���s��
	int   flag;		// �ėp�t���O
};

// �n�[�t�G�b�W�N���X�̒�`
class HEedge{
public:
	HEedge() {vert=NULL; pair=NULL; face=NULL; ne=NULL; index=-1; mom = NULL; InitQEMP(); InitQEMD();}	// �R���X�g���N�^
	HEvert	*GetStartVert()		{return vert;}			// ���̃n�[�t�G�b�W�̎n�_��Ԃ�
	Coord	GetStartVcoord()	{return vert->cod;}		// ���̃n�[�t�G�b�W�̎n�_���W��Ԃ�
	HEedge	*GetPairHE()		{return pair;}			// ���̃n�[�t�G�b�W�̑΂ƂȂ�n�[�t�G�b�W��Ԃ�
	HEface	*GetFace()	{return face;}					// ���̃n�[�t�G�b�W��������ʂ�Ԃ�
	HEedge	*GetNextHE()		{return ne;}			// ���̃n�[�t�G�b�W�̎��̃n�[�t�G�b�W��Ԃ�
	HEedge  *GetPrevHE();								// ���̃n�[�t�G�b�W��(���L����ʂɂ�����)1�O�̃n�[�t�G�b�W��Ԃ�
	int		GetIndex()			{return index;}			// ���̃n�[�t�G�b�W�̃C���f�b�N�X��Ԃ�
	int		GetStartVindex()	{return vert->index;}	// ���̃n�[�t�G�b�W�̎n�_�̃C���f�b�N�X��Ԃ�
	int		GetFlag()			{return flag;}			// �t���O��Ԃ�
	
	void InitQEMP()				{qemP = InitCoord();}	// qemP�̏�����
	void InitQEMD()				{qemD = 0;}				// qemD�̏�����
	void SetQEMP(Coord p)		{qemP = p;}				// qemP���Z�b�g
	void SetQEMD(double d)		{qemD = d;}				// qemD���Z�b�g
	Coord GetQEMP()				{return qemP;}			// qemP��Ԃ�
	double GetQEMD()			{return qemD;}			// qemD��Ԃ�

public:
	void SetVert(HEvert *v)		{vert = v;}				// ���̃n�[�t�G�b�W�̎n�_���Z�b�g
	void SetPair(HEedge *e)		{pair = e;}				// ���̃n�[�t�G�b�W�̑΂ƂȂ�n�[�t�G�b�W���Z�b�g
	void SetFace(HEface *f)		{face = f;}				// ���̃n�[�t�G�b�W��������ʂ��Z�b�g
	void SetNextHE(HEedge *e)	{ne = e;}				// ���̃n�[�t�G�b�W�̎��̃n�[�t�G�b�W���Z�b�g
	void SetIndex(int n)		{index = n;}			// ���̃n�[�t�G�b�W�̃C���f�b�N�X���Z�b�g
	void SetFlag(int f)			{flag = f;}				// �t���O���Z�b�g

public:
	HEvert *vert;	// ���̃n�[�t�G�b�W�̎n�_�ւ̃|�C���^
	HEedge *pair;	// �΂ƂȂ�t�����n�[�t�G�b�W�ւ̃|�C���^
	HEface *face;	// ���̃n�[�t�G�b�W�ɐڂ��Ă���ʂւ̃|�C���^
	HEedge *ne;		// �ʎ���̎��̃n�[�t�G�b�W�ւ̃|�C���^
	int    index;	// �C���f�b�N�X
	Data   *mom;	// ������������e�̃A�h���X

	Coord  qemP;		// QEM�@�p(Edge Collapse�K�p��̒��_�ʒu)
	double qemD;		// QEM�@�p(Edge Collapse�K�p�̃R�X�g)
	int   flag;			// �ėp�t���O
};

// �ʃN���X�̒�`
class HEface{
public:
	HEface() {edge=NULL; index=-1; vertnum=0; mom = NULL;}	// �R���X�g���N�^
	HEedge	*GetStartHE()	{return edge;}		// ���̖ʂ����n�[�t�G�b�W��1��Ԃ�
	Coord	GetNormVec()	{return norm;}		// ���̖ʂ̖@���x�N�g����Ԃ�
	double	GetArea()		{return area;}		// ���̖ʂ̖ʐς�Ԃ�
	int		GetIndex()		{return index;}		// ���̖ʂ̃C���f�b�N�X��Ԃ�
	int		GetVetexNum()	{return vertnum;}	// ���̖ʂ̎����_����Ԃ�
	DispStat GetDispStat()	{return Dstat;}		// ���̖ʂ̕\��������Ԃ�

public:
	void SetNormVec(Coord n)	{norm = n;}		// ���̖ʂ̖@���x�N�g�����Z�b�g
	void SetFaceArea(double a)	{area = a;}		// ���̖ʂ̖ʐς��Z�b�g
	void SetEdge(HEedge *e)		{edge = e;}		// ���̖ʂ����n�[�t�G�b�W��1���Z�b�g
	void SetVertNum(int n)		{vertnum = n;}	// ���̖ʂ̎����_�����Z�b�g
	void SetIndex(int n)		{index = n;}	// ���̖ʂ̃C���f�b�N�X���Z�b�g

public:
	HEedge *edge;	// ���̖ʂɐڂ��Ă���n�[�t�G�b�W�̍ŏ�
	Coord  norm;	// �@���x�N�g��
	double area;	// �ʐ�
	int    vertnum;	// ���_��
	int    index;	// �C���f�b�N�X
	DispStat Dstat;	// �\�������i�Fr,g,b�j
	Data   *mom;	// ������������e�̃A�h���X
};

// ���b�V���N���X�̒�`
class MESH
{
public:
	MESH();										// �R���X�g���N�^
	int clear();								// ���b�V���S����
	void DelVert(HEvert *);						// �w�肵�����_������
	void DelEdge(HEedge *);						// �w�肵���n�[�t�G�b�W������
	void DelFace(HEface *);						// �w�肵���ʂ�����
	HEvert *getIndexedVert(int);				// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�������_��T���o��
	HEedge *getIndexedEdge(int);				// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�����n�[�t�G�b�W��T���o��
	HEface *getIndexedFace(int);				// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�����ʂ�T���o��
	HEedge *setSentinelOnIndexedEdge(int);		// �w�肵���C���f�b�N�X�̃G�b�W�f�[�^�Ɋ��𗧂Ă�
	HEedge *getIndexedEdgeFromSentinel(int);	// ������n�Ԗڌ�or�O�̃G�b�W�f�[�^�𓾂�
	int CalcFaceNorm(HEface *);					// �w�肵����f�̖@���x�N�g�������߁Af->norm�ɃZ�b�g����
	int CalcFaceArea(HEface *);					// �w�肵����f�̖ʐς����߁Af->area�ɃZ�b�g����
	void EdgeSortbyQEMD();						// Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����
	void InsertQEMD(HEedge *);					//	�����\�[�g���ꂽEdge���X�g�ɁA�ύX���ꂽQEM�R�X�g������HEedge��}��


private:
	Data *MergeSortForQEMD(Data *);				// Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����Sub1
	Data *MergeListForQEMD(Data *,Data *);		// Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����Sub2


public:
	KODlistData Face;		// �ʃ��X�g�̃��[�g
	KODlistData Edge;		// �n�[�t�G�b�W���X�g�̃��[�g
	KODlistData Vert;		// ���_���X�g�̃��[�g
	int VertNum;			// �ʂ̑���
	int EdgeNum;			// �n�[�t�G�b�W�̑���
	int FaceNum;			// ���_�̑���
};

#endif