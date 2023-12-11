/*--------------------------------------------------------------------
@file            : z_btnmgr.h
@brief           : A management component belonging to a key in MCU or other.
                   Easy to use single key, multiple key and combination key functions.
                   Includes click, double - click, long - press functions.
----------------------------------------------------------------------
@author          : Zeta-Zero
 Release Version : V1.00
 Release Date    : 2023/12/11
----------------------------------------------------------------------
@attention       :
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
      http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 
--------------------------------------------------------------------*/
#define __BUTTON_MARGER_ENABLE__  1

#if  __BUTTON_MARGER_ENABLE__ == 1
#ifndef __z_BTNMGR_H__
#define __z_BTNMGR_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "string.h"

// DEFINE --------------------------------------------------------------------
#define Z_BTNMGR_SHORTTIME_ACTIVE   20
#define Z_BTNMGR_LONGTIME_ACTIVE    1300
#define Z_BTNMGR_LONGTIME_PEER      100
#define Z_BTNMGR_DOUBLECLICK_ACTIVE 300

#define Z_4BYTESPLITE(_DATA_,_VAL_)   {_DATA_[0]=(_VAL_>>0)&0xFF;_DATA_[1]=(_VAL_>>8)&0xFF;_DATA_[2]=(_VAL_>>16)&0xFF;_DATA_[3]=(_VAL_>>24)&0xFF;}
#define Z_4BYTECOMBINE(_DATA_,_VAL_)  {_DATA_ =((_VAL_[0]<<0)&0xFF)|((_VAL_[1]<<8)&0xFF00)|((_VAL_[2]<<16)&0xFF0000)|((_VAL_[3]<<24)&0xFF000000);}

#define Z_ERR_OK                     0x00
#define Z_ERR_BADPARAM               0x01
#define Z_ERR_FAILD                  0x02
#define Z_ERR_BADRECEIVE             0x03
#define Z_ERR_NORECEIVE              0x04
#define Z_ERR_SETPIN                 0x05
#define Z_ERR_OVERRANGE              0x06
#define Z_ERR_BLE_CFG                0x24
#define Z_ERR_BLE_GAP                0x25
#define Z_ERR_BLE_GATT               0x26
#define Z_ERR_BLE_ADV                0x27
#define Z_ERR_BLE_HVX                0x28
#define Z_ERR_BLE_VALUE              0x29


#define Z_ERR_OTHER                  0xFF

#ifndef Z_BLIST_NODE_D
#define Z_BLIST_NODE_D
/* Basic functions of linked list operation */
#define OFFSETOF(_TYPE_,_MEMBER_)                (uint32_t)&(((_TYPE_)0)->_MEMBER_)
#define CONTRAINER_OF(_PV_,_TYPE_,_MEMBER_)      (_TYPE_)( (uint8_t*)(_PV_) -  OFFSETOF(_TYPE_,_MEMBER_))
#define LIST_FIRST_ENTRY(_PV_,_TYPE_,_MEMBER_)   CONTRAINER_OF((_PV_)->NextNode,_TYPE_,_MEMBER_)
#define LIST_INIT(_HEAD_)                        {(_HEAD_)->PreNode = (_HEAD_);(_HEAD_)->NextNode = (_HEAD_);}
// Add to linked list
#define LIST_ADD(_NEW_,_HEAD_)                   {(_NEW_)->NextNode = (_HEAD_);(_NEW_)->PreNode = (_HEAD_)->PreNode;(_HEAD_)->PreNode->NextNode = (_NEW_);(_HEAD_)->PreNode = (_NEW_);}
// Insert into a linked list
#define LIST_INSERT(_NEW_,_HEAD_)                {(_NEW_)->NextNode = (_HEAD_)->NextNode;(_NEW_)->PreNode = (_HEAD_);(_HEAD_)->NextNode->PreNode = (_NEW_);(_HEAD_)->NextNode = (_NEW_);}
#define LIST_LINK(_PRE_,_NEXT_)                  {(_PRE_)->NextNode = (_NEXT_);(_NEXT_)->PreNode = (_PRE_);}
#define LIST_DEL(_MEMBER_)                       {LIST_LINK((_MEMBER_)->PreNode,(_MEMBER_)->NextNode);(_MEMBER_)->PreNode = (_MEMBER_);(_MEMBER_)->NextNode = (_MEMBER_);}
#endif
    
#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif
    
// TYPE ----------------------------------------------------------------------
    
#ifndef Z_BLIST_NODE_T
#define Z_BLIST_NODE_T
//bidirectional linked list nodes
struct z_blist_node
{
	struct z_blist_node *PreNode;
	struct z_blist_node *NextNode;
};
typedef struct z_blist_node z_blist_t;
#endif

typedef enum {
    BtnSta_None = 0x00,
    BtnSta_Pressing = 0x01,
    BtnSta_Pressed,
    BtnSta_Releasing,
    BtnSta_Clicked,
    BtnSta_DoubleClicked,
    BtnSta_LongPressing,
    BtnSta_LongPressed_Repeat,
}z_btn_state_t;

typedef enum {
    BtnType_SingleClicked = 0x01,
    BtnType_DoubleClicked,
    BtnType_BothClicked,

    BtnType_Normal = 0x10,
    BtnType_Toggle = 0x20,
}z_btn_type_t;

typedef enum {
    BrnGrpProp_None = 0x00,
    BrnGrpProp_Parallel,     /* Functional parallelism
                                When the keyset function is triggered, individual keys can still trigger callback events */
    BrnGrpProp_Mutex,        /* Functional exclusion
                                When the key group function is triggered, independent keys do not trigger callback events */
}z_btngrp_property;

typedef struct {
    const void*  Obj;
    z_btn_state_t State;
}z_btn_args_t;

// presing : 1,released : 0
typedef uint8_t(*z_readbtn_cb)(void);
typedef void (*z_click_event)(z_btn_args_t _args);

// One Button Object
typedef struct {
  z_readbtn_cb ClickAction;
  z_click_event Event;
  uint32_t StartPresseTime;
  uint32_t PressTimeBuf;
  uint32_t StartReleaseTime;
  z_btn_state_t State;
  z_btn_state_t PreState;
  z_blist_t List;
  struct {
      uint8_t NoResp : 1;
  }Flags;
}z_btn_t;

// Button Group Object
typedef struct {
    z_blist_t List;
    z_blist_t BtnsList;
    z_click_event Event;
    z_btn_state_t State;
    z_btngrp_property Property;
}z_btngroup_t;


typedef uint32_t z_err_t;
// VLAUE ---------------------------------------------------------------------


// FUNC ----------------------------------------------------------------------
void z_btnmgr_init(void);
z_err_t z_btnmgr_creategBtn(z_btn_t* _btn,z_readbtn_cb _readbtn, z_click_event _event);
z_err_t z_btnmgr_regBtn(z_btn_t* _btn);
z_err_t z_btnmgr_regGrounp(z_btngroup_t *_group,z_click_event _event);
z_err_t z_btnmgr_setGrounp(z_btngroup_t *_group,z_btn_t* _btn);
z_err_t z_btnmgr_setGrpProperty(z_btngroup_t *_group,z_btngrp_property _val);
z_err_t z_btnmgr_setType(z_btn_t* _btn,z_btn_type_t _prop);

uint8_t z_btnmgr_isPressing(z_btn_t* _btn);
uint8_t z_btnmgr_wasPressed(z_btn_t *_btn);
uint8_t z_btnmgr_isReleasing(z_btn_t* _btn);

void z_btnmgr_tick(uint32_t _ms);

#ifdef __cplusplus
}
#endif
#endif // __z_BTNMGR_H__
#endif // __BUTTON_MARGER_ENABLE__
