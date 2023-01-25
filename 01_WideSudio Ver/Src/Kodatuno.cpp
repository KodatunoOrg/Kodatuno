//WINDOW BUILDER PROJECT MASTER SRC//
#include <WScom.h>
#include <WSDappDev.h>
#include <WSCbaseList.h>

#include <WSClocaleSet.h>

#ifdef MSW
WS_WINMAIN
#endif

char* mou[] = {
WS_DEFAULT_MOUSE_ID,
NULL};

void _mouse_init_(){
  WSGFloadMouseCursorData(mou);
}

char* fon[] = {
"13,0,0,0,400,0,0,0,128,3,2,1,1,ＭＳ ゴシック",
"15,0,0,0,400,0,0,0,128,3,2,1,1,ＭＳ ゴシック",
"17,0,0,0,400,0,0,0,128,3,2,1,1,ＭＳ ゴシック",
"19,0,0,0,400,0,0,0,128,3,2,1,1,ＭＳ ゴシック",
"22,0,0,0,400,0,0,0,128,3,2,1,1,ＭＳ ゴシック",
"24,0,0,0,400,0,0,0,128,3,2,1,1,ＭＳ ゴシック",
"26,0,0,0,400,0,0,0,128,3,2,1,1,ＭＳ ゴシック",
"34,0,0,0,400,0,0,0,128,3,2,1,1,ＭＳ ゴシック",
NULL};

void _font_init_(){
  WSGFloadFontData(fon);
}
class _init_app_class_{
  public: _init_app_class_(){
    WSGFdeviceInitialize();
    WSGIappDev()->setInitHandler(_font_init_);
    WSGIappDev()->setInitHandler(_mouse_init_);
    WSGIappLocaleSet()->setDefaultEncoding(21);
    WSDappDev* app = WSGIappDev();
    app->setInstanceName("Kodatuno");
  };
};
static _init_app_class_ _execute_app_class_;

//--- OBJECT execute auto initialize ---//
extern WSDLEX WSCbase* _create_win_Kodatuno_Win();
class _initialize_window_0{
 public: _initialize_window_0(){
  WSGIappDev()->setInitHandler((void(*)())_create_win_Kodatuno_Win);
 };
};
static _initialize_window_0 _execute_initialize_0;

