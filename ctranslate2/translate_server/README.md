# 编译环境准备

本程序目前使用依赖库有
protobuf-3.14.0
brpc-1.8.0
libcurl
sentencepiece和ctranslate2用最新release版本

## 编译安装依赖库
1. 安装protobuf
a) 先将系统之前安装的protobuf相关包全部卸载
    ubuntu下运行 `apt search proto` 将其中protobuf相关的包用`apt remove`删除
b) 下载protobuf-3.14.0 运行
        ./autogen
        ./configure --prefix=/usr
        make -j
        make install
        ldconfig
   完成安装

2. 安装brpc依赖库，执行
    `apt install -y libleveldb-dev libssl-dev libsnappy-dev libglog-dev -libgflags-dev`

3. 安装brpc库
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

(具体参数根据实际情况调整，通过 `./server --help` translate_server/server.cc 部分查看更多参数选项)

2. restful api测试
英德
curl http://127.0.0.1:8000/api/translate -d '{"articles" : "Hello world!"}'; echo
curl http://127.0.0.1:8000/api/translate -d '{"articles" : ["This is a machine translation program running on NPU of Huawei server. Hello. How are you today.", "All formatting is locale-independent by default. Use the format specifier to insert the appropriate number separator characters from the locale."], "src" : "en", "dst" : "de"}'; echo
中英
curl http://127.0.0.1:8000/api/translate -d '{"articles" : ["您吃饭了吗？", "你好吗？"], "src" : "zh", "dst" : "en"}'; echo
