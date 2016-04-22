#!/bin/bash

#预处理
function preprocess()
{

    #清理内核信息
    sudo dmesg --clear

    #清理屏幕信息
    clear

    #进入毕设代码目录
    cd ~/bs
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
    make clean > tmp
    rm tmp

    #编译
    make

    echo "共有${device_num}个设备"
    echo "状态位依次为${global_status}"

    #挂载驱动、设备
    sudo insmod drivers.ko
    sudo insmod devices.ko device_num=${device_num} global_status=${global_status}

    #清理中间文件
    make clean
}

#控制逻辑
function controlMod()
{
	#IFS=','
	#arr=(${global_status})

	#for (( i = 0; i < ${device_num}; i++ )); do
	#	#statements
	#	echo ${arr[$i]}
	#done

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
	            else
	                echo '该设备为：冷型'
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

	echo -n '请输入创建设备个数:'
	read device_num

	echo -n '请依次输入各个设备的状态位:(以逗号隔开)'
	read global_status

	#挂载驱动设备
	mnt;

	#控制逻辑
	controlMod;

	echo -n "输入想要模拟损坏的设备号："
	read dev_id

	echo "${dev_id}设备损坏"

	#表示损坏
	#arr[${dev_id}]=3

	#卸载
	unmnt;

	echo "进程号："$$

	#global_status=""
	for (( i = 0; i < ${device_num}; i++ )); do
		global_status="${global_status},${arr[${i}]}"
	done

	echo $global_status

	#重新挂载
	#mnt ${device_num} 

	#退出的时候卸载设备
	#unmnt;
}


main;
