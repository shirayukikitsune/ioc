#pragma once

#include "Injector.h"
#include <memory>

#include <string_view>
#include <type_traits>

#ifdef _MSC_VER
#define KITSUNE_INJECTABLE(Base, Type, Helper)
#else
#define KITSUNE_INJECTABLE(Base, Type, Helper) static inline ::kitsune::ioc::InjectionHelper< Type , Base > Helper = ::kitsune::ioc::InjectionHelper< Type , Base >();
#endif

namespace kitsune::ioc {

    enum struct InjectionFlags {
        None,
        Primary,
    };

    template <typename Implementation, typename BaseType>
    class InjectionHelper {
    public:
        InjectionHelper() noexcept {
            Injector<BaseType>::getInstance().addServicePointer(new Implementation);
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
            auto Signature = std::string(__PRETTY_FUNCTION__);
            return Signature;
        }

        static std::string getClassName() {
            return getName();
        }
#else
#warning "Unable to deduce class name"

        static std::string getName() = 0;
        static std::string getClassName() = 0;
#endif

    };

    template <typename ServiceType, typename ServiceBaseType, InjectionFlags injectionFlags = InjectionFlags::None>
    class Service : public ServiceBaseType {
    private:
        template<InjectionFlags F = injectionFlags>
        typename std::enable_if_t<F == InjectionFlags::None> addService() {
            Injector<ServiceBaseType>::getInstance().addServicePointer(this);
        }

        template<InjectionFlags F = injectionFlags>
        typename std::enable_if_t<F == InjectionFlags::Primary> addService() {
            Injector<ServiceBaseType>::getInstance().addPrimaryService(this);
        }

        template<InjectionFlags F = injectionFlags>
        typename std::enable_if_t<F == InjectionFlags::None> removeService() {
            Injector<ServiceBaseType>::getInstance().removeService(this);
        }

        template<InjectionFlags F = injectionFlags>
        typename std::enable_if_t<F == InjectionFlags::Primary> removeService() {
            Injector<ServiceBaseType>::getInstance().removePrimaryService(this);
        }

        static inline const InjectionHelper<ServiceType, ServiceBaseType> t = InjectionHelper<ServiceType, ServiceBaseType>();

    public:
        Service() = default;

        virtual ~Service() {
            removeService();
        }

    };
}
