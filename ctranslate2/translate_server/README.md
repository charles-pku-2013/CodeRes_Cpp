# 编译环境准备

本程序目前使用依赖库有
protobuf-3.14.0  (手动编译安装)
brpc-1.8.0  (手动编译安装)
libcurl
fmt-11.2.0  (手动编译安装)
boost
sentencepiece和ctranslate2用最新release版本

执行:
sudo apt update
sudo apt install -y libleveldb-dev libssl-dev libsnappy-dev libgoogle-glog-dev libgflags-dev libboost-all-dev libcurl4-openssl-dev

## 编译安装依赖库
1. 安装protobuf
a) 先将系统之前安装的protobuf相关包全部卸载
    ubuntu下运行 `apt search proto` 将其中protobuf相关的包用`apt remove`删除
b) 下载 protobuf-cpp-3.14.0 运行
        ./autogen.sh
        ./configure --prefix=/usr
        make -j
        make install
        ldconfig
   完成安装

2. 安装brpc库
    下载 brpc-1.8.0 后用cmake编译:
        mkdir -p /tmp/build
        cmake -B /tmp/build . -DWITH_GLOG=ON -DWITH_DEBUG_SYMBOLS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local
        cd /tmp/build
        make -j install
        ldconfig

# 用cmake编译本程序
mkdir build && cd build
cmake ..
make -j
得到可执行文件server
若要去掉 debug log 执行
cmake .. -DCMAKE_BUILD_TYPE=Release

# 测试
启动server
先启动断句服务，见 sentence_split/split.py
python3 split.py --nobreak_file ./nonbreaking_prefix.en  --host 0.0.0.0 --port 7003

英德翻译
./server -smodel 418M/spm.model -tmodel opus-mt-en-de-convert -split_svr http://127.0.0.1:7003/split -port 8000

指定任务在队列里等待时间500ms
./server -smodel 418M/spm.model -tmodel opus-mt-en-de-convert -worker_que_timeout 500 -split_svr http://127.0.0.1:7003/split

中英翻译
./server -smodel 418M/spm.model -tmodel 418M -worker_que_timeout 500 -split_svr http://127.0.0.1:7003/split
./server -smodel 418M/spm.model -tmodel 418M -split_svr http://127.0.0.1:7003/split -port 9501 -device gpu -n_workers 3

使用gpu 加上启动参数 -device gpu

(具体参数根据实际情况调整，通过 `./server ----helpshort` 部分查看更多参数选项)

2. restful api测试
英德
curl http://127.0.0.1:8000/api/translate -d '{"text" : "Hello world!"}'; echo
curl http://127.0.0.1:8000/api/translate -d '{"text" : ["This is a machine translation program running on NPU of Huawei server. Hello. How are you today.", "All formatting is locale-independent by default. Use the format specifier to insert the appropriate number separator characters from the locale."], "srcCode" : "en", "tgtCode" : "de"}'; echo
中英
curl http://127.0.0.1:8000/api/translate -d '{"text" : ["您吃饭了吗？", "你好吗？"], "srcCode" : "zh", "tgtCode" : "en"}'; echo
curl http://127.0.0.1:9501/api/translate -d '{"text" : ["龚六堂长期研究居民收入、劳动力市场及人口 经济等话题，也曾就构建生育友好型社会、降低居民生育成本等议题向政协提交提案。在与腾讯财经的对话中，龚六堂认为，建设生育友好型社会是一 个系统、长期的工程，不可能通过一项政策或单纯的资金补贴就能完成，也不可能通过补贴就能立竿见影的提升生育率水平。", "给三岁以下的婴幼儿 每月补贴300元，是建设生育友好型社会的一个积极信号，特别是对减轻低收入人群的生育压力有很好的帮助。"], "srcCode" : "zh", "tgtCode" : "en"}'; echo

3. benchmark with wrk
/tmp/build/server -smodel 418M/spm.model -tmodel 418M -split_svr http://127.0.0.1:7003/split -port 9501 -device gpu -n_devices 8 -n_workers 4 -worker_que_timeout 10000
./wrk -c20 -t10 -d20s -s wrk.lua --timeout 10000 http://127.0.0.1:9501/api/translate
