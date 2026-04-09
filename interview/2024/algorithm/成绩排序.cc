/*
https://www.nowcoder.com/share/jump/71217891775705437857
 */
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

struct Student {
    using pointer = std::shared_ptr<Student>;

    Student(const std::string& _name, int _score, int _id)
        : name_(_name), score_(_score), id_(_id) {}

    std::string name_;
    int score_ = 0;
    int id_ = 0;
};

using StudentSet = std::vector<Student::pointer>;

int main() {
    int n = 0, sorter = 0;

    cin >> n >> sorter;

    StudentSet students;
    for (auto id = 0; id < n; ++id) {
        std::string name;
        int score;
        cin >> name >> score;
        students.emplace_back(std::make_shared<Student>(name, score, id));
    }

    // std::sort(students.begin(), students.end(), StudentCmp(sorter));
    std::sort(students.begin(), students.end(),
              [&sorter](const auto& lhs, const auto& rhs) -> bool {
                  int diff = lhs->score_ - rhs->score_;

                  if (diff) {
                      if (sorter) {
                          return diff < 0;
                      } else {
                          return diff > 0;
                      }
                  }

                  return lhs->id_ < rhs->id_;
              });

    for (auto& item : students) {
        cout << item->name_ << " " << item->score_ << endl;
    }

    return 0;
}

