
void configDegrader(const std::string& appkey) {
    auto* c = subscribeConf(appkey);
    if (!c) {
        SFLOG(ERROR) << "subscibe " << appkey << " failed";
        return;
    }
    auto s = c->getAll();
    Json::Reader reader;
    Json::Value j;
    reader.parse(s, j);
    Json::Value::Members members = j.getMemberNames();
    for (auto it = members.begin(); it != members.end(); ++it) {
        Json::ValueType type = j[*it].type();
        switch (type) {
            case Json::booleanValue:
                SFLOG(INFO) << "Degrader: setup name=" << *it << ", val=" << sf::json::to<bool>(j[*it]);
                singleton<DegraderManager>::getInstance()
                    ->setupDegrader<ValDegrader<bool> >(*it, sf::json::to<bool>(j[*it]));
                break;
            case Json::intValue:
                SFLOG(INFO) << "Degrader: setup name=" << *it << ", val=" << sf::json::to<int>(j[*it]);
                singleton<DegraderManager>::getInstance()
                    ->setupDegrader<ValDegrader<int> >(*it, sf::json::to<int>(j[*it]));
                break;
            case Json::stringValue:
                SFLOG(INFO) << "Degrader: setup name=" << *it << ", val=" << sf::json::to<string>(j[*it]);
                singleton<DegraderManager>::getInstance()
                    ->setupDegrader<ValDegrader<std::string> >(*it, sf::json::to<string>(j[*it]));
                break;
            case Json::objectValue:
                // 限流
                if (j[*it].isMember("limit") && j[*it].isMember("gap")) {
                    auto open = sf::json::get(j[*it], "open", false);
                    auto limit = sf::json::get(j[*it], "limit", 0);
                    auto gap = sf::json::get(j[*it], "gap", 1);
                    SFLOG(INFO) << "Degrader: setup name=" << *it << ", limit=" << limit
                        << ", gap=" << gap << ", open=" << open;
                    singleton<DegraderManager>::getInstance()
                        ->setupDegrader<LimitDegrader>(*it, open, limit, gap);
                }
                break;
            default:
                break;
        }
    }

    singleton<DegraderManager>::getInstance()
        ->changeDegrader();
    SFLOG(INFO) << "subscibe " << appkey << " success";
}
