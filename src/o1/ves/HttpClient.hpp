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

/* This file contains functions to support the preparation of VES common header
   parameters*/

#ifndef __HTTP_CLIENT_HPP__
#define __HTTP_CLIENT_HPP__

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include "VesUtils.hpp"
#include "JsonHelper.hpp"

using namespace std;

struct response {
    char *res;
    size_t size;
};

class HttpClient
{

   public:
      /* Default constructor/Destructor */
      HttpClient(){}
      ~HttpClient(){}

      HttpClient(string ip, string port, string username, string password);
      bool send(const char *send_data);

   private:
      //member functions
      static size_t writeCb(void *data, size_t size, size_t nmemb, void *userp);
      bool prepareHttpHeader(struct curl_slist *header, CURL *curl);
      bool sendHttpRequest(struct curl_slist *header, CURL *curl);
      bool setCurlOptions(CURL *curl);
      bool setUserPassword(CURL *curl);
      bool createUrl(CURL *curl);

      //member variable
      string mServerIp;
      string mServerPort;
      string mServerUsername;
      string mServerPassword;
};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
