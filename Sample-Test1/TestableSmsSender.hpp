#include <iostream>
#include "pch.h"
#include "../Project21/SmsSender.hpp"

class TestableSmsSender : public SmsSender {
public:
	void send(Schedule* schedule) override {
		std::cout << "테스트용 SmsSender class의 send메서드가 실행됨" << std::endl;
		sendMethodIsCalled = true;
	}

	bool isSendMthodIsCalled() {
		return sendMethodIsCalled;
	}
private:
	bool sendMethodIsCalled;
};