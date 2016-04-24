device_num=$1

status=$2

echo "device_num=${device_num}"
echo "status=${status}"

OLD_IFS="$IFS" 
IFS="," 
arr=($status) 
IFS="$OLD_IFS" 

global_status="${arr[0]}"
for (( i = 1; i < ${device_num}; i++ )); do
	global_status="${global_status},${arr[${i}]}"
done

echo $global_status