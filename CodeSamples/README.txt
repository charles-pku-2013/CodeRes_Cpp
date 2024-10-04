├── cpp                              # C++代码样本
│   └── servable_manager            # 受tfs模型版本管理功能所启发开发的通用线上servable版本管理工具
                                        # 支持线上无感知热更新。Servable可以是任意类如模型、字典等。
                                        # 主要展示技能: 模板、多态、宏、数据结构、多线程、boost库、absl库
│   └── trace_logger                # 请求日志追踪工具
                                        # 主要展示技能: 模板、设计模式、boot log库
│   ├── timer_task.h                # 定时器任务基类，该类广泛应用与我所参与开发的各种工程项目中
                                        # 主要展示技能: boost asio库、多态
│   ├── knn_benchmark.cc            # tfs-knn 压测工具
                                        # 主要展示技能: 模板（RpcClient）、多线程、RPC

└── shell                            # Linux shell script代码样本
    └── lib                          # 笔记
    ├── proxy.sh                     # 将手机变成软路由，共享vpn. https://blog.csdn.net/chaosun2012/article/details/142442017?spm=1001.2014.3001.5501
    ├── sync_dir.sh                  # 同步手机本地目录到远程nas目录
    ├── videomerge.sh                # 手机上无损合并短视频工具，保留原始meta和创建、修改时间. https://blog.csdn.net/chaosun2012/article/details/142455231?spm=1001.2014.3001.5501
    ├── batch_video_convert.sh       # 批量视频格式转换工具，4k转1080p降低码率，便于远程观看nas
