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

/* This file contains functions to support the curl/http related operation*/

#include <string.h>
#include <stdlib.h>
#include <sstream>
#include "HttpClient.hpp"

/* Overloaded constructor */
HttpClient::HttpClient(string url, string username, string password):
                       mUrl(url), mServerUsername(username),
                       mServerPassword(password)
{

}

/*******************************************************************
 *
 * @brief set options in the curl object
 *
 * @details
 *
 *    Function : setCurlOptions
 *
 *    Functionality:
 *      - sets curl options in its object
 *
 *
 * @params[in] pointer to curl
 * @return true  : success
 *         false : failure
 ******************************************************************/

bool HttpClient::setCurlOptions(CURL *curl)
{
   const char *method = "POST";
   curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1L);
   curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
   curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, 1L);
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

    // disable SSL verifications
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
   curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYHOST, 0L);

   curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
   return true;
}

/*******************************************************************
 *
 * @brief prepare HTTP Header componentes
 *
 * @details
 *
 *    Function : prepareHttpHeader
 *
 *    Functionality:
 *      - prepares HTTP Header componentes and set in the
 *        header object
 *
 *
 * @params[in] pointer to curl and header
 * @return true  : success
 *         false : failure
 ******************************************************************/

bool HttpClient::prepareHttpHeader(struct curl_slist *header, CURL *curl)
{

   if(!setUserPassword(curl))
   {
      O1_LOG("\nO1 HttpClient : unable to set user:password");
      curl_slist_free_all(header);
   }
   setCurlOptions(curl);

   header = curl_slist_append(header, "Content-Type: application/json");
   if(!header) {
      O1_LOG("\nO1 HttpClient : curl_slist_append failed");
      curl_easy_cleanup(curl);
      return false;
   }

   header = curl_slist_append(header, "Accept: application/json");
   if(!header) {
      O1_LOG("\nO1 HttpClient : curl_slist_append failed");
      curl_easy_cleanup(curl);
      return false;
   }
   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
   
   curl_easy_setopt(curl, CURLOPT_URL, mUrl.c_str());
   
   return true;
}

/*******************************************************************
 *
 * @brief set username and password into the curl header
 *
 * @details
 *
 *    Function : setUserPassword
 *
 *    Functionality:
 *      - prepares username and password string and set into the
 *        header object
 *
 *
 * @params[in] pointer to curl
 * @return true  : success
 *         false : failure
 ******************************************************************/


bool HttpClient::setUserPassword(CURL *curl)
{
   //make string username:password
   char *credentials = 0;
   if((mServerUsername.c_str()) && (mServerPassword.c_str())) {
      asprintf(&credentials, "%s:%s", mServerUsername.c_str(), mServerPassword.c_str());
      if(credentials == 0) {
         O1_LOG("\nO1 HttpClient : Credential is blank");
         curl_easy_cleanup(curl);
         return false;
      }
   }
   else {
      return false;
   }
   curl_easy_setopt(curl, CURLOPT_USERPWD, credentials);
   free(credentials);
   return true;
}


/*******************************************************************
 *
 * @brief sends HTTP Request with help of curl API
 *
 * @details
 *
 *    Function : sendHttpRequest
 *
 *    Functionality:
 *      - set curl callback function
 *      - sends HTTP Request with help of curl API
 *
 *
 * @params[in] pointer to curl, pointer to header
 * @return true  : success
 *         false : failure
 ******************************************************************/


bool HttpClient::sendHttpRequest(struct curl_slist *header, CURL *curl)
{
   struct response response_data = {0};
   char **recvData = 0;
   int *response_code = 0;
   if(recvData) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_data);
    }

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(header);

    if(res != CURLE_OK) {
        O1_LOG("\nO1 HttpClient : Could not send data, error: %s", \
               curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return false;
    }

    if(response_code) {
        long http_rc;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_rc);
        *response_code = http_rc;
    }

    if(recvData) {
        *recvData = response_data.res;
    }
   return true;
}

/*******************************************************************
 *
 * @brief sends VES event to VES collector
 *
 * @details
 *
 *    Function : send
 *
 *    Functionality:
 *      - prepare curl header
 *      - sends VES event with help of curl API
 *
 *
 * @params[in] pointer to data buffer
 * @return true  : success
 *         false : failure
 ******************************************************************/

bool HttpClient::send(const char *sendData)
{
   assert(sendData);

   struct curl_slist *header = 0;
   const char *sendMsg = sendData;
   if(!sendMsg) {
       sendMsg = "";
   }

   CURL *curl = curl_easy_init();
   if(curl == 0) {
       O1_LOG("\nO1 HttpClient : could not initialize CURL");
       return false;
   }

   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sendMsg);
   if(!prepareHttpHeader(header, curl))
   {
      O1_LOG("\nO1 HttpClient : could not prepare HTTP header");
      curl_easy_cleanup(curl);
      return false;
   }
   else if(!sendHttpRequest(header, curl))
   {
      O1_LOG("\nO1 HttpClient : could not send HTTP request");
      return false;
   }
   curl_easy_cleanup(curl);
   return true;
}

/*******************************************************************
 *
 * @brief callback of VES event send
 *
 * @details
 *
 *    Function : writeCb
 *
 *    Functionality:
 *      - callback function of HTTP curl request
 *
 *
 * @params[in] void
 * @return size of return buffer  : success
 *         zero                   : failure
 ******************************************************************/

size_t HttpClient::writeCb(void *data, size_t size, size_t nmemb, void *userp) {
    O1_LOG("\nO1 HttpClient : write Callback called");
    size_t realsize = size * nmemb;
    struct response *mem = (struct response *)userp;

    char *ptr = (char *) realloc(mem->res, mem->size + realsize + 1);
    if(ptr == NULL) {
        O1_LOG("\nO1 HttpClient : realloc failed");
        return 0;
    }

    mem->res = ptr;
    memcpy(&(mem->res[mem->size]), data, realsize);
    mem->size += realsize;
    mem->res[mem->size] = 0;

    return realsize;
}

/**********************************************************************
  End of file
 **********************************************************************/
