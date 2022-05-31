/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2022] [HCL Technologies Ltd.]                          #
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

#include <iostream>
#include "Subject.hpp"
#include "GlobalDefs.hpp"

/* Constructor*/
Subject::Subject() {
   mMessage=NULL;
}

/* Default destructor*/
Subject::~Subject() {

}

/**********************************************************************
   Description : Register an Observer
   Params[In]  : Observer
   Return      : void
**********************************************************************/
void Subject::registerObserver(Observer *observer)
{
   mObserversList.push_back(observer);
}

/**********************************************************************
   Description : Unregister an Observer
   Params[In]  : Observer
   Return      : void
**********************************************************************/
void Subject::unregisterObserver(Observer *observer)
{
   mObserversList.remove(observer);
}

/**********************************************************************
   Description : Notify all observers on new message
   Params[In]  : void
   Return      : void
**********************************************************************/
void Subject::notifyObservers()
{
   std::list<Observer *>::iterator iterator = mObserversList.begin();
   while (iterator != mObserversList.end())
      {
        (*iterator)->update(mMessage);
         iterator++;
      }
}

/**********************************************************************
   Description : Accept a message 
   Params[In]  : Message
   Return      : void
**********************************************************************/
void Subject::createMessage(char* message )
{
   mMessage = message;
   notifyObservers();
}


/**********************************************************************
         End of file
**********************************************************************/
