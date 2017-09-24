#!/bin/bash
# This script is the interface betweenthe camera heartbeat and the key-value
# store. It retrieve the id of the camera in the DB based on the MAC address
# and post the receoved data to that id

. /root/commonconf

read request
# ignore the header
while /bin/true; do
	read header
	[ "$header" == $'\r' ] && break
done

url="${request#POST }"
url="${url% HTTP/*}"
path="${url%\?*}"
query="${url#*\?}"

if [[ $path == $query ]]
then
	# No query then
	query=""
fi

# Retrieve body
body=""
while read -n 1 part
do
	body+="${part}"
	# We are expecting a json string
	# Check if json is complete
	[[ $(echo $body | jq '.') != "" ]] && break
done

ip=$(echo $body | jq '. | .ip')
# sed here is used to trim the double quotes at the beginning and end of the value
ip=$(sed -e 's/^"//' -e 's/"$//' <<<"$ip")
mac=$(echo $body | jq '. | .mac')
mac=$(sed -e 's/^"//' -e 's/"$//' <<<"$mac")

echo "Camera $ip heartbeat received" > /dev/kmsg

# Get the id for this MAC address
id=$(curl -s http://localhost:$KEY_VALUE_STORE_PORT/api/camera?mac=$mac | jq '.[] | .id')
# Post the new IP to this id
curl -sL \
	-X POST \
	-d "$body" \
	-H 'Content-type: application/json' \
	http://localhost:$KEY_VALUE_STORE_PORT/api/camera/$id

echo "200 OK"
exec 1>&-

