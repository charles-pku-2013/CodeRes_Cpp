class TextEditor {
    list<char> l;
    list<char>::iterator cur = l.begin();

public:
    void addText(string text) {
        for (char ch : text)
            l.insert(cur, ch);
    }

    int deleteText(int k) {
        int k0 = k;
        for (; k && cur != l.begin(); --k)
            cur = l.erase(prev(cur));
        return k0 - k;
    }

    string text() {
        string s;
        auto it = cur;
        for (int k = 10; k && it != l.begin(); --k)
            s += *--it;
        reverse(s.begin(), s.end());
        return s;
    }

    string cursorLeft(int k) {
        for (; k && cur != l.begin(); --k)
            --cur;
        return text();
    }

    string cursorRight(int k) {
        for (; k && cur != l.end(); --k)
            ++cur;
        return text();
    }
};
