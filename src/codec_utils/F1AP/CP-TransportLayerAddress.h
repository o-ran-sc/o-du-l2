/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_CP_TransportLayerAddress_H_
#define	_CP_TransportLayerAddress_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TransportLayerAddress.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CP_TransportLayerAddress_PR {
	CP_TransportLayerAddress_PR_NOTHING,	/* No components present */
	CP_TransportLayerAddress_PR_endpoint_IP_address,
	CP_TransportLayerAddress_PR_endpoint_IP_address_and_port,
	CP_TransportLayerAddress_PR_choice_extension
} CP_TransportLayerAddress_PR;

/* Forward declarations */
struct Endpoint_IP_address_and_port;
struct ProtocolIE_SingleContainer;

/* CP-TransportLayerAddress */
typedef struct CP_TransportLayerAddress {
	CP_TransportLayerAddress_PR present;
	union CP_TransportLayerAddress_u {
		TransportLayerAddress_t	 endpoint_IP_address;
		struct Endpoint_IP_address_and_port	*endpoint_IP_address_and_port;
		struct ProtocolIE_SingleContainer	*choice_extension;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CP_TransportLayerAddress_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CP_TransportLayerAddress;
extern asn_CHOICE_specifics_t asn_SPC_CP_TransportLayerAddress_specs_1;
extern asn_TYPE_member_t asn_MBR_CP_TransportLayerAddress_1[3];
extern asn_per_constraints_t asn_PER_type_CP_TransportLayerAddress_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _CP_TransportLayerAddress_H_ */
#include <asn_internal.h>
