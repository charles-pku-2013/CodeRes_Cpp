int IntentionHandler::work(std::shared_ptr<EventResult> &result, Event* pevent)
{
    MultiSearchEvent *pe = (MultiSearchEvent *)pevent;

    if (FILTER == ((MultiSearchEvent *)pevent)->getSearchFilterType()) {
        //目前所有筛选的策略树都是一样的
        pe->m_next_stg = "stg_mobfilter";
        //string global_id;

        //get_global_id(*(((MultiSearchEvent *)pevent)->m_search_req), global_id);
        //((MultiSearchEvent *)pevent)->setGlobalId(global_id);

        //运行一次, 以确保打上tag
        pe->getFlow();
        MTEVLOG(DEBUG, pe) << "skip IntentionHandler";

        return 0;
    } // if

    try {
        if (!m_isUsed
                || pe->isMobile_PoiFilter_Search()
                || pe->isTextCardSearch()
                || pe->isCinemaSearchV2()
                || pe->isShoppingMallFilterSearch()
                || pe->isShoppingMallDealPoi()
                || pe->isCDCSearch()
                || pe->isNewGuestSearch()
                || pe->isFoodSearch()
                || pe->isBeautySearch()
                || pe->isMeishiNearbySearch()
                || pe->isEnd()) {
            MTEVLOG(DEBUG, pe) << "skip IntentionHandler";
            return 0;
        } // if

        //SearchQueryReq *req = ((MultiSearchEvent *)pevent)->m_search_req.get();
        MTEVLOG(DEBUG, pe) << "use IntentionHandler on " << (pe->isTrunk() ? "trunk" : "leaf");
        MTEVLOG_REQRES(DEBUG, pe, "SearchQueryReq is ", *(pe->m_search_req));

        std::shared_ptr<QueryAnalysisReq>   pIntentReq;
        QueryAnalysisReq trunk_intention_req;

        bool needRpc = false;
        auto& dictExData = pe->m_search_req->exdata;

        if (pe->isTrunk()) {
            needRpc = true;
        } else {        // leaf
            // get qa req res string from req exdata
            auto it1 = dictExData.find("qa_req_str");
            auto it2 = dictExData.find("qa_res_str");
            if (it1 == dictExData.end() || it2 == dictExData.end()) {
                needRpc = true;
                MTEVLOG(DEBUG, pe) << "qa_req_str or qa_res_str not set in trunk";
            } else {
                std::string qaReqStr, qaResStr;
                qaReqStr.swap(it1->second);
                qaResStr.swap(it2->second);
                dictExData.erase(it1);  // 必须做，makeRequest要用exdata
                dictExData.erase(it2);
                if (qaReqStr.empty() || qaResStr.empty()) {
                    needRpc = true;
                    MTEVLOG(DEBUG, pe) << "qa_req_str or qa_res_str is empty";
                } else {    // 读到trunk发来的字串
                    try {
                        sf::thrift::unserializeFromBin(qaReqStr, trunk_intention_req);
                        sf::thrift::unserializeFromBin(qaResStr, pe->m_query_analysis_res);
                    } catch (const std::exception &ex) {
                        MTEVLOG(ERROR, pe) << "Intention unserialize error: " << ex.what();
                        needRpc = true;
                    } catch (...) {
                        MTEVLOG(ERROR, pe) << "Intention unserialize error!";
                        needRpc = true;
                    } // try
                    // ❌❌❌❌ Hey!!! this not return, just set value!!!
                    // leaf read trunk qa success
                    MTEVLOG_REQRES(DEBUG, pe, "QaReq from trunk ", trunk_intention_req);
                    MTEVLOG_REQRES(DEBUG, pe, "QaRes from trunk ", pe->m_query_analysis_res);
                    pIntentReq.reset(new QueryAnalysisReq);
                    makeRequest(pe, *pIntentReq);
                    if (*pIntentReq != trunk_intention_req) {
                        needRpc = true;
                        MTEVLOG(DEBUG, pe) << "QaReq trunk leaf differ";
                    } else { // save serialized req res
                        pe->m_query_analysis_req_string.swap(qaReqStr); // useless on leaf
                        pe->m_query_analysis_res_string.swap(qaResStr);
                        MTEVLOG(DEBUG, pe) << "QaReq trunk leaf equal";
                    } // if
                } // if
            } // if
        } // if isTrunk

        if (needRpc) {
            std::string host = "";
            int port = 0;

            std::shared_ptr<CsSevInfo> p = singleton<ConfSevClient>::getInstance()->getSevInfo(m_appkey);
            if (!p || p->getHost().empty() || (p->getPort() <= 0)) {
                LOG_ERROR("fetch %s %s server from configservice fail!", m_appkey.c_str(), m_env.c_str());
                return -1;
            } // if
            host = p->getHost();
            port = p->getPort();
            MTEVLOG(DEBUG, pe) << "Connecting intention host: " << host << ":" << port;

            sf::TAsyncClient<QueryAnalysisClient> client(
                    m_appkey, host, port, m_conn_timeout*1000, m_read_timeout*1000, m_write_timeout*1000);
            if (!client.connect()) {
                MTEVLOG(ERROR, pe) << "connect intention failed";
                return -1;
            } // if

            if (!pIntentReq) {
                pIntentReq.reset(new QueryAnalysisReq);
                makeRequest(pe, *pIntentReq);
            } // if

            client.fetch(&QueryAnalysisClient::recv_analyze, pe->m_query_analysis_res,
                         &QueryAnalysisClient::send_analyze, *pIntentReq);
            
            MTEVLOG_REQRES(DEBUG, pe, "QaReq for rpc ", *pIntentReq);
            MTEVLOG_REQRES(DEBUG, pe, "QaRes from rpc ", pe->m_query_analysis_res);

            // serialize, write to subreq in sphinxsearch
            pe->m_query_analysis_req_string = sf::thrift::serializeToBin(*pIntentReq);
            pe->m_query_analysis_res_string = sf::thrift::serializeToBin(pe->m_query_analysis_res);

            // set log
            sf::ClientInfo ci = client.clientInfo();
            MTEVLOG(INFO, pe) << "intention request: appkey=" << ci.key
                << " peer=" << ci.peer
                << " status=" << ci.status
                << " cost=" << ci.cost/1000;
            pe->m_stat_log->addBackendCost(ci);
        } // if needRpc
        
        // main routine
        _generate_subquery_list(pe);
        MTEVLOG(DEBUG, pe) << "Generated " << pe->m_subquery_list.size() << " subqueries";

        setlog(pevent);
        if (pe->m_query_analysis_res.status != 0){
            pe->setQsError("intention_status_error", INTENTION_STATUS_ERROR);
        } // if

    } catch(const std::exception& e) {
        MTEVLOG(ERROR, pe) << "intention throw error!" << e.what();
    } catch (...) {
        MTEVLOG(ERROR, pe) << "intention throw error!";
        return -1;
    }
    return 0;
}
