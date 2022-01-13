/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "2022_E2AP.asn1"
 * 	`asn1c -D ./E2AP/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CauseE2_H_
#define	_CauseE2_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CauseRICrequest.h"
#include "CauseRICservice.h"
#include "CauseE2node.h"
#include "CauseTransport.h"
#include "CauseProtocol.h"
#include "CauseMisc.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CauseE2_PR {
	CauseE2_PR_NOTHING,	/* No components present */
	CauseE2_PR_ricRequest,
	CauseE2_PR_ricService,
	CauseE2_PR_e2Node,
	CauseE2_PR_transport,
	CauseE2_PR_protocol,
	CauseE2_PR_misc
	/* Extensions may appear below */
	
} CauseE2_PR;

/* CauseE2 */
typedef struct CauseE2 {
	CauseE2_PR present;
	union CauseE2_u {
		CauseRICrequest_t	 ricRequest;
		CauseRICservice_t	 ricService;
		CauseE2node_t	 e2Node;
		CauseTransport_t	 transport;
		CauseProtocol_t	 protocol;
		CauseMisc_t	 misc;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CauseE2_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CauseE2;
extern asn_CHOICE_specifics_t asn_SPC_CauseE2_specs_1;
extern asn_TYPE_member_t asn_MBR_CauseE2_1[6];
extern asn_per_constraints_t asn_PER_type_CauseE2_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _CauseE2_H_ */
#include <asn_internal.h>
