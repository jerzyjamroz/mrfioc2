
#include <stdlib.h>
#include <epicsExport.h>
#include <dbAccess.h>
#include <devSup.h>
#include <recGbl.h>
#include <devLib.h> // For S_dev_*

#include <longinRecord.h>
#include <longoutRecord.h>
#include <biRecord.h>
#include <boRecord.h>

#include "cardmap.h"
#include "evr/evr.h"
#include "property.h"

#include <stdexcept>
#include <string>

/***************** BI/BO *****************/

static long binary_init_record(dbCommon *prec, DBLINK* lnk)
{
try {
//  prec->dpvt=static_cast<void*>(priv);

  EVR* card=getEVR<EVR>(lnk->value.vmeio.card);
  if(!card)
    throw std::runtime_error("Failed to lookup device");

  property<EVR,bool> *prop;
  std::string parm(lnk->value.vmeio.parm);

  if( parm=="Enable" ){
    prop=new property<EVR,bool>(
        card,
        &EVR::enabled,
        &EVR::enable
    );
  }else if( parm=="PLL Lock Status" ){
    prop=new property<EVR,bool>(
        card,
        &EVR::pllLocked,
        0,
        &EVR::pllChanged
    );
  }else
    throw std::runtime_error("Invalid parm string in link");

  prec->dpvt=static_cast<void*>(prop);

  return 0;

} catch(std::runtime_error& e) {
  recGblRecordError(S_dev_noDevice, (void*)prec, e.what());
  return S_dev_noDevice;
} catch(std::exception& e) {
  recGblRecordError(S_db_noMemory, (void*)prec, e.what());
  return S_db_noMemory;
}
}

static long init_bi(biRecord *pbi)
{
  return binary_init_record((dbCommon*)pbi, &pbi->inp);
}

static long read_bi(biRecord* pbi)
{
try {
  property<EVR,bool> *priv=static_cast<property<EVR,bool>*>(pbi->dpvt);

  pbi->rval = priv->get();

  return 0;
} catch(std::exception& e) {
  recGblRecordError(S_db_noMemory, (void*)pbi, e.what());
  return S_db_noMemory;
}
}

static long init_bo(boRecord *pbo)
{
  return binary_init_record((dbCommon*)pbo, &pbo->out);
}

static long get_ioint_info_bi(int dir,dbCommon* prec,IOSCANPVT* io)
{
  return get_ioint_info<EVR,bool,bool>(dir,prec,io);
}

static long write_bo(boRecord* pbo)
{
try {
  property<EVR,bool> *priv=static_cast<property<EVR,bool>*>(pbo->dpvt);

  priv->set(pbo->rval);

  return 0;
} catch(std::exception& e) {
  recGblRecordError(S_db_noMemory, (void*)pbo, e.what());
  return S_db_noMemory;
}
}

extern "C" {

struct {
  long num;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  get_ioint_info;
  DEVSUPFUN  read;
} devBIEVR = {
  5,
  NULL,
  NULL,
  (DEVSUPFUN) init_bi,
  (DEVSUPFUN) get_ioint_info_bi,
  (DEVSUPFUN) read_bi
};
epicsExportAddress(dset,devBIEVR);

struct {
  long num;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  get_ioint_info;
  DEVSUPFUN  write;
} devBOEVR = {
  5,
  NULL,
  NULL,
  (DEVSUPFUN) init_bo,
  NULL,
  (DEVSUPFUN) write_bo
};
epicsExportAddress(dset,devBOEVR);

};

/***************** Longin/Longout *****************/

static long long_init_record(dbCommon *prec, DBLINK* lnk)
{
try {
//  prec->dpvt=static_cast<void*>(priv);

  EVR* card=getEVR<EVR>(lnk->value.vmeio.card);
  if(!card)
    throw std::runtime_error("Failed to lookup device");

  property<EVR,epicsUInt32> *prop;
  std::string parm(lnk->value.vmeio.parm);

  if( parm=="Model" ){
    prop=new property<EVR,epicsUInt32>(
        card,
        &EVR::model
    );
  }else if( parm=="Version" ){
    prop=new property<EVR,epicsUInt32>(
        card,
        &EVR::version
    );
  }else if( parm=="PLL Control" ){
    prop=new property<EVR,epicsUInt32>(
        card,
        &EVR::pllCtrl,
        &EVR::pllSetCtrl
    );
  }else if( parm=="Event Clock Div" ){
    prop=new property<EVR,epicsUInt32>(
        card,
        &EVR::eventClockDiv,
        &EVR::setEventClockDiv
    );
  }else if( parm=="Receive Error" ){
    prop=new property<EVR,epicsUInt32>(
        card,
        &EVR::recvErrorCount,
        0,
        &EVR::recvError
    );
  }else if( parm=="Timestamp Prescaler" ){
    prop=new property<EVR,epicsUInt32>(
        card,
        &EVR::tsDiv,
        &EVR::setTsDiv
    );
  }else if( parm=="Timestamp Seconds" ){
    prop=new property<EVR,epicsUInt32>(
        card,
        &EVR::tsLatchSec
    );
  }else if( parm=="Timestamp Counter" ){
    prop=new property<EVR,epicsUInt32>(
        card,
        &EVR::tsLatchCount
    );
  }else
    throw std::runtime_error("Invalid parm string in link");

  prec->dpvt=static_cast<void*>(prop);

  return 0;

} catch(std::runtime_error& e) {
  recGblRecordError(S_dev_noDevice, (void*)prec, e.what());
  return S_dev_noDevice;
} catch(std::exception& e) {
  recGblRecordError(S_db_noMemory, (void*)prec, e.what());
  return S_db_noMemory;
}
}

static long init_li(longinRecord *pli)
{
  return long_init_record((dbCommon*)pli, &pli->inp);
}

static long read_li(longinRecord* pli)
{
try {
  property<EVR,epicsUInt32> *priv=static_cast<property<EVR,epicsUInt32>*>(pli->dpvt);

  pli->val = priv->get();

  return 0;
} catch(std::exception& e) {
  recGblRecordError(S_db_noMemory, (void*)pli, e.what());
  return S_db_noMemory;
}
}

static long init_lo(longoutRecord *plo)
{
  return binary_init_record((dbCommon*)plo, &plo->out);
}

static long get_ioint_info_li(int dir,dbCommon* prec,IOSCANPVT* io)
{
  return get_ioint_info<EVR,epicsUInt32,epicsUInt32>(dir,prec,io);
}

static long write_lo(longoutRecord* plo)
{
try {
  property<EVR,epicsUInt32> *priv=static_cast<property<EVR,epicsUInt32>*>(plo->dpvt);

  priv->set(plo->val);

  return 0;
} catch(std::exception& e) {
  recGblRecordError(S_db_noMemory, (void*)plo, e.what());
  return S_db_noMemory;
}
}

extern "C" {

struct {
  long num;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  get_ioint_info;
  DEVSUPFUN  read;
} devLIEVR = {
  5,
  NULL,
  NULL,
  (DEVSUPFUN) init_li,
  (DEVSUPFUN) get_ioint_info_li,
  (DEVSUPFUN) read_li
};
epicsExportAddress(dset,devLIEVR);

struct {
  long num;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  get_ioint_info;
  DEVSUPFUN  write;
} devLOEVR = {
  5,
  NULL,
  NULL,
  (DEVSUPFUN) init_lo,
  NULL,
  (DEVSUPFUN) write_lo
};
epicsExportAddress(dset,devLOEVR);

};