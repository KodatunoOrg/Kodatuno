#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void close1(WSCbase* object){
  //do something...
}
static WSCfunctionRegister  op("close1",(void*)close1);
