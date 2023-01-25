//----------------------------------------------------//
// MWT C++ Application Source File                    //
//      created by WideStudio Application Builder     //
//----------------------------------------------------//
#include <WScom.h>
#include <WSDappDev.h>

//--- OBJECT includes ---//
#include <WSCmainWindow.h>
#include <WSCfileSelect.h>

//--- OBJECT instance variable ---//
WSCmainWindow* GeneralFile_Dlg = NULL;
WSCfileSelect* GeneFile_Dlg = NULL;

//--- OBJECT src ---//

WSCbase* _create_win_GeneralFile_Dlg(){


  GeneralFile_Dlg = new  WSCmainWindow(NULL,"GeneralFile_Dlg");
      GeneralFile_Dlg->initialize();
  GeneralFile_Dlg->setPropertyV(WSNname,"GeneralFile_Dlg");
  GeneralFile_Dlg->setPropertyV(WSNtitleString,"title2");
  GeneralFile_Dlg->setPropertyV(WSNx,(short)100);
  GeneralFile_Dlg->setPropertyV(WSNy,(short)100);
  GeneralFile_Dlg->setPropertyV(WSNwidth,(unsigned short)400);
  GeneralFile_Dlg->setPropertyV(WSNheight,(unsigned short)400);
  GeneralFile_Dlg->setPropertyV(WSNvis,(WSCbool)1);

  GeneFile_Dlg = new  WSCfileSelect(GeneralFile_Dlg,"GeneFile_Dlg");
      GeneFile_Dlg->initialize();
  GeneFile_Dlg->setPropertyV(WSNdirName,"C:/Documents and Settings/Keigo-Takasugi/My Documents/WideStudio/Kodatuno_ktaka/WS");
  GeneFile_Dlg->setPropertyV(WSNname,"GeneFile_Dlg");
  GeneFile_Dlg->setPropertyV(WSNtitleString,"title3");
  GeneFile_Dlg->setPropertyV(WSNx,(short)135);
  GeneFile_Dlg->setPropertyV(WSNy,(short)106);
  GeneFile_Dlg->setPropertyV(WSNwidth,(unsigned short)290);
  GeneFile_Dlg->setPropertyV(WSNheight,(unsigned short)360);
  GeneFile_Dlg->setPropertyV(WSNvis,(WSCbool)1);

   GeneralFile_Dlg->setVisible(True);
   return GeneralFile_Dlg;
}

//--- end of src ---//
