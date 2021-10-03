#! /bin/sh

case "$1" in
	start)
		echo "staring aesdsocket"
		start-stop-daemon -S -n aesdsocket -a /usr/bin/aesdsocket
		;;
	stop)
		echo "stopping aesdsocket"
		start-stop-daemon -K -n aesdsocket
		;;
	*)
		echo "Usage: $0 {start|stop}"
	exit 1
esac

exit 0