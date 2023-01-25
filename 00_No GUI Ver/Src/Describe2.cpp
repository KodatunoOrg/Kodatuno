/**********************************************************
* IGES_Parser()で読み込んだIGESファイルをOpenGLで描画する *
***********************************************************/

#include "Describe.h"


// IGESデータの描画
void DESCRIBE::glDrawBODY()
{	
	int i;
	BODY *body;

	// body描画
	for(int k=0;k<BodyList.getNum();k++){					// 現在リストに登録されているBODYの数だけループ
		if((body = (BODY *)BodyList.getData(k)) != NULL){	// k番目のリストに登録されているBODYのデータを取得
			glPushName(k);									// セレクションバッファを階層化
			// 各タイプごとに描画を実行していく(内部表現をNURBSに限定したので、線分や円などは描画しない)
			for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
				if(i == _CIRCLE_ARC){						// 円・円弧
					// 円・円弧はNRBS曲線に変換される
					//glPushName(_CIRCLE_ARC);				// セレクションバッファを階層化
					//Draw_CircleArc();
					//glPopName();
				}
				else if(i == _CONIC_ARC){					// 円錐曲線
					//glPushName(_CONIC_ARC);				// セレクションバッファを階層化
					//Draw_ConicArc();
					//glPopName();
				}
				else if(i == _LINE){						// 線分
					// 線分はNURBS曲線に変換される
					//glPushName(_LINE);					// セレクションバッファを階層化
					//Draw_Line();
					//glPopName();
				}
				else if(i == _NURBSC){						// NURBS曲線
					//glPushName(_NURBSC);					// セレクションバッファを階層化
					//Draw_NurbsCurve(body);
					//glPopName();
				}
				else if(i == _NURBSS){
					glPushName(_NURBSS);
					Draw_NurbsSurface(body);
					glPopName();
				}
				else if(i == _TRIMMED_SURFACE){				// トリム面(NURBS曲面)
					glPushName(_TRIMMED_SURFACE);
					Draw_TrimSurfe(body);
					glPopName();
				}
			}
			glPopName();
		}
	}
}

// 線分の描画
void DESCRIBE::glDraw_Line()
{
	for(int i=0;i<glbody->TypeNum[_LINE];i++){
		glPushName(i);		// ネームスタックの先頭にiを挿入
		glColor3f(glbody->Line[i].Dstat.Color[0],glbody->Line[i].Dstat.Color[1],glbody->Line[i].Dstat.Color[2]);
		if(glbody->Line[i].EntUseFlag == GEOMTRYELEM){				// IGESディレクトリ部の"Entity Use Flag"が0の場合は実際のモデル要素として描画する
			BH.DrawLine(glbody->Line[i]);
		}
		glPopName();	// ネームスタックの先頭を削除
	}
	glFlush();
}

// NURBS曲線の描画
void DESCRIBE::Draw_NurbsCurve(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSC];i++){
		glPushName(i);		// ネームスタックの先頭にiを挿入
		glColor3f(Curr_body->NurbsC[i].Dstat.Color[0],Curr_body->NurbsC[i].Dstat.Color[1],Curr_body->NurbsC[i].Dstat.Color[2]);
		if(Curr_body->NurbsC[i].EntUseFlag == GEOMTRYELEM){				// IGESディレクトリ部の"Entity Use Flag"が0の場合は実際のモデル要素として描画する
			BH.DrawNurbsCurve(Curr_body->NurbsC[i]);						// 描画
		}
		glPopName();		// ネームスタックの先頭を削除
	}
}

// NURBS曲面の描画
void DESCRIBE::Draw_NurbsSurface(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSS];i++){
		if(Curr_body->NurbsS[i].TrmdSurfFlag == KOD_TRUE)	// トリム面としてNURBS曲面が登録されているなら
			continue;		// 描画しない
		else{
			glPushName(i);
			BH.DrawNurbsSurfe(Curr_body->NurbsS[i]);	// NURBS曲面描画
			glPopName();
		}
	}
}
// トリム面の描画
void DESCRIBE::Draw_TrimSurfe(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_TRIMMED_SURFACE];i++){
		glPushName(i);			// ネームスタックの先頭にiを挿入
		BH.DrawTrimdSurf(Curr_body->TrmS[i]);
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