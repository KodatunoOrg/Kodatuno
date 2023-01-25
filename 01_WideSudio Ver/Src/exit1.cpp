#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void exit1(WSCbase* object){
  //do something...
}
static WSCfunctionRegister  op("exit1",(void*)exit1);
