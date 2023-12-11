# ButtonMarger
A management component belonging to a key in MCU or other.Easy to use single key, multiple key and combination key functions. Includes click, double - click, long - press functions.


# How to Use

## Necessary functions to use.

Include header file.
Use the initialization function and the tick function.

```c
#include "../src/z_btnmgr.h"
int main(void){
    z_btnmgr_init();
    ...
    while(1){
        ...
        /* Suppose every 10ms cycle */
        z_btnmgr_tick(10);
        ...
    }
    return 0;
}
```
## Take a Button

Initializes STM32 with a pin GPIOA1 as a button function.

```c
/* Initialize the pins of stm32 */
void demo_btn_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* pin of stm32, it is cative when GPIOA1 is high level */
uint8_t demo_btn1_read(void){
    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1?1:0;
}

/* pin of stm32, it is cative when GPIOA2 is high level */
uint8_t demo_btn2_read(void){
    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == 1?1:0;
}
```

## No Use Callback Event

- step 1 : Create a button object

```c
z_btn_t demo_btn;
/* create one button and regeister it*/
z_btnmgr_creategBtn(&demo_btn,demo_btn1_read,NULL);
z_btnmgr_regBtn(&demo_btn);
```

- step 2 : Try it.

```c
if(z_btnmgr_isPressing(&demo_btn) == true){
    /* todo */
}

if(z_btnmgr_wasPressed(&demo_btn) == true){
    /* todo */
}

if(z_btnmgr_isReleasing(&demo_btn) == true){
    /* todo */
}
```

## Use Callback Event

- step 1 : Create a button object

```c
z_btn_t demo_btn;
/* create one button and regeister it */
z_btnmgr_creategBtn(&demo_btn,demo_btn1_read,button_event);
z_btnmgr_regBtn(&demo_btn);
```

- step 2 : Create a Event and Try it

```c
void button_event(z_btn_args_t _args){
    
    if (_args.Obj != &demo_btn) {
        return;
    }
    switch (_args.State) {
        case BtnSta_Pressing: {
            /* todo */
        }break;
        case BtnSta_Pressed: {
            /* todo */
        }break;
        case BtnSta_Releasing: {
            /* todo */
        }break;
        case BtnSta_Clicked: {
            /* todo */
        }break;
        case BtnSta_DoubleClicked: {
            /* todo */
        }break;
        case BtnSta_LongPressing: {
            /* todo */
        }break;
        case BtnSta_LongPressed_Repeat: {
            /* todo */
        }break;
        default:break;
    }
}
```

## Use Button Group

- step 1 : Create Two button object and One Goup object.


```c
z_btn_t demo_btn1,demo_btn2;
z_btngroup_t demo_group;
/* create 2 button */
z_btnmgr_creategBtn(&demo_btn1,demo_btn1_read,button_event);
z_btnmgr_creategBtn(&demo_btn2,demo_btn2_read,button_event);
/* create 1 Group */
z_btnmgr_regGrounp(&demo_group, button_event);
/* Add Button to Group */
z_btnmgr_setGrounp(&demo_group,&demo_btn1);
z_btnmgr_setGrounp(&demo_group, &demo_btn2);
z_btnmgr_setGrpProperty(&demo_group, BrnGrpProp_Mutex);

```

- step 2 : Create a Event and Try it

```c
void button_event(z_btn_args_t _args){
    
    if (_args.Obj == &demo_btn1) {
        if(_args.State == BtnSta_LongPressed_Repeat){
            // todo
        }
    }
    else if (_args.Obj == &demo_btn2) {
        if(_args.State == BtnSta_Pressed){
            // todo
        }
    }
    else if (_args.Obj == &demo_group) {
        if(_args.State == BtnSta_Clicked){
            // todo
        }
    }
}
```

# Advance Config

In 'z_btnmgr.h', here can change the button detection time (unit: ms) as required.

```c
// DEFINE --------------------------------------------------------------------
#define Z_BTNMGR_SHORTTIME_ACTIVE   20
#define Z_BTNMGR_LONGTIME_ACTIVE    1300
#define Z_BTNMGR_LONGTIME_PEER      100
#define Z_BTNMGR_DOUBLECLICK_ACTIVE 300
```

# Update log

- version 1.00 / 2023-12-11
  - Create the repository
  - Add the README.md

# Enjoy It

