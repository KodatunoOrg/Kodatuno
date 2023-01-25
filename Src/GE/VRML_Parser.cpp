/*********************************************************
* VRML1.0�t�@�C����ǂݍ��݁A�n�[�t�G�b�W�\���֊i�[����  *
* 2012/4 K.Takasugi                                      *
**********************************************************/

#include "VRML_Parser.h"

int VRML_PARSER::Vrml_Parser_Main(BODY *body, const char *fname)
{
	FILE *fp;
	char mes[BUFSIZEMAX];		// �o�͗p���b�Z�[�W�i�[�o�b�t�@
	char buf[BUFSIZEMAX];		// ������ꎞ�i�[�p�o�b�t�@
	int tag;					// �^�O���ʗp
	int flag;

	// VRML�t�@�C���I�[�v��
	if((fp = fopen(fname,"r")) == NULL){
		sprintf(mes,"KOD_ERROR: Cannot open %s",fname);
        GuiIF.SetMessage(mes);
		return(KOD_ERR);
	}

	// �w�b�_�ǂݍ���
	fgets(buf,sizeof(buf),fp);
	if(strcmp(buf,"#VRML V1.0 ascii\n")){
		sprintf(mes,"KOD_ERROR: Kodatuno supports only #VRML V1.0 ascii.     Please check VRML file version.");
        GuiIF.SetMessage(mes);
		fclose(fp);
		return(KOD_ERR);
	}

	body->Mesh = new MESH;		// MESH�N���X�̃������[�m��

	// 1�s���ǂݍ��݂Ȃ���A�^�O��͂��Ă���
	while(fgets(buf,sizeof(buf),fp) != NULL){
		if((tag = CheckTags(buf)) == KOD_ERR){	// �^�O���
			continue;							// �o�^���ꂽ�^�O��������Ă��Ȃ��s�͓ǂݔ�΂�
		}

		if(tag == Coordinate3){					// �O�������W�p�^�O�𔭌�
			flag = GetCoords(fp,body->Mesh);	// �O�������W�l��MESH�Ɋi�[
			if(flag == KOD_ERR){
				body->Mesh->clear();
				return KOD_ERR;
			}

		}
		else if(tag == IndexedFaceSet){			// �t�@�Z�b�g��`�p�^�O�𔭌�
			flag = GetFacets(fp,body->Mesh);	// �t�@�Z�b�g��MESH�Ɋi�[
			if(flag == KOD_ERR){
				body->Mesh->clear();
				return KOD_ERR;
			}
		}
	}

	fclose(fp);

	// �n�[�t�G�b�W�A���_�A�ʂ��ꂼ��̑����𓾂�
	body->Mesh->EdgeNum = body->Mesh->Edge.getNum();
	body->Mesh->VertNum = body->Mesh->Vert.getNum();
	body->Mesh->FaceNum = body->Mesh->Face.getNum();

	GetHalfEdgePair(body->Mesh);	// �΂ƂȂ�t�����n�[�t�G�b�W�ւ̃|�C���^�𓾂�

	SetFaceParam(body->Mesh);		// �e�ʂɖʐςƒP�ʖ@���x�N�g���̏���t������

	body->MaxCoord = 10;			// �{���͂Ƃ肠����10�ɂ��Ă���

	body->TypeNum[_MESH] = 1;		// TypeNum��1

	return KOD_TRUE;
}

// �e�ʂɖʐς̏���t������
void VRML_PARSER::SetFaceParam(MESH *mesh)
{
	for(int i=0;i<mesh->FaceNum;i++){
		HEface *f = (HEface *)mesh->Face.getData(i);
		mesh->CalcFaceArea(f);		// �ʐς̏���t������
		mesh->CalcFaceNorm(f);		// �P�ʖ@���x�N�g���̏���t������
	}
}

// �΂ƂȂ�t�����n�[�t�G�b�W�ւ̃|�C���^�𓾂�
int VRML_PARSER::GetHalfEdgePair(MESH *mesh)
{
	bool flag = false;

	// �S�n�[�t�G�b�W�ɑ΂���
	for(int i=0;i<mesh->EdgeNum;i++){
		HEedge *e = (HEedge *)mesh->Edge.getData(i);	// i�Ԃ̃n�[�t�G�b�W�����o��
		mesh->Edge.setSentinel(0);						// �����ŏ��̃n�[�t�G�b�W�ɗ��Ă�
		flag = false;									// �������t���O�n�e�e
		for(int j=0;j<mesh->EdgeNum;j++){				// i�Ԃ̃n�[�t�G�b�W�̑΂�T��
			if(j==i){									// �����n�[�t�G�b�W�����Ă���ꍇ��
				mesh->Edge.shiftSentinel(1);			// ����1���炷
				continue;
			}
			HEedge *e_ = (HEedge *)mesh->Edge.getSentinelData();	// ���̗����Ă���n�[�t�G�b�W�̃f�[�^�𓾂�
			if(e_->ne->vert->index == e->vert->index &&				// j�Ԃ̃n�[�t�G�b�W�̎��̃n�[�t�G�b�W�̃C���f�b�N�X��i�Ԃ̃n�[�t�G�b�W�̃C���f�b�N�X�Ɠ���������
				e_->vert->index == e->ne->vert->index){				// i�Ԃ̃n�[�t�G�b�W�̎��̃n�[�t�G�b�W�̃C���f�b�N�X��j�Ԃ̃n�[�t�G�b�W�̃C���f�b�N�X�Ɠ������Ȃ�
				e->pair = e_;										// �΂�������
				flag = true;							// �������t���O�n�m
				break;									// ���̃n�[�t�G�b�W�Ɉړ�
			}
			mesh->Edge.shiftSentinel(1);				// �΂������Ă��Ȃ��ꍇ�́A����1���炷
		}
		if(flag == false){								// ������Ȃ�������(�ʂ̋��E�G�b�W)
			e->pair = NULL;								// �΂�NULL
		}
	}

	return KOD_TRUE;
}

// �t�@�Z�b�g��MESH�Ɋi�[
int VRML_PARSER::GetFacets(FILE *fp,MESH *mesh)
{
	char *p,*q;
	char buf[BUFSIZEMAX],word[BUFSIZEMAX];
	int index[MAXVERTNUMINFACE]={-1,-1,-1,-1};	// ���_�̃C���f�b�N�X�Z�b�g
	int edgecount = 0;							// �G�b�W�̑���

	// �O�������W�l��S�ēǂݍ��ނ܂�1�s�Âǂݍ���
	while(!feof(fp)){
		GetLine(fp,buf);						// 1�s�擾
		if(strstr(buf,"coordIndex") == NULL)	// "coordIndex"�^�O��������Ȃ�������
			continue;							// ������܂œǂݑ�����

		//  "coordIndex"�^�O���������Ă����璸�_���W�擾
		while(GetLine(fp,buf)){							// 1�s�擾
			if(strchr(buf,']') != NULL) break;			// �f�[�^�I���������g�[�N��']'�����̍s��������ǂݍ��ݏI��
			int i=0;
			p = strtok(buf,",");						// �J���}��؂�ŕ�����𒊏o���Ă���
			sscanf(p,"%d",&index[i]);					// ���o����������𐮐��ɕϊ�
			i++;
			while(p != NULL){
				if((p = strtok(NULL,",")) != NULL){
					sscanf(p,"%d",&index[i]);
					if(index[i] == -1){					// �t�@�Z�b�g�̏I��������-1����������
						edgecount += i;					// �G�b�W�������C���N�������g
						SetMesh(mesh,index,edgecount);	// ����܂ł̃C���f�b�N�X�Z�b�g�Ń��b�V���𐶐�
						i = 0;							// �C���f�b�N�X�ԍ������Z�b�g
					}
					else i++;
				}
			}
		}

		return KOD_TRUE;	// ����ɓǂݍ��ݏI��
	}

	return KOD_ERR;			// �O�������W�l���擾���Ƀt�@�C�����I��:�G���[
}

// ���_�̃C���f�b�N�X�Z�b�g����A���b�V���f�[�^�𐶐�����
void VRML_PARSER::SetMesh(MESH *mesh,int vindex[],int edgecount)
{
	int vcount=MAXVERTNUMINFACE;
	HEface *f;
	HEvert *v;

	// ���_����F��
	for(int i=0;i<MAXVERTNUMINFACE;i++){
		if(vindex[i] == -1)
			vcount = i;
	}

	int startnum = edgecount - vcount;

	// �n�[�t�G�b�W��o�^
	for(int i=0;i<vcount;i++){
		HEedge *e = new HEedge();		// �n�[�t�G�b�W��1�������m��
		v = (HEvert *)mesh->Vert.getData(vindex[i]);
		e->SetVert(v);					// ��ɓo�^���Ă��������_���X�g���瓯���C���f�b�N�X�̂��̂�T���o���G�b�W�Ɗ֘A�t��
		v->SetEdge(e);
		e->SetIndex(startnum + i);		// �G�b�W�̃C���f�b�N�X��o�^
		e->mom = mesh->Edge.add(e);				// �n�[�t�G�b�W�����b�V���̃G�b�W���X�g�ɓo�^(e����ꂽ���̃A�h���X���ێ�)
		// �ʂ�o�^
		if(!i){
			f = new HEface();					// �ʂ�1�������m��
			f->SetEdge(e);						// �ʂɑ�����n�[�t�G�b�W��1����
			f->SetIndex(int(edgecount/vcount)-1);	// �ʂ̃C���f�b�N�X��o�^
			f->SetVertNum(vcount);				// �ʂ̒��_����o�^
			SetColorStat(&f->Dstat,0.2,0.2,1);	// �F�w��
			f->mom = mesh->Face.add(f);			// �ʂ����b�V���̖ʃ��X�g�ɓo�^(f����ꂽ���̃A�h���X���ێ�)
		}
		e->SetFace(f);							// �n�[�t�G�b�W��������ʂ�o�^
	}

	// �n�[�t�G�b�W�̎��ւ̃|�C���^�ƁA���ꂪ������ʂ�o�^
	HEedge *e_ = (HEedge *)mesh->Edge.setSentinel(startnum);		// 1�{�ڂ̃G�b�W�Ɋ��𗧂Ă�
	for(int i=0;i<vcount;i++){
		if(i==vcount-1){
			e_->ne = (HEedge *)mesh->Edge.getSentinelData();		// �Ō�̃G�b�W��next�͍ŏ��̃G�b�W
		}
		else{
			e_->ne = (HEedge *)mesh->Edge.getDataFromSentinel(i+1);	// �G�b�W�����X�g��
			e_ = e_->ne;
		}
	}

}

// �O�������W�l��MESH�Ɋi�[
int VRML_PARSER::GetCoords(FILE *fp,MESH *mesh)
{
	char *p,*q;
	char buf[BUFSIZEMAX],words[BUFSIZEMAX];
	int index=0;	// ���_�̃C���f�b�N�X
	Coord c;

	// �O�������W�l��S�ēǂݍ��ނ܂�1�s�Âǂݍ���
	while(!feof(fp)){
		GetLine(fp,buf);				// 1�s�擾
		if(strstr(buf,"point") == NULL)	// "point"�^�O��������Ȃ�������
			continue;					// ������܂œǂݑ�����

		//  "point"�^�O���������Ă����璸�_���W�擾
		while(GetLine(fp,buf)){							// 1�s�擾
			if(strchr(buf,']') != NULL) break;			// �f�[�^�I���������g�[�N��']'�����̍s��������ǂݍ��ݏI��
			HEvert *v = new HEvert();
			p = strtok(buf,",");						// �J���}��؂�ŕ�����𒊏o���Ă���
			sscanf(p,"%lf %lf %lf",&c.x,&c.y,&c.z);					// �f�[�^�i�[
			v->SetCod(c);
			v->SetIndex(index);
			v->mom = mesh->Vert.add(v);		// v�����X�g�ɓo�^(v����ꂽ���̃A�h���X���ێ�)
			//fprintf(stderr,"%p (%lf,%lf,%lf)\n",v,v->cod.x,v->cod.y,v->cod.z);	// debug
			index++;
			while(p != NULL){
				if((p = strtok(NULL,",")) != NULL){
					HEvert *v = new HEvert();
					sscanf(p,"%lf %lf %lf",&v->cod.x,&v->cod.y,&v->cod.z);
					v->index = index;
					v->mom = mesh->Vert.add(v);
					//fprintf(stderr,"%p (%lf,%lf,%lf)\n",v,v->cod.x,v->cod.y,v->cod.z);	// debug
					index++;
				}
			}
		}
		return KOD_TRUE;	// ����ɓǂݍ��ݏI��
	}

	return KOD_ERR;		// �O�������W�l���擾���Ƀt�@�C�����I��:�G���[
}

// �t�@�C������1�s�擾���A���s�������I�[�����ɕύX����
int VRML_PARSER::GetLine(FILE *fp,char buf[])
{
	fgets(buf,BUFSIZEMAX,fp);			// 1�s�擾
	int len = strlen(buf);				// 1�s�̒����𒲂ׂ�
	if(len > 0 && buf[len-1] == '\n')	// �Ō�̉��s����\n���I�[����\0�ɕύX
		buf[len-1] = '\0';

	return len-1;
}

// �^�O���
int VRML_PARSER::CheckTags(char buf[])
{
	if(strstr(buf,"AsciiText") != NULL)			return AsciiText;		// 1
	if(strstr(buf,"Cone") != NULL)				return Cone;			// 2
	if(strstr(buf,"Coordinate3") != NULL)		return Coordinate3;		// 3:�O�������W
	if(strstr(buf,"Cube") != NULL)				return Cube;			// 4
	if(strstr(buf,"Cylinder") != NULL)			return Cylinder;		// 5
	if(strstr(buf,"DirectionalLight") != NULL)	return DirectionalLight;// 6
	if(strstr(buf,"FontStyle") != NULL)			return FontStyle;		// 7
	if(strstr(buf,"Group") != NULL)				return Group;			// 8
	if(strstr(buf,"IndexedFaceSet") != NULL)	return IndexedFaceSet;	// 9:�t�@�Z�b�g���_�̃C���f�b�N�X
	if(strstr(buf,"IndexedLineSet") != NULL)	return IndexedLineSet;	// 10
	if(strstr(buf,"Info") != NULL)				return Info;			// 11
	if(strstr(buf,"LOD") != NULL)				return LOD;				// 12
	if(strstr(buf,"MaterialBinding") != NULL)	return MaterialBinding;	// 14
	if(strstr(buf,"Material") != NULL)			return Material;		// 13
	if(strstr(buf,"MatrixTransform") != NULL)	return MatrixTransform;	// 15
	if(strstr(buf,"NormalBinding") != NULL)		return NormalBinding;	// 17
	if(strstr(buf,"Normal") != NULL)			return Normal;			// 16
	if(strstr(buf,"OrthographicCamera") != NULL)return OrthographicCamera;	// 18
	if(strstr(buf,"PerspectiveCamera") != NULL)	return PerspectiveCamera;	// 19
	if(strstr(buf,"PointLight") != NULL)		return PointLight;		// 20
	if(strstr(buf,"PointSet") != NULL)			return PointSet;		// 21
	if(strstr(buf,"Rotation") != NULL)			return Rotation;		// 22
	if(strstr(buf,"Scale") != NULL)				return Scale;			// 23
	if(strstr(buf,"Separator") != NULL)			return Separator;		// 24
	if(strstr(buf,"ShapeHints") != NULL)		return ShapeHints;		// 25
	if(strstr(buf,"Sphere") != NULL)			return Sphere;			// 26
	if(strstr(buf,"SpotLight") != NULL)			return SpotLight;		// 27
	if(strstr(buf,"Switch") != NULL)			return Switch;			// 28
	if(strstr(buf,"Texture2Transform") != NULL)	return Texture2Transform;	// 30
	if(strstr(buf,"Texture2") != NULL)			return Texture2;		// 29
	if(strstr(buf,"TextureCoordinate2") != NULL)return TextureCoordinate2;	// 31
	if(strstr(buf,"TextureCoordinateBinding") != NULL)	return TextureCoordinateBinding;// 32
	if(strstr(buf,"TransformSeparator") != NULL)		return TransformSeparator;		// 34
	if(strstr(buf,"Transform") != NULL)			return Transform;		// 33
	if(strstr(buf,"Translation") != NULL)		return Translation;		// 35
	if(strstr(buf,"WWWAnchor") != NULL)			return WWWAnchor;		// 36
	if(strstr(buf,"WWWInline") != NULL)			return WWWInline;		// 37

	return KOD_ERR;	// TAG��������Ȃ�����
}
