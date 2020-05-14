using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceMan.Model
{
    public enum ServiceStatus
    {
        SERVICE_STOPPED				= 1,
        SERVICE_START_PENDING		= 2,
        SERVICE_STOP_PENDING		= 3,
        SERVICE_RUNNING				= 4,
        SERVICE_CONTINUE_PENDING	= 5,
        SERVICE_PAUSE_PENDING		= 6,
        SERVICE_PAUSED			    = 7,
    }
}
