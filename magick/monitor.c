/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%               M   M   OOO   N   N  IIIII  TTTTT   OOO   RRRR                %
%               MM MM  O   O  NN  N    I      T    O   O  R   R               %
%               M M M  O   O  N N N    I      T    O   O  RRRR                %
%               M   M  O   O  N  NN    I      T    O   O  R R                 %
%               M   M   OOO   N   N  IIIII    T     OOO   R  R                %
%                                                                             %
%                                                                             %
%                    ImageMagick Progress Monitor Methods                     %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                               December 1995                                 %
%                                                                             %
%                                                                             %
%  Copyright (C) 2002 ImageMagick Studio, a non-profit organization dedicated %
%  to making software imaging solutions freely available.                     %
%                                                                             %
%  Permission is hereby granted, free of charge, to any person obtaining a    %
%  copy of this software and associated documentation files ("ImageMagick"),  %
%  to deal in ImageMagick without restriction, including without limitation   %
%  the rights to use, copy, modify, merge, publish, distribute, sublicense,   %
%  and/or sell copies of ImageMagick, and to permit persons to whom the       %
%  ImageMagick is furnished to do so, subject to the following conditions:    %
%                                                                             %
%  The above copyright notice and this permission notice shall be included in %
%  all copies or substantial portions of ImageMagick.                         %
%                                                                             %
%  The software is provided "as is", without warranty of any kind, express or %
%  implied, including but not limited to the warranties of merchantability,   %
%  fitness for a particular purpose and noninfringement.  In no event shall   %
%  ImageMagick Studio be liable for any claim, damages or other liability,    %
%  whether in an action of contract, tort or otherwise, arising from, out of  %
%  or in connection with ImageMagick or the use or other dealings in          %
%  ImageMagick.                                                               %
%                                                                             %
%  Except as contained in this notice, the name of the ImageMagick Studio     %
%  shall not be used in advertising or otherwise to promote the sale, use or  %
%  other dealings in ImageMagick without prior written authorization from the %
%  ImageMagick Studio.                                                        %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%
*/

/*
  Include declarations.
*/
#include "studio.h"
#include "monitor.h"

/*
  Global declarations.
*/
static MonitorHandler
  monitor_handler = (MonitorHandler) NULL;

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   M a g i c k M o n i t o r                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  MagickMonitor() calls the monitor handler method with a text string that
%  describes the task and a measure of completion.
%
%  The format of the MagickMonitor method is:
%
%      void MagickMonitor(const char *text,const off_t quantum,
%        const size_t span,ExceptionInfo *exception)
%
%  A description of each parameter follows:
%
%    o quantum: The position relative to the span parameter which represents
%      how much progress has been made toward completing a task.
%
%    o span: The span relative to completing a task.
%
%    o exception: Return any errors or warnings in this structure.
%
*/
MagickExport unsigned int MagickMonitor(const char *text,const off_t quantum,
  const size_t span,ExceptionInfo *exception)
{
  assert(text != (const char *) NULL);
  ProcessPendingEvents(text);
  if (monitor_handler != (MonitorHandler) NULL)
    (*monitor_handler)(text,quantum,span,exception);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   S e t M o n i t o r H a n d l e r                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  SetMonitorHandler() sets the monitor handler to the specified method
%  and returns the previous monitor handler.
%
%  The format of the SetMonitorHandler method is:
%
%      MonitorHandler SetMonitorHandler(MonitorHandler handler)
%
%  A description of each parameter follows:
%
%    o handler: Specifies a pointer to a method to handle monitors.
%
%
*/
MagickExport MonitorHandler SetMonitorHandler(MonitorHandler handler)
{
  MonitorHandler
    previous_handler;

  previous_handler=monitor_handler;
  monitor_handler=handler;
  return(previous_handler);
}
