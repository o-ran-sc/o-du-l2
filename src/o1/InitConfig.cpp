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

/* This file contains methods of InitConfig and Interfaces to
   YANG modules */

#include "InitConfig.hpp"


/* Default constructor */
InitConfig::InitConfig()
{    
}


/* Destructor */
InitConfig::~InitConfig()
{  
}

/*******************************************************************
 *
 * @brief Initialize Configuration
 *
 * @details
 *
 *    Function : init 
 *
 *    Functionality:
 *      - Initialize Configuration
 *
 * @params[in] Reference to S_Session 
 * @return bool
 *
 ******************************************************************/

bool InitConfig::init(sysrepo::S_Session sess)
{
   mSess =  sess;
   InitConfig::InterfaceMap map;
   if(!getInterfaceConfig(sess, map))
   {
      O1_LOG("\nInitConfig::getInterfaceConfig InitConfig data error ");
      return false;
   }
   return true;
}

/*******************************************************************
 *
 * @brief Get running Interface Configuration
 *
 * @details
 *
 *    Function : getCurrInterfaceConfig 
 *
 *    Functionality:
 *      - Get running Interface Configuration
 *
 * @params[in] Reference of StartupConfig
 * @return bool
 *
 ******************************************************************/

bool InitConfig::getCurrInterfaceConfig(StartupConfig & cfg)
{
   //O1_LOG("\n InitConfig::getCurrInterfaceConfig entry");
   InitConfig::InterfaceMap::iterator it;
   for (it=mInterfaceList.begin(); it != mInterfaceList.end(); it++)
   {
      //O1_LOG("\n InitConfig::getCurrInterfaceConfig iterator");
      switch (it->first)
      {
         case Interface::ODU :
            if(it->second.first != "")
               strcpy(cfg.DU_IPV4_Addr, it->second.first.c_str());
            else
               return false;

            if(it->second.second != 0)
               cfg.DU_Port = (uint16_t)it->second.second;
            else
               return false;
            break;
 
         case Interface::OCU :
           
            if(it->second.first != "")
               strcpy(cfg.CU_IPV4_Addr, it->second.first.c_str());
            else
               return false;
            
          
            if(it->second.second != 0)
               cfg.CU_Port = (uint16_t) it->second.second;
            else
               return false;
         
            break;

      	 case Interface::RIC :

            if(it->second.first != "")
               strcpy(cfg.RIC_IPV4_Addr, it->second.first.c_str());
            else
               return false;
               
            
            if(it->second.second != 0)
               cfg.RIC_Port = (uint16_t) it->second.second;
            else
               return false;

            break;

         default :
            O1_LOG("\nno matching interface");
            break;
    }
   }
   return true;
}

/*******************************************************************
 *
 * @brief Get Interface Configuration
 *
 * @details
 *
 *    Function : getInterfaceConfig 
 *
 *    Functionality:
 *      - Get running Interface Configuration
 *
 * @params[in] Reference of S_Session
 * @return reference of InterfaceMap
 *
 ******************************************************************/
bool InitConfig::getInterfaceConfig(sysrepo::S_Session sess , InitConfig::InterfaceMap &map)
{
   InitConfig::Address oduAddr; 
   InitConfig::Address ocuAddr; 
   InitConfig::Address ricAddr; 
   if(getInterfaceData(sess,Interface::ODU, oduAddr) && \
      getInterfaceData(sess,Interface::OCU, ocuAddr) && \
      getInterfaceData(sess,Interface::RIC, ricAddr))
   {
      mInterfaceList.insert(std::make_pair(Interface::ODU, oduAddr));
      mInterfaceList.insert(std::make_pair(Interface::OCU, ocuAddr));
      mInterfaceList.insert(std::make_pair(Interface::RIC, ricAddr));
      map =  mInterfaceList; 
      return true;
   }
   else
   {
      return false;
   }
}

/*******************************************************************
 *
 * @brief Get Interface Data
 *
 * @details
 *
 *    Function : getInterfaceData 
 *
 *    Functionality:
 *      - Get running Interface Data 
 *
 * @params[in] Reference of S_Session and Interface
 * @return reference of Address
 *
 ******************************************************************/
bool InitConfig::getInterfaceData(sysrepo::S_Session sess, \
                                  Interface inf, InitConfig::Address & addr)
{
   //O1_LOG("\nInitConfig::getInterfaceData started");
   string sInf = interfaceToString(inf);
   string ip;
   string port;

   if(getData(sess, getInterfaceXpath(sInf, IP_ADDRESS), ip))
   {
      if(getData(sess, getInterfaceXpath(sInf, PORT), port))
      {
         addr = std::make_pair(ip, atoi(port.c_str()));
         return true;
      }
      else
         return false;
   }
   else
      return false;
}

/*******************************************************************
 *
 * @brief create xpath for interfaces
 *
 * @details
 *
 *    Function : getInterfaceXpath
 *
 *    Functionality:
 *      - create xpath for interfaces
 *
 * @params[in] String of sInf and String of param
 * @return pointer to xpath
 *
 ******************************************************************/
char * InitConfig::getInterfaceXpath( string sInf, string param)
{
   sprintf(xpath, "%s/interfaces/interface[interface-name='%s']/%s", \
           INTERFACE_MODULE_NAME_ORAN, sInf.c_str(),param.c_str());
   return xpath;
}

/*******************************************************************
 *
 * @brief  Get data of the xpath
 *
 * @details
 *
 *    Function : getData
 *
 *    Functionality:
 *      -  get data of the xpath
 *
 * @params[in] reference of S_Session and pointer to xpath
 * @return value against the xpath
 *
 ******************************************************************/

bool InitConfig::getData(sysrepo::S_Session sess,char* xpath, string &val)
{
   //O1_LOG("\nInitConfig::getData of xpath = %s", \
              xpath); //enable for debugging only
   try
   {
   auto value = sess->get_item(xpath);
   if (value == nullptr)
   {
      O1_LOG("\nInitConfig::getData no data available at xpath= %s", \
              xpath);
      //O1_LOG("\nget_item value are null for xpath = %s", \
                 xpath); //enable for debugging only
      return false;
   }
   val = value->val_to_string();
   return true;

   }
   catch (...)
   {
      O1_LOG("\nInitConfig::getData exception occured for xpath= %s", \
              xpath);
      return false; 
   }
}
 
/*******************************************************************
 *
 * @brief  Convert interface value to string
 *
 * @details
 *
 *    Function : interfaceToString
 *
 *    Functionality:
 *      -  Convert interface value to string
 *
 * @params[in] reference to Interface
 * @return string value of Interface
 *
 ******************************************************************/
string InitConfig::interfaceToString(Interface inf)
{
   string interface;
    switch (inf)
    {
	case Interface::ODU :
           interface = "odu";
           break; 
   case Interface::OCU :
           interface = "ocu"; 
           break;
	case Interface::RIC :
           interface = "ric";
           break;
   default :
          O1_LOG("\n\n no matching interface ");
          break;
    }
    return interface;
 
}


/*******************************************************************
 *
 * @brief  Print Interface configuration
 *
 * @details
 *
 *    Function : printInterfaceConfig
 *
 *    Functionality:
 *      -  Print Interface configuration
 *
 * @params[in] void
 * @return bool
 *
 ******************************************************************/
bool InitConfig::printInterfaceConfig()
{
   InitConfig::InterfaceMap::iterator it;
   for (it=mInterfaceList.begin(); it != mInterfaceList.end(); it++)
   {
      O1_LOG("\ninterface [%s] : \n \t IP = %s \n \t Port = %d", \
              interfaceToString(it->first).c_str(), it->second.first.c_str(), \
              it->second.second);
   }
   return true;
}

/**********************************************************************
         End of file
**********************************************************************/
