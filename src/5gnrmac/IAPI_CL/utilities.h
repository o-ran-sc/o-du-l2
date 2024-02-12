typedef struct rachInfo
{
   uint16_t phyCellId;
}RachInfo;

typedef struct pucchInfo
{
   uint32_t handle;
   uint8_t pucchFormat;
}PucchInfo;

typedef struct puschInfo
{
   uint32_t handle;
   uint8_t harqId;
   uint8_t ulCqi;
   uint16_t timingAdvance;
}PuschInfo;

typedef struct ulSlotInfo
{
   uint16_t sfn;
   uint16_t slot;
   bool rachPresent;
   RachInfo rachInfo;
   uint8_t numPucch;
   PucchInfo pucchInfo[FAPI_MAX_NUMBER_UCI_PDUS_PER_SLOT];
   uint8_t numPusch;
   PuschInfo puschInfo[FAPI_MAX_NUMBER_OF_ULSCH_PDUS_PER_SLOT];
}UlSlotInfo;

UlSlotInfo gUlSlotInfo[MAX_SFN][MAX_SLOTS];
