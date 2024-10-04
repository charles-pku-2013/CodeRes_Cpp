#!/system/bin/sh
#
# https://blog.csdn.net/chaosun2012/article/details/142442017?spm=1001.2014.3001.5501

# PATH set in start_proxy.sh
# export PATH=/data/data/com.termux/files/usr/bin:$PATH

dev='wlan0'    # 物理接口名称，eth0、wlan0
interval=5     # 检测网络状态间隔(秒)
pref=18000     # 路由策略优先级

tun=$(ifconfig 2>/dev/null | grep -o -w -E '^tun[0-9]+' || echo "tun0")

# NOTE don't forget set DNS

pidfile="/data/data/com.termux/files/usr/var/run/proxy.pid"
logfile="/sdcard/Logs/proxy.log"
# redirect output to logfile
exec >> $logfile
exec 2>&1

. /sdcard/usr/lib/log.sh
. /sdcard/usr/lib/kill_old_process.sh

kill_old_process

case $1 in
    -k)
        exit 0  # only kill old instance
        ;;
esac

echo "$$" > $pidfile

# 开启IP转发功能
sysctl -w net.ipv4.ip_forward=1

# 清除filter表转发链规则
iptables -F FORWARD

# 添加NAT转换，部分第三方VPN需要此设置否则无法上网，若要关闭请注释掉
iptables -t nat -A POSTROUTING -o $tun -j MASQUERADE

# 添加路由策略
ip rule add from all table main pref $pref
ip rule add from all iif $dev table $tun pref $(expr $pref - 1)

LOG "Proxy server $$ started on ${dev} -> ${tun}."

function terminate() {
    sysctl -w net.ipv4.ip_forward=0
    ip rule delete from all table main pref $pref
    ip rule delete from all iif $dev table $tun pref $(expr $pref - 1)
    iptables -t nat -D POSTROUTING -o $tun -j MASQUERADE
    iptables -F FORWARD
    rm -f $pidfile
    pkill -9 -P $$
    LOG "Proxy server $$ terminated."
}
trap "" SIGPIPE   # SIGPIPE when caller die
trap "terminate; trap - EXIT; exit" EXIT SIGINT SIGHUP SIGQUIT SIGABRT SIGTERM SIGSTOP

contain="from all iif $dev lookup $tun"
while true; do
    if [[ $(ip rule) != *$contain* ]]; then
        if [[ $(ip ad | grep 'state UP') != *$dev* ]]; then
            LOG "dev has been lost."
        else
            ip rule delete from all iif $dev table $tun pref $(expr $pref - 1)
            ip rule add from all iif $dev table $tun pref $(expr $pref - 1)
            LOG "network changed, reset the routing policy."
        fi
    fi
    sleep $interval
done

# supervisor conf
# [program:proxy]
# command=/data/data/com.termux/files/usr/bin/start_proxy.sh
# autostart=false
# autorestart=true
# stdout_logfile=/data/data/com.termux/files/usr/var/log/supervisor/%(program_name)s.log
# stdout_logfile_maxbytes=10MB
# redirect_stderr=true
# stopasgroup=true
# killasgroup=true

# NOTE
# trap terminate 调试log不会打印出来是因为 start_proxy.sh进程先于proxy.sh退出了。

