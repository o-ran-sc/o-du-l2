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

/* This file contains methods of Session/Connection creation and Subscription to
   YANG modules */

#include <stdio.h>
#include <stdlib.h>
#include "sysrepo.h"
#include "SessionHandler.hpp"
#include "InitConfig.hpp"
#include "NrCellCb.hpp"
#include "NrCellDuCb.hpp"
#include "RrmPolicyCb.hpp"

using namespace std;
/* Default constructor */
SessionHandler::SessionHandler()
{    
}


/* Destructor */
SessionHandler::~SessionHandler()
{
}

/********************************************************************** 
   Description : This function will create Connection, Session, and 
                 subscribe. These sysrepo class provide netconf connection
                 related services.
   Params[In]  : void
   Return      : true  - started successful
                 false - start failed
**********************************************************************/
bool SessionHandler::init()
{
   try
   {
      mConn = createConnection();
      if(mConn != NULL)
      {
         O1_LOG("\nO1 SessionHandler : Connection created");
         //removing nacm module temperary for auth issue resolve
         //mConn.remove_module("ietf-netconf-acm");
         mSess = createSession(mConn);
         if(mSess != NULL)
         {
            O1_LOG("\nO1 SessionHandler : Session created");
            mSub  = createSubscribe(mSess);
            if(mSub != NULL)
            {
               O1_LOG("\nO1 SessionHandler : Subscription created");
               if(InitConfig::instance().init(mSess))
               {
                  return true;
               }
               else
               {
                  return false;
               }
            }
            else 
            {
               O1_LOG("\nO1 SessionHandler : Subscription failed");
               return false;
            }
         }
         else
         {
            O1_LOG("\nO1 SessionHandler : Session failed");
            return false;
         }
      }
      else
      {
         O1_LOG("\nO1 SessionHandler : connection failed");
         return false;
      }
   }
   catch( const std::exception& e )
   {
      O1_LOG("\nO1 SessionHandler : Exception : %s", e.what());
      return false;
   }
}

/********************************************************************** 
   Description : This function will create Connection instance and 
                 return the same  
   Params[In]  : void
   Return      : sysrepo::S_Connection instance
**********************************************************************/
sysrepo::S_Connection SessionHandler::createConnection()
{
   sysrepo::S_Connection conn(new sysrepo::Connection());
   return conn;
}


/********************************************************************** 
   Description : This function will create Session instance and
                 return the same
   Params[In]  : sysrepo::S_Connection
   Return      : sysrepo::S_Session instance
**********************************************************************/
sysrepo::S_Session SessionHandler::createSession(sysrepo::S_Connection conn)
{
   sysrepo::S_Session sess(new sysrepo::Session(conn));
   return sess;
}



/**********************************************************************
   Description : This function will create Subscribe instance and
                 return the same
   Params[In]  : sysrepo::S_Session
   Return      : sysrepo::S_Subscribe instance
**********************************************************************/
sysrepo::S_Subscribe SessionHandler::createSubscribe(sysrepo::S_Session sess)
{
   sysrepo::S_Subscribe subscrb(new sysrepo::Subscribe(sess));
   if(subscribeModule(subscrb))
   {
      O1_LOG("\nO1 SessionHandler : Subscription done successfully");
   }
   return subscrb;
}


/********************************************************************** 
   Description : This function will create a callback object and register
                 it for callback. 
   Params[In]  : sysrepo::S_Subscribe
   Return      : true   - on success
**********************************************************************/
bool SessionHandler::subscribeModule(sysrepo::S_Subscribe subscrb)
{
   sysrepo::S_Callback alarmOranCb(new AlarmOranYangModel());

   subscrb->oper_get_items_subscribe(ALARM_MODULE_NAME_ORAN, \
                                     ALARM_MODULE_PATH_ORAN, \
                                     alarmOranCb);

   sysrepo::S_Callback nrCellCb(new NrCellCb());

   subscrb->oper_get_items_subscribe(CELL_STATE_MODULE_NAME, \
                                     CELL_STATE_MODULE_PATH, \
                                     nrCellCb);
   subscrb->module_change_subscribe(CELL_STATE_MODULE_NAME, nrCellCb);

   sysrepo::S_Callback nrCellDuCb(new NrCellDuCb());
   subscrb->oper_get_items_subscribe(MANAGED_ELEMENT_MODULE_NAME, \
                                     MANAGED_ELEMENT_MODULE_PATH, \
                                     nrCellDuCb);
   subscrb->module_change_subscribe(MANAGED_ELEMENT_MODULE_NAME, nrCellDuCb);

   sysrepo::S_Callback rrmPolicyCb(new RrmPolicyCb());
   subscrb->oper_get_items_subscribe(RRMPOLICY_MODULE_NAME, \
                                     RRMPOLICY_MODULE_PATH, \
                                     rrmPolicyCb);
   subscrb->module_change_subscribe(RRMPOLICY_MODULE_NAME, rrmPolicyCb);

   return true;
}

/**********************************************************************
         End of file
**********************************************************************/
