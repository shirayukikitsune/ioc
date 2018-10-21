#include "bandit/bandit.h"

#include "kitsune/ioc/Injector.h"
#include "kitsune/ioc/Service.h"

#include <iostream>
#include <string>

using namespace snowhouse;
using namespace bandit;

// Service interface
class TestService : public kitsune::ioc::ServiceBase<TestService> {
public:
    virtual int testFunction() = 0;
};

class OtherTestServiceImplementation : public kitsune::ioc::Service<OtherTestServiceImplementation, TestService> {
public:
    int testFunction() override {
        return 0x6661696c;
    }
};

class TestServiceImplementation : public kitsune::ioc::Service<TestServiceImplementation, TestService, kitsune::ioc::InjectionFlags::Primary> {
public:
    int testFunction() override {
        return 0x74657374;
    }
};

class NotInjectedService : public kitsune::ioc::ServiceBase<NotInjectedService> {
};

go_bandit([]() {
    describe("primary service injection", []() {
        it("should inject the primary service", []() {
            kitsune::ioc::Injectable<TestService> testService;

            AssertThat((bool)testService, Equals(true));
            AssertThat(testService->testFunction(), Equals(0x74657374));
        });

        it("should be able to retrieve both services", []() {
            auto services = kitsune::ioc::Injector::getInstance().findServices<TestService>();

            AssertThat(services.empty(), Equals(false));
            AssertThat(services.size(), Equals(2));

            for (auto &servicePtr : services) {
                auto &service = servicePtr.lock();
                AssertThat((bool)service, Equals(true));
                if (std::dynamic_pointer_cast<OtherTestServiceImplementation>(service)) {
                    AssertThat(service->testFunction(), Equals(0x6661696c));
                }
                else {
                    AssertThat(service->testFunction(), Equals(0x74657374));
                }
            }
        });

        it("should not inject wrong classes", []() {
            kitsune::ioc::Injectable<NotInjectedService> testService;

            AssertThat((bool)testService, Equals(false));
        });
    });
});

int main(int count, char ** ref) {
    return bandit::run(count, ref);
}
