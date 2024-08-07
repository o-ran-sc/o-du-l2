#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../../src/intel_fapi/fapi_interface.h"
#include "../../src/intel_fapi/fapi_vendor_extension.h"
#include "../../src/intel_fapi/nfapi_interface.h"
#include "../../src/intel_fapi/nr5g_fapi_common_types.h"
#include "../../src/intel_fapi/nr5g_fapi_internal.h"
#include "fapi_interface.c"

#define PORT 8080



uint8_t processFapiMsg(uint8_t *buffer, size_t buffer_size) 
{
    if (buffer == NULL || buffer_size == 0) 
    {
        fprintf(stderr, "ERROR: Invalid buffer or buffer size\n");
        return 0;
    }
     
    if(buffer_size == sizeof(fapi_param_req_t))
    {
       printf("\nPrinting fapi_param_req\n");
       fapi_param_req_t *paramReq = (fapi_param_req_t *)buffer;
       decode_fapi_param_req(paramReq);
    }
    else if(buffer_size == sizeof(fapi_config_req_t))
    {
       printf("\nPrinting fapi_config_req_t\n");
       fapi_config_req_t *configReq = (fapi_config_req_t *)buffer;
       decode_fapi_config_req(configReq);
    }
    else if(buffer_size == sizeof(fapi_start_req_t))
    {
       printf("\nPrinting fapi_start_req_t\n");
       fapi_start_req_t *startReq = (fapi_start_req_t *)buffer;
       decode_fapi_start_req(startReq);
    }
    else if(buffer_size == sizeof(fapi_dl_tti_req_t))
    {
       printf("\nPrinting fapi_dl_tti_req_t\n");
       fapi_dl_tti_req_t *dlTtiReq = (fapi_dl_tti_req_t *)buffer;
       decode_fapi_dl_tti_req(dlTtiReq);
    }
    else if(buffer_size == sizeof(fapi_tx_data_req_t))
    {
       printf("\nPrinting fapi_tx_data_req_t\n");
       fapi_tx_data_req_t *txDataReq = (fapi_tx_data_req_t *)buffer;
       decode_fapi_tx_data_req(txDataReq);
    }
    else if(buffer_size == sizeof(fapi_stop_req_t))
    {
       printf("\nPrinting fapi_stop_req_t\n");
       fapi_stop_req_t *stopReq = (fapi_stop_req_t *)buffer;
       decode_fapi_stop_req(stopReq);
    }
    else if(buffer_size == sizeof(fapi_ul_dci_req_t))
    {
       printf("\nPrinting fapi_ul_dci_req_t\n");
       fapi_ul_dci_req_t *ulDciReq = (fapi_ul_dci_req_t *)buffer;
       decode_fapi_ul_dci_req(ulDciReq);
    }
    else if(buffer_size == sizeof(fapi_ul_tti_req_t))
    {
       printf("\nPrinting fapi_ul_tti_req_t\n");
       fapi_ul_tti_req_t *ulTtiReq = (fapi_ul_tti_req_t *)buffer;
       decode_fapi_ul_tti_req(ulTtiReq);
    }
       printf("\nPrinting fapi_config_req_t\n");
       printf("\nPrinting fapi_config_req_t\n");
    return 1;
}

int create_server_socket(int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Set up the server address struct
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        close(sockfd);
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 5);

    return sockfd;
}

int handle_client(int client_sockfd) 
{
    uint8_t buffer[60000];
    ssize_t bytes_received;
    size_t buffer_size;

    // Receive the message from the client
    bytes_received = recv(client_sockfd, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) 
    {
        perror("ERROR receiving message");
        return 0;
    }

    // Set buffer_size to the number of bytes received
    buffer_size = (size_t)bytes_received;
    printf("\n################################ bytes_received %lu", bytes_received);
    // Process the received message
    if(processFapiMsg(buffer, bytes_received) == 0)
    {
       printf("failed to process");
       return 0;
    }
    return 1;
}

int main() 
{
   int server_sockfd, client_sockfd;
   struct sockaddr_in cli_addr;
   socklen_t cli_len = sizeof(cli_addr);

   // Create server socket
   server_sockfd = create_server_socket(PORT);

   while (1) 
   {
      // Accept an incoming connection
      client_sockfd = accept(server_sockfd, (struct sockaddr *)&cli_addr, &cli_len);
      if (client_sockfd < 0) {
         perror("ERROR on accept");
         continue;
      }
      else
         break;
   }
   
   while(1)
   {
      printf("\n############################################################################################\n");
      // Handle the client connection
      if(handle_client(client_sockfd) == 0)
      {
         printf("\nfailed to handle the recevied message\n");
         break;
      }
      printf("\n############################################################################################\n");
   }
   // Close the server socket
   close(client_sockfd);
   close(server_sockfd);
   return 0;
}

