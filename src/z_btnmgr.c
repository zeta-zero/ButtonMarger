/*--------------------------------------------------------------------
@file            : z_btnmgr.c
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
#include "z_btnmgr.h"

#if  __BUTTON_MARGER_ENABLE__ == 1
// DEFINE --------------------------------------------------------------------

// TYPE ----------------------------------------------------------------------

// All global variable definitions for this file
typedef struct{
    z_blist_t Btns_BListHead;      // The list head of a button
    z_blist_t BtnGrounp_BListHead; // The list head of a button group collection
    uint32_t TickCount;
}z_btnmgr_params_t;

// FUNCTION ------------------------------------------------------------------
void z_btnmgr_btnProc(z_btn_t* _btn);
void z_btnmgr_groupProc(z_btngroup_t* _group);


// VLAUE ---------------------------------------------------------------------
static z_btnmgr_params_t  z_btnmgr_Params = {0};
static z_btnmgr_params_t *const base = &z_btnmgr_Params;

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_init
 * @brief  : Initialization function
 * @param  : none
 * @return : none
 */
void z_btnmgr_init(void)
{
    LIST_INIT(&base->Btns_BListHead);
    LIST_INIT(&base->BtnGrounp_BListHead);
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_creategBtn
 * @brief  : Create a button object
 * @param  : _btn      - point of button object.
 *           _readbtn  - callback function that reads button state.
 *           _event    - callback function that button status update event.
 * @return : res  - error status
 */
z_err_t z_btnmgr_creategBtn(z_btn_t* _btn,z_readbtn_cb _readbtn, z_click_event _event)
{
    z_err_t res = Z_ERR_OK;
    if (_btn == 0 || _readbtn == 0) {
        res = Z_ERR_BADPARAM;
        goto error;
    }
    _btn->ClickAction = _readbtn;
    _btn->Event = _event;

error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_regBtn
 * @brief  : Register a button object in the button manager
 * @param  : _btn      - point of button object.
 * @return : res  - error status
 */
z_err_t z_btnmgr_regBtn(z_btn_t* _btn)
{
    z_err_t res = Z_ERR_OK;
    if (_btn == 0 || _btn->ClickAction == 0) {
        res = Z_ERR_BADPARAM;
        goto error;
    }
    
    LIST_INIT(&_btn->List);
    LIST_ADD(&_btn->List,&base->Btns_BListHead);
error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_regGrounp
 * @brief  : Register a button group in the button manager
 * @param  : _group      - point of button group.
 *           _event      - callback function that button group status update event.
 * @return : res  - error status
 */
z_err_t z_btnmgr_regGrounp(z_btngroup_t* _group, z_click_event _event)
{
    z_err_t res = Z_ERR_OK;
    if (_group == 0) {
        res = Z_ERR_BADPARAM;
        goto error;
    }
    _group->Event = _event;
    LIST_INIT(&_group->List);
    LIST_INIT(&_group->BtnsList);
    LIST_ADD(&_group->List,&base->BtnGrounp_BListHead);

error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_setGrounp
 * @brief  : Adds a button object to a button group
 * @param  : _group  - point of button group.
 *           _btn    - point of button object.
 * @return : res  - error status
 */
z_err_t z_btnmgr_setGrounp(z_btngroup_t* _group, z_btn_t* _btn)
{
    z_err_t res = Z_ERR_OK;
    if (_group == 0 || _btn == 0) {
        res = Z_ERR_BADPARAM;
        goto error;
    }
    LIST_INIT(&_btn->List);
    LIST_ADD(&_btn->List, &_group->BtnsList);

    if (_btn->Event == 0) {
        _btn->Event = _group->Event;
    }

error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_setGrpProperty
 * @brief  : Set button group properties
 * @param  : _group      - point of button group.
 *           _val        - sets the properties between keys in the press and button combination.
 * @return : res  - error status
 */
z_err_t z_btnmgr_setGrpProperty(z_btngroup_t* _group, z_btngrp_property _val)
{
    z_err_t res = Z_ERR_OK;
    if (_group == 0) {
        res = Z_ERR_BADPARAM;
        goto error;
    }
    if (_val == BrnGrpProp_None) {
        _group->Property = BrnGrpProp_Parallel;
    }
    else {
        _group->Property = _val;
    }


error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_isPressing
 * @brief  : Returns whether the button is pressed
 * @param  : _btn  - point of button object.
 * @return : res  - ture when button is pressing
 *                - false that button is not pressing
 */
uint8_t z_btnmgr_isPressing(z_btn_t* _btn)
{
    uint8_t res = false;
    if (_btn->State == BtnSta_Pressing ||
        _btn->State == BtnSta_LongPressing) {
        res = true;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_wasPressed
 * @brief  : Returns whether the button has been pressed
 * @param  : _btn  - point of button object.
 * @return : res  - ture when button was pressed
 *                - false that button was not pressed
 */
uint8_t z_btnmgr_wasPressed(z_btn_t* _btn)
{
    uint8_t res = false;
    if (_btn->PreState == BtnSta_Pressed) {
        _btn->PreState = BtnSta_None;
        res = true;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_isReleasing
 * @brief  : Returns whether the button is released
 * @param  : _btn  - point of button object.
 * @return : res  - ture when button is prleasing
 *                - false that button is not prleasing
 */
uint8_t z_btnmgr_isReleasing(z_btn_t* _btn)
{
    uint8_t res = false;
    if (_btn->State == BtnSta_Releasing) {
        res = true;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_tick
 * @brief  : Cycle and operation
 * @param  : _ms  The recommended time interval for each call is 1-10ms
 * @return : res  - error status
 */
inline void z_btnmgr_tick(uint32_t _ms)
{

    z_err_t res = Z_ERR_OK;
    z_blist_t *blist_pbuf = &base->Btns_BListHead;
    z_btn_t* btn_p = 0;
    z_btngroup_t* group_p = 0;

    base->TickCount += _ms;
    // One Button
    while (blist_pbuf->NextNode != &base->Btns_BListHead)
    {
        btn_p = LIST_FIRST_ENTRY(blist_pbuf,
                                  z_btn_t*,
                                  List);
        z_btnmgr_btnProc(btn_p);
        blist_pbuf = blist_pbuf->NextNode;
    }
    // Button Group
    blist_pbuf = &base->BtnGrounp_BListHead;
    while (blist_pbuf->NextNode != &base->BtnGrounp_BListHead)
    {
        group_p = LIST_FIRST_ENTRY(blist_pbuf,
                                   z_btngroup_t*,
                                   List);
        // todo
        z_btnmgr_groupProc(group_p);
        blist_pbuf = blist_pbuf->NextNode;
    }
    
}

/**-------------------------------------------------------------------
 * @fn     : __btnReleasingProc
 * @brief  : Event handling of button release status
 * @param  : _btn  - a Button object
 *           _sta  - status of button
 * @return : res  - error status
 */
static inline void __btnCallEventProc(z_btn_t* _btn,z_btn_state_t _sta)
{
    z_btn_args_t args;
    if (_btn == 0 || _btn->Event == 0 || _btn->Flags.NoResp == 1) {
        goto error;
    }
    args.Obj = _btn;
    args.State = _sta;
    _btn->Event(args);

error:
    return;
}

/**-------------------------------------------------------------------
 * @fn     : __btnReleasingProc
 * @brief  : Event handling of button release status
 * @param  : _btn  - a Button object
 * @return : res   - state of button
 */
static inline z_btn_state_t __btnReleasingProc(z_btn_t* _btn)
{
    z_btn_state_t res = BtnSta_Releasing;
    if (_btn == 0 || _btn->ClickAction == 0) {
        goto error;
    }
    if (_btn->ClickAction() == 0) {
        goto error;
    }
    if (_btn->StartPresseTime == 0) {
        _btn->StartPresseTime = base->TickCount;
    }
    if (_btn->StartPresseTime + Z_BTNMGR_SHORTTIME_ACTIVE > base->TickCount + 1) {
        goto error;
    }
    if (_btn->ClickAction() == 1) {
        _btn->StartReleaseTime = 0;
        _btn->PressTimeBuf = base->TickCount;
        res = BtnSta_Pressing;
        __btnCallEventProc(_btn,res);
    }
error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : __btnPressingProc
 * @brief  : Event handling of button press state
 * @param  : _btn  - a Button object
 * @return : res   - state of button
 */
static inline z_btn_state_t __btnPressingProc(z_btn_t* _btn)
{
    z_btn_state_t res = BtnSta_Pressing;
    if (_btn == 0 || _btn->ClickAction == 0) {
        goto error;
    }
    // Button release detected
    if (_btn->ClickAction() == 0) {
        if (_btn->StartReleaseTime == 0) {
            _btn->StartReleaseTime = base->TickCount;
        }
        else if (_btn->StartReleaseTime + Z_BTNMGR_SHORTTIME_ACTIVE < base->TickCount + 1) {
            _btn->StartReleaseTime = 0;
            res = BtnSta_Clicked;
            __btnCallEventProc(_btn,res);
            goto error;
        }
    }
    else if (_btn->StartReleaseTime + Z_BTNMGR_SHORTTIME_ACTIVE < base->TickCount + 1) {
        _btn->StartReleaseTime = 0;
    }
    // btn is Keep pressing
    if (_btn->StartPresseTime == 0) {
        _btn->StartPresseTime = base->TickCount;
    }
    if (_btn->StartPresseTime + Z_BTNMGR_LONGTIME_ACTIVE < base->TickCount + 1) {
        _btn->StartReleaseTime = 0;
        res = BtnSta_LongPressing;
        __btnCallEventProc(_btn,res);
        goto error;
    }
    else if ((base->TickCount - _btn->PressTimeBuf) > Z_BTNMGR_SHORTTIME_ACTIVE + 1) {
        _btn->PressTimeBuf = base->TickCount;
        __btnCallEventProc(_btn,res);
    }
    
error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : __btnLongPressingProc
 * @brief  : Handling the event that the button is pressed for a long time
 * @param  : _btn  - a Button object
 * @return : res   - state of button
 */
static inline z_btn_state_t __btnLongPressingProc(z_btn_t* _btn)
{
    z_btn_state_t res = BtnSta_LongPressing;
    if (_btn == 0 || _btn->ClickAction == 0) {
        goto error;
    }
    // button release detected
    if (_btn->ClickAction() == 0) {
        if (_btn->StartReleaseTime == 0) {
            _btn->StartReleaseTime = base->TickCount;
        }
        else if (_btn->StartReleaseTime + Z_BTNMGR_SHORTTIME_ACTIVE < base->TickCount + 1) {
            _btn->StartReleaseTime = 0;
            res = BtnSta_Pressed;
            __btnCallEventProc(_btn,res);
            goto error;
        }
    }
    else if (_btn->StartReleaseTime + Z_BTNMGR_SHORTTIME_ACTIVE < base->TickCount + 1) {
        _btn->StartReleaseTime = 0;
    }
    // btn is Keep pressing
    if (_btn->StartPresseTime == 0) {
        _btn->StartPresseTime = base->TickCount;
    }
    else if (((base->TickCount - _btn->StartPresseTime - Z_BTNMGR_LONGTIME_ACTIVE) % Z_BTNMGR_LONGTIME_PEER) == 0) {
        __btnCallEventProc(_btn, BtnSta_LongPressed_Repeat);
    }
    
error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : __btnPressedOrClickedProc
 * @brief  : Event handling of button click completed
 * @param  : _btn  - a Button object
 * @return : res  - state of button
 */
static inline z_btn_state_t __btnPressedOrClickedProc(z_btn_t* _btn)
{
    z_btn_state_t res = BtnSta_Releasing;
    if (_btn == 0 || _btn->ClickAction == 0) {
        goto error;
    }
    _btn->StartReleaseTime = 0;
    _btn->StartPresseTime = 0;
    _btn->PreState = BtnSta_Pressed;
    __btnCallEventProc(_btn,res);
error:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_btnProc
 * @brief  : Button handling function
 * @param  : _btn  - a Button object
 * @return : none
 */
inline void z_btnmgr_btnProc(z_btn_t* _btn)
{
    if (_btn == 0 || _btn->ClickAction == 0) {
        goto error;
    }
    switch (_btn->State) {
    case BtnSta_Releasing: {
        _btn->State = __btnReleasingProc(_btn);
    }break;
    case BtnSta_Pressing: {
        _btn->State = __btnPressingProc(_btn);
    }break;
    case BtnSta_LongPressing: {
        _btn->State = __btnLongPressingProc(_btn);
    }break;
    case BtnSta_Clicked: 
    case BtnSta_Pressed: {
        _btn->State = __btnPressedOrClickedProc(_btn);
    }break;
    default:_btn->State = BtnSta_Releasing;break;
    }

error:
    return;
}

/**-------------------------------------------------------------------
 * @fn     : z_btnmgr_grpStaChg
 * @brief  : Change the state of the button sets and change the properties of the sub-buttons
 * @param  : _group  - a Button group
 * @return : none
 */
static inline void __grpStaChg(z_btngroup_t* _group)
{
    z_blist_t* blist_pbuf = &_group->BtnsList;
    z_btn_t* btn_p = 0;
    switch (_group->Property) {
        case BrnGrpProp_None:
        case BrnGrpProp_Parallel: {

        }break;
        case BrnGrpProp_Mutex: {
            while (blist_pbuf->NextNode != &_group->BtnsList)
            {
                btn_p = LIST_FIRST_ENTRY(blist_pbuf,
                                        z_btn_t*,
                                        List);
                if (_group->State == BtnSta_Pressing) {
                    btn_p->Flags.NoResp = 1;
                }
                else {
                    btn_p->Flags.NoResp = 0;
                }

                blist_pbuf = blist_pbuf->NextNode;
            }

        }break;
        default:break;
    }
}

 /**-------------------------------------------------------------------
 * @fn     : z_btnmgr_groupProc
 * @brief  : Button group handling function
 * @param  : _group  - a Button group
 * @return : none
 */
inline void z_btnmgr_groupProc(z_btngroup_t* _group)
{
    z_blist_t* blist_pbuf = &_group->BtnsList;
    uint8_t btncount = 0;
    uint8_t btnpress = 0;
    z_btn_t* btn_p = 0;
    z_btn_args_t args;
    if (_group == 0) {
        goto error;
    }
    while (blist_pbuf->NextNode != &_group->BtnsList)
    {
        btn_p = LIST_FIRST_ENTRY(blist_pbuf,
                                  z_btn_t*,
                                  List);
        z_btnmgr_btnProc(btn_p);
        blist_pbuf = blist_pbuf->NextNode;

        //
        btncount++;
        if (z_btnmgr_isPressing(btn_p) == 1) {
            btnpress++;
        }
    }
    if (_group->Event != 0 && btncount != 0) {
        if (btncount == btnpress) {
            _group->State = BtnSta_Pressing;
            __grpStaChg(_group);
        }
        else if (_group->State == BtnSta_Pressing && btnpress == 0) {
            _group->State = BtnSta_Clicked;
        }
        else if(_group->State == BtnSta_Clicked && btnpress == 0){
            _group->State = BtnSta_Releasing;
            __grpStaChg(_group);
        }
        else {
            goto error;
        }
        args.Obj = _group;
        args.State = _group->State;
        _group->Event(args);
    }
error:
    return;
}

#endif // __BUTTON_MARGER_ENABLE__
