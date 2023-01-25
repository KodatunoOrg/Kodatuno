#ifndef _KOD_LIST_FUNC_H_
#define _KOD_LIST_FUNC_H_

#include "StdAfx.h"

// リストKODListadata用データの実体を格納するためのクラス
class Data
{
	friend class KODlistData;

private:
	void *data;
	Data *next;

public:
	Data(void *a) { data = a; next = NULL; };
};

// リスト操作クラス
class KODlistData
{
public:
	KODlistData();			// コンストラクタ
	int getNum();			// リストへの登録数を得る
	void *getData(int);		// i番目のデータを得る
	int clear();			// 自身の消滅
	int add(void *);		// 新たなデータをリストに登録
	int delPos(int);		// i番目のデータをリストからはずす

private:
	Data *head;				// データ探索用
	Data *curr;				// データ探索用
	Data *old;				// データ探索用
	int  num;				// データ数
};

#endif