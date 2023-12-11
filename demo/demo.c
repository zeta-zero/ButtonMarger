#include "../src/z_btnmgr.h"

void button_event(z_btn_args_t _args);
void delay(uint8_t _ms);
uint8_t demo0_btn_read(void);
uint8_t demo1_btn_read(void);
uint8_t demo2_btn1_read(void);
uint8_t demo2_btn2_read(void);

z_btn_t demo0_btn;
z_btn_t demo1_btn;
z_btn_t demo2_btn1, demo2_btn2;
z_btngroup_t demo2_group;

/**-------------------------------------------------------------------
 * @fn     : main
 * @brief  : none
 * @param  : none
 * @return : res   - 0
 */
int main(void)
{
    z_btnmgr_init();

    // demo 0
    z_btnmgr_creategBtn(&demo0_btn,demo0_btn_read,NULL);

    // demo 1
    z_btnmgr_creategBtn(&demo1_btn, demo1_btn_read, button_event);

    // demo 2
    z_btnmgr_creategBtn(&demo2_btn1, demo2_btn1_read , button_event);
    z_btnmgr_creategBtn(&demo2_btn2, demo2_btn2_read , button_event);
    z_btnmgr_regGrounp(&demo2_group, button_event);
    z_btnmgr_setGrounp(&demo2_group,&demo2_btn1);
    z_btnmgr_setGrounp(&demo2_group, &demo2_btn2);
    z_btnmgr_setGrpProperty(&demo2_group, BrnGrpProp_Mutex);

    while (1) {
        // demo 0 - button was preseed
        if (z_btnmgr_wasPressed(&demo0_btn) == true) {
            // todo
        }
        z_btnmgr_tick(10);
        delay(10);
    }

    return 0;
}

/**-------------------------------------------------------------------
 * @fn     : delay
 * @brief  : a delay function that need to commpleting it
 * @param  : _ms  - a time of millisecond
 * @return : res   - state of button
 */
void button_event(z_btn_args_t _args)
{
    // demo 1
    if (_args.Obj == &demo1_btn) {
        switch (_args.State) {
        case BtnSta_Pressing: {

        }break;
        case BtnSta_Pressed: {

        }break;
        case BtnSta_Releasing: {

        }break;
        case BtnSta_Clicked: {

        }break;
        case BtnSta_DoubleClicked: {

        }break;
        case BtnSta_LongPressing: {

        }break;
        case BtnSta_LongPressed_Repeat: {

        }break;
        default:break;
        }
    }
    // demo 2
    else if (_args.Obj == &demo2_btn1) {
        if (_args.State == BtnSta_LongPressed_Repeat) {
            // todo
        }
    }
    else if (_args.Obj == &demo2_btn2) {
        if (_args.State == BtnSta_Pressed) {
            // todo
        }
    }
    else if (_args.Obj == &demo2_group) {
        if (_args.State == BtnSta_Clicked) {
            // todo
        }
    }
}

/**-------------------------------------------------------------------
 * @fn     : delay
 * @brief  : a delay function that need to commpleting it
 * @param  : _ms  - a time of millisecond
 * @return : res   - state of button
 */
void delay(uint8_t _ms)
{
    // todo
}


/**-------------------------------------------------------------------
 * @fn     : demo0_btn_read
 * @brief  : demo 0 button read function
 * @param  : none
 * @return : res   - button operation state
 */
uint8_t demo0_btn_read(void)
{
    // todo
    return 0;
}


/**-------------------------------------------------------------------
 * @fn     : demo1_btn_read
 * @brief  : demo 1 button read function
 * @param  : none
 * @return : res   - button operation state
 */
uint8_t demo1_btn_read(void)
{
    // todo
    return 0;
}

/**-------------------------------------------------------------------
 * @fn     : demo2_btn1_read
 * @brief  : demo 2 button 1 read function
 * @param  : none
 * @return : res   - button operation state
 */
uint8_t demo2_btn1_read(void)
{
    // todo
    return 0;
}

/**-------------------------------------------------------------------
 * @fn     : demo2_btn2_read
 * @brief  : demo 2 button 2 read function
 * @param  : none
 * @return : res   - button operation state
 */
uint8_t demo2_btn2_read(void)
{
    // todo
    return 0;
}