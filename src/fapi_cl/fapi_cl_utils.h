/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

#define CL_MEM_REGION   4
#define CL_POOL         1

#ifdef ODU_MEMORY_DEBUG_LOG
#define FAPI_CL_MEM_LOG(_macro, _file, _line, _func, _size, _datPtr)\
{\
   printf("\n%s=== %s +%d, %s, %d, %p\n",           \
      _macro, _file, _line, _func, _size, _datPtr); \
}
#else
#define FAPI_CL_MEM_LOG(_macro, _file, _line, _func, _size, _dataPtr) {}
#endif

/* free a static buffer */
#define FAPI_CL_FREE(_datPtr, _size)                         \
{                                                            \
   if(_datPtr)                                               \
   {                                                         \
      FAPI_CL_MEM_LOG("FAPI_CL_FREE", __FILE__, __LINE__, __FUNCTION__, _size, _datPtr);\
      SPutSBuf(CL_MEM_REGION, CL_POOL,                       \
       (Data *)_datPtr, _size);                              \
      _datPtr = NULLP;                                       \
   }                                                         \
}

/**********************************************************************
                               End of file
 **********************************************************************/
