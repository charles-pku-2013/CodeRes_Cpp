// #include <iostream>

#define VAR int _test1__LINE__ = 0 // cannot expand line
#define VAR1(name) const char _test_##name[] = #name // this right

int main() {
    // VAR;
    VAR1(test);
    // int _test@7 = 0;

    return 0;
}

#define REGISTER_PROCESSOR(name, processor, is_default) \
    static bool _register_processor_helper_##name = \
        TokenizerMgr::instance().register_processor(#name, processor, is_default)

REGISTER_PROCESSOR(default_parser, jd_search_level::default_parser, true);
REGISTER_PROCESSOR(trigrams_and_en_trigram_with_whole_alpha_parser,
        jd_search_level::trigrams_and_en_trigram_with_whole_alpha_parser, false);
REGISTER_PROCESSOR(bigrams_and_en_trigram_parser,
        jd_search_level::bigrams_and_en_trigram_parser, false);
REGISTER_PROCESSOR(bigrams_and_en_trigram_with_whole_alpha_parser,
        jd_search_level::bigrams_and_en_trigram_with_whole_alpha_parser, false);
