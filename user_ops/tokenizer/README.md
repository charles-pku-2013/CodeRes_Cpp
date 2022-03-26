# 环境：centos with gcc4.8.5, boost lib

# 文件目录说明：
export_tokenizer.py: 用tokenizer op生成样例模型
export_zero_out.py: ...

# 编译：
1. 编译tfs model_server
bazel build -c opt //tfs/tensorflow_serving/model_servers:tensorflow_model_server
2. 编译op
bazel build -c opt @org_tensorflow//tensorflow/core/user_ops:zero_out.so
bazel build -c opt @org_tensorflow//tensorflow/core/user_ops:tokenizer.so
(注：发现cornerstone项目里有boost库，将来会考虑用项目里的boost而不是系统预编译好的，也许能解决某些环境model_server加载tokenizer op找不到符号失败的问题)

# 启动model_server, 加载op lib
修改后的model_server新增两个启动参数：
--libdir: 指定含有oplib的目录，加载其中所有.so
--ops: 打印所有支持的op name，用于验证oplib是否加载成功，op列表里有自定义的op说明加载成功

# test zero_out
## start model_server
bazel-bin/tfs/tensorflow_serving/model_servers/tensorflow_model_server --port=8500  --rest_api_port=8501 --model_name=test_model --model_base_path=`pwd`/tfs/tensorflow_serving/servables/tensorflow/testdata/zero_out --libdir=/tmp/libs
## Send restful request
curl -d '{"instances": [5,4,3,2,1]}' -X POST http://localhost:8501/v1/models/test_model:predict
## expected resp
{
    "predictions": [5, 0, 0, 0, 0]
}

# test tokenizer
## start
bazel-bin/tfs/tensorflow_serving/model_servers/tensorflow_model_server --port=8500  --rest_api_port=8501 --model_name=test_model --model_base_path=`pwd`/tfs/tensorflow_serving/servables/tensorflow/testdata/tokenizer --libdir=/tmp/libs
## Send request
curl -d '{"instances": ["水写布", "仿宣纸"]}' -X POST http://localhost:8501/v1/models/test_model:predict
## expected resp
{
    "predictions": ["^ 水写布", "水写布 仿宣纸", "仿宣纸 $"]
}






