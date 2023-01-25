#include "stdafx.h"
#include "Quaternion.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

// Function: QInit
// �N�H�[�^�j�I���������Ŏw�肵�����l�ŏ�����
//
// Parameters: 
// t - ����
// x,y,z -����
// 
// Return:
// ���������ꂽQuat
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
// ��]�N�H�[�^�j�I��q�𐶐� q = {cos(r/2):x*sin(r/2),y*sin(r/2),z*sin(r/2)}
//
// Parameters: 
// r - ��]�p[rad] 
// x,y,z - ��]���x�N�g��
// 
// Return:
// ��]�N�H�[�^�j�I��q
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
// �����N�H�[�^�j�I���𐶐�
//
// Parameters: 
// q - �N�H�[�^�j�I��
// 
// Return:
// q�̋����N�H�[�^�j�I��
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
// �N�H�[�^�j�I���̐ς����߂�
//
// Parameters: 
// p,q - p x q
//
// Return:
// �v�Z����
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
// �N�H�[�^�j�I���ɂ���]�x�N�g�����Z
//
// Parameters: 
// r - q�̋����N�H�[�^�j�I��  
// p - ��]���������W�l���������N�H�[�^�j�I��  
// q - ��]�N�H�[�^�j�I�� 
//
// Return:
// �v�Z����
Quat QUATERNION::QRot(Quat r,Quat p,Quat q)
{
	return QMult(QMult(r,p),q);
}

// Function: QtoR
// �N�H�[�^�j�I��q����]�s��r�֕ϊ�
//
// Parameters: 
// r[16] - 4*4�s����P�����z��ŕ\����������
// q - �ϊ�����N�H�[�^�j�I��
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
// �N�H�[�^�j�I��q����]�s��r�֕ϊ�(�I�[�o�[���[�h)
//
// Parameters: 
// r[16] - 4*4�s����P�����z��ŕ\����������
// t,x,y,z - �ϊ�����N�H�[�^�j�I���̗v�f�\��
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
// �N�H�[�^�j�I���̃R�s�[ p <- q
//
// Parameters: 
// q - �R�s�[���N�H�[�^�j�I��
//
// Return:
// �R�s�[��N�H�[�^�j�I��
Quat QUATERNION::QCopy(Quat q)
{
	return q;
}

// Function: QtoC
// �N�H�[�^�j�I����3�̋�����Coord�\���̂ɑ������
// 
// Parameters: 
// q - �������N�H�[�^�j�I��
//
// Return:
// �N�H�[�^�j�I������(x,y,z)�����̂܂�Coord�l(x,y,z)�Ƃ�������
Coord QUATERNION::QtoC(Quat q)
{
	Coord a;

	a.x = q.x;
	a.y = q.y;
	a.z = q.z;

	return a;
}

// Function: CtoQ
// �N�H�[�^�j�I��������0�A������Coord�Ƃ��ăN�H�[�^�j�I���𐶐�
//
// Parameters: 
// a - �����ƂȂ�Coord�l
//
// Return:
// �������ꂽ�N�H�[�^�j�I�� 
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
// �N�H�[�^�j�I���ɂ�鋅�ʐ��`���
//
// 2�̃x�N�g��p(t=0)�Aq(t=1)�Ԃ�t�Ŏw�肵��������ɋ��ʐ��`��Ԃ���.
// p,q�͒P�ʃx�N�g���ł��邱��.
//
// Parameters: 
// p, q - �������������[�̓_�̍��W�������Ƃ����N�H�[�^�j�I��
// t - ������
//
// Return:
// ��Ԃ��ꂽ�_��\���N�H�[�^�j�I��
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


