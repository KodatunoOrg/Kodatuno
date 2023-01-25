// VRML V1.0 �p�[�T�[

#ifndef _VRML_PARSER_MAIN_H_
#define _VRML_PARSER_MAIN_H_

#include "BODY.h"

#define BUFSIZEMAX 256		// ������ꎞ�i�[�p�o�b�t�@�̃T�C�Y
#define ALLTAGNUM  37		// VRML�^�O�̑���

// VRML�t�@�C���ɗp������^�O�̃V���{����`
enum VRMLTags{
	AsciiText=1,		// 1:����
	Cone,				// 2:�~��
	Coordinate3,		// 3:�O�������W
	Cube,				// 4:������
	Cylinder,			// 5:�~��
	DirectionalLight,	// 6:�w��������
	FontStyle,			// 7:����
	Group,				// 8:�O���[�v 
	IndexedFaceSet,		// 9:�ʂ𒸓_�̃C���f�b�N�X�Ŏw��
	IndexedLineSet,		// 10:���𒸓_�̃C���f�b�N�X�Ŏw��
	Info,				// 11:�C���t�H���[�V����
	LOD,				// 12:�ڍ׎w��
	Material,			// 13:�ގ�
	MaterialBinding,	// 14:�ގ��i�����j
	MatrixTransform,	// 15:�O�������W�ϊ�
	Normal,				// 16:�@��
	NormalBinding,		// 17:�@���i�����j
	OrthographicCamera,	// 18:���s���e�J����
	PerspectiveCamera,	// 19:�����J����
	PointLight,			// 20:�_����
	PointSet,			// 21:�_
	Rotation,			// 22:��]
	Scale,				// 23:�I�u�W�F�N�g�̃X�P�[��
	Separator,			// 24:��ԕ���
	ShapeHints,			// 25:�`����
	Sphere,				// 26:��
	SpotLight,			// 27:�X�|�b�g���C�g�i�w�����j
	Switch,				// 28:�I��
	Texture2,			// 29:�e�N�X�`���摜
	Texture2Transform,	// 30:�e�N�X�`�����W
	TextureCoordinate2,	// 31:���W
	TextureCoordinateBinding,	// 32
	Transform,			// 33:�����ړ�
	TransformSeparator,	// 34:���W����
	Translation,		// 35:���s�ړ�
	WWWAnchor,			// 36:WWW�A���J�[ 
	WWWInline,			// 37:�t�@�C���ǂݍ���
};

// VRML V1.0 �p�[�T�[�p�N���X���`
class VRML_PARSER
{
public:
    int Vrml_Parser_Main(BODY *, const char *);	// VRML�t�@�C���̃p�[�Tmain

private:
	int CheckTags(char []);					// �^�O���
	int GetCoords(FILE *,MESH *);			// �O�������W�l��MESH�Ɋi�[����
	int GetFacets(FILE *,MESH *);			// �t�@�Z�b�g��MESH�Ɋi�[����
	int GetLine(FILE *,char []);			// �t�@�C������1�s�擾���A���s�������I�[�����ɕύX����
	void SetMesh(MESH *,int [],int);		// ���_�̃C���f�b�N�X�Z�b�g����A���b�V���f�[�^�𐶐�����
	int GetHalfEdgePair(MESH *);			// �΂ƂȂ�t�����n�[�t�G�b�W�ւ̃|�C���^�𓾂�
	void SetFaceParam(MESH *);				// �e�ʂɖʐςƒP�ʖ@���x�N�g���̏���t������
};

#endif
