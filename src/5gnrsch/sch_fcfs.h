
typedef struct schFcfsCellCb
{
   CmLListCp     ueToBeScheduled;                   /*!< Linked list to store UEs pending to be scheduled, */
}SchFcfsCellCb;

typedef struct schFcfsLcCb
{
   /* TODO: For Multiple RRMPolicies, Make DedicatedLcInfo as array/Double Pointer 
    * and have separate DedLCInfo for each RRMPolcyMemberList*/
   /* Dedicated LC List will be allocated, if any available*/
   CmLListCp dedLcList;	/*Contain LCInfo per RRMPolicy*/
   CmLListCp defLcList; /*Linklist of LC assoc with Default S-NSSAI(s)*/
   /* SharedPRB number can be used by any LC.
    * Need to calculate in every Slot based on PRB availability*/
   uint16_t sharedNumPrb;
}SchFcfsLcCb;

typedef struct schFcfsHqProcCb
{
   SchFcfsLcCb lcCb; 
}SchFcfsHqProcCb;

typedef struct schFcfsHqCb
{
   CmLListCp  ulRetxHqList;
   CmLListCp  dlRetxHqList;
}SchFcfsHqCb;

typedef struct schFcfsUeCb
{
   SchFcfsHqCb   hqRetxCb;
}SchFcfsUeCb;

uint8_t schFcfsAddUeToSchedule(SchCellCb *cellCb, uint16_t ueIdToAdd);
void schFcfsAllApisInit(SchAllApis *allFcfsApi);
