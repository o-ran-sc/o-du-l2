/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SRS_Resources_H_
#define	_SRS_Resources_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SRS_Resources__maxNumberAperiodicSRS_PerBWP {
	SRS_Resources__maxNumberAperiodicSRS_PerBWP_n1	= 0,
	SRS_Resources__maxNumberAperiodicSRS_PerBWP_n2	= 1,
	SRS_Resources__maxNumberAperiodicSRS_PerBWP_n4	= 2,
	SRS_Resources__maxNumberAperiodicSRS_PerBWP_n8	= 3,
	SRS_Resources__maxNumberAperiodicSRS_PerBWP_n16	= 4
} e_SRS_Resources__maxNumberAperiodicSRS_PerBWP;
typedef enum SRS_Resources__maxNumberPeriodicSRS_PerBWP {
	SRS_Resources__maxNumberPeriodicSRS_PerBWP_n1	= 0,
	SRS_Resources__maxNumberPeriodicSRS_PerBWP_n2	= 1,
	SRS_Resources__maxNumberPeriodicSRS_PerBWP_n4	= 2,
	SRS_Resources__maxNumberPeriodicSRS_PerBWP_n8	= 3,
	SRS_Resources__maxNumberPeriodicSRS_PerBWP_n16	= 4
} e_SRS_Resources__maxNumberPeriodicSRS_PerBWP;
typedef enum SRS_Resources__maxNumberSemiPersitentSRS_PerBWP {
	SRS_Resources__maxNumberSemiPersitentSRS_PerBWP_n1	= 0,
	SRS_Resources__maxNumberSemiPersitentSRS_PerBWP_n2	= 1,
	SRS_Resources__maxNumberSemiPersitentSRS_PerBWP_n4	= 2,
	SRS_Resources__maxNumberSemiPersitentSRS_PerBWP_n8	= 3,
	SRS_Resources__maxNumberSemiPersitentSRS_PerBWP_n16	= 4
} e_SRS_Resources__maxNumberSemiPersitentSRS_PerBWP;
typedef enum SRS_Resources__maxNumberSRS_Ports_PerResource {
	SRS_Resources__maxNumberSRS_Ports_PerResource_n1	= 0,
	SRS_Resources__maxNumberSRS_Ports_PerResource_n2	= 1,
	SRS_Resources__maxNumberSRS_Ports_PerResource_n4	= 2
} e_SRS_Resources__maxNumberSRS_Ports_PerResource;

/* SRS-Resources */
typedef struct SRS_Resources {
	long	 maxNumberAperiodicSRS_PerBWP;
	long	 maxNumberAperiodicSRS_PerBWP_PerSlot;
	long	 maxNumberPeriodicSRS_PerBWP;
	long	 maxNumberPeriodicSRS_PerBWP_PerSlot;
	long	 maxNumberSemiPersitentSRS_PerBWP;
	long	 maxNumberSP_SRS_PerBWP_PerSlot;
	long	 maxNumberSRS_Ports_PerResource;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SRS_Resources_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_maxNumberAperiodicSRS_PerBWP_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_maxNumberPeriodicSRS_PerBWP_9;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_maxNumberSemiPersitentSRS_PerBWP_16;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_maxNumberSRS_Ports_PerResource_23;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SRS_Resources;
extern asn_SEQUENCE_specifics_t asn_SPC_SRS_Resources_specs_1;
extern asn_TYPE_member_t asn_MBR_SRS_Resources_1[7];

#ifdef __cplusplus
}
#endif

#endif	/* _SRS_Resources_H_ */
#include <asn_internal.h>
