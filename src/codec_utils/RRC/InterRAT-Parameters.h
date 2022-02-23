/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_InterRAT_Parameters_H_
#define	_InterRAT_Parameters_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct EUTRA_Parameters;

/* InterRAT-Parameters */
typedef struct InterRAT_Parameters {
	struct EUTRA_Parameters	*eutra;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} InterRAT_Parameters_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_InterRAT_Parameters;

#ifdef __cplusplus
}
#endif

#endif	/* _InterRAT_Parameters_H_ */
#include <asn_internal.h>
