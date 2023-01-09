# 个人额外技能展示
## 基于STM32H743VIT6的拍照器
未来计划：改进为家居监控摄像器
## 个人工作
绘制原理图，PCB，芯片焊接，代码编写（STM32代码为根据CUBEMX配置生成后再根据个人需求添加修改，上位机代码来源为网络上的串口上位机代码，根据需要添加BMP文件保存功能）
摄像头模块为外购
## 过程中遇到的问题
芯片引脚与接口连接错误，飞线处理解决
上电后芯片无反应，问题为晶振的地引脚没有与PCB地连接，修改PCB解决
程序运行后在保存SD卡中的BMP图片显示异常，修改DMA外设代码无效，将硬件IIC改为软件IIC无效，修改摄像头模块初始化代码无效，使能UART接口，通过串口将RGB数据保存至上位机，将上位机所保存的BMP文件数据和逻辑分析仪获取的数据比较，发现相同，将BMP文件结构代码中的保存格式由RGB565修改为RGB888，问题解决
## 实物
![](https://github.com/dSZfAjvn/stm32h743_cam/blob/main/img/1.png?raw=true) ![](https://github.com/dSZfAjvn/stm32h743_cam/blob/main/img/2.png?raw=true)
## 原理图
![](https://github.com/dSZfAjvn/stm32h743_cam/blob/main/img/3.png?raw=true)
## PCB
![](https://github.com/dSZfAjvn/stm32h743_cam/blob/main/img/4.png?raw=true)
## 单片机部分代码一览
![](https://github.com/dSZfAjvn/stm32h743_cam/blob/main/img/5.png?raw=true)
## 上位机部分代码一览
![](https://github.com/dSZfAjvn/stm32h743_cam/blob/main/img/6.png?raw=true)
