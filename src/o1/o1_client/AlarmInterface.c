/*******************************************************************************
################################################################################
#   Copyright (c) [2020] [HCL Technologies Ltd.]                               #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/* This file contains interfaces to raise and clear alarms */

#include "AlarmInterface.h"
#include "TcpClient.h"


/********************************************************************** 
   Description : Raise an alarm by sending alarm info to O1 module over
                 TCP socket with action set to RAISE
   Params[In]  : Alarm information
   Return      : ROK     - success
                 RFAILED - failure
**********************************************************************/
int raiseAlarm(AlarmRecord* alrm)
{
   if (openSocket(TCP_SERVER_IP,TCP_PORT) == RFAILED)
   {
      return RFAILED;
   }
   alrm->msgHeader.action = RAISE;
   if (sendData(alrm,sizeof(AlarmRecord)) < 0 )
   {
      closeSocket();
      return RFAILED;
   }
   closeSocket();
   return ROK;
}

/********************************************************************** 
   Description : Clears an alarm raised earlier by sending the alrm
                 information to O1 module over TCP socket with action
                 set to CLEAR 
   Params[In]  : Alarm information
   Return      : ROK     - success
                 RFAILED - failure
**********************************************************************/
int clearAlarm(AlarmRecord* alrm)
{
   if (openSocket(TCP_SERVER_IP,TCP_PORT) == RFAILED)
   {
      return RFAILED;
   }
   alrm->msgHeader.action = CLEAR;
   if (sendData(alrm,sizeof(AlarmRecord)) < 0)
   {
      closeSocket();
      return RFAILED;
   }
   closeSocket();
   return ROK;
}


/********************************************************************** 
   Description : Fill the cell specific alarm parmeters and generate
                 the alarm
   Params[In]  : alarm Id, cell Id
   Return      : ROK     - success
                 RFAILED - failure
**********************************************************************/
int raiseCellAlrm(uint16_t alrmId, uint16_t cellId)
{
  char buff[BUFF_SIZE];
  time_t now = time(NULL);

  AlarmRecord alrm;
  alrm.eventType = COMMUNICATIONS_ALARM;
  snprintf (alrm.alarmId, sizeof(alrm.alarmId), "%d",alrmId);
  alrm.perceivedSeverity = INDETERMINATE;
  
  strftime(buff, BUFF_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&now));
  if(CELL_UP_ALARM_ID == alrmId)
  {
    /* Fill cell up parameters */
    snprintf (alrm.additionalText, sizeof(alrm.additionalText), \
             "cell id  [%d] is up",cellId);
    strcpy(alrm.additionalInfo , "cell UP");
    strcpy(alrm.specificProblem, "Active");
    strcpy(alrm.alarmRaiseTime, buff);
  }
  else
  {
    /* Clear the cell up alarm */
    clearCellAlrm(CELL_UP_ALARM_ID);
    /* Fill the cell down parameters */
    snprintf (alrm.additionalText, sizeof(alrm.additionalText), \
              "cell [%d] is down",cellId);
    strcpy(alrm.additionalInfo , "cell down");
    strcpy(alrm.specificProblem, "Active");
    strcpy(alrm.alarmRaiseTime, buff);
  }
  /*Raise the alarm */
  return raiseAlarm(&alrm);
}

/********************************************************************** 
   Description : Clear the cell alarm
   Params[In]  : alarm Id
   Return      : ROK     - success
                 RFAILED - failure
**********************************************************************/
int clearCellAlrm(uint16_t alrmId)
{
  AlarmRecord alrm;
  snprintf (alrm.alarmId, sizeof(alrm.alarmId), "%d",alrmId);
  return clearAlarm(&alrm);
}
  

/**********************************************************************
         End of file
**********************************************************************/
