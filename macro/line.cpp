// 变量名中用__LINE__, 必须用这种格式
// https://stackoverflow.com/questions/35087781/using-line-in-a-macro-definition
// 否则出现redefinition错误

#define TOKEN_PASTE(x, y) x##y
#define CAT(x, y) TOKEN_PASTE(x, y)

#define REGISTER_SERVICE_FACTORY(name, type) \
    static bool CAT(_register_service_factory_dummy, __LINE__) = \
        luca::ServiceFactory::Instance().RegisterFactory(name, []()-> type* { return new type(); });
