#pragma once

#include <algorithm>
#include <exception>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace kitsune::ioc {
    
    ///! Provides access to the managed services
    /// This class is the core of this IoC implementation.
    /// Services will register themselves here
    class Injector {
    private:
        typedef std::unordered_multimap<std::string, std::shared_ptr<void>> RepositoryType;

        std::unordered_map<std::string, std::shared_ptr<void>> PrimaryServiceRepository;
        RepositoryType ServiceRepository;

        Injector() {}
        Injector(const Injector&) = delete;

        template <typename T>
        std::string getPrimaryRegisteredMessage() {
            return std::string("Service ") + T::getClassName() + std::string(" already registered as primary");
        }

    public:
        static Injector& getInstance() {
            static Injector *instance = new Injector;
            return *instance;
        }

        template <typename T>
        void addPrimaryService(const std::string &Name, std::shared_ptr<T> && Service) {
            auto ServiceIterator = PrimaryServiceRepository.find(T::getClassName());

            if (ServiceIterator != PrimaryServiceRepository.end()) {
                throw std::logic_error(getPrimaryRegisteredMessage<T>().data());
            }

            addService(Name, Service);

            PrimaryServiceRepository[T::getClassName()] = std::move(Service);
        }

        template <typename T>
        void addPrimaryServicePointer(const std::string &Name, T* Service) {
            auto ServiceIterator = PrimaryServiceRepository.find(T::getClassName());

            if (ServiceIterator != PrimaryServiceRepository.end()) {
                throw std::logic_error(getPrimaryRegisteredMessage<T>().data());
            }

            auto Pointer = std::shared_ptr<T>(Service);

            addService(Name, Pointer);

            PrimaryServiceRepository[T::getClassName()] = std::move(Pointer);
        }

        template <typename T>
        void addService(const std::string &Name, std::shared_ptr<T> Service) {
            ServiceRepository.emplace(Name, Service);
        }

        template <typename T>
        void addServicePointer(const std::string &Name, T* Service) {
            ServiceRepository.emplace(Name, std::shared_ptr<T>(Service));
        }

        template <typename T>
        void removePrimaryService(const std::string &Name, T* Service) {
            PrimaryServiceRepository.erase(Name);
            removeService(Name, Service);
        }

        template <typename T>
        void removeService(const std::string &Name, T* Service) {
            auto ServiceIteratorRange = ServiceRepository.equal_range(T::getClassName());

            for (auto i = ServiceIteratorRange.first, End = ServiceIteratorRange.second; i != End; ++i) {
                if (i->second.get() == Service) {
                    ServiceRepository.erase(i);
                    return;
                }
            }
        }

        template <typename BaseServiceType, typename ImplementationType = BaseServiceType>
        std::weak_ptr<ImplementationType> findService() {
            auto ServiceIterator = PrimaryServiceRepository.find(std::string(BaseServiceType::getClassName()));

            if (ServiceIterator != PrimaryServiceRepository.end()) {
                auto Base = std::reinterpret_pointer_cast<BaseServiceType, void>(ServiceIterator->second);
                return std::dynamic_pointer_cast<ImplementationType, BaseServiceType>(Base);
            }

            return findAnyService<BaseServiceType, ImplementationType>();
        }

        template <typename BaseServiceType, typename ImplementationType = BaseServiceType>
        std::weak_ptr<ImplementationType> findAnyService() {
            auto Services = findServices<BaseServiceType, ImplementationType>();

            if (Services.size() > 0) {
                return Services.front();
            }

            return std::weak_ptr<ImplementationType>();
        }

        template <typename BaseServiceType, typename ImplementationType = BaseServiceType>
        std::list<std::weak_ptr<ImplementationType>> findServices() {
            auto ServiceIteratorRange = ServiceRepository.equal_range(BaseServiceType::getClassName());

            std::list<std::weak_ptr<ImplementationType>> Services;

            for (auto i = ServiceIteratorRange.first, End = ServiceIteratorRange.second; i != End; ++i) {
                auto Pointer = std::dynamic_pointer_cast<ImplementationType, BaseServiceType>(std::reinterpret_pointer_cast<BaseServiceType, void>(i->second));

                if (Pointer) {
                    Services.emplace_back(Pointer);
                }
            }

            return Services;
        }
    };

    template <typename T, typename U = T>
    class Injectable {
        std::weak_ptr<U> InjectedService;

    public:
        Injectable() {
            reset();
        }

        void reset() {
            InjectedService = Injector::getInstance().findService<T, U>();
        }

        template<class _T = U,
            std::enable_if_t<!std::is_array_v<_T>, int> = 0>
        T* operator -> () {
            return InjectedService.lock().get();
        }

        template<class _T = U,
            std::enable_if_t<std::is_array_v<_T>, int> = 0>
            T& operator[](ptrdiff_t i) {
            return InjectedService.lock()[i];
        }

        explicit operator bool() const noexcept
        {
            return !InjectedService.expired();
        }
    };

}

