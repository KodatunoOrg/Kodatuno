#ifndef _KOD_LIST_FUNC_H_
#define _KOD_LIST_FUNC_H_

#include "StdAfx.h"

// Constants: General Defines
// Non

// Class: Data (friend class KODlistData)
// リストKODListadata用データの実体を格納するためのクラス
class Data
{
	friend class KODlistData;

public:
	// Constructor: Data
	// コンストラクタ(データをセットする)
	//
	// Parameters:
	// *a - データのアドレスを格納
	Data(void *a) { data = a; next = NULL; before = NULL;}

	// Constructor: Data
	// コンストラクタ(全てをNULLで初期化)
	Data() { data = NULL; next = NULL; before = NULL;}

	// Function: SetNextpData
	// 次のDataへのポインタをセットする
	//
	// Parameters:
	// *d - 次のデータへのポインタ
	void SetNextpData(Data *d) {next = d;}

	// Function: SetBeforepData
	// 前のDataへのポインタをセットする
	//
	// Parameters:
	// *d - 前のデータへのポインタ
	void SetBeforepData(Data *d) {before = d;}

	// Function: GetNextpData
	// 次のDataへのポインタを得る
	//
	// Return:
	// 次のデータへのポインタ
	Data *GetNextpData() {return next;}	

	// Function: GetNextpData
	// 前のDataへのポインタを得る
	//
	// Return:
	// 前のデータへのポインタ
	Data *GetBeforepData() {return before;}

	// Function: GetData
	// データの実体を得る
	//
	// Return:
	// データの実体
	void *GetData() {return data;}		


protected:
	// Variable: *data
	// (protected)データの実体はこのアドレスに格納される
	void *data;		

	// Variable: *next
	// (protected)リストの前方を指すポインタ
	Data *next;			

	// Variable: *before
	// (protected)リストの後方を指すポインタ
	Data *before;		
};

// Class: KODlistData
// リスト操作クラス
class KODlistData
{
public:

	// Function: KODlistData
	// コンストラクタ
	KODlistData();					
	
	// Function: getNum
	// リストへの登録数を得る
	int getNum();					
	
	// Function: getData
	// n番のデータを得る(データ番号は0からスタートする）
	void *getData(int);				
	
	// Function: clear
	// 自身の消滅
	int clear();					
	
	// Function: add
	// 新たなデータをリストに登録
	Data *add(void *);				

	// Function: addData
	// 新たなデータを指定したデータの後ろに加える	
	Data *addData(Data *,void *);	

	// Function: addData
	// 新たなデータをn番のデータの後ろに加える
	Data *addData(int n,void *);	

	// Function: delData
	// n番のデータをリストから消去
	int delData(int);				

	// Function: delData
	// 指定したデータをリストから消去
	int delData(Data *);			

	// Function: snipData
	// 指定したデータをリストからはずす（消去はしない）
	Data *snipData(Data *);			

	// Function: insData
	// 指定したデータAを指定したデータBの後ろに変更する
	int insData(Data *,Data *);		

	// Function: ChangeHead
	// リストの先頭を変更
	void ChangeHead(Data *);		

	// Function: ChangeEnd
	// リストの末尾を変更
	void ChangeEnd(Data *);			

	// Function: getpData
	// n番のDataオブジェクトへのポインタを得る(データ番号は0からスタートする）
	Data *getpData(int);			

	// Function: getNextpData
	// 指定したDataの次のDataオブジェクトへのポインタを得る
	Data *getNextpData(Data *);		

	// Function: getPrevpData
	// 指定したDataの前のDataオブジェクトへのポインタを得る
	Data *getPrevpData(Data *);		

	// Function: getEndpData
	// 最後のDataオブジェクトへのポインタを得る
	Data *getEndpData();			

	// センチネル操作

	// Function: setSentinel
	// 指定した位置に旗を立てる
	void *setSentinel(int);			
	
	// Function: getDataFromSentinel
	// 旗からn番目後or前のデータを得る
	void *getDataFromSentinel(int);	
	
	// Function: shiftSentinel
	// 旗を現在の位置からn番目後or前に移動させる
	void *shiftSentinel(int);		
	
	// Function: getSentinelData
	// 旗のある場所のデータを取得する
	void *getSentinelData();		
	
	// Function: getNextSentData
	// 旗のあるデータの次のデータを取得する
	void *getNextSentData();		

private:

	// Variable: *head
	// (private)リストの先頭
	Data *head;

	// Variable: *end
	// リストの末尾
	Data *end;	

	// Variable: *sentinel
	// データ探索作業用
	Data *sentinel;

	// Variable: *curr
	// データ探索作業用
	Data *curr;	

	// Variable: *old
	// データ探索作業用
	Data *old;

	// Variable: num
	// データ数（最初のデータ(0番)が1個目）
	int  num;				
};

#endif