#include <array>
#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Student {
    int                    id;
    std::string            name;
    int                    score;
    std::array<char, 4096> extraInfo;
};

enum class ViewType { NameAsc = 0, ScoreDesc = 1, ScoreAsc = 2 };

class StudentDatabase {
private:
    struct NameAscCmp {
        bool operator()(const Student* a, const Student* b) const {
            if (a->name != b->name) {
                return a->name < b->name;
            }
            return a->id < b->id;
        }
    };

    struct ScoreDescCmp {
        bool operator()(const Student* a, const Student* b) const {
            if (a->score != b->score) {
                return a->score > b->score;
            }
            return a->id < b->id;
        }
    };

    struct ScoreAscCmp {
        bool operator()(const Student* a, const Student* b) const {
            if (a->score != b->score) {
                return a->score < b->score;
            }
            return a->id < b->id;
        }
    };

    std::unordered_map<int, std::unique_ptr<Student>> table_;

    std::set<Student*, NameAscCmp>   nameAscView_;
    std::set<Student*, ScoreDescCmp> scoreDescView_;
    std::set<Student*, ScoreAscCmp>  scoreAscView_;

public:
    StudentDatabase() = default;

    bool insert(Student student) {
        int id = student.id;

        if (table_.find(id) != table_.end()) {
            return false;
        }

        auto     ptr = std::make_unique<Student>(std::move(student));
        Student* raw = ptr.get();

        table_.emplace(id, std::move(ptr));

        nameAscView_.insert(raw);
        scoreDescView_.insert(raw);
        scoreAscView_.insert(raw);

        return true;
    }

    bool erase(int id) {
        auto it = table_.find(id);
        if (it == table_.end()) {
            return false;
        }

        Student* raw = it->second.get();

        nameAscView_.erase(raw);
        scoreDescView_.erase(raw);
        scoreAscView_.erase(raw);

        table_.erase(it);
        return true;
    }

    bool contains(int id) const {
        return table_.find(id) != table_.end();
    }

    std::size_t size() const {
        return table_.size();
    }

    auto query(ViewType view) const {
        std::vector<const Student*> result;
        result.reserve(table_.size());

        if (view == ViewType::NameAsc) {
            for (const Student* s : nameAscView_) {
                result.push_back(s);
            }
        } else if (view == ViewType::ScoreDesc) {
            for (const Student* s : scoreDescView_) {
                result.push_back(s);
            }
        } else if (view == ViewType::ScoreAsc) {
            for (const Student* s : scoreAscView_) {
                result.push_back(s);
            }
        }

        return result;
    }
};

int main() {
    StudentDatabase db;

    return 0;
}

Student low;
low.score = 70;
low.id = std::numeric_limits<int>::min();

Student high;
high.score = 70;
high.id = std::numeric_limits<int>::max();

auto begin = scoreAscView_.lower_bound(&low);
auto end = scoreAscView_.upper_bound(&high);
