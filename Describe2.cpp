/**********************************************************
* IGES_Parser()で読み込んだIGESファイルをOpenGLで描画する *
***********************************************************/

#include "Describe.h"


// IGESデータの描画
void DESCRIBE::glDrawBODY()
{	
	int i;
	Curr_body = glbody;

	// body描画
	while(Curr_body != NULL){
		glPushName(Curr_body->key);						// セレクションバッファを階層化

		// 各タイプごとに描画を実行していく(内部表現をNURBSに限定したので、線分や円などは描画しない)
		for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
			if(i == _CIRCLE_ARC){						// 円・円弧
				// 円・円弧はNRBS曲線に変換される
				//glPushName(_CIRCLE_ARC);				// セレクションバッファを階層化
				//glDraw_CircleArc();
				//glPopName();
			}
			else if(i == _CONIC_ARC){					// 円錐曲線
				//glPushName(_CONIC_ARC);				// セレクションバッファを階層化
				//glDraw_ConicArc();
				//glPopName();
			}
			else if(i == _LINE){						// 線分
				// 線分はNURBS曲線に変換される
				//glPushName(_LINE);					// セレクションバッファを階層化
				//glDraw_Line();
				//glPopName();
			}
			else if(i == _NURBSC){						// NURBS曲線
				glPushName(_NURBSC);					// セレクションバッファを階層化
				glDraw_NurbsCurve();
				glPopName();
			}
			else if(i == _TRIMMED_SURFACE){				// トリム面(NURBS曲面)
				glPushName(_TRIMMED_SURFACE);
				glDraw_TrimSurfe();
				glPopName();
			}
		}
		Curr_body = ShiftBodyList(Curr_body);
		glPopName();
	}
}

// 線分の描画
void DESCRIBE::glDraw_Line()
{
	for(int i=0;i<glbody->TypeNum[_LINE];i++){
		glPushName(i);		// ネームスタックの先頭にiを挿入
		glColor3f(glbody->Line[i].Dstat.Color[0],glbody->Line[i].Dstat.Color[1],glbody->Line[i].Dstat.Color[2]);
		if(glbody->Line[i].EntUseFlag == GEOMTRYELEM){				// IGESディレクトリ部の"Entity Use Flag"が0の場合は実際のモデル要素として描画する
			DrawLine(glbody->Line[i]);
		}
		glPopName();	// ネームスタックの先頭を削除
	}
	glFlush();
}

// NRBS曲線の描画
void DESCRIBE::glDraw_NurbsCurve()
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSC];i++){
		glPushName(i);		// ネームスタックの先頭にiを挿入
		glColor3f(Curr_body->NurbsC[i].Dstat.Color[0],Curr_body->NurbsC[i].Dstat.Color[1],Curr_body->NurbsC[i].Dstat.Color[2]);
		if(Curr_body->NurbsC[i].EntUseFlag == GEOMTRYELEM){				// IGESディレクトリ部の"Entity Use Flag"が0の場合は実際のモデル要素として描画する
			DrawNurbsCurve(Curr_body->NurbsC[i]);						// 描画
		}
		glPopName();		// ネームスタックの先頭を削除
	}
}

// トリム面の描画
void DESCRIBE::glDraw_TrimSurfe()
{
	for(int i=0;i<Curr_body->TypeNum[_TRIMMED_SURFACE];i++){
		glPushName(i);			// ネームスタックの先頭にiを挿入
		DrawTrimdSurf(Curr_body->TrmS[i]);
		glPopName();			// ネームスタックの先頭を削除
	}
}

// NURBS曲面のネット表示
void DESCRIBE::glDraw_SurfLine()
{
	Coord p;
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glColor3f(0.5,0.5,0.5);

	for(int i=0;i<glbody->TypeNum[_TRIMMED_SURFACE];i++){
		for(int j=0;j<11;j++){			
			glBegin(GL_LINE_STRIP);
			for(int k=0;k<51;k++){		// v方向パラメータライン描画
				p = NFunc.CalcNurbsSCoord(glbody->TrmS[i].pts,0.1*(double)j,0.02*(double)k);
				glVertex3d(p.x,p.y,p.z);
			}
			glEnd();
			glBegin(GL_LINE_STRIP);
			for(int k=0;k<51;k++){		// u方向パラメータライン描画
				p = NFunc.CalcNurbsSCoord(glbody->TrmS[i].pts,0.02*(double)k,0.1*(double)j);
				glVertex3d(p.x,p.y,p.z);
			}
			glEnd();
		}
	}
			
	glEnable(GL_LIGHTING);
}