#include <locale>
#include <iostream>
#include <errno.h>

struct _locale_t_wrapper {
    _locale_t_wrapper(int category_mask, const char *locale) { 
        loc_ = newlocale(category_mask, locale, (locale_t)0); 
        if (!loc_)
            std::cout << "newlocale faile: " << strerror(errno) << std::endl;
    }

    ~_locale_t_wrapper() { 
        if (loc_)
            freelocale(loc_); 
    }

    const locale_t& locale() const { return loc_; }

    locale_t loc_;
};

static _locale_t_wrapper locale_t_obj(LC_ALL, "");

int main() {
    using namespace std;

    locale_t stat = uselocale(locale_t_obj.locale());

    return 0;
}
