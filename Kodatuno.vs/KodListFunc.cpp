#include "KodListFunc.h"

// Function: KODlistData
// コンストラクタ(初期化)
KODlistData::KODlistData()
{
	num	 = 0;
	head = NULL;
	end  = NULL;
	sentinel = NULL;
}

// Function: getNum
// リストへの登録数を得る
//
// Return:
// リストへの登録数
int KODlistData::getNum()
{
	return num;
}

// Function: add
// 新たなデータをリスト最後尾に登録
//
// Parameters:
// *idata - 新たに登録するデータへのポインタ
//
// Return:
// データを入れた箱のアドレス
Data *KODlistData::add(void *idata)
{
	if(idata == NULL) return NULL;

	Data *newdata = new Data(idata);
	if(newdata == NULL)	return NULL;

	if(head == NULL){
		head = newdata;
		head->next = NULL;
		head->before = NULL;
		end = head;
	}
	else{
		curr = end;
		curr->next = newdata;
		end = curr->next;
		end->next = NULL;
		end->before = curr;
	}
	num++;

	return newdata;			
}

// Function: addData
// 新たなデータを指定したデータの後ろに加える
//
// Parameters:
// *tag - 指定データ (NULLを指定した場合は先頭に加える)    
// *ins - 加えるデータ
//
// Return:
// データを入れた箱のアドレス
Data *KODlistData::addData(Data *tag,void *ins)
{
	if(ins == NULL) return NULL;
		
	Data *nd = new Data(ins);
	if(nd == NULL)	return NULL;

	if(tag == NULL){
		nd->next = head;
		head->before = nd;
		head = nd;
	}
	else{
		nd->before = tag;
		nd->next = tag->next;
		tag->next = nd;
		tag->next->before = nd;
		if(nd->next == NULL) end = nd;
	}

	num++;

	return nd;
}

// Function: addData
// 新たなデータをn番のデータの後ろに加える(オーバーロード)
//
// Parameters:
// n - n番のデータ    
// *ins - 加えるデータ
//
// Return:
// データを入れた箱のアドレス
Data *KODlistData::addData(int n,void *ins)
{
	if(n >= num) return NULL;
	if(ins == NULL) return NULL;

	if(n < int(num/2)){
		curr = head;
		for(int i=0;i<n;i++){
			curr = curr->next;
		}
	}
	else{
		curr = end;
		for(int i=num-1;i>n;i--){
			curr = curr->before;
		}
	}

	return (addData(curr,ins));
}

// Function: insData
// 指定したデータAを指定したデータBの後ろに変更する
//
// Parameters:
// *A - 変更するデータポインタ
// *B - Aの前のデータを示すポインタ (NULLの場合は先頭に挿入する)
//
// Return:
// KOD_TRUE
int KODlistData::insData(Data *A,Data *B)
{
	if(A == NULL) return KOD_ERR;

	//snipData(A);
	//fprintf(stderr,"Out:%p-%p-%p\n",A->before,A,A->next);

	
	if(B == NULL){
		A->next = head;
		head->before = A;
		head = A;
		A->before = NULL;
	}
	else{
		A->before = B;
		A->next = B->next;
		if(B->next != NULL)
			B->next->before = A;
		B->next = A;
		if(A->next == NULL) end = A;
	}
	//fprintf(stderr,"In:%p-%p-%p\n",A->before,A,A->next);
	num++;

	return KOD_TRUE;
}

// Function: getData
// n番のデータを得る
//
// Parameters:
// n - データ番号
//
// Return:
// n番のデータへのポインタ
void *KODlistData::getData(int n)
{
	if(n >= num) return NULL;
	if(n == 0)	return head->data;
	if(n == num-1) return end->data;

	if(n < int(num/2)){
		curr = head;
		for(int i=0;i<n;i++){
			curr = curr->next;
		}
	}
	else{
		curr = end;
		for(int i=num-1;i>n;i--){
			curr = curr->before;
		}
	}
	return curr->data;
}

// Function: clear
// 自身の消滅
//
// Return:
// KOD_TRUE (そもそもデータがなかったらKOD_FALSEをリターン)
int KODlistData::clear()
{
	if(head == NULL)	return KOD_FALSE;	// そもそも何もなかったらFALSEをリターン

	curr = old = head;
	while(curr != NULL){
		curr = old->next;
		delete old;
		old = curr;
	}
	head = NULL;
	end = NULL;
	num = 0;

	return KOD_TRUE;
}

// Function: delData
// n番のデータをリストから消去
//
// Parameters:
// n - 消去するデータの番号
//
// Return:
// KOD_TRUE (nがリストに登録されているデータ数より多い場合：KOD_ERR)
int KODlistData::delData(int n)
{
	if(n >= num) return KOD_ERR;

	if(head->next == NULL){
		delete head;
		head = NULL;
		num=0;
		return KOD_TRUE;
	}

	curr = head;
	if(n==0){
		head = head->next;
		head->before = NULL;
	}
	else{
		for(int i=0;i<n;i++){
			old = curr;
			curr = curr->next;
		}
		old->next = curr->next;
		if(curr != end)
			curr->next->before = old;
		else
			end = old;
	}
	delete curr;
	num--;

	return KOD_TRUE;
}

// Function: delData
// 指定したデータをリストから消去 (オーバーロード)
//
// Parameters:
// *d - 消去するデータのポインタ
//
// Return:
// KOD_TRUE (dがNULL：KOD_ERR)
int KODlistData::delData(Data *d)
{
	if(d == NULL)	return KOD_ERR;

	if(head->next == NULL){
		delete head;
		head = NULL;
	}
	else if(d->before == NULL){
		curr = d->next;
		curr->before = NULL;
		head = curr;
	}
	else{
		curr = d->before;
		curr->next = d->next;
		if(d->next != NULL){
			curr = d->next;
			curr->before = d->before;
		}
		else
			end = curr;
	}

	delete d;
	num--;

	return KOD_TRUE;
}

// Function: snipData
// 指定したデータをリストからはずす
//
// 注意：消去はしないので、はずされたデータは自分で消去すること
//
// Parameters:
// *d - はずすデータのポインタ
//
// Return:
// はずされたデータへのポインタ (dがNULLだった場合，NULLをリターン)
Data *KODlistData::snipData(Data *d)
{
	if(d == NULL)	return NULL;

	if(d->before == NULL){
		curr = d->next;
		curr->before = NULL;
		head = curr;
	}
	else{
		curr = d->before;
		curr->next = d->next;
		if(d->next != NULL){
			curr = d->next;
			curr->before = d->before;
		}
		else
			end = curr;
	}
	d->before = NULL;
	d->next = NULL;

	num--;

	return d;
}

// Function: ChangeHead
// リストの先頭を指定したデータに変更する
// 
// Parameters:
// *d - 変更するデータのポインタ
void KODlistData::ChangeHead(Data *d)
{
	head = d;
}

// Function: ChangeEnd
// リストの末尾を指定したデータに変更する
// 
// Parameters:
// *d - 変更するデータのポインタ
void KODlistData::ChangeEnd(Data *d)
{
	end = d;
}

// Function: getpData
// n番のDataクラスへのポインタを得る(データ番号は0からスタートする）
//
// Parameters:
// n - データ番号
//
// Return:
// 指定されたn番のデータのポインタ
Data *KODlistData::getpData(int n)
{
	if(n >= num) return NULL;

	if(n < int(num/2)){
		curr = head;
		for(int i=0;i<n;i++){
			curr = curr->next;
		}
	}
	else{
		curr = end;
		for(int i=num-1;i>n;i--){
			curr = curr->before;
		}
	}

	return curr;
}

// Function: getNextpData
// 指定したDataの次のDataへのポインタを得る
// 
// Parameters:
// *data - 指定するDataへのポインタ
//
// Return:
// 指定したDataの次のDataへのポインタ
Data *KODlistData::getNextpData(Data *data)
{
	return data->next;
}

// Function: getPrevpData
// 指定したDataの前のDataへのポインタを得る
// 
// Parameters:
// *data - 指定するDataへのポインタ
//
// Return:
// 指定したDataの前のDataへのポインタ
Data *KODlistData::getPrevpData(Data *data)
{
	return data->before;
}

// Function: getEndpData
// 最後のDataへのポインタを得る
//
// Return:
// 最後のDataへのポインタ
Data *KODlistData::getEndpData()
{
	return end;
}

// Function: setSentinel
// 指定したデータに旗を立てる
//
// Parameters:
// n - 旗を立てるデータの番号
// 
// Return:
// 旗を立てたデータのアドレス (nがリスト数よりも大き場合はNULLをリターン)
void *KODlistData::setSentinel(int n)
{
	if(n >= num) return NULL;

	sentinel = head;
	for(int i=0;i<n;i++){
		sentinel = sentinel->next;
	}

	return sentinel->data;
}

// Function: getDataFromSentinel
// 旗からdn番目後or前のデータを得る (dnがマイナスの場合は後ろ向きの探索)
// 
// Parameters:
// dn - 旗から何番目かを表わす
//
// Return:
// 指定データへのポインタ (旗が設定されていなかった場合および,データが見つからなかった場合はNULLをリターン)
void *KODlistData::getDataFromSentinel(int dn)
{
	if(sentinel == NULL)		// 旗が設定されていなかったらNULLを返す
		return NULL;

	bool flag;
	if(dn < 0){
		flag = false;
		dn *= -1;
	}
	else{
		flag = true;
	}

	// 前向き
	if(flag == true){
		curr = sentinel;
		for(int i=0;i<dn;i++){
			curr = curr->next;
			if(curr == NULL){		// データが終端へ来てしまったら
				return NULL;		// NULLを返す
			}
		}
		return curr->data;
	}
	// 後向き
	else if(flag == false){
		curr = sentinel;
		for(int i=0;i<dn;i++){
			curr = curr->before;	
			if(curr == NULL){		// データが先頭へ来てしまったら
				return NULL;		// NULLを返す
			}
		}
		return curr->data;
	}

	return NULL;
}

// Function: shiftSentinel
// 旗を現在の位置からdn番目後or前に移動させる (dnがマイナスの場合は後ろ向きに移動)
//
// Parameters:
// 旗の移動量
//
// Return:
// 移動後の旗のアドレス (旗が設定されていなかったらNULLを返す)
void *KODlistData::shiftSentinel(int dn)
{
	if(sentinel == NULL)		// 旗が設定されていなかったらNULLを返す
		return NULL;

	bool flag;
	if(dn < 0){
		flag = false;
		dn *= -1;
	}
	else{
		flag = true;
	}

	// 前向き
	if(flag == true){
		for(int i=0;i<dn;i++){
			sentinel = sentinel->next;
			if(sentinel == NULL) return NULL;
		}
		return sentinel->data;
	}
	// 後向き
	else if(flag == false){
		for(int i=0;i<dn;i++){
			sentinel = sentinel->before;
			if(sentinel == NULL) return NULL;
		}
		return sentinel->data;
	}

	return NULL;
}

// Function: getSentinelData
// 旗のある場所のデータを取得する
//
// Return:
// 旗のある場所のデータへのポインタ
void *KODlistData::getSentinelData()
{
	if(sentinel == NULL)		// 旗が設定されていなかったらNULLを返す
		return NULL;

	return sentinel->data;
}

// Function: getNextSentData
// 旗のあるデータの次のデータを取得する
//
// Return:
// 旗のある場所の次のデータへのポインタ
void *KODlistData::getNextSentData()
{
	if(sentinel == NULL)		// 旗が設定されていなかったらNULLを返す
		return NULL;

	return sentinel->next->data;
}
