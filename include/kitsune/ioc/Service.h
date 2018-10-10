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

    template <typename T>
    class ServiceBase {

    public:
#ifdef _MSC_VER
        static constexpr std::string_view getName() {
            auto Signature = std::string_view(__FUNCSIG__);
            return Signature;
        }
#elif defined __PRETTY_FUNCTION__
        static constexpr std::string_view getName() {
            auto Signature = std::string_view(__PRETTY_FUNCTION__);
            return Signature;
        }
#else
#error "This compiler is not supported"
#endif

        static std::string getClassName() {
            return std::string(getName().data(), getName().length());
        }

    };

    template <typename T, InjectionFlags injectionFlags = InjectionFlags::None>
    class Service : public T {
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

    public:
        Service() {
            addService();
        }

        virtual ~Service() {
            removeService();
        }

    };

}
