#!/bin/bash

#清理屏幕信息
clear

#清理内核信息
sudo dmesg --clear

#进入毕设代码目录
cd ~/bs

#将lsmod信息读入modInfo
lsmod > tmp
modInfo=$(cat tmp)
rm tmp

result=$(echo ${modInfo} | grep ${devices})

#如果当前挂载了驱动和设备，卸载
if [ "$result" != "" ]; then
    sudo rmmod ${devices}
fi

if [ "$result" != "" ]; then
    sudo rmmod ${drivers}
fi

echo -n '请输入创建设备个数:'
read device_num

echo -n '请依次输入各个设备的状态位:(以逗号隔开)'
read global_status

#驱动名
drivers=drivers

#设备名
devices=devices


#清理中间文件
make clean > tmp
rm tmp

#编译
make

#挂载驱动、设备
sudo insmod drivers.ko
sudo insmod devices.ko $device_num $global_status

#清理中间文件
make clean

#依次读取设备对应字符文件中的内容
for((i=0; i<$device_num; i++)); do
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


echo -n "输入想要模拟损坏的设备号："
read dev_id

echo ${dev_id}
