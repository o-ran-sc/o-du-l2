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

/* This file constains a singleton class template */

#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

template<class T>
class Singleton
{
  public:
  static T& instance();
  static T  *instancePtr();

  protected:
  Singleton() { } 
  ~Singleton() { }

  private:
  static T* mPtr;
};

/* Define and initialize the static instance pointer */
template<class T>
T* Singleton<T>::mPtr = 0;


/********************************************************************** 
   Description : Check if instance already exists.
                 Create a new instance if none exists  
   Params[In]  : None
   Return      : Reference to the instance 
***********************************************************************/
template<class T>
T& Singleton<T>::instance()
{
    
  if( mPtr == 0)
    mPtr = new T();

  return *mPtr;
}


/********************************************************************** 
   Description : Check if instance already exists.
                 Create a new instance if none exists  
   Params[In]  : None
   Return      : Pointer to the instance 
***********************************************************************/
template<class T>
T *Singleton<T>::instancePtr()
{
    return &(instance());
}

#endif

/**********************************************************************
         End of file
**********************************************************************/

