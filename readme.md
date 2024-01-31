# State Machine Framework (EFSM)

## 项目简介

这个项目提供了一个用于实现有限状态机（Finite State Machine, FSM）的简单框架，用于管理状态和处理事件。该框架支持状态的初始化、退出、周期性任务执行，以及状态切换和事件处理。

## 使用方法

### 1. 包含头文件

```c
#include "efsm.h"
```

### 2. 定义状态和事件

在使用状态机前，你需要定义状态和事件。状态通过 `efsm_state_t` 结构体表示，事件通过命令（cmd）进行触发。

```c
// 定义状态
efsm_state_t stateA = {NULL, initA, exitA, actionA};
efsm_state_t stateB = {NULL, initB, exitB, actionB};

// 定义事件命令
#define CMD_START  (EFSM_STATE_USER_CMD_BASE + 1)
#define CMD_STOP   (EFSM_STATE_USER_CMD_BASE + 2)
```

### 3. 定义状态机管理结构体

```c
efsm_manage_t myStateMachine;
```

### 4. 初始化状态机

```c
efsm_manage_init(&myStateMachine);
```

### 5. 注册状态机

```c
efsm_register(&myStateMachine);
```

### 6. 定义状态机管理函数

```c
void myInitFunction(void *obj) {
    // 初始化操作
}

void myTickFunction(void *obj) {
    // 周期性任务
}

void myExitFunction(void *obj) {
    // 退出操作
}

void myControlFunction(void *obj, uint32_t cmd, void *param) {
    // 控制函数，根据cmd执行相应的操作
}
```

### 7. 设置状态机管理函数

```c
myStateMachine.init = myInitFunction;
myStateMachine.tick = myTickFunction;
myStateMachine.exit = myExitFunction;
myStateMachine.control = myControlFunction;
```

### 8. 定义状态事件处理函数

```c
void actionA(void *obj, uint32_t cmd, void *param) {
    // 处理状态A的事件
}

void actionB(void *obj, uint32_t cmd, void *param) {
    // 处理状态B的事件
}
```

### 9. 初始化状态机状态

```c
efsm_transition(&myStateMachine, &stateA);  // 初始状态为A
```

### 10. 执行状态机

```c
efsm_manage_tick();
```

## 注意事项

- 在使用状态机之前，确保你的状态、事件和函数都已经正确定义和设置。
- 通过设置命令（cmd）来触发相应的事件处理函数。
- 使用 `efsm_transition` 函数进行状态切换。
- 可以通过修改状态机管理结构体的成员来控制状态机的行为。

## 示例

查看 `example.c` 文件以获取一个简单的状态机使用示例。

## 许可证

该项目基于 [MIT 许可证](LICENSE)。详细信息请参阅许可证文件。

## 贡献

欢迎贡献！如果你发现问题或有改进建议，请提出 [issue](https://github.com/your-username/your-project/issues) 或提交 Pull Request。

## 联系方式

如有任何问题，请通过邮件联系：chengmeng_2@outlook,com。

