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

#include "Observer.hpp"
#include "GlobalDefs.hpp"
#include "Subject.hpp"

/* Default Constructor*/
Observer::Observer()
{

}

/* Default destructor*/
Observer::~Observer()
{

}

/**********************************************************************
   Description : Getting message from Subject
   Params[In]  : char*
   Return      : void
**********************************************************************/

void Observer::update(char* message) {
   this->mMessage = message;

}

/********************************************************************** 
   Description : Subscribe Observer to Subject
   Params[In]  : Subject
   Return      : void 
**********************************************************************/

void Observer::subscribe(Subject *subject)
{
   this->mSubject = subject;
   this->mSubject->registerObserver(this);
}

/********************************************************************** 
   Description : Unsubscribe observer from Subject
   Params[In]  : void
   Return      : void
**********************************************************************/

void Observer::unsubscribe() {
   this->mSubject->unregisterObserver(this);
}


/**********************************************************************
         End of file
**********************************************************************/
