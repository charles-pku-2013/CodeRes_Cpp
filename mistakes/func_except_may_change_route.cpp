
static
void do_request(const std::vector<LogReq::pointer> &vecReq,
                std::vector<QsRes::pointer> &vecRes)
{
    typedef void (MTSearchClient::*QsApi)(SearchMultiRes&, const SearchQueryReq&);
    static std::unordered_map<std::string, QsApi> dictApi = {
        {"MTSearchDeal", &MTSearchClient::MTSearchDeal},
        {"MTSearchPoi", &MTSearchClient::MTSearchPoi},
        {"MTMultiSearch", &MTSearchClient::MTMultiSearch},
        {"MTMultiSearchDealPoi", &MTSearchClient::MTMultiSearchDealPoi},
        {"AttributeFilter", &MTSearchClient::AttributeFilter}
    };

    cerr << "Requesting " << g_strHost << ":" << g_nPort << endl;

    vecRes.clear();

    ThriftClient<MTSearchClient>    thriftCli(g_strHost, (uint16_t)g_nPort);
    THROW_RUNTIME_ERROR_IF(!thriftCli.start(10), "Connect to remote server fail!");

    auto pClient = thriftCli.client().get();
    for (auto it = vecReq.begin(); it != vecReq.end(); ++it) {
        auto apiIt = dictApi.find((*it)->ifName());
        if (apiIt == dictApi.end()) {
            LOG(ERROR) << (*it)->ifName() << " is not a valid ifname! LogNO = " << (*it)->logNO();
            continue;
        } // if
        auto pRes = std::make_shared<QsRes>();
        // NOTE!!! RPC 调用很有可能抛出异常，如不捕获处理，程序流程将被改写。不能按预期流程执行
        (pClient->*(apiIt->second))(pRes->res(), (*it)->query());
        pRes->setNO((*it)->logNO());
        vecRes.push_back(pRes);
    } // for it
}

#if 0
try {
    (pClient->*(apiIt->second))(pRes->res(), (*it)->query());
} catch (const std::exception &ex) {
    LOG(ERROR) << "RPC request error: " << ex.what();
    continue;
} catch (...) {
    LOG(ERROR) << "RPC request error!";
    continue;
} // try
#endif
