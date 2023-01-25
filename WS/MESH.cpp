#include "MESH.h"

//////////////////////////////////////MESH class////////////////////////////////////////////////////////////////////////

// コンストラクタ
MESH::MESH()
{
	VertNum = EdgeNum = FaceNum = 0;
}

// メッシュの全クリア
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

// 指定した頂点を消去
void MESH::DelVert(HEvert *v)
{
	Vert.delData(v->mom);	// リストから外す
	//fprintf(stderr,"del v:%p\n",v->mom);		// debug
	v->DelQEM();
	VertNum--;
	delete v;				// 自身の消滅
}
// 指定したハーフエッジを消去
void MESH::DelEdge(HEedge *e)
{
	Edge.delData(e->mom);	// リストから外す
	// fprintf(stderr,"del e:%p\n",e->mom);		// debug
	EdgeNum--;
	delete e;				// 自身の消滅
}

// 指定した面を消去
void MESH::DelFace(HEface *f)
{
	Face.delData(f->mom);	// リストから外す
	//fprintf(stderr,"del f:%p\n",f->mom);		// debug
	FaceNum--;
	delete f;				// 自身の消滅
}

// 指定したインデックスと同じインデックスを持つ頂点を探し出す
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

// 指定したインデックスと同じインデックスを持つエッジを探し出す
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

// 指定したインデックスと同じインデックスを持つ面を探し出す
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

// 指定したインデックスのエッジデータに旗を立てる
HEedge *MESH::setSentinelOnIndexedEdge(int index)
{
	int n = Edge.getNum();	// エッジリストの総数を得る

	// 指定したindexと同じインデックスを持つデータを探す
	for(int i=0;i<n;i++){
		HEedge *e = (HEedge *)Edge.getData(i);	// データ取得
		if(e->index == index){
			Edge.setSentinel(i);				// 見つかれば、そこに旗を立てる
			return e;							// そのデータを返す
		}
	}

	return NULL;
}

// 旗からdn番目後or前のエッジデータを得る
// nがマイナスの場合は後ろ向きの探索
HEedge *MESH::getIndexedEdgeFromSentinel(int dn)
{
	return (HEedge *)Edge.getDataFromSentinel(dn);
}

// 指定した面fの法線ベクトルを求め、f->normにセットする
int MESH::CalcFaceNorm(HEface *f)
{
	Coord a[MAXVERTNUMINFACE];

	HEedge *e = f->GetStartHE();
	for(int j=0;j<f->GetVetexNum();j++){	// 面を構成する点の座標値を取得
		a[j] = e->GetStartVcoord();
		e = e->GetNextHE();
	}
	if(f->GetVetexNum() == TRIMESHVERTNUM){						// 三角メッシュの場合は
		f->SetNormVec(CalcNormVecFrom3Pts(a[0],a[1],a[2]));		// 3点から法線ベクトルを計算し、f->normにセット
		return KOD_TRUE;
	}
	else{									// 三角メッシュ以外の場合はFALSE
		f->SetNormVec(SetCoord(0,0,0));
		return KOD_FALSE;
	}
}

// 指定した面fの面積を求め、f->areaにセットする
int MESH::CalcFaceArea(HEface *f)
{
	Coord a[MAXVERTNUMINFACE];

	HEedge *e = f->GetStartHE();
	for(int j=0;j<f->GetVetexNum();j++){	// 面を構成する点の座標値を取得
		a[j] = e->GetStartVcoord();
		e = e->GetNextHE();
	}
	if(f->GetVetexNum() == TRIMESHVERTNUM){
		f->SetFaceArea(CalcEuclid((a[1]-a[0])&&(a[2]-a[0]))/2);		// 三角メッシュの面積を計算し、f->areaにセット
		return KOD_TRUE;
	}
	else{
		f->SetFaceArea(0);		// 三角メッシュ以外は非対応とする
		return KOD_FALSE;
	}
}

//	昇順ソートされたEdgeリストに、変更されたQEMコストを持つHEedgeを挿入
void MESH::InsertQEMD(HEedge *ins)
{
	Data *d;
	HEedge *e;
	int i=0;

	Edge.snipData(ins->mom);		// 順番を変更するデータinsをリストから抜き出す

	e = (HEedge *)Edge.getData(Edge.getNum()-1);


	d = Edge.getpData(0);
	for(int i=0;i<Edge.getNum();i++){
		e = (HEedge *)d->GetData();
		if(e->GetQEMD() > ins->GetQEMD()){		// リストの注目中のデータのQEMDがinsのQEMDより大きくなったら、そのデータの前にinsを挿入する
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

// EdgeをQEMコスト(Edge.qemD)で昇順ソートする
void MESH::EdgeSortbyQEMD()
{
	Data *d;

	d = MergeSortForQEMD(Edge.getpData(0));	// QEMDでEdgeをソート
	Edge.ChangeHead(d);						// リストの先頭をつけなおす

	while(d->GetNextpData() != NULL)		// リストの末尾を探索
		d = d->GetNextpData();
	Edge.ChangeEnd(d);						// リストの末尾をつけなおす

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

// EdgeをQEMコスト(Edge.qemD)で昇順ソートするSub1
// http://www.geocities.jp/ky_webid/algorithm/021.html を引用
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

// EdgeをQEMコスト(Edge.qemD)で昇順ソートするSub2
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

// qemQ[][]の初期化
void HEvert::NewQEM()
{
	qemQ = NewMatrix(QUADINDEX,QUADINDEX);
	for(int i=0;i<QUADINDEX;i++){
		for(int j=0;j<QUADINDEX;j++){
			qemQ[i][j] = 0;
		}
	}
}

// qemQのメモリ解放
void HEvert::DelQEM()
{
	if(qemQ != NULL)
		FreeMatrix(qemQ,QUADINDEX);
}

// qemQの初期化
void HEvert::InitQEM()
{
	for(int i=0;i<QUADINDEX;i++){
		for(int j=0;j<QUADINDEX;j++){
			qemQ[i][j] = 0;
		}
	}
}

// qemパラメータをセットする
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

// qemパラメータを加える
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

// qemパラメータを得る
Matrix HEvert::GetQEM()
{
	return qemQ;
}

////////////////////////////////////HEvert class////////////////////////////////////////////////////////////////////////



////////////////////////////////////HEedge class////////////////////////////////////////////////////////////////////////

// このハーフエッジの(共有する面における)1つ前のハーフエッジを返す
HEedge *HEedge::GetPrevHE()
{
	int n = this->face->GetVetexNum();

	HEedge *e,*e_;
	
	e_ = this;

	for(int i=0;i<n-1;i++){
		e = e_->ne;
		e_ = e;
	}

	return e;
}

////////////////////////////////////HEedge class////////////////////////////////////////////////////////////////////////
