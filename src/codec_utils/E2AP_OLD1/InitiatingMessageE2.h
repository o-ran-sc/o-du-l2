/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Descriptions"
 * 	found in "E2AP.asn1"
 * 	`asn1c -D ./out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_InitiatingMessageE2_H_
#define	_InitiatingMessageE2_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProcedureCodeE2.h"
#include "CriticalityE2.h"
#include <ANY.h>
#include <asn_ioc.h>
#include "RICsubscriptionRequest.h"
#include "RICsubscriptionResponse.h"
#include "RICsubscriptionFailure.h"
#include "RICsubscriptionDeleteRequest.h"
#include "RICsubscriptionDeleteResponse.h"
#include "RICsubscriptionDeleteFailure.h"
#include "RICserviceUpdate.h"
#include "RICserviceUpdateAcknowledge.h"
#include "RICserviceUpdateFailure.h"
#include "RICcontrolRequest.h"
#include "RICcontrolAcknowledge.h"
#include "RICcontrolFailure.h"
#include "E2setupRequest.h"
#include "E2setupResponse.h"
#include "E2setupFailure.h"
#include "ResetRequest.h"
#include "ResetResponse.h"
#include "RICindication.h"
#include "RICserviceQuery.h"
#include "ErrorIndicationE2.h"
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum InitiatingMessageE2__value_PR {
	InitiatingMessageE2__value_PR_NOTHING,	/* No components present */
	InitiatingMessageE2__value_PR_RICsubscriptionRequest,
	InitiatingMessageE2__value_PR_RICsubscriptionDeleteRequest,
	InitiatingMessageE2__value_PR_RICserviceUpdate,
	InitiatingMessageE2__value_PR_RICcontrolRequest,
	InitiatingMessageE2__value_PR_E2setupRequest,
	InitiatingMessageE2__value_PR_ResetRequest,
	InitiatingMessageE2__value_PR_RICindication,
	InitiatingMessageE2__value_PR_RICserviceQuery,
	InitiatingMessageE2__value_PR_ErrorIndicationE2
} InitiatingMessageE2__value_PR;

/* InitiatingMessageE2 */
typedef struct InitiatingMessageE2 {
	ProcedureCodeE2_t	 procedureCode;
	CriticalityE2_t	 criticality;
	struct InitiatingMessageE2__value {
		InitiatingMessageE2__value_PR present;
		union InitiatingMessageE2__value_u {
			RICsubscriptionRequest_t	 RICsubscriptionRequest;
			RICsubscriptionDeleteRequest_t	 RICsubscriptionDeleteRequest;
			RICserviceUpdate_t	 RICserviceUpdate;
			RICcontrolRequest_t	 RICcontrolRequest;
			E2setupRequest_t	 E2setupRequest;
			ResetRequest_t	 ResetRequest;
			RICindication_t	 RICindication;
			RICserviceQuery_t	 RICserviceQuery;
			ErrorIndicationE2_t	 ErrorIndicationE2;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} value;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} InitiatingMessageE2_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_InitiatingMessageE2;
extern asn_SEQUENCE_specifics_t asn_SPC_InitiatingMessageE2_specs_1;
extern asn_TYPE_member_t asn_MBR_InitiatingMessageE2_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _InitiatingMessageE2_H_ */
#include <asn_internal.h>
