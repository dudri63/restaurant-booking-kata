#include <iostream>
#include "pch.h"
#include "../Project21/SmsSender.hpp"

class TestableSmsSender : public SmsSender {
public:
	void send(Schedule* schedule) override {
		std::cout << "�׽�Ʈ�� SmsSender class�� send�޼��尡 �����" << std::endl;
		sendMethodIsCalled = true;
	}

	bool isSendMthodIsCalled() {
		return sendMethodIsCalled;
	}
private:
	bool sendMethodIsCalled;
};