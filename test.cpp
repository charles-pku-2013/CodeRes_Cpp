#include <iostream>
#include <memory>
#include <thread>

using namespace std;

std::string s = "{\n"
"    unite_topics = [\n"
"        {\n"
"            topic_prefix = \"new_arch_search\",\n"
"            # 没有配置在list中的topic，仍然采用自己的线程管理\n"
"            unite_topic_list = [\"new_arch_search_croco\", \"new_arch_search_feature_dump\",\n"
"                                \"new_arch_search_falcon_sku_online\"],\n"
"            thread_num = 15,\n"
"            queue_max = 150,\n"
"            args = {\n"
"                broker_list = \"jdq4jinshajianght.jdq.jd.local:9000\",\n"
"                sasl_username = \"85865cf1\",\n"
"                sasl_password = \"V7pw4Csu2OruZGw7\",\n"
"                compression_type = \"gzip\",\n"
"                max_batch_size = 2000000,\n"
"                queue_buffer_ms = 300,  # queue.buffering.max.ms\n"
"            }\n"
"        }\n"
"    ]\n"
"    sender_topic = [\n"
"        # 用于埋点的kafka配置\n"
"        {\n"
"            key = \"bp_data_kafka\",\n"
"            sender_type = \"kafka_sender\",\n"
"            thread_num = 5,\n"
"            queue_max = 100,\n"
"            module = \"smerger\",\n"
"            use_random_key = true,\n"
"            args = {\n"
"                topic : \"new_arch_search_croco\",\n"
"                sasl_username : \"Ped78a55f\",\n"
"                sasl_password : \"pQc0M6b78y3tTJZx\",\n"
"                broker_list : \"jdq4jinshajianght.jdq.jd.local:9000\",\n"
"            }\n"
"        }\n"
"        # 用于feature dump kafka配置\n"
"        {\n"
"            key = \"feature_dump_kafka\",\n"
"            sender_type = \"kafka_sender\",\n"
"            thread_num = 5,\n"
"            queue_max = 100,\n"
"            module = \"smerger\",\n"
"            use_random_key = true,\n"
"            env = [\"demix3\", \"a3\", \"c3\", \"c4\", \"c5\"],  # 增加白名单，降低连接数\n"
"            partiton_distribution = {\n"
"                parallelism = 300,\n"
"                maxParallelism = 32768,\n"
"            },\n"
"            args = {\n"
"                topic : \"new_arch_search_feature_dump\",\n"
"                broker_list : \"jdq4jinshajianght.jdq.jd.local:9000\",\n"
"                sasl_username : \"85865cf1\",\n"
"                sasl_password : \"V7pw4Csu2OruZGw7\",\n"
"                compression_type : \"gzip\",\n"
"                max_batch_size : 2000000,\n"
"                queue_buffer_ms : 300,  # queue.buffering.max.ms\n"
"            }\n"
"        }\n"
"        {\n"
"            key = \"falcon_sku_data_kafka\",\n"
"            sender_type = \"kafka_sender\",\n"
"            thread_num = 5,\n"
"            queue_max = 100,\n"
"            module = \"smerger\",\n"
"            args = {\n"
"                topic : \"new_arch_search_falcon_sku_online\",\n"
"                sasl_username : \"P4044c9da\",\n"
"                sasl_password : \"gbQrDuDxPeBQHZG8\",\n"
"                broker_list : \"jdq4jinshajianght.jdq.jd.local:9000\",\n"
"                compression_type : \"gzip\",\n"
"            }\n"
"        }\n"
"        # 特征溯源配置\n"
"        {\n"
"            key = \"trace_report_data_kafka\",\n"
"            sender_type = \"kafka_sender\",\n"
"            thread_num = 5,\n"
"            queue_max = 100,\n"
"            module = \"smerger\",\n"
"            send_raw_direct = true,\n"
"            args = {\n"
"                topic : \"search_unified_data_trace_src_prod\",\n"
"                sasl_username : \"Pb83e91ad\",\n"
"                sasl_password : \"4O6IF1lZPP8MlEjo\",\n"
"                broker_list : \"qianhelf.jdq.jd.local:9000\",\n"
"                compression_type : \"gzip\",\n"
"            }\n"
"        }\n"
"    ]\n"
"}\n"
"";

int main() {
    cout << s << endl;

    cout << "main terminates" << endl;
    return 0;
}

/*
Test begin
Foo construct
Hello
Test end
main terminates
Foo destruct
*/
