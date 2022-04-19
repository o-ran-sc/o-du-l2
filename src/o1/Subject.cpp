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

#include <iostream>
#include "Subject.hpp"
#include "GlobalDefs.hpp"

/* Constructor*/
Subject::Subject() {
   O1_LOG("\nSubject created");
   this->message=NULL;
}

/* Default destructor*/
Subject::~Subject() {
   O1_LOG("\nO1 Subject :Subject destroyed");
}

/********************************************************************** 
   Description : Observer subscribing to Subject
   Params[In]  : Subject
   Return      : void 
**********************************************************************/
void Subject::registerObserver(Observer *observer)
{
   observersList.push_back(observer);
   O1_LOG("\nO1 Subject :observer registerd");
}

/********************************************************************** 
   Description : Observer unsubscribing to Subject
   Params[In]  : Subject
   Return      : void 
**********************************************************************/
void Subject::unregisterObserver(Observer *observer)
{
   observersList.remove(observer);
}

/********************************************************************** 
   Description : Subject sending message to subscribers
   Params[In]  : Subject
   Return      : void 
**********************************************************************/
void Subject::notifyObservers()
{
   std::list<Observer *>::iterator iterator = observersList.begin();
   while (iterator != observersList.end()) 
      {
        (*iterator)->update(message);
	O1_LOG("\nO1 Subject :message recived");
         iterator++;
      }
}

/********************************************************************** 
   Description : Observer subscribing to Subject
   Params[In]  : Subject
   Return      : void 
**********************************************************************/
void Subject::createMessage(char* message ) 
{
   this->message = message;
   notifyObservers();
}


/**********************************************************************
         End of file
**********************************************************************/
