void test_framework() {
    using searcher::SearcherService;
    auto desc = SearcherService::descriptor();
    cout << desc->DebugString() << endl;
    for (int i = 0; i < SearcherService::descriptor()->method_count(); ++i) {
        cout << desc->method(i)->DebugString() << endl;
    }
}
