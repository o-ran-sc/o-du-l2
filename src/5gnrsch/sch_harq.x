#define SCH_MAX_UL_HQ_PROC 16
#define SCH_MAX_DL_HQ_PROC 16

typedef struct schUlHqProcCb   SchUlHqProcCb;
typedef struct schDlHqProcCb   SchDlHqProcCb;
typedef struct schUlHqEnt SchUlHqEnt;
typedef struct schDlHqEnt SchDlHqEnt;
struct schUlHqProcCb
{
};
struct schDlHqProcCb
{
};
struct schUlHqEnt
{
   SchUlHqProcCb  procs[SCH_MAX_UL_HQ_PROC];
};
struct schDlHqEnt
{
   SchDlHqProcCb  procs[SCH_MAX_DL_HQ_PROC];
};
