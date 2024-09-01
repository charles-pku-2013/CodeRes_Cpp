# input reeader
## 指定分隔符 delimiter
std::getline(cin, str, ch_delim)

# string
## substr
substr(pos, count)
size_t find(str, pos)
size_t find(ch, pos)
rfind...
size_t find_first / last_of ()  注意不是返回迭代器
## compare
compare(str)
compare(pos, count, str), count canbe very big
starts_with
ends_with
## number conversion
stoi stol stoll (str, size_t* pos = nullptr, int base = 10)
pos: address of an integer to store the number of characters processed
except: invalid_argument, out_of_range
stof, stod(str, size_t* pos = nullptr)

append(count, ch)
append(str, pos, count)

# bit op
## bitset
. construct
bitset<size>(unsigned long long val)
bitset<size>(const char *str, char zero, char one)
. check
all any none
count
reset: all to false
flip() flip(pos)
. conversions:
to_string() to_ulong()

# algorithm
uniquee

