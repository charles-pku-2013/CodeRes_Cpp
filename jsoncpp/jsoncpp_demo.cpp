/*
 * c++ -o /tmp/test jsoncpp_demo.cpp -ljsoncpp -std=c++11 -g
 * brew install jsoncpp
 * c++ -o /tmp/test jsoncpp/jsoncpp_demo.cpp -I$(brew --prefix jsoncpp)/include -L$(brew --prefix jsoncpp)/lib -ljsoncpp -std=c++17 -g
 */
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#if 0
// NOTE
// 新版本 write
Json::Value jvResult;
// ...
Json::StreamWriterBuilder jswBuilder;
std::string out = Json::writeString(jswBuilder, jvResult);
// read
Json::CharReaderBuilder readerBuilder;
std::shared_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
Json::Value jvFtpData;
JSONCPP_STRING errs;
if (!reader->parse(strLine.data(), strLine.data() + strLine.size(), &jvFtpData, &errs)) {
    LogError << "json parse fail content:" << strLine;
}
#endif


using namespace std;

// #if 0
// 读取json的标准流程
#if 0
void readJson()
{
    Json::Value  response;
    Json::Reader reader;
    if (!reader.parse(respString(), response)) {
        m_strErrMsg = "Parse response json error.";
        return INVALID_JSON;
    } // if

    try {
        int status = response["status"].asInt();
        if (status) {
            m_strErrMsg = response["errmsg"].asString();
            return status;
        } // if
        Json::Value &itemArray = response["result"][0]["most_like"];
        result.reserve(itemArray.size());
        for (Json::Value::iterator it = itemArray.begin(); it != itemArray.end(); ++it)
            result.emplace_back(it->asString());
        return 0;
    } catch (const std::exception &ex) {
        m_strErrMsg = "Parse response json error: ";
        m_strErrMsg.append( ex.what() );
        return INVALID_JSON;
    } // try
}
        /*
         * 判断成员是否存在
         * if (root.isMember("LogNO"))
         *     pReq->setNO(root["LogNO"].asUInt64());
         */
#endif

void WriteJsonData(const char* filename)
{
    Json::Reader reader;
    Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array

    std::ifstream is;
    is.open (filename, std::ios::binary );
    if (reader.parse(is, root))
    {
        Json::Value arrayObj;   // 构建对象
        Json::Value new_item, new_item1;
        new_item["date"] = "2011-11-11";
        new_item1["time"] = "11:11:11";
        arrayObj.append(new_item);  // 插入数组成员
        arrayObj.append(new_item1); // 插入数组成员
        int file_size = root["files"].size();
        for(int i = 0; i < file_size; ++i)
            root["files"][i]["exifs"] = arrayObj;   // 插入原json中
        std::string out = root.toStyledString();
        // 输出无格式json字符串
        Json::FastWriter writer;
        std::string strWrite = writer.write(root);
        std::ofstream ofs;
        ofs.open("test_write.json");
        ofs << strWrite;
        ofs.close();
    }

    is.close();
}

void test()
{
    std::string str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(str, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
    {
        // std::string upload_id = root["uploadid"].asString();  // 访问节点，upload_id = "UP000000"
        // cout << "uploadid = " << upload_id << endl;
        // int code = root["code"].asInt();    // 访问节点，code = 100
        // cout << "code = " << code << endl;
    }

    for (auto it = root.begin(); it != root.end(); ++it)
        cout << it.key().asString() << " : " << (*it).asString() << endl;
}

void test1()
{
    Json::Reader    reader;
    Json::Value     root;
    if (!reader.parse(cin, root, false))
        return;

    string    name;
    int       age;
    bool      alive;
    name = root["firstName"].asString();
    age = root["age"].asInt();
    alive = root["isAlive"].asBool();

    Json::Value val = root["MyName"];
    if (val.isNull())
        cout << "no property named MyName" << endl;
}
// #endif

void test2()
{
    Json::Value root;
    root["multi"] = false;
    root["sep"] = ":";
    root["key1"]["key2"] = 123;

    Json::StyledWriter writer;
    string outStr = writer.write(root);
    cout << outStr << endl;
}

int main()
{
    // test();
    // test1();
    test2();

    return 0;
}

/*
 * void KnnServiceHandler::handleRequest(std::string& _return, const std::string& request)
 * {
 *     Json::Reader    reader;
 *     Json::Value     root;
 *     int             n = 0;
 *
 *     // DLOG(INFO) << "KnnService received request: " << request;
 *
 *     typedef std::vector<std::string> StringArray;
 *     typedef std::map< std::string, StringArray> Query;
 *     Query           query;
 *
 *     if (!reader.parse(request, root))
 *         THROW_INVALID_REQUEST("Json parse fail!");
 *
 *     // get n
 *     try {
 *         n = root["n"].asInt();
 *         if (n <= 0)
 *             THROW_INVALID_REQUEST("Invalid n value");
 *     } catch (const std::exception &ex) {
 *         THROW_INVALID_REQUEST("Json parse fail! " << ex.what());
 *     } // try
 *
 *     // get query items
 *     auto items = root["items"];
 *     if (items.isNull())
 *         THROW_INVALID_REQUEST("Bad request format!");
 *
 *     if (items.isArray()) {
 *         for (auto it = items.begin(); it != items.end(); ++it) {
 *             try {
 *                 string item = (*it).asString();
 *                 if (!item.empty())
 *                     query.insert( std::make_pair(item, StringArray()) );
 *             } catch (const std::exception &ex) {;}
 *         } // for
 *     } else {
 *         try {
 *             string item = items.asString();
 *             if (item.empty())
 *                 THROW_INVALID_REQUEST("Bad request format!");
 *             query.insert( std::make_pair(item, StringArray()) );
 *         } catch (const std::exception &ex) {
 *             THROW_INVALID_REQUEST("Json parse fail! " << ex.what());
 *         } // try
 *     } // if
 *
 *     if (query.empty())
 *         THROW_INVALID_REQUEST("Bad request format! no valid query item");
 *
 *     for (auto &v : query)
 *         queryByItemNoWeight(v.second, v.first, n);
 *
 *     Json::Value     outRoot; // array of record
 *     for (auto &v : query) {
 *         Json::Value record, mostLike;
 *         record["item"] = v.first;
 *         for (auto &str : v.second)
 *             mostLike.append(str);
 *         record["most_like"].swap(mostLike);
 *         outRoot.append(Json::Value());
 *         outRoot[ outRoot.size()-1 ].swap(record);
 *     } // for
 *     Json::FastWriter writer;
 *     _return = writer.write(outRoot);
 * }
 */

/*
 * {
 *   "firstName": "John",
 *   "lastName": "Smith",
 *   "isAlive": true,
 *   "age": 25,
 *   "address": {
 *     "streetAddress": "21 2nd Street",
 *     "city": "New York",
 *     "state": "NY",
 *     "postalCode": "10021-3100"
 *   },
 *   "phoneNumbers": [
 *     {
 *       "type": "home",
 *       "number": "212 555-1234"
 *     },
 *     {
 *       "type": "office",
 *       "number": "646 555-4567"
 *     },
 *     {
 *       "type": "mobile",
 *       "number": "123 456-7890"
 *     }
 *   ],
 *   "children": [],
 *   "spouse": null
 * }
 */

