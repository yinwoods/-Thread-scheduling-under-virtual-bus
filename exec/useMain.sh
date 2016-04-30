#!/bin/bash

#预处理
function preprocess()
{

    #清理内核信息
    echo "yinhua..228" | sudo -S dmesg --clear

    #清理屏幕信息
    clear

    #进入毕设代码目录
    cd ~/bs/exec
}

#卸载驱动设备函数
function unmnt()
{
    #设备名
    devices=devices

    #驱动名
    drivers=drivers

    #将lsmod信息读入modInfo
    lsmod > tmp
    modInfo=$(cat tmp)
    rm tmp

    result=$(echo ${modInfo} | grep ${devices})
    #如果当前挂载了设备，卸载
    if [ "$result" != "" ]; then
    	echo '正在卸载devices'
        sudo rmmod ${devices}
    fi

    result=$(echo ${modInfo} | grep ${drivers})
    #如果当前挂载了驱动，卸载
    if [ "$result" != "" ]; then
    	echo '正在卸载drivers'
        sudo rmmod ${drivers}
    fi
}

#挂载驱动设备函数
function mnt()
{
    #清理中间文件
    make clean > /dev/null 2>&1

    #编译
    make -s > /dev/null 2>&1

    echo "共有${device_num}个设备"
    echo "状态位依次为${global_status}"

    #挂载驱动、设备
    sudo insmod drivers.ko
    sudo insmod devices.ko device_num=${device_num} global_status=${global_status}

    #清理中间文件
    make clean > /dev/null 2>&1
}

#控制逻辑
function controlMod()
{
	OLD_IFS="$IFS"
	IFS=','
	arr=(${global_status})
	IFS="$OLD_IFS"

	#依次读取设备对应字符文件中的内容
	for((i=0; i<${device_num}; i++)); do
	    echo "设备${i}中的内容为："
	    echo '---------------------------------'

	    lineNum=0
	    cat /dev/memdev$i | while read line; do
	        #对设备类型进行判断
	        if [ $lineNum = 1 ]; then
	            status=${line##*：}
	            if [ $status = 0 ]; then
	                echo '该设备为：主型'
	            elif [ $status = 1 ]; then
	                echo '该设备为：热型'
	            elif [ $status = 2 ]; then
	                echo '该设备为：冷型'
	            elif [ $status = 3 ]; then
	            	echo '该设备为：损坏'
	            fi
	        else
	            echo ${line}
	        fi
	        let lineNum=$[lineNum+1]
	    done
	    echo -e "\n\n\n"
	done
}

function main()
{

	#预处理
	preprocess;

	#卸载驱动设备
	unmnt;

	cpuUsage=$(echo $$)

	echo "进程号：$cpuUsage"

	device_num=$1

	global_status=$2

	#挂载驱动设备
	mnt;

	#控制逻辑
	controlMod;

	#卸载
	unmnt;

	#是否进入for循环
	echo -n "按任意键继续（q退出）："
	read prompt
	
	while [[ "$prompt"x != "q"x ]]; do
		echo -n "输入想要模拟损坏的设备号："
		read dev_id

		echo "${dev_id}设备损坏"

		#保存当前状态位
		tmp=${arr[${dev_id}]}

		for (( i = 0; i < ${device_num}; i++ )); do
			echo -e "$i --- ${arr[${i}]}"
		done

		echo -e "tmp = $tmp"

		#表示损坏
		arr[${dev_id}]=3

		#当前状态位为主，取一个热切换为主
		if [[ $tmp = 0 ]]; then
			echo "tmp = 0"
			for (( i = 0; i < ${device_num}; i++ )); do
				if [[ $i = ${dev_id} ]]; then
					continue
				elif [[ ${arr[${i}]} = 1 ]]; then
					echo "arr[${i}] = 0"
					arr[${i}]=0
					tmp=1
					break
				fi
			done
		fi

		#当前状态位为热，取一个冷切换为热
		if [[ $tmp = 1 ]]; then
			echo "tmp = 1"
			for (( i = 0; i < ${device_num}; i++ )); do
				if [[ $i = ${dev_id} ]]; then
					continue
				elif [[ ${arr[${i}]} = 2 ]]; then
					echo "arr[${i}] = 1"
					arr[${i}]=1
					tmp=2
					break
				fi
			done
		fi

		#当前状态位为冷，不用管
		if [[ $tmp = 2 ]]; then
			echo "tmp = 2"
		fi

		for (( i = 0; i < ${device_num}; i++ )); do
			echo -e "$i --- ${arr[${i}]}"
		done

		global_status="${arr[0]}"
		for (( i = 1; i < ${device_num}; i++ )); do
			global_status="${global_status},${arr[${i}]}"
		done

		echo $global_status

		#重新挂载
		mnt ${device_num} 

		#控制逻辑
		controlMod

		#退出的时候卸载设备
		unmnt;

		#是否进入for循环
		echo -n "按任意键继续（q退出）："
		read prompt
	done

	
}

#使用cgroup
function cgroup() {
	
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
}


main $1 $2;
