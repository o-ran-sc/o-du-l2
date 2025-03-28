/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_PTRS_UplinkConfig_H_
#define	_PTRS_UplinkConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PTRS_UplinkConfig__transformPrecoderDisabled__maxNrofPorts {
	PTRS_UplinkConfig__transformPrecoderDisabled__maxNrofPorts_n1	= 0,
	PTRS_UplinkConfig__transformPrecoderDisabled__maxNrofPorts_n2	= 1
} e_PTRS_UplinkConfig__transformPrecoderDisabled__maxNrofPorts;
typedef enum PTRS_UplinkConfig__transformPrecoderDisabled__resourceElementOffset {
	PTRS_UplinkConfig__transformPrecoderDisabled__resourceElementOffset_offset01	= 0,
	PTRS_UplinkConfig__transformPrecoderDisabled__resourceElementOffset_offset10	= 1,
	PTRS_UplinkConfig__transformPrecoderDisabled__resourceElementOffset_offset11	= 2
} e_PTRS_UplinkConfig__transformPrecoderDisabled__resourceElementOffset;
typedef enum PTRS_UplinkConfig__transformPrecoderDisabled__ptrs_Power {
	PTRS_UplinkConfig__transformPrecoderDisabled__ptrs_Power_p00	= 0,
	PTRS_UplinkConfig__transformPrecoderDisabled__ptrs_Power_p01	= 1,
	PTRS_UplinkConfig__transformPrecoderDisabled__ptrs_Power_p10	= 2,
	PTRS_UplinkConfig__transformPrecoderDisabled__ptrs_Power_p11	= 3
} e_PTRS_UplinkConfig__transformPrecoderDisabled__ptrs_Power;
typedef enum PTRS_UplinkConfig__transformPrecoderEnabled__timeDensityTransformPrecoding {
	PTRS_UplinkConfig__transformPrecoderEnabled__timeDensityTransformPrecoding_d2	= 0
} e_PTRS_UplinkConfig__transformPrecoderEnabled__timeDensityTransformPrecoding;

/* PTRS-UplinkConfig */
typedef struct PTRS_UplinkConfig {
	struct PTRS_UplinkConfig__transformPrecoderDisabled {
		struct PTRS_UplinkConfig__transformPrecoderDisabled__frequencyDensity {
			A_SEQUENCE_OF(long) list;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *frequencyDensity;
		struct PTRS_UplinkConfig__transformPrecoderDisabled__timeDensity {
			A_SEQUENCE_OF(long) list;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *timeDensity;
		long	 maxNrofPorts;
		long	*resourceElementOffset;	/* OPTIONAL */
		long	 ptrs_Power;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *transformPrecoderDisabled;
	struct PTRS_UplinkConfig__transformPrecoderEnabled {
		struct PTRS_UplinkConfig__transformPrecoderEnabled__sampleDensity {
			A_SEQUENCE_OF(long) list;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} sampleDensity;
		long	*timeDensityTransformPrecoding;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *transformPrecoderEnabled;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PTRS_UplinkConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_maxNrofPorts_7;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_resourceElementOffset_10;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_ptrs_Power_14;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_timeDensityTransformPrecoding_22;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PTRS_UplinkConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_PTRS_UplinkConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_PTRS_UplinkConfig_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _PTRS_UplinkConfig_H_ */
#include <asn_internal.h>
