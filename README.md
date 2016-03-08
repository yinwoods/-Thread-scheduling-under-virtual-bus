# Graduation-Project
My Graduation Project

这是本人的毕设项目，目标是做出linux内核下虚拟总线上的线程调度

具体来说最后要能够动态创建多个驱动，并在每个驱动上挂载不同个数的设备；

而每个设备可以理解为一个进程，同一驱动下的每个进程要完成相同的计算任务；

在每个驱动下还要有一个专门为其他进程赋予状态位的进程。状态位分为三种，可以用数字区分为0,1,2

> 0代表“主”：即参与运算，并输出结果。

> 1代表“热”：即参与运算，但不输出结果。

> 2代表“冷”：即不参与运算。

创建这个特有进程的意义就在于方便更改其他进程的状态位，从而方便得到不同状态位下的结果。

1-31日	实现一个驱动下的不同设备通信。

2-16日 实现一个驱动下创建指定个数的设备，但是无法卸载drivers驱动。

2-17日 实现一个驱动下动态创建指定个数个设备并动态为不同设备赋予不同状态位。仍然无法卸载drivers驱动。

2-24日 基本功能已经实现，解决无法卸载drivers或devices的bug。但还有两个问题有待解决
	
	> - 1、每次赋值状态位，device[1]的状态位总是出错。

	> - 2、同时创建的设备数大于3时，第三个之后的设备都无法执行drivers.c中的if语句。

下一步要解决这两个bug，并尝试同时创建多个驱动。

2-24日 两个bug得到解决，但对于bug1来说还是没有弄明白什么原因

经过讨论，了解到下一步的目标是创建设备的同时创建字符设备，从而可以通过其他程序来对设备信息进行修改。

3-5日 实现了一个设备创建过程中的字符设备创建

3-6日 实现了platform总线上对字符设备进行读写

下一步要实现的是创建多个设备时，为每一个设备创建额外的字符设备。并通过总线进行消息的传递。

3-8日 实现了多个字符设备的创建，在内核空间可以通过总线对其对写，在用户空间可以通过fopen fread fwrite对其进行操作。
