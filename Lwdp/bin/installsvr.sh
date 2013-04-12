#!/bin/sh

mkdir /usr/local/TcpServer
cp tsd /etc/init.d
chmod 777 /etc/init.d/tsd
#chkconfig --add tsd
cp -rf * /usr/local/TcpServer
ln -s /usr/local/TcpServer/TSDaemon /usr/bin/tsd

service tsd restart

