#include <iostream>
#include <set>

using namespace std;

struct DocId {
    DocId() : poiid_(0), dealid_(0), channel_(0) {}
    DocId(uint32_t _Poiid, uint32_t _Dealid, int _Channel)
        : poiid_(_Poiid), dealid_(_Dealid), channel_(_Channel) {}

    bool operator<(const DocId &rhs) const
    {
        int64_t diff = (int64_t)poiid_ - (int64_t)rhs.poiid_;
        if (diff) { cout << "poi diff = " << diff << endl; return (diff < 0); }

        diff = (int64_t)dealid_ - (int64_t)rhs.dealid_;
        if (diff) {  cout << "deal diff = " << diff << endl; return (diff < 0); }

        diff = (int64_t)channel_ - (int64_t)rhs.channel_;
        cout << "channel diff = " << diff;
        return (diff < 0);
    }
    // bool operator<(const DocId &rhs) const
    // {
        // int64_t diff = poiid_ - rhs.poiid_;
        // if (diff) { cout << "poi diff = " << diff << endl; return (diff < 0); }

        // diff = dealid_ - rhs.dealid_;
        // if (diff) {  cout << "deal diff = " << diff << endl; return (diff < 0); }

        // diff = channel_ - rhs.channel_;
        // cout << "channel diff = " << diff;
        // return (diff < 0);
    // }

    uint32_t poiid_, dealid_;
    int channel_;
};

std::ostream& operator<<(std::ostream &os, const DocId &id)
{
    os << id.poiid_ << " " << id.dealid_ << " " << id.channel_;
    return os;
}

int main()
{
    std::set<DocId> idlist;
    idlist.insert(DocId(157101770, 45020099, 228));
    idlist.insert(DocId(118565412, 48361575, 228));
    idlist.insert(DocId(162700105, 48913747, 228));

    cout << idlist.size() << endl;
    cout << *idlist.begin() << endl;

    return 0;
}

