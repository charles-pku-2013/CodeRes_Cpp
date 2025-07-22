# input reeader
## 指定分隔符 delimiter
std::getline(cin, str, ch_delim)

# string
## substr
substr(pos, count)
size_t find(str, pos)
size_t find(ch, pos)
rfind...
size_t find_first / last_of ()  注意不是返回迭代器, 参数格式 (sub_str_or_char, pos)
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
## reader delimiter
for( char *p = strtok( const_cast<char*>(line.c_str()), BLANK_CHAR); p; p = strtok(NULL, BLANK_CHAR) )
NOTE prefer std::getline(..., ..., delim)

append(count, ch)
append(str, pos, count)
NOTE! 插入单个字符用 push_back

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
unique unique_copy
all_of any_of none_of (beg, end, pred)
find find_if find_if_not

mismatch:
std::pair<InputIt1, InputIt2>
    mismatch( InputIt1 first1, InputIt1 last1,
              InputIt2 first2 );

equal (beg, end, beg1, [BinaryPred])

search (sbustr)
std::swap iter_swap
std::transform() (toupper, tolower) 和 for_each 区别
std::replace, replace_if

std::rotate(first, middle, last)

std::partition stable_partition is_partitioned partition_point
NOTE nth_element 返回值是void，用 begin() + n 取结果 参数和partial_sort相同
sort stable_sort partial_sort is_sorted nth_element
lower_bound upper_bound equal_range
binary_search

set_union / intersection / difference, includes, merge

make_heap push/pop_heap sort_heap is_heap is_heap_until
