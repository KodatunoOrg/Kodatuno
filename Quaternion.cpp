#include "Quaternion.h"

// クォータニオンの初期化
Quat QUATERNION::QInit(double t,double x,double y,double z)
{
	Quat q;

	q.t = t;
	q.x = x;
	q.y = y;
	q.z = z;

	return q;
}


// 回転クォータニオンqを生成 q = {cos(r/2):x*sin(r/2),y*sin(r/2),z*sin(r/2)}
Quat QUATERNION::QGenRot(double r,double x,double y,double z)
{
	Quat q;

	q.t = cos(r/2);
	q.x = x*sin(r/2);
	q.y = y*sin(r/2);
	q.z = z*sin(r/2);

	return q;
}


// 共役クォータニオンを生成
Quat QUATERNION::QConjugation(Quat q)
{
	Quat p;

	p.t = q.t;
	p.x = -q.x;
	p.y = -q.y;
	p.z = -q.z;

	return p;
}

// クォータニオンの積 r <- p x q
Quat QUATERNION::QMult(Quat p,Quat q)
{
	Quat r;

	r.t = p.t*q.t - p.x*q.x - p.y*q.y - p.z*q.z;
	r.x = p.t*q.x + p.x*q.t + p.y*q.z - p.z*q.y;
	r.y = p.t*q.y - p.x*q.z + p.y*q.t + p.z*q.x;
	r.z = p.t*q.z + p.x*q.y - p.y*q.x + p.z*q.t;

	return r;
}

// クォータニオンによる回転ベクトル演算
Quat QUATERNION::QRot(Quat r,Quat p,Quat q)
{
	return QMult(QMult(r,p),q);
}

// クォータニオンqを回転行列rへ変換
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

// クォータニオンqを回転行列rへ変換(オーバーロード)
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

// クォータニオンのコピー p <- q
Quat QUATERNION::QCopy(Quat q)
{
	Quat p;

	p.t = q.t;
	p.x = q.x;
	p.y = q.y;
	p.z = q.z;

	return p;
}

Coord QUATERNION::QtoC(Quat q)
{
	Coord a;

	a.x = q.x;
	a.y = q.y;
	a.z = q.z;

	return a;
}
