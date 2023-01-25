#include "Quaternion.h"

// Function: QInit
// クォータニオンを引数で指定した数値で初期化
//
// Parameters: 
// t - 実部
// x,y,z -虚部
// 
// Return:
// 初期化されたQuat
Quat QUATERNION::QInit(double t,double x,double y,double z)
{
	Quat q;

	q.t = t;
	q.x = x;
	q.y = y;
	q.z = z;

	return q;
}

// Function: QGenRot
// 回転クォータニオンqを生成 q = {cos(r/2):x*sin(r/2),y*sin(r/2),z*sin(r/2)}
//
// Parameters: 
// r - 回転角[rad] 
// x,y,z - 回転軸ベクトル
// 
// Return:
// 回転クォータニオンq
Quat QUATERNION::QGenRot(double r,double x,double y,double z)
{
	Quat q;

	q.t = cos(r/2);
	q.x = x*sin(r/2);
	q.y = y*sin(r/2);
	q.z = z*sin(r/2);

	return q;
}

// Function: QConjugation
// 共役クォータニオンを生成
//
// Parameters: 
// q - クォータニオン
// 
// Return:
// qの共役クォータニオン
Quat QUATERNION::QConjugation(Quat q)
{
	Quat p;

	p.t = q.t;
	p.x = -q.x;
	p.y = -q.y;
	p.z = -q.z;

	return p;
}

// Function: QMult
// クォータニオンの積を求める
//
// Parameters: 
// p,q - p x q
//
// Return:
// 計算結果
Quat QUATERNION::QMult(Quat p,Quat q)
{
	Quat r;

	r.t = p.t*q.t - p.x*q.x - p.y*q.y - p.z*q.z;
	r.x = p.t*q.x + p.x*q.t + p.y*q.z - p.z*q.y;
	r.y = p.t*q.y - p.x*q.z + p.y*q.t + p.z*q.x;
	r.z = p.t*q.z + p.x*q.y - p.y*q.x + p.z*q.t;

	return r;
}

// Function: QRot
// クォータニオンによる回転ベクトル演算
//
// Parameters: 
// r - qの共役クォータニオン  
// p - 回転したい座標値を持ったクォータニオン  
// q - 回転クォータニオン 
//
// Return:
// 計算結果
Quat QUATERNION::QRot(Quat r,Quat p,Quat q)
{
	return QMult(QMult(r,p),q);
}

// Function: QtoR
// クォータニオンqを回転行列rへ変換
//
// Parameters: 
// r[16] - 4*4行列を１次元配列で表現したもの
// q - 変換するクォータニオン
void QUATERNION::QtoR(double r[],Quat q)
{
	double x2 = q.x * q.x * 2.0;
	double y2 = q.y * q.y * 2.0;
	double z2 = q.z * q.z * 2.0;
	double xy = q.x * q.y * 2.0;
	double yz = q.y * q.z * 2.0;
	double zx = q.z * q.x * 2.0;
	double xw = q.x * q.t * 2.0;
	double yw = q.y * q.t * 2.0;
	double zw = q.z * q.t * 2.0;

	r[ 0] = 1.0 - y2 - z2;
	r[ 1] = xy + zw;
	r[ 2] = zx - yw;
	r[ 4] = xy - zw;
	r[ 5] = 1.0 - z2 - x2;
	r[ 6] = yz + xw;
	r[ 8] = zx + yw;
	r[ 9] = yz - xw;
	r[10] = 1.0 - x2 - y2;
	r[ 3] = r[ 7] = r[11] = r[12] = r[13] = r[14] = 0.0;
	r[15] = 1.0;
}

// Function: QtoR
// クォータニオンqを回転行列rへ変換(オーバーロード)
//
// Parameters: 
// r[16] - 4*4行列を１次元配列で表現したもの
// t,x,y,z - 変換するクォータニオンの要素表現
void QUATERNION::QtoR(double r[],double t,double x,double y,double z)
{
	double x2 = x * x * 2.0;
	double y2 = y * y * 2.0;
	double z2 = z * z * 2.0;
	double xy = x * y * 2.0;
	double yz = y * z * 2.0;
	double zx = z * x * 2.0;
	double xw = x * t * 2.0;
	double yw = y * t * 2.0;
	double zw = z * t * 2.0;

	r[ 0] = 1.0 - y2 - z2;
	r[ 1] = xy + zw;
	r[ 2] = zx - yw;
	r[ 4] = xy - zw;
	r[ 5] = 1.0 - z2 - x2;
	r[ 6] = yz + xw;
	r[ 8] = zx + yw;
	r[ 9] = yz - xw;
	r[10] = 1.0 - x2 - y2;
	r[ 3] = r[ 7] = r[11] = r[12] = r[13] = r[14] = 0.0;
	r[15] = 1.0;
}

// Function: QCopy
// クォータニオンのコピー p <- q
//
// Parameters: 
// q - コピー元クォータニオン
//
// Return:
// コピー先クォータニオン
Quat QUATERNION::QCopy(Quat q)
{
	return q;
}

// Function: QtoC
// クォータニオンの3つの虚部をCoord構造体に代入する
// 
// Parameters: 
// q - 代入するクォータニオン
//
// Return:
// クォータニオン虚部(x,y,z)をそのままCoord値(x,y,z)としたもの
Coord QUATERNION::QtoC(Quat q)
{
	Coord a;

	a.x = q.x;
	a.y = q.y;
	a.z = q.z;

	return a;
}

// Function: CtoQ
// クォータニオン実部を0、虚部をCoordとしてクォータニオンを生成
//
// Parameters: 
// a - 虚部となるCoord値
//
// Return:
// 生成されたクォータニオン 
Quat QUATERNION::CtoQ(Coord a)
{
	Quat q;

	q.t = 0;
	q.x = a.x;
	q.y = a.y;
	q.z = a.z;

	return q;
}

// Function: QSlerp
// クォータニオンによる球面線形補間
//
// 2つのベクトルp(t=0)、q(t=1)間をtで指定した内分上に球面線形補間する.
// p,qは単位ベクトルであること.
//
// Parameters: 
// p, q - 分割したい両端の点の座標を虚部としたクォータニオン
// t - 内分比
//
// Return:
// 補間された点を表すクォータニオン
Quat QUATERNION::QSlerp(Quat p,Quat q,double t)
{
	double ca = CalcInnerProduct(QtoC(p),QtoC(q));	// cos(a)
	double sa = sqrt(1-ca*ca);			// sin(a)
	
	if(sa == 0.0)
		return(p);
	else{
		double a = acos(ca);			// a
		double at = a*t;				// a*t
		double t1 = sin(at)/sa;
		double t0 = sin(a-at)/sa;

		Quat r;
		r.t = p.t*t0 + q.t*t1;
		r.x = p.x*t0 + q.x*t1;
		r.y = p.y*t0 + q.y*t1;
		r.z = p.z*t0 + q.z*t1;
		return r;
	}		
}


