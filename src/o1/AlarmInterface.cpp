/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2021] [HCL Technologies Ltd.]                          #
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

/* This file contains C interfaces for ODU to raise and clear alarms */

#include <stdio.h>
#include <time.h>
#include "GlobalDefs.hpp"
#include "AlarmInterface.h"
#include "UnixSocketClient.hpp"

/*******************************************************************
 *
 * @brief Raise an alarm
 *
 * @details
 *
 *    Function : raiseAlarm
 *
 *    Functionality:
 *      - Raise an alarm by sending alarm info to O1 module over
 *        Unix socket with action set to RAISE
 *
 * @params[in] alarm information  
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
uint8_t raiseAlarm(AlarmRecord* alrm)
{

   UnixSocketClient uxClient(O1::ALARM_SOCK_PATH);
   if (uxClient.openSocket() == O1::FAILURE)
   {
      return O1::FAILURE;
   }
   alrm->msgHeader.msgType = ALARM;
   alrm->msgHeader.action = RAISE_ALARM;
   if (uxClient.sendData(alrm,sizeof(AlarmRecord)) < 0 )
   {
      uxClient.closeSocket();
      return O1::FAILURE;
   }
   uxClient.closeSocket();
   return O1::SUCCESS;
}

/*******************************************************************
 *
 * @brief Clear an alarm
 *
 * @details
 *
 *    Function : clearAlarm
 *
 *    Functionality:
 *      - Clears an alarm raised earlier by sending the alrm
 *        information to O1 module over Unix socket with action
 *        set to CLEAR
 *
 * @params[in] alarm information  
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
uint8_t clearAlarm(AlarmRecord* alrm)
{
   UnixSocketClient uxClient(O1::ALARM_SOCK_PATH);
   if (uxClient.openSocket() == O1::FAILURE)
   {
      return O1::FAILURE;
   }
   alrm->msgHeader.msgType = ALARM;
   alrm->msgHeader.action = CLEAR_ALARM;
   if (uxClient.sendData(alrm,sizeof(AlarmRecord)) < 0)
   {
      uxClient.closeSocket();
      return O1::FAILURE;
   }
   uxClient.closeSocket();
   return O1::SUCCESS;
}


/*******************************************************************
 *
 * @brief Raise a cell specific alarm
 *
 * @details
 *
 *    Function : raiseCellAlrm
 *
 *    Functionality:
 *      - Fills the cell specific alarm parmeters and generate
 *        the alarm
 *
 * @params[in] alarm Id, cell Id  
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
uint8_t raiseCellAlrm(uint16_t alrmId, uint16_t cellId)
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

/*******************************************************************
 *
 * @brief Clear the cell alarm
 *
 * @details
 *
 *    Function : clearCellAlrm
 *
 *    Functionality:
 *      - Clears the cell specific alarm using alarm id
 *
 * @params[in] alarm Id  
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
uint8_t clearCellAlrm(uint16_t alrmId)
{
  AlarmRecord alrm;
  snprintf (alrm.alarmId, sizeof(alrm.alarmId), "%d",alrmId);
  return clearAlarm(&alrm);
}
  

/**********************************************************************
         End of file
**********************************************************************/
