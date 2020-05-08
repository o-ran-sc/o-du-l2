#include "stdio.h"
#include "stdbool.h"
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
 
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
 
#include "gen.x"           /* General */
#include "ssi.x"   /* System services */

void l1HdlSlotIndicaion(bool);
S16 buildAndSendSlotIndication();
S16 duSendEgtpSlotInd();
