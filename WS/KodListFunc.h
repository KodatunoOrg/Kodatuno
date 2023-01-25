#ifndef _KOD_LIST_FUNC_H_
#define _KOD_LIST_FUNC_H_

#include "StdAfx.h"

// リストKODListadata用データの実体を格納するためのクラス
class Data
{
	friend class KODlistData;

public:
	Data(void *a) { data = a; next = NULL; before = NULL;}		// コンストラクタ
	Data()		  { data = NULL; next = NULL; before = NULL;}	// コンストラクタ
	void SetNextpData(Data *d)		{next = d;}					// 次のDataオブジェクトへのポインタをセットする
	void SetBeforepData(Data *d)	{before = d;}				// 前のDataオブジェクトへのポインタをセットする
	Data *GetNextpData()			{return next;}				// 次のDataオブジェクトへのポインタを得る
	Data *GetBeforepData()			{return before;}			// 前のDataオブジェクトへのポインタを得る
	void *GetData()					{return data;}				// データの実体を得る

protected:
	void *data;			// データの実体はこのアドレスに格納される
	Data *next;			// リストの前方を指すポインタ
	Data *before;		// リストの後方を指すポインタ
};

// リスト操作クラス
class KODlistData
{
public:
	KODlistData();					// コンストラクタ
	int getNum();					// リストへの登録数を得る
	void *getData(int);				// n番のデータを得る(データ番号は0からスタートする）
	int clear();					// 自身の消滅
	Data *add(void *);				// 新たなデータをリストに登録
	Data *addData(Data *,void *);	// 新たなデータを指定したデータの後ろに加える	
	Data *addData(int n,void *);	// 新たなデータをn番のデータの後ろに加える
	int delData(int);				// n番のデータをリストから消去
	int delData(Data *);			// 指定したデータをリストから消去
	Data *snipData(Data *);			// 指定したデータをリストからはずす（消去はしない）
	int insData(Data *,Data *);		// 指定したデータAを指定したデータBの後ろに変更する
	void ChangeHead(Data *);		// リストの先頭を変更
	void ChangeEnd(Data *);			// リストの末尾を変更

	Data *getpData(int);			// n番のDataオブジェクトへのポインタを得る(データ番号は0からスタートする）
	Data *getNextpData(Data *);		// 指定したDataの次のDataオブジェクトへのポインタを得る
	Data *getPrevpData(Data *);		// 指定したDataの前のDataオブジェクトへのポインタを得る
	Data *getEndpData();			// 最後のDataオブジェクトへのポインタを得る

	// センチネル操作
	void *setSentinel(int);			// 指定した位置に旗を立てる
	void *getDataFromSentinel(int);	// 旗からn番目後or前のデータを得る
	void *shiftSentinel(int);		// 旗を現在の位置からn番目後or前に移動させる
	void *getSentinelData();		// 旗のある場所のデータを取得する
	void *getNextSentData();		// 旗のあるデータの次のデータを取得する

private:
	Data *head;				// リストの先頭
	Data *end;				// リストの末尾
	Data *sentinel;			// データ探索用
	Data *curr;				// 作業用
	Data *old;				// 作業用
	int  num;				// データ数（最初のデータ(0番)が1個目）
};

#endif