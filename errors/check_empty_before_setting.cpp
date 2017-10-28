#define SET_REQ_QA_INFO(pevent, req) \
            do {    \
                if ((pevent)->isTrunk() && !(pevent)->m_query_analysis_req_string.empty()) { \
                    (req).exdata["qa_req_str"] = (pevent)->m_query_analysis_req_string; \
                    (req).exdata["qa_res_str"] = (pevent)->m_query_analysis_res_string; \
                    (req).__isset.exdata = true;    \
                }        \
            } while (0)
