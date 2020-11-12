
#define GEN_SUBQUERY_ID(subquery) \
        do { \
            std::ostringstream _oss; \
            for (auto _it = (subquery).m_setIdStr.begin(); _it != (subquery).m_setIdStr.end(); ++_it) \
                _oss << *_it; \
            _oss.flush(); \
            (subquery).setSubQueryId(sf::crc32(_oss.str())); \
        } while (0)
