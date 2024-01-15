#define DATABUF_H_INC_LEVEL2

#include <epicsThread.h>
#include <epicsTime.h>
#include <generalTimeSup.h>

#include <epicsExport.h>

#include "evgOutput.h"
#include "evgAcTrig.h"
#include "evgDbus.h"
#include "evgInput.h"
#include "evgTrigEvt.h"
#include "evgMxc.h"
#include "evgEvtClk.h"
#include "evgMrm.h"

namespace mrf
{
  template <>
  ObjectInst<evgAcTrig, Object>::m_props_t *ObjectInst<evgAcTrig, Object>::m_props = 0;
  template <>
  int ObjectInst<evgAcTrig, Object>::initObject()
  {
    const char *klassname = "evgAcTrig";
    (void)klassname;
    try
    {
      mrf::auto_ptr<m_props_t> props(new m_props_t);
      {

        props->insert(std::make_pair(static_cast<const char *>("Divider"), detail::makeUnboundProperty("Divider", &evgAcTrig::getDivider, &evgAcTrig::setDivider)));
      }
      m_props = props.release();
      return 1;
    }
    catch (std::exception &e)
    {
      std::cerr << "Failed to build property table for " << typeid(evgAcTrig).name() << "\n"
                << e.what() << "\n";
      throw std::runtime_error("Failed to build");
    }
  }
}
static int done_evgAcTrig __attribute__((unused)) = evgAcTrig::initObject();
