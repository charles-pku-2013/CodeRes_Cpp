
struct _RegisterDcFactoryHelper {
    _RegisterDcFactoryHelper(const std::string &name, const DataContainerSet::DataContainerFactory &factory) {
        DataContainerSet::Instance().RegisterFactory(name, factory);
    }
};

#define REGISTER_DATA_CONTAINER_FACTORY(name, type) \
    static _RegisterDcFactoryHelper \
        _register_dc_factory_helper_##__COUNTER__(name, []()-> type* { return new type(); });

// usage
REGISTER_DATA_CONTAINER_FACTORY("CvrModelGBDT", CvrModelGBDTDataContainer)
