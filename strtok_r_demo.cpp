#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

/*
 * 1,Diapers,"Huggies Little Snugglers Diapers, Size 1",37.49,USD
 */
// error example:
Product::Product(const std::string &csv) : Product()
{
    using namespace std;

    // DLOG(INFO) << "Product constructor csv = " << csv;

    char *pCsv = const_cast<char*>(csv.c_str());
    char *saveEnd1 = NULL, *saveEnd2 = NULL;
    char *pField = NULL;

    // read id
    pField = strtok_r(pCsv, ",", &saveEnd1);
    if (!pField) 
        THROW_RUNTIME_ERROR("Cannot read id field!");
    m_nID = boost::lexical_cast<IdType>(pField);
    if (m_nID < 0)
        THROW_RUNTIME_ERROR("Invalid ID value!");
    // read type
    pField = strtok_r(NULL, ",", &saveEnd1);
    if (!pField) 
        THROW_RUNTIME_ERROR("Cannot read type field!");
    m_strType = pField;
    // read name
    pField = strtok_r(NULL, "\"", &saveEnd1); // NOTE!!! 同一个字符串用同一个saveEnd!!!
    if (!pField) 
        THROW_RUNTIME_ERROR("Cannot read name field!");
    m_strName = pField;
    // read price
    pField = strtok_r(NULL, ",", &saveEnd1);
    if (!pField) 
        THROW_RUNTIME_ERROR("Cannot read price field!");
    m_fPrice = boost::lexical_cast<float>(pField);
    if (m_fPrice < 0.0)
        THROW_RUNTIME_ERROR("Invalid price value!");
    // read price unit
    // pField = strtok_r(NULL, ",", &saveEnd1);
    // m_ePriceUnit = s_mapPriceUnit[pField];
}

int main()
{
    string s("1,2,3 4,5,6, 7 8,9");
    char *pStr = const_cast<char*>(s.c_str());
    char *saveEnd1 = NULL, *saveEnd2 = NULL;

    for( char *p = strtok_r(pStr, " ", &saveEnd1); p; p = strtok_r(NULL, " ", &saveEnd1) ) {
        cout << p << endl;
        for( char *q = strtok_r(p, ",", &saveEnd2); q; q = strtok_r(NULL, ",", &saveEnd2) )
            cout << q << " ";
        cout << endl;
    }

    return 0;
}

