#pragma once

#include "Injector.h"
#include <memory>

#include <string_view>
#include <type_traits>

namespace kitsune::ioc {

    enum struct InjectionFlags {
        None,
        Primary,
    };

    template <typename Implementation, InjectionFlags flags>
    class InjectionHelper {
    public:
        InjectionHelper() {
            Injector::getInstance().addServicePointer(Implementation::getClassName(), new Implementation);
        }
    };

    template <typename Implementation>
    class InjectionHelper<Implementation, InjectionFlags::Primary> {
    public:
        InjectionHelper() {
            Injector::getInstance().addPrimaryServicePointer(Implementation::getClassName(), new Implementation);
        }
    };

    template <typename T>
    class ServiceBase {

    public:
#ifdef _MSC_VER
        static constexpr std::string_view getName() {
            auto Signature = std::string_view(__FUNCSIG__);
            return Signature;
        }

        static std::string getClassName() {
            return std::string(getName().data(), getName().length());
        }
#elif defined __GNUC__
        static std::string getName() {
            auto Signature = std::string_view(__PRETTY_FUNCTION__);
            return Signature;
        }

        static std::string getClassName() {
            return getName();
        }
#else
#error "This compiler is not supported"
#endif

    };

    template <typename ServiceType, typename ServiceBaseType, InjectionFlags injectionFlags = InjectionFlags::None>
    class Service : public ServiceBaseType {
    private:
        template<InjectionFlags F = injectionFlags>
        typename std::enable_if_t<F == InjectionFlags::None> addService() {
            Injector::getInstance().addServicePointer(this->getClassName(), this);
        }

        template<InjectionFlags F = injectionFlags>
        typename std::enable_if_t<F == InjectionFlags::Primary> addService() {
            Injector::getInstance().addPrimaryService(this->getClassName(), this);
        }

        template<InjectionFlags F = injectionFlags>
        typename std::enable_if_t<F == InjectionFlags::None> removeService() {
            Injector::getInstance().removeService(this->getClassName(), this);
        }

        template<InjectionFlags F = injectionFlags>
        typename std::enable_if_t<F == InjectionFlags::Primary> removeService() {
            Injector::getInstance().removePrimaryService(this->getClassName(), this);
        }

        inline static InjectionHelper<ServiceType, injectionFlags> t;

    public:
        Service() {
        }

        virtual ~Service() {
            removeService();
        }

    };

}
