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

go_bandit([]() {
    describe("primary service injection", []() {
        it("should inject the primary service", []() {
            kitsune::ioc::Injectable<TestService> testService;

            AssertThat((bool)testService, Equals(true));
            AssertThat(testService->testFunction(), Equals(0x74657374));
        });
    });
});

int main(int count, char ** ref) {
    return bandit::run(count, ref);
}
