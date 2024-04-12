#pragma once
#include "Schedule.hpp"

class MailSender {
public:
	virtual void sendMail(Schedule* schedule) {
		//send mail
	}
};