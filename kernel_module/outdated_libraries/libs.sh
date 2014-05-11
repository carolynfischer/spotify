#!/bin/bash

for dir in `cat /etc/ld.so.conf`; do
	echo "checking libraries from $dir..."
	OLDIFS=$IFS
	IFS=$'\n'

	# finds matches in open files that have been deleted and include the directory name from /etc/ld.so.conf
	# assumes the old library is indeed deleted, but for example in case of openssl upgrade it would be

	for process in `lsof -P | grep DEL | grep $dir`; do
		libname=`echo $process | awk {'print $8'}`

		if [[ $libname =~ ^$dir.*$ ]]; then
			pid=`echo $process | awk {'print $2'}`
			library=`echo $process | awk {'print $8'}`
			processname=`cat /proc/$pid/comm`
			echo -e "\tProcess $processname with pid $pid that is running outdated library $library"
		fi
	done
done
# put old delimiter back
IFS=$OLDIFS
