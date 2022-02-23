/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_UE_TimersAndConstants_H_
#define	_UE_TimersAndConstants_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum UE_TimersAndConstants__t300 {
	UE_TimersAndConstants__t300_ms100	= 0,
	UE_TimersAndConstants__t300_ms200	= 1,
	UE_TimersAndConstants__t300_ms300	= 2,
	UE_TimersAndConstants__t300_ms400	= 3,
	UE_TimersAndConstants__t300_ms600	= 4,
	UE_TimersAndConstants__t300_ms1000	= 5,
	UE_TimersAndConstants__t300_ms1500	= 6,
	UE_TimersAndConstants__t300_ms2000	= 7
} e_UE_TimersAndConstants__t300;
typedef enum UE_TimersAndConstants__t301 {
	UE_TimersAndConstants__t301_ms100	= 0,
	UE_TimersAndConstants__t301_ms200	= 1,
	UE_TimersAndConstants__t301_ms300	= 2,
	UE_TimersAndConstants__t301_ms400	= 3,
	UE_TimersAndConstants__t301_ms600	= 4,
	UE_TimersAndConstants__t301_ms1000	= 5,
	UE_TimersAndConstants__t301_ms1500	= 6,
	UE_TimersAndConstants__t301_ms2000	= 7
} e_UE_TimersAndConstants__t301;
typedef enum UE_TimersAndConstants__t310 {
	UE_TimersAndConstants__t310_ms0	= 0,
	UE_TimersAndConstants__t310_ms50	= 1,
	UE_TimersAndConstants__t310_ms100	= 2,
	UE_TimersAndConstants__t310_ms200	= 3,
	UE_TimersAndConstants__t310_ms500	= 4,
	UE_TimersAndConstants__t310_ms1000	= 5,
	UE_TimersAndConstants__t310_ms2000	= 6
} e_UE_TimersAndConstants__t310;
typedef enum UE_TimersAndConstants__n310 {
	UE_TimersAndConstants__n310_n1	= 0,
	UE_TimersAndConstants__n310_n2	= 1,
	UE_TimersAndConstants__n310_n3	= 2,
	UE_TimersAndConstants__n310_n4	= 3,
	UE_TimersAndConstants__n310_n6	= 4,
	UE_TimersAndConstants__n310_n8	= 5,
	UE_TimersAndConstants__n310_n10	= 6,
	UE_TimersAndConstants__n310_n20	= 7
} e_UE_TimersAndConstants__n310;
typedef enum UE_TimersAndConstants__t311 {
	UE_TimersAndConstants__t311_ms1000	= 0,
	UE_TimersAndConstants__t311_ms3000	= 1,
	UE_TimersAndConstants__t311_ms5000	= 2,
	UE_TimersAndConstants__t311_ms10000	= 3,
	UE_TimersAndConstants__t311_ms15000	= 4,
	UE_TimersAndConstants__t311_ms20000	= 5,
	UE_TimersAndConstants__t311_ms30000	= 6
} e_UE_TimersAndConstants__t311;
typedef enum UE_TimersAndConstants__n311 {
	UE_TimersAndConstants__n311_n1	= 0,
	UE_TimersAndConstants__n311_n2	= 1,
	UE_TimersAndConstants__n311_n3	= 2,
	UE_TimersAndConstants__n311_n4	= 3,
	UE_TimersAndConstants__n311_n5	= 4,
	UE_TimersAndConstants__n311_n6	= 5,
	UE_TimersAndConstants__n311_n8	= 6,
	UE_TimersAndConstants__n311_n10	= 7
} e_UE_TimersAndConstants__n311;
typedef enum UE_TimersAndConstants__t319 {
	UE_TimersAndConstants__t319_ms100	= 0,
	UE_TimersAndConstants__t319_ms200	= 1,
	UE_TimersAndConstants__t319_ms300	= 2,
	UE_TimersAndConstants__t319_ms400	= 3,
	UE_TimersAndConstants__t319_ms600	= 4,
	UE_TimersAndConstants__t319_ms1000	= 5,
	UE_TimersAndConstants__t319_ms1500	= 6,
	UE_TimersAndConstants__t319_ms2000	= 7
} e_UE_TimersAndConstants__t319;

/* UE-TimersAndConstants */
typedef struct UE_TimersAndConstants {
	long	 t300;
	long	 t301;
	long	 t310;
	long	 n310;
	long	 t311;
	long	 n311;
	long	 t319;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UE_TimersAndConstants_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_t300_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_t301_11;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_t310_20;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_n310_28;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_t311_37;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_n311_45;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_t319_54;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_UE_TimersAndConstants;
extern asn_SEQUENCE_specifics_t asn_SPC_UE_TimersAndConstants_specs_1;
extern asn_TYPE_member_t asn_MBR_UE_TimersAndConstants_1[7];

#ifdef __cplusplus
}
#endif

#endif	/* _UE_TimersAndConstants_H_ */
#include <asn_internal.h>
