/**********************************************************
* IGES_Parser()�œǂݍ���IGES�t�@�C����OpenGL�ŕ`�悷�� *
***********************************************************/

#include "Describe.h"


// IGES�f�[�^�̕`��
void DESCRIBE::glDrawBODY()
{	
	int i;
	BODY *body;

	// body�`��
	for(int k=0;k<BodyList.getNum();k++){					// ���݃��X�g�ɓo�^����Ă���BODY�̐��������[�v
		if((body = (BODY *)BodyList.getData(k)) != NULL){	// k�Ԗڂ̃��X�g�ɓo�^����Ă���BODY�̃f�[�^���擾
			glPushName(k);									// �Z���N�V�����o�b�t�@���K�w��
			// �e�^�C�v���Ƃɕ`������s���Ă���(�����\����NURBS�Ɍ��肵���̂ŁA������~�Ȃǂ͕`�悵�Ȃ�)
			for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
				if(i == _CIRCLE_ARC){						// �~�E�~��
					// �~�E�~�ʂ�NRBS�Ȑ��ɕϊ������
					//glPushName(_CIRCLE_ARC);				// �Z���N�V�����o�b�t�@���K�w��
					//Draw_CircleArc();
					//glPopName();
				}
				else if(i == _CONIC_ARC){					// �~���Ȑ�
					//glPushName(_CONIC_ARC);				// �Z���N�V�����o�b�t�@���K�w��
					//Draw_ConicArc();
					//glPopName();
				}
				else if(i == _LINE){						// ����
					// ������NURBS�Ȑ��ɕϊ������
					//glPushName(_LINE);					// �Z���N�V�����o�b�t�@���K�w��
					//Draw_Line();
					//glPopName();
				}
				else if(i == _NURBSC){						// NURBS�Ȑ�
					//glPushName(_NURBSC);					// �Z���N�V�����o�b�t�@���K�w��
					//Draw_NurbsCurve(body);
					//glPopName();
				}
				else if(i == _NURBSS){
					glPushName(_NURBSS);
					Draw_NurbsSurface(body);
					glPopName();
				}
				else if(i == _TRIMMED_SURFACE){				// �g������(NURBS�Ȗ�)
					glPushName(_TRIMMED_SURFACE);
					Draw_TrimSurfe(body);
					glPopName();
				}
			}
			glPopName();
		}
	}
}

// �����̕`��
void DESCRIBE::glDraw_Line()
{
	for(int i=0;i<glbody->TypeNum[_LINE];i++){
		glPushName(i);		// �l�[���X�^�b�N�̐擪��i��}��
		glColor3f(glbody->Line[i].Dstat.Color[0],glbody->Line[i].Dstat.Color[1],glbody->Line[i].Dstat.Color[2]);
		if(glbody->Line[i].EntUseFlag == GEOMTRYELEM){				// IGES�f�B���N�g������"Entity Use Flag"��0�̏ꍇ�͎��ۂ̃��f���v�f�Ƃ��ĕ`�悷��
			BH.DrawLine(glbody->Line[i]);
		}
		glPopName();	// �l�[���X�^�b�N�̐擪���폜
	}
	glFlush();
}

// NURBS�Ȑ��̕`��
void DESCRIBE::Draw_NurbsCurve(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSC];i++){
		glPushName(i);		// �l�[���X�^�b�N�̐擪��i��}��
		glColor3f(Curr_body->NurbsC[i].Dstat.Color[0],Curr_body->NurbsC[i].Dstat.Color[1],Curr_body->NurbsC[i].Dstat.Color[2]);
		if(Curr_body->NurbsC[i].EntUseFlag == GEOMTRYELEM){				// IGES�f�B���N�g������"Entity Use Flag"��0�̏ꍇ�͎��ۂ̃��f���v�f�Ƃ��ĕ`�悷��
			BH.DrawNurbsCurve(Curr_body->NurbsC[i]);						// �`��
		}
		glPopName();		// �l�[���X�^�b�N�̐擪���폜
	}
}

// NURBS�Ȗʂ̕`��
void DESCRIBE::Draw_NurbsSurface(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSS];i++){
		if(Curr_body->NurbsS[i].TrmdSurfFlag == KOD_TRUE)	// �g�����ʂƂ���NURBS�Ȗʂ��o�^����Ă���Ȃ�
			continue;		// �`�悵�Ȃ�
		else{
			glPushName(i);
			BH.DrawNurbsSurfe(Curr_body->NurbsS[i]);	// NURBS�Ȗʕ`��
			glPopName();
		}
	}
}
// �g�����ʂ̕`��
void DESCRIBE::Draw_TrimSurfe(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_TRIMMED_SURFACE];i++){
		glPushName(i);			// �l�[���X�^�b�N�̐擪��i��}��
		BH.DrawTrimdSurf(Curr_body->TrmS[i]);
		glPopName();			// �l�[���X�^�b�N�̐擪���폜
	}
}

// NURBS�Ȗʂ̃l�b�g�\��
void DESCRIBE::glDraw_SurfLine()
{
	Coord p;
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glColor3f(0.5,0.5,0.5);

	for(int i=0;i<glbody->TypeNum[_TRIMMED_SURFACE];i++){
		for(int j=0;j<11;j++){			
			glBegin(GL_LINE_STRIP);
			for(int k=0;k<51;k++){		// v�����p�����[�^���C���`��
				p = NFunc.CalcNurbsSCoord(glbody->TrmS[i].pts,0.1*(double)j,0.02*(double)k);
				glVertex3d(p.x,p.y,p.z);
			}
			glEnd();
			glBegin(GL_LINE_STRIP);
			for(int k=0;k<51;k++){		// u�����p�����[�^���C���`��
				p = NFunc.CalcNurbsSCoord(glbody->TrmS[i].pts,0.02*(double)k,0.1*(double)j);
				glVertex3d(p.x,p.y,p.z);
			}
			glEnd();
		}
	}
			
	glEnable(GL_LIGHTING);
}