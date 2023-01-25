#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void open1(WSCbase* object){
  //do something...
}
static WSCfunctionRegister  op("open1",(void*)open1);
