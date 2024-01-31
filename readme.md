# State Machine Framework (EFSM)

## ��Ŀ���

�����Ŀ�ṩ��һ������ʵ������״̬����Finite State Machine, FSM���ļ򵥿�ܣ����ڹ���״̬�ʹ����¼����ÿ��֧��״̬�ĳ�ʼ�����˳�������������ִ�У��Լ�״̬�л����¼�����

## ʹ�÷���

### 1. ����ͷ�ļ�

```c
#include "efsm.h"
```

### 2. ����״̬���¼�

��ʹ��״̬��ǰ������Ҫ����״̬���¼���״̬ͨ�� `efsm_state_t` �ṹ���ʾ���¼�ͨ�����cmd�����д�����

```c
// ����״̬
efsm_state_t stateA = {NULL, initA, exitA, actionA};
efsm_state_t stateB = {NULL, initB, exitB, actionB};

// �����¼�����
#define CMD_START  (EFSM_STATE_USER_CMD_BASE + 1)
#define CMD_STOP   (EFSM_STATE_USER_CMD_BASE + 2)
```

### 3. ����״̬������ṹ��

```c
efsm_manage_t myStateMachine;
```

### 4. ��ʼ��״̬��

```c
efsm_manage_init(&myStateMachine);
```

### 5. ע��״̬��

```c
efsm_register(&myStateMachine);
```

### 6. ����״̬��������

```c
void myInitFunction(void *obj) {
    // ��ʼ������
}

void myTickFunction(void *obj) {
    // ����������
}

void myExitFunction(void *obj) {
    // �˳�����
}

void myControlFunction(void *obj, uint32_t cmd, void *param) {
    // ���ƺ���������cmdִ����Ӧ�Ĳ���
}
```

### 7. ����״̬��������

```c
myStateMachine.init = myInitFunction;
myStateMachine.tick = myTickFunction;
myStateMachine.exit = myExitFunction;
myStateMachine.control = myControlFunction;
```

### 8. ����״̬�¼�������

```c
void actionA(void *obj, uint32_t cmd, void *param) {
    // ����״̬A���¼�
}

void actionB(void *obj, uint32_t cmd, void *param) {
    // ����״̬B���¼�
}
```

### 9. ��ʼ��״̬��״̬

```c
efsm_transition(&myStateMachine, &stateA);  // ��ʼ״̬ΪA
```

### 10. ִ��״̬��

```c
efsm_manage_tick();
```

## ע������

- ��ʹ��״̬��֮ǰ��ȷ�����״̬���¼��ͺ������Ѿ���ȷ��������á�
- ͨ���������cmd����������Ӧ���¼���������
- ʹ�� `efsm_transition` ��������״̬�л���
- ����ͨ���޸�״̬������ṹ��ĳ�Ա������״̬������Ϊ��

## ʾ��

�鿴 `example.c` �ļ��Ի�ȡһ���򵥵�״̬��ʹ��ʾ����

## ���֤

����Ŀ���� [MIT ���֤](LICENSE)����ϸ��Ϣ��������֤�ļ���

## ����

��ӭ���ף�����㷢��������иĽ����飬����� [issue](https://github.com/your-username/your-project/issues) ���ύ Pull Request��

## ��ϵ��ʽ

�����κ����⣬��ͨ���ʼ���ϵ��chengmeng_2@outlook,com��

