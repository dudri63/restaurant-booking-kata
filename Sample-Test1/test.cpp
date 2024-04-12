#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Project21/BookingScheduler.hpp"
#include "TestableSmsSender.hpp"
#include "TestableMailSender.hpp"

using namespace testing;

class BookingItem : public testing::Test {
protected:
	void SetUp() override {
		NOT_ON_THE_HOUR = getTime(2021, 3, 26, 9, 5);
		ON_THE_HOUR = getTime(2021, 3, 26, 9, 0);

		bookingSecheduler.setSmsSender(&testableSmsSender);
	}
public:
	tm getTime(int year, int mon, int day, int hour, int min) {
		tm result = { 0, min, hour, day, mon - 1, year - 1900, 0, 0, -1 };
		mktime(&result);
		return result;
	}

	tm plusHour(tm base, int hour) {
		base.tm_hour += hour;
		mktime(&base);
		return base;
	}

	tm ON_THE_HOUR;
	tm NOT_ON_THE_HOUR;
	Customer CUSTOMER{ "Fake name", "010-1234-5678" };
	const int UNDER_CAPACITY = 1;
	const int CAPACITY_PER_HOUR = 3;

	BookingScheduler bookingSecheduler{ CAPACITY_PER_HOUR };
	TestableSmsSender testableSmsSender;
};

TEST_F(BookingItem, ������_���ÿ���_�����ϴ�_���ð�_�ƴѰ��_����Ұ�) {
	//given
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	//when
	EXPECT_THROW({ bookingSecheduler.addSchedule(schedule); }, std::runtime_error);
}

TEST_F(BookingItem, ������_���ÿ���_�����ϴ�_������_���_���డ��) {
	//given
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	//when
	bookingSecheduler.addSchedule(schedule);

	//then
	EXPECT_EQ(true, bookingSecheduler.hasSchedule(schedule));
}

TEST_F(BookingItem, �ð��뺰_�ο�������_�ִ�_����_�ð��뿡_Capacity_�ʰ���_���_���ܹ߻�) {
	//given
	Schedule* schedule = new Schedule(ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER);
	bookingSecheduler.addSchedule(schedule);

	//when
	try {
		Schedule* schedule = new Schedule(ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER);
		bookingSecheduler.addSchedule(schedule);
		FAIL();
	}
	catch (std::runtime_error& e) {
		//then
		EXPECT_EQ(string{ e.what() }, string{ "Number of people is over restaurant capacity per hour" });
	}
}

TEST_F(BookingItem, �ð��뺰_�ο�������_�ִ�_����_�ð��밡_�ٸ���_Capacity_���־_������_�߰�_����) {
	//given
	Schedule* schedule = new Schedule(ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER);
	bookingSecheduler.addSchedule(schedule);

	//when
	tm differentHour = plusHour(ON_THE_HOUR, 1);
	Schedule* newSchedule = new Schedule{ differentHour, UNDER_CAPACITY, CUSTOMER };
	bookingSecheduler.addSchedule(newSchedule);

	//then
	EXPECT_EQ(true, bookingSecheduler.hasSchedule(schedule));
}

TEST_F(BookingItem, ����Ϸ��_SMS��_������_�߼�) {
	//given
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	//when
	bookingSecheduler.addSchedule(schedule);

	//then
	EXPECT_EQ(true, testableSmsSender.isSendMthodIsCalled());
}

TEST_F(BookingItem, �̸�����_����_��쿡��_�̸���_�̹߼�) {
	//given
	TestableMailSender tsetableMailSender;
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
	bookingSecheduler.setMailSender(&tsetableMailSender);

	//when
	bookingSecheduler.addSchedule(schedule);

	//then
	EXPECT_EQ(0, tsetableMailSender.getCountSendMailMethodIsCalled());
}

TEST_F(BookingItem, �̸�����_�ִ�_��쿡��_�̸���_�߼�) {
	//given
	Customer customerWithMail{ "asdf", "010-9876-5411", "test@test.com" };
	TestableMailSender testableMailSender;
	Schedule* schedule = new schedule{ ON_THE_HOUR, UNDER_CAPACITY, customerWithMail };

}


TEST_F(BookingItem, ���糯¥��_�Ͽ�����_���_����Ұ�_����ó��) {

}

TEST_F(BookingItem, ���糯¥��_�Ͽ�����_�ƴѰ��_���డ��) {

}