/******************************************************************************
*
*   Copyright (c) 2019 Intel.
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*******************************************************************************/
#ifndef _NR5G_FAPI_COMMON_TYPES_H_
#define _NR5G_FAPI_COMMON_TYPES_H_

#ifndef TRUE
/** TRUE = 1 */
#define TRUE 1
#endif                          /* #ifndef TRUE */

#ifndef FALSE
/** FALSE = 0 */
#define FALSE 0
#endif                          /* #ifndef FALSE */

#ifndef SUCCESS
/** SUCCESS = 0 */
#define SUCCESS     0
#endif                          /* #ifndef SUCCESS */

#ifndef FAILURE
/** FAILURE = 1 */
#define FAILURE     1
#endif                          /* #ifndef FAILURE */

#define RUP512B(x) (((x)+511)&(~511))
#define RUP256B(x) (((x)+255)&(~255))
#define RUP128B(x) (((x)+127)&(~127))
#define RUP64B(x) (((x)+63)&(~63))
#define RUP32B(x) (((x)+31)&(~31))
#define RUP16B(x) (((x)+15)&(~15))
#define RUP8B(x)  (((x)+7)&(~7))
#define RUP4B(x)  (((x)+3)&(~3))
#define RUP2B(x)  (((x)+1)&(~1))

//#define UNUSED(x) (void)(x)

#endif                          // _NR5G_FAPI_COMMON_TYPES_H_
