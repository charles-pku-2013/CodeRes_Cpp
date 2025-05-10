#!/system/bin/sh

# PATH set in start_proxy.sh
# export PATH=/data/data/com.termux/files/usr/bin:$PATH

. /sdcard/usr/lib/log.sh
. /sdcard/usr/lib/kill_old_process.sh

function get_ip() {
    while [[ $# -gt 0 ]]; do
        local dev="$1"
        local ip_addr=$(ifconfig 2>/dev/null | grep -A1 -w "$dev" | grep -o -E 'inet\s+[0-9]{1,3}[^[:space:]]*\s' | awk '{print $2}')
        if [ ! -z "$ip_addr" ]; then
            echo "$dev $ip_addr"
            return
        fi
        shift
    done
}

# 物理端口优先顺序 热点(wlan2) > wifi(wlan0)
network_info=$(get_ip eth0 wlan2 wlan0)
read dev gateway_ip <<< "$network_info"
[ -z "$dev" ] && { LOG "Failed to get network interface!"; exit 1; }

interval=5     # 检测网络状态间隔(秒)
pref=18000     # 路由策略优先级

tun=$(ifconfig 2>/dev/null | grep -o -w -E '^tun[0-9]+' || echo "tun0")

IP=/system/bin/ip
IPTABLES=/system/bin/iptables
SYSCTL=/system/bin/sysctl

# NOTE don't forget set DNS

pidfile="/data/data/com.termux/files/usr/var/run/proxy.pid"
logfile="/sdcard/Logs/proxy.log"
# redirect output to logfile
exec >> $logfile
exec 2>&1

kill_old_process

case $1 in
    -k)
        exit 0  # only kill old instance
        ;;
esac

echo "$$" > $pidfile

# 开启IP转发功能
$SYSCTL -w net.ipv4.ip_forward=1

# 清除filter表转发链规则
$IPTABLES -F FORWARD

# 添加NAT转换，部分第三方VPN需要此设置否则无法上网，若要关闭请注释掉
$IPTABLES -t nat -A POSTROUTING -o $tun -j MASQUERADE

# 添加路由策略
$IP rule add from all table main pref $pref
$IP rule add from all iif $dev table $tun pref $(expr $pref - 1)

LOG "Proxy server $$ started on ${dev} -> ${tun}, gateway: ${gateway_ip}"

function terminate() {
    $SYSCTL -w net.ipv4.ip_forward=0
    $IP rule delete from all table main pref $pref
    $IP rule delete from all iif $dev table $tun pref $(expr $pref - 1)
    $IPTABLES -t nat -D POSTROUTING -o $tun -j MASQUERADE
    $IPTABLES -F FORWARD
    rm -f $pidfile
    pkill -9 -P $$
    LOG "Proxy server $$ terminated."
}
trap "" SIGPIPE   # SIGPIPE when caller die
trap "terminate; trap - EXIT; exit" EXIT SIGINT SIGHUP SIGQUIT SIGABRT SIGTERM SIGSTOP

contain="from all iif $dev lookup $tun"
while true; do
    if [[ $($IP rule) != *$contain* ]]; then
        if [[ $($IP ad | grep 'state UP') != *$dev* ]]; then
            LOG "dev has been lost."
        else
            $IP rule delete from all iif $dev table $tun pref $(expr $pref - 1)
            $IP rule add from all iif $dev table $tun pref $(expr $pref - 1)
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

