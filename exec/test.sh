echo "正在执行一个死循环程序"
python useCpu.py &

ps -o pid,cmd | grep "useCpu.py" > tmp
pidInfo=$(head -1 tmp)

OLD_IFS="$IFS"
IFS=' '
arr=(${pidInfo})
IFS="$OLD_IFS"
pid=${arr[0]}

echo "该进程号为：$pid"

#function cgroup() {
	
	cd /sys/fs/cgroup/cpu
	sudo mkdir bs

	#创建bs层次结构
	sudo cgcreate -g cpu:/bs
	cd bs

	read -p "输入想要分配的单位时间cpu百分比：" cpuTime
	let cpuTime=cpuTime*1000

	echo $cpuTime

	#更改单位时间内使用cpu时间为50%，该命令需要以root身份运行
	#sudo cgset -r cpu.cfs_quota_es=30000 bs

	echo $cpuTime > /sys/fs/cgroup/cpu/bs/cpu.cfs_quota_us

	#将进程pid加入bs下的tasks文件，限制cpu使用时间
	echo $pid
	echo $pid > tasks

	#查看进程号为pid的进程
	top -p $pid

	#卸载
	sudo cgdelete cpu:bs

	#杀死进程
	kill $pid
#}

#cgroup