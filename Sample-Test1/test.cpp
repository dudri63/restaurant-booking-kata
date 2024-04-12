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

TEST_F(BookingItem, 예약은_정시에만_가능하다_정시가_아닌경우_예약불가) {
	//given
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	//when
	EXPECT_THROW({ bookingSecheduler.addSchedule(schedule); }, std::runtime_error);
}

TEST_F(BookingItem, 예약은_정시에만_가능하다_정시인_경우_예약가능) {
	//given
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	//when
	bookingSecheduler.addSchedule(schedule);

	//then
	EXPECT_EQ(true, bookingSecheduler.hasSchedule(schedule));
}

TEST_F(BookingItem, 시간대별_인원제한이_있다_같은_시간대에_Capacity_초과할_경우_예외발생) {
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

TEST_F(BookingItem, 시간대별_인원제한이_있다_같은_시간대가_다르면_Capacity_차있어도_스케쥴_추가_성공) {
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

TEST_F(BookingItem, 예약완료시_SMS는_무조건_발송) {
	//given
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	//when
	bookingSecheduler.addSchedule(schedule);

	//then
	EXPECT_EQ(true, testableSmsSender.isSendMthodIsCalled());
}

TEST_F(BookingItem, 이메일이_없는_경우에는_이메일_미발송) {
	//given
	TestableMailSender tsetableMailSender;
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
	bookingSecheduler.setMailSender(&tsetableMailSender);

	//when
	bookingSecheduler.addSchedule(schedule);

	//then
	EXPECT_EQ(0, tsetableMailSender.getCountSendMailMethodIsCalled());
}

TEST_F(BookingItem, 이메일이_있는_경우에는_이메일_발송) {
	//given
	Customer customerWithMail{ "asdf", "010-9876-5411", "test@test.com" };
	TestableMailSender testableMailSender;
	Schedule* schedule = new schedule{ ON_THE_HOUR, UNDER_CAPACITY, customerWithMail };

}


TEST_F(BookingItem, 현재날짜가_일요일인_경우_예약불가_예외처리) {

}

TEST_F(BookingItem, 현재날짜가_일요일이_아닌경우_예약가능) {

}