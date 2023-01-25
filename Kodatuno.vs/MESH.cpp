#include "stdafx.h"
#include "MESH.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

//////////////////////////////////////MESH class////////////////////////////////////////////////////////////////////////

// Function: MESH
// �R���X�g���N�^
MESH::MESH()
{
	VertNum = EdgeNum = FaceNum = 0;
}

// Function: clear
// ���b�V���̑S�N���A
//
// Return:
// KOD_TRUE
int MESH::clear()
{
	int fnum = Face.getNum();
	int vnum = Vert.getNum();
	int ednum = Edge.getNum();

	for(int i=0;i<fnum;i++){
		HEface *f = (HEface *)Face.getData(i);
		delete f;
	}

	for(int i=0;i<vnum;i++){
		HEvert *v = (HEvert *)Vert.getData(i);
		v->DelQEM();
		delete v;
	}
	
	for(int i=0;i<ednum;i++){
		HEedge *e = (HEedge *)Edge.getData(i);
		delete e;
	}

	Face.clear();
	Vert.clear();
	Edge.clear();

	VertNum = EdgeNum = FaceNum = 0;

	return KOD_TRUE;
}

// Function: DelVert
// �w�肵�����_������
//
// Parameter:
// *v - �������钸�_�ւ̃|�C���^
void MESH::DelVert(HEvert *v)
{
	Vert.delData(v->mom);	// ���X�g����O��
	//fprintf(stderr,"del v:%p\n",v->mom);		// debug
	v->DelQEM();
	VertNum--;
	delete v;				// ���g�̏���
}

// Function: DelEdge
// �w�肵���n�[�t�G�b�W������
//
// Parameter:
// *e - ��������n�[�t�G�b�W�ւ̃|�C���^
void MESH::DelEdge(HEedge *e)
{
	Edge.delData(e->mom);	// ���X�g����O��
	// fprintf(stderr,"del e:%p\n",e->mom);		// debug
	EdgeNum--;
	delete e;				// ���g�̏���
}

// Function: DelFace
// �w�肵���ʂ�����
//
// Parameter:
// *f - ��������ʂւ̃|�C���^
void MESH::DelFace(HEface *f)
{
	Face.delData(f->mom);	// ���X�g����O��
	//fprintf(stderr,"del f:%p\n",f->mom);		// debug
	FaceNum--;
	delete f;				// ���g�̏���
}

// Function: getIndexedVert
// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�������_��T���o��
//
// Parameter:
// index - �C���f�b�N�X�ԍ�
//
// Return:
// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�������_�ւ̃|�C���^�D������Ȃ�������NULL
HEvert *MESH::getIndexedVert(int index)
{
	int n = Vert.getNum();

	for(int i=0;i<n;i++){
		HEvert *v = (HEvert *)Vert.getData(i);
		if(v->index == index){
			return v;
		}
	}

	return NULL;
}

// Function: getIndexedEdge
// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�����G�b�W��T���o��
//
// Parameter:
// index - �C���f�b�N�X�ԍ�
//
// Return:
// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�����G�b�W�ւ̃|�C���^�D������Ȃ�������NULL
HEedge *MESH::getIndexedEdge(int index)
{
	int n = Edge.getNum();

	for(int i=0;i<n;i++){
		HEedge *e = (HEedge *)Edge.getData(i);
		if(e->index == index){
			return e;
		}
	}

	return NULL;
}

// Function: getIndexedFace
// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�����ʂ�T���o��
//
// Parameter:
// index - �C���f�b�N�X�ԍ�
//
// Return:
// �w�肵���C���f�b�N�X�Ɠ����C���f�b�N�X�����ʂւ̃|�C���^�D������Ȃ�������NULL
HEface *MESH::getIndexedFace(int index)
{
	int n = Face.getNum();

	for(int i=0;i<n;i++){
		HEface *f = (HEface *)Face.getData(i);
		if(f->index == index){
			return f;
		}
	}

	return NULL;
}

// Function: setSentinelOnIndexedEdge
// �w�肵���C���f�b�N�X�̃G�b�W�f�[�^�Ɋ��𗧂Ă�
//
// Parameter:
// index - �C���f�b�N�X�ԍ�
//
// Return:
// �w�肵���C���f�b�N�X�����G�b�W�ւ̃|�C���^�D������Ȃ�������NULL
HEedge *MESH::setSentinelOnIndexedEdge(int index)
{
	int n = Edge.getNum();	// �G�b�W���X�g�̑����𓾂�

	// �w�肵��index�Ɠ����C���f�b�N�X�����f�[�^��T��
	for(int i=0;i<n;i++){
		HEedge *e = (HEedge *)Edge.getData(i);	// �f�[�^�擾
		if(e->index == index){
			Edge.setSentinel(i);				// ������΁A�����Ɋ��𗧂Ă�
			return e;							// ���̃f�[�^��Ԃ�
		}
	}

	return NULL;
}

// Function: getIndexedEdgeFromSentinel
// ������dn�Ԗڌ�or�O�̃G�b�W�f�[�^�𓾂�(dn���}�C�i�X�̏ꍇ�͌������̒T��)
//
// Parameter:
// dn - ��(�Z���`�l��)���牽�Ԗڂ�
//
// Return:
// ������dn�Ԗڌ�or�O�̃G�b�W�ւ̃|�C���^�D������Ȃ�������NULL
HEedge *MESH::getIndexedEdgeFromSentinel(int dn)
{
	return (HEedge *)Edge.getDataFromSentinel(dn);
}

// Function: CalcFaceNorm
// �w�肵����f�̖@���x�N�g�������߁Af->norm�ɃZ�b�g����
//
// Parameter:
// *f - ��
//
// Return:
// �O�p���b�V���ȊO�̏ꍇ��KOD_FALSE�D�����FKOD_TRUE
int MESH::CalcFaceNorm(HEface *f)
{
	Coord a[MAXVERTNUMINFACE];

	HEedge *e = f->GetStartHE();
	for(int j=0;j<f->GetVetexNum();j++){	// �ʂ��\������_�̍��W�l���擾
		a[j] = e->GetStartVcoord();
		e = e->GetNextHE();
	}
	if(f->GetVetexNum() == TRIMESHVERTNUM){						// �O�p���b�V���̏ꍇ��
		f->SetNormVec(CalcNormVecFrom3Pts(a[0],a[1],a[2]));		// 3�_����@���x�N�g�����v�Z���Af->norm�ɃZ�b�g
		return KOD_TRUE;
	}
	else{									// �O�p���b�V���ȊO�̏ꍇ��FALSE
		f->SetNormVec(SetCoord(0,0,0));
		return KOD_FALSE;
	}
}

// Function: CalcFaceArea
// �w�肵����f�̖ʐς����߁Af->area�ɃZ�b�g����
//
// Parameter:
// *f - ��
//
// Return:
// �O�p���b�V���ȊO�̏ꍇ��KOD_FALSE�D�����FKOD_TRUE
int MESH::CalcFaceArea(HEface *f)
{
	Coord a[MAXVERTNUMINFACE];

	HEedge *e = f->GetStartHE();
	for(int j=0;j<f->GetVetexNum();j++){	// �ʂ��\������_�̍��W�l���擾
		a[j] = e->GetStartVcoord();
		e = e->GetNextHE();
	}
	if(f->GetVetexNum() == TRIMESHVERTNUM){
		f->SetFaceArea(CalcEuclid((a[1]-a[0])&&(a[2]-a[0]))/2);		// �O�p���b�V���̖ʐς��v�Z���Af->area�ɃZ�b�g
		return KOD_TRUE;
	}
	else{
		f->SetFaceArea(0);		// �O�p���b�V���ȊO�͔�Ή��Ƃ���
		return KOD_FALSE;
	}
}

// Function: InsertQEMD
// �����\�[�g���ꂽEdge���X�g�ɁA�ύX���ꂽQEM�R�X�g������HEedge��}��
// 
// Parameter:
// *ins - ���Ԃ�ύX����G�b�W�f�[�^
void MESH::InsertQEMD(HEedge *ins)
{
	Data *d;
	HEedge *e;
	int i=0;

	Edge.snipData(ins->mom);		// ���Ԃ�ύX����f�[�^ins�����X�g���甲���o��

	e = (HEedge *)Edge.getData(Edge.getNum()-1);


	d = Edge.getpData(0);
	for(int i=0;i<Edge.getNum();i++){
		e = (HEedge *)d->GetData();
		if(e->GetQEMD() > ins->GetQEMD()){		// ���X�g�̒��ڒ��̃f�[�^��QEMD��ins��QEMD���傫���Ȃ�����A���̃f�[�^�̑O��ins��}������
			if(!i){
				//fprintf(stderr,"a:%d-%d %p(%lf)-%p(%lf)\n",Edge.getNum(),i,e->mom,e->GetQEMD(),ins->mom,ins->GetQEMD());		// debug
				Edge.insData(ins->mom,NULL);
			}
			else{
				//fprintf(stderr,"b:%d-%d %p(%lf)-%p(%lf)\n",Edge.getNum(),i,e->mom,e->GetQEMD(),ins->mom,ins->GetQEMD());	// debug
				e = (HEedge *)((Edge.getPrevpData(d))->GetData());
				Edge.insData(ins->mom,e->mom);
			}
			//fprintf(stderr,"The edge is inserted before No.%d Edge\n",i);
			return;
		}
		d = Edge.getNextpData(d);
	}

	//fprintf(stderr,"The edge is inserted before No.%d Edge (END)\n",Edge.getNum()-1);
	d = Edge.getEndpData();
	e = (HEedge *)d->GetData();
	//fprintf(stderr,"c: %p,%p(%lf)-%p(%lf)\n",d,e->mom,e->GetQEMD(),ins->mom,ins->GetQEMD());
	Edge.insData(ins->mom,d);

}

// Function: EdgeSortbyQEMD
// Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����
void MESH::EdgeSortbyQEMD()
{
	Data *d;

	d = MergeSortForQEMD(Edge.getpData(0));	// QEMD��Edge���\�[�g
	Edge.ChangeHead(d);						// ���X�g�̐擪�����Ȃ���

	while(d->GetNextpData() != NULL)		// ���X�g�̖�����T��
		d = d->GetNextpData();
	Edge.ChangeEnd(d);						// ���X�g�̖��������Ȃ���

	// debug
	//fprintf(stderr,"////Edge List////\n");
	//HEedge *e;
	//Edge.setSentinel(0);
	//while((e = (HEedge *)Edge.getSentinelData()) != NULL){
	//	fprintf(stderr,"%p(%lf)\n",e->mom,e->GetQEMD());
	//	Edge.shiftSentinel(1);
	//}
	//fprintf(stderr,"\n");
}

// Function: MergeSortForQEMD
// Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����Sub1
//
// Parameter:
// *x - ���X�g�̐擪�A�h���X
//
// Return:
// �\�[�g��̐擪�A�h���X
Data *MESH::MergeSortForQEMD(Data *x)
{
	Data *a, *b, *y;

	if(x == NULL || x->GetNextpData() == NULL)
		return x;

	a = x;
	b = x->GetNextpData();
	if(b != NULL)
		b = b->GetNextpData();

	while(b != NULL){
		a = a->GetNextpData();
		b = b->GetNextpData();
		if(b != NULL)
			b = b->GetNextpData();
	}

	y = a->GetNextpData();
	a->SetNextpData(NULL);

	return MergeListForQEMD(MergeSortForQEMD(x),MergeSortForQEMD(y));
}

// Function: MergeListForQEMD
// Edge��QEM�R�X�g(Edge.qemD)�ŏ����\�[�g����Sub2
Data *MESH::MergeListForQEMD(Data *x,Data *y)
{
	Data z,*p;

	p = &z;

	while(x != NULL && y != NULL){
		if(((HEedge *)x->GetData())->qemD <= ((HEedge *)y->GetData())->qemD){
			p->SetNextpData(x);
			x->SetBeforepData(p);
			p = x;
			x = x->GetNextpData();
		}
		else{
			p->SetNextpData(y);
			y->SetBeforepData(p);
			p = y;
			y = y->GetNextpData();
		}
	}

	if(x == NULL){
		p->SetNextpData(y);
		y->SetBeforepData(p);
	}
	else{
		p->SetNextpData(x);
		x->SetBeforepData(p);
	}

	(z.GetNextpData())->SetBeforepData(NULL);

	return z.GetNextpData();
}

//////////////////////////////////////MESH class////////////////////////////////////////////////////////////////////////


////////////////////////////////////HEvert class////////////////////////////////////////////////////////////////////////

// Function: NewQEM
// qemQ[][]�̏�����
void HEvert::NewQEM()
{
	qemQ = NewMatrix(QUADINDEX,QUADINDEX);
	for(int i=0;i<QUADINDEX;i++){
		for(int j=0;j<QUADINDEX;j++){
			qemQ[i][j] = 0;
		}
	}
}

// Function: DelQEM
// qemQ�̃��������
void HEvert::DelQEM()
{
	if(qemQ != NULL)
		FreeMatrix(qemQ,QUADINDEX);
}

// Function: InitQEM
// qemQ�̏�����
void HEvert::InitQEM()
{
	for(int i=0;i<QUADINDEX;i++){
		for(int j=0;j<QUADINDEX;j++){
			qemQ[i][j] = 0;
		}
	}
}

// Function: SetQEM
// qem�p�����[�^���Z�b�g����
// 
// Parameter:
// buf - �Z�b�g����qem�p�����[�^�}�g���b�N�X
//
// Return:
// qemQ��NULL�FKOD_ERR, �����FKOD_TURE
int HEvert::SetQEM(Matrix buf)
{

	if(qemQ == NULL) return KOD_ERR;

	for(int i=0;i<QUADINDEX;i++){
		for(int j=0;j<QUADINDEX;j++){
			qemQ[i][j] = buf[i][j];
		}
	}

	return KOD_TRUE;
}

// Function: AddQEM
// qem�p�����[�^��������
// 
// Parameter:
// buf - ������qem�p�����[�^�}�g���b�N�X
//
// Return:
// qemQ��NULL�FKOD_ERR, �����FKOD_TURE
int HEvert::AddQEM(Matrix buf)
{
	if(qemQ == NULL) return KOD_ERR;

	for(int i=0;i<QUADINDEX;i++){
		for(int j=0;j<QUADINDEX;j++){
			qemQ[i][j] += buf[i][j];
		}
	}

	return KOD_TRUE;
}

// Function: GetQEM
// qem�p�����[�^�𓾂�
//
// Return:
// qemQ
Matrix HEvert::GetQEM()
{
	return qemQ;
}

////////////////////////////////////HEvert class////////////////////////////////////////////////////////////////////////



////////////////////////////////////HEedge class////////////////////////////////////////////////////////////////////////

// Function: GetPrevHE
// ���̃n�[�t�G�b�W��(���L����ʂɂ�����)1�O�̃n�[�t�G�b�W��Ԃ�
//
// Return:
// ���̃n�[�t�G�b�W��(���L����ʂɂ�����)1�O�̃n�[�t�G�b�W
HEedge *HEedge::GetPrevHE()
{
	int n = this->face->GetVetexNum();

	HEedge *e = nullptr,*e_;
	
	e_ = this;

	for(int i=0;i<n-1;i++){
		e = e_->ne;
		e_ = e;
	}

	return e;
}

////////////////////////////////////HEedge class////////////////////////////////////////////////////////////////////////
