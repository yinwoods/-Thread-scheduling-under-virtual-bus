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

	if [[ $1 = '' ]]; then
		echo -n '请输入创建设备个数:'
		read device_num
	else
		device_num=$1	
	fi
	

	if [[ $2 = '' ]]; then
		echo -n '请依次输入各个设备的状态位(以逗号隔开):'
		read global_status
	else
		global_status=$2
	fi

	if [[ $3 = '' ]]; then
		echo -n '请输入想要模拟损坏的设备编号(以逗号隔开)：'
		read damage_device
	else
		damage_device=$3
	fi

	OLD_IFS="$IFS"
	IFS=','
	damage_device_arr=(${damage_device})
	IFS="$OLD_IFS"

	OLD_IFS="$IFS"
	IFS=','
	arr=(${global_status})
	IFS="$OLD_IFS"

	for damage_device_id in ${damage_device_arr[@]}; do
		echo "${damage_device_id}设备损坏"	

		#保存当前状态位
		tmp=${arr[${damage_device_id}]}

		#表示损坏
		arr[${damage_device_id}]=3

		#当前状态位为主，取一个热切换为主
		if [[ $tmp = 0 ]]; then
			echo '当前状态位为主，取一个热切换为主'
			for (( i = 0; i < ${device_num}; i++ )); do
				if [[ $i = ${damage_device_id} ]]; then
					continue
				elif [[ ${arr[${i}]} = 1 ]]; then
					echo '将设备${i}切换为主'
					arr[${i}]=0
					tmp=1
					break
				fi
			done
		fi

		#当前状态位为热，取一个冷切换为热
		if [[ $tmp = 1 ]]; then
			echo '当前状态位为热，取一个冷切换为热'
			for (( i = 0; i < ${device_num}; i++ )); do
				if [[ $i = ${damage_device_id} ]]; then
					continue
				elif [[ ${arr[${i}]} = 2 ]]; then
					echo '将设备${i}切换为冷'
					arr[${i}]=1
					tmp=2
					break
				fi
			done
		fi

		#当前状态位为冷，不用管
		if [[ $tmp = 2 ]]; then
			echo ""
		fi
	done

	global_status="${arr[0]}"
	for (( i = 1; i < ${device_num}; i++ )); do
		global_status="${global_status},${arr[${i}]}"
	done

	#重新挂载
	mnt ${device_num} 

	#控制逻辑
	controlMod

	#退出的时候卸载设备
	unmnt;
}

main $1 $2 $3;
