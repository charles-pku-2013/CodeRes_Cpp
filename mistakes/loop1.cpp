
void Discrete::genNeFv()
{
    using namespace std;

    LOG(INFO) << "Creating new feature vector...";

    IFvFile ifv(m_strInput);
    OFvFile ofv(m_strOutput);

    FeatureVector fv;
    uint32_t count = 0;
    while (ifv.readOne(fv)) {
        FeatureVectorHandle hfv(fv);
        ++count;
        DLOG(INFO) << "Reading fv " << count;

        for (auto fit = fv.floatFeatures.begin(); fit != fv.floatFeatures.end();) {
            const string &key = fit->first;
            DLOG(INFO) << "Handling floatfeature " << key;
            if (fit->second.empty())        // !! NOTE, erase not take place, infinite loop!
                continue;
            const double &val = fit->second.begin()->second;
            auto it = m_dictRange.find(key);
            if (it == m_dictRange.end())
                continue;
            const RangeArray &arrRange = it->second;
            const string& newVal = range(arrRange, val);
            string newFtName = key + "_sec";
            hfv.setFeature(newFtName, newVal);
            fit = fv.floatFeatures.erase(fit);
        } // for fit

        if (fv.floatFeatures.empty())
            fv.__isset.floatFeatures = false;

        DLOG(INFO) << "Write fv " << count;
        ofv.writeOne(fv);
    } // while
}
