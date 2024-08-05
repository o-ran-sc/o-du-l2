#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "/root/pborla/29July/l2/src/intel_fapi/fapi_interface.h"
#include "/root/pborla/29July/l2/src/intel_fapi/fapi_vendor_extension.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nfapi_interface.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nr5g_fapi_internal.h"
#include "fapi_interface.c"
// Define constants
#define PORT 8080
#define FAPI_MAX_NUM_TLVS_CONFIG 10
#define BUFFER_SIZE 65536


// Create a new queue element
p_fapi_api_queue_elem_t create_queue_elem(uint8_t msg_type, const void *msg_data, size_t data_size) {
    p_fapi_api_queue_elem_t elem = (p_fapi_api_queue_elem_t)malloc(sizeof(fapi_api_queue_elem_t) + data_size);
    if (elem) {
        elem->p_next = NULL;
        elem->p_tx_data_elm_list = NULL;
        elem->msg_type = msg_type;
        elem->num_message_in_block = 1;  // Example value
        elem->msg_len = data_size;
        elem->align_offset = 0;  // Example value
        elem->time_stamp = 0;  // Example value
        memcpy((char *)elem + sizeof(fapi_api_queue_elem_t), msg_data, data_size);
    }
    return elem;
}

// Process all messages in the queue
void process_queue(p_fapi_api_queue_elem_t head) {
    p_fapi_api_queue_elem_t current = head;
    while (current) {
        const char *msg_data = (const char *)current + sizeof(fapi_api_queue_elem_t);
        size_t msg_size = current->msg_len;

        switch (current->msg_type) {
            case FAPI_VENDOR_MSG_HEADER_IND:
                if (msg_size >= sizeof(fapi_vendor_msg_t)) {
                    fapi_vendor_msg_t *vendorMsg = (fapi_vendor_msg_t *)msg_data;
                    printf("Message size for vendor message\n");
                 //   handle_vendor_msg(vendorMsg);
                } else {
                    printf("Message size mismatch for vendor message\n");
                }
                break;
            case FAPI_CONFIG_REQUEST:
                if (msg_size >= sizeof(fapi_config_req_t)) {
                    fapi_config_req_t *configReq = (fapi_config_req_t *)msg_data;
                    printf("Message size for config request\n");
                    //handle_config_req(configReq);
                } else {
                    printf("Message size mismatch for config request\n");
                }
                break;
            default:
                printf("Unknown message type: %u\n", current->msg_type);
                break;
        }
        p_fapi_api_queue_elem_t next = current->p_next;
        free(current);
        current = next;
    }
}


void print_queue_elem(const fapi_api_queue_elem_t *elem) {
   if (elem == NULL) {
      printf("Element is NULL\n");
      return;
   }

   printf("Queue Element:\n");
   printf("  msg_type: %u\n", elem->msg_type);
   printf("  num_message_in_block: %u\n", elem->num_message_in_block);
   printf("  msg_len: %u\n", elem->msg_len);
   printf("  align_offset: %u\n", elem->align_offset);
   printf("  time_stamp: %llu\n", (unsigned long long)elem->time_stamp);
   printf("  p_next: %p\n", (void*)elem->p_next);
   printf("  p_tx_data_elm_list: %p\n", (void*)elem->p_tx_data_elm_list);
}
void print_hexdump(const uint8_t *buffer, size_t length) {
   if (buffer == NULL) {
      fprintf(stderr, "Buffer is NULL\n");
      return;
   }

   printf("Hexdump of %zu bytes:\n", length);
   for (size_t i = 0; i < length; ++i) {
      if (i % 16 == 0 && i != 0) {
         printf("\n");
      }
      printf("%02x ", buffer[i]);
   }
   printf("\n");
}

// Deserialization function
p_fapi_api_queue_elem_t deserialize_queue_elem(const uint8_t *buffer, size_t *offset) {
    p_fapi_api_queue_elem_t elem = (p_fapi_api_queue_elem_t)malloc(sizeof(p_fapi_api_queue_elem_t));
    if (!elem) return NULL;

    // Deserialize p_fapi_api_queue_elem_t 
   printf("elem->msg_type) of  %d %zu bytes:\n", *offset, elem->msg_type);
    memcpy(elem, buffer + *offset, sizeof(p_fapi_api_queue_elem_t));
    
    print_queue_elem(elem);
    print_hexdump(elem,   sizeof(p_fapi_api_queue_elem_t));
    *offset += sizeof(p_fapi_api_queue_elem_t);

    // Allocate space for the associated message and deserialize it
   printf("elem->msg_type) of  %d %zu bytes:\n", *offset, elem->msg_type);
    switch (elem->msg_type) {
        case FAPI_VENDOR_MSG_HEADER_IND: // fapi_msg_header_t
            elem->p_tx_data_elm_list = malloc(sizeof(fapi_msg_header_t));
            if (!elem->p_tx_data_elm_list) {
                free(elem);
                return NULL;
            }
            memcpy(elem->p_tx_data_elm_list, buffer + *offset, sizeof(fapi_msg_header_t));
            *offset += sizeof(fapi_msg_header_t);
            break;
        case FAPI_CONFIG_REQUEST: // fapi_config_req_t
            elem->p_tx_data_elm_list = malloc(sizeof(fapi_config_req_t));
            if (!elem->p_tx_data_elm_list) {
                free(elem);
                return NULL;
            }
            memcpy(elem->p_tx_data_elm_list, buffer + *offset, sizeof(fapi_config_req_t));
            *offset += sizeof(fapi_config_req_t);
            break;
        case FAPI_VENDOR_MESSAGE: // fapi_vendor_msg_t
            elem->p_tx_data_elm_list = malloc(sizeof(fapi_vendor_msg_t));
            if (!elem->p_tx_data_elm_list) {
                free(elem);
                return NULL;
            }
            memcpy(elem->p_tx_data_elm_list, buffer + *offset, sizeof(fapi_vendor_msg_t));
            *offset += sizeof(fapi_vendor_msg_t);
            break;
        default:
            fprintf(stderr, "Unknown message type: %u\n", elem->msg_type);
            free(elem);
            return NULL; // Indicate an error
    }

    elem->p_next = NULL; // Will be linked later
    return elem;
}
// Print function for fapi_msg_header_t
void print_msg_header(const fapi_msg_header_t *header) {
    printf("fapi_msg_header_t:\n");
    printf("  num_msg: %u\n", header->num_msg);
    printf("  handle: %u\n", header->handle);
    printf("  pad: %02x %02x\n", header->pad[0], header->pad[1]);
}

// Print function for fapi_config_req_t
void print_config_req(const fapi_config_req_t *config_req) {
    printf("fapi_config_req_t:\n");
    printf("  num_msg: %u\n", config_req->header.msg_id);
    printf("  handle: %u\n", config_req->header.length);
    printf("  number_of_tlvs: %u\n", config_req->number_of_tlvs);
    for (int i = 0; i < config_req->number_of_tlvs; i++) {
        printf("  tlvs[%d]: %u\n", i, config_req->tlvs[i]);
    }
}

// Print function for fapi_vendor_msg_t
void print_vendor_msg(const fapi_vendor_msg_t *vendor_msg) {
    printf("fapi_vendor_msg_t:\n");
    // Add fields to print as necessary
}

int main() {
    int listenfd, connfd,new_socket;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    uint8_t buffer[BUFFER_SIZE];
      ssize_t bytes_read;

    // Create socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(listenfd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(listenfd, 5) < 0) {
        perror("Listen failed");
        close(listenfd);
        exit(EXIT_FAILURE);
    }

    // Accept a connection
    new_socket = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    if (connfd < 0) {
       perror("Accept failed");
       close(listenfd);
       exit(EXIT_FAILURE);
    }
    p_fapi_api_queue_elem_t head = NULL;
    p_fapi_api_queue_elem_t tail = NULL;

    // Read data from the client
    while ((bytes_read = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
       printf("Received %zd bytes\n", bytes_read);

       // Deserialize and enqueue messages
       const char *ptr = buffer;
       while (ptr < buffer + bytes_read) {
          if (ptr + sizeof(fapi_msg_header_t) > buffer + bytes_read) {
             printf("Incomplete message header\n");
             break;
          }

          // Create and enqueue the message element
          p_fapi_api_queue_elem_t elem = create_queue_elem(header->num_msg, ptr, bytes_read);
          if (!head) {
             head = elem;
          } else {
             tail->p_next = elem;
          }
          tail = elem;

          ptr += msg_size;
       }

       // Process the entire queue
       process_queue(head);
       // Clear the buffer and reset the queue
       memset(buffer, 0, BUFFER_SIZE);
       head = NULL;
       tail = NULL;
    }

    if (bytes_read < 0) {
       perror("read");
    }

    // Close sockets
    close(connfd);
    close(listenfd);

    return 0;
}

