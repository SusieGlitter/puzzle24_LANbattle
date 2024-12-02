# 联机24点对战

## 功能简单介绍
这是上海交通大学-cs金课-(2024-2025-1)-CS2309-02-问题求解与实践的大作业

主题为算24点

编写了一个有图形化界面的程序, 包含```自动求解```, ```单人模式```, ```联机模式```

**自动求解**
1. 可以使用文本框输入每行的四个数字进行求解.
2. 可以读取文件, 对每一行的四个数字进行求解.
3. 求解结果显示在另一文本框中

**单人模式**
1. 自动随机生成题目, 需要保证存在解.
2. 通过文本框输入解答, 程序进行答案验证.
3. 可以查看参考答案.

**联机模式**
1. 多个程序同时运行, 相互进行通信进行联机游戏, 采用P2P模式.

2. 实现以下功能:设置id, 发起对战(作为房主), 显示他人发起的对战, 加入对战, 开启游戏, 拒绝发起对战(同一端口已存在对战), 房主生成有解题目并广播, 不同用户分别作答并验证, 广播答对者id, 中途退出, 房主结束游戏, 排行榜, 保存排行榜, 消息显示与局内聊天等.

---
## ui结构
<!-- 
+ modeSelectTabWidget
  + solveMode
    + solveModeTextEdit
    + solveModeTextShowcase
    + solveModeFileButton
    + solveModeSolveButton
  + offlineMode
    + offlineModeTextEdit
    + offlineModePointShowcase
    + offlineModeNumShowcase1
    + offlineModeNumShowcase2
    + offlineModeNumShowcase3
    + offlineModeNumShowcase4
    + offlineModeGiveupButton
    + offlineModeCheckButton
  + onlineMode
    + onlineModeStackedWidget
      + onlineModeCreatPage
        + onlineModeCreatLabel
        + onlineModeIDEdit
        + onlineModeMemberTabel
        + onlineModeQuitRoomButton
        + onlineModeStartButton
      + onlineModeGamePage
        + onlineModeGameLabel
        + onlineModeTextEdit
        + onlineModePointShowcase
        + onlineModeNumShowcase1
        + onlineModeNumShowcase2
        + onlineModeNumShowcase3
        + onlineModeNumShowcase4
        + onlineModeQuitGameButton
        + onlineModeCheckButton
      + onlineModeRankPage
        + onlineModeRankLabel
        + onlineModeRankTabel
        + onlineModeSaveButton
        + onlineModeExitButton
-->

```text
modeSelectTabWidget
├── solveMode
│   ├── solveModeTextEdit
│   ├── solveModeTextShowcase
│   ├── solveModeFileButton
│   └── solveModeSolveButton
├── offlineMode
│   ├── offlineModeTextEdit
│   ├── offlineModePointShowcase
│   ├── offlineModeNumShowcase1
│   ├── offlineModeNumShowcase2
│   ├── offlineModeNumShowcase3
│   ├── offlineModeNumShowcase4
│   ├── offlineModeGiveupButton
│   └── offlineModeCheckButton
└── onlineMode
    └── onlineModeStackedWidget
        ├── onlineModeCreatPage
        │   ├── onlineModeCreatLabel
        │   ├── onlineModeIDEdit
        │   ├── onlineModeMemberTabel
        │   ├── onlineModeQuitRoomButton
        │   └── onlineModeStartButton
        ├── onlineModeGamePage
        │   ├── onlineModeGameLabel
        │   ├── onlineModeTextEdit
        │   ├── onlineModePointShowcase
        │   ├── onlineModeNumShowcase1
        │   ├── onlineModeNumShowcase2
        │   ├── onlineModeNumShowcase3
        │   ├── onlineModeNumShowcase4
        │   ├── onlineModeQuitGameButton
        │   └── onlineModeCheckButton
        └── onlineModeRankPage
            ├── onlineModeRankLabel
            ├── onlineModeRankTabel
            ├── onlineModeSaveButton
            └── onlineModeExitButton

```