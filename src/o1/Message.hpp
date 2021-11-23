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

/* This file contains Message base class for generic messages  */

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include "VesUtils.hpp"


class Message {

public:

    Message() {};
    virtual ~Message() {};
    
protected:
    MessageType mMsgType;
    virtual MessageType getMessageType() { return mMsgType; }

};

#endif

/**********************************************************************
         End of file
**********************************************************************/
