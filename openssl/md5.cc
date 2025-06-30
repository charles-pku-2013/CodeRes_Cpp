/*
apt install -y libssl-dev
c++ -o /tmp/test test.cc -lcrypto -lssl -std=c++17 -g
 */
#include <openssl/evp.h>
#include <cstdio>
#include <string>
#include <iterator>
#include <iostream>
#include <fstream>

using namespace std;

string md5get(const string& content)
{
  EVP_MD_CTX*   context = EVP_MD_CTX_new();
  const EVP_MD* md = EVP_md5();
  unsigned char md_value[EVP_MAX_MD_SIZE];
  unsigned int  md_len;
  string        output;

  EVP_DigestInit_ex2(context, md, NULL);
  EVP_DigestUpdate(context, content.c_str(), content.length());
  EVP_DigestFinal_ex(context, md_value, &md_len);
  EVP_MD_CTX_free(context);

  output.resize(md_len * 2);
  for (unsigned int i = 0 ; i < md_len ; ++i)
    std::sprintf(&output[i * 2], "%02x", md_value[i]);
  return output;
}

int main(int argc, char **argv) {
    // std::string md5 = md5get("hello, world!");
    std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
    std::istreambuf_iterator<char> beg(ifs), end;
    std::string str(beg, end);
    cout << md5get(str) << endl;

    return 0;
}
