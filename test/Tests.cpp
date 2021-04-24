#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "ish/Event.h"
#include "Globals.h"


TEST_CASE("Event Class Test", "[EventClass]")
{
	// These are renewed before each section independently
	ish::Event<> e_Event;
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	int count4 = 0;

	auto del_1 = [&]() { count1 += 1;};
	auto del_2 = [&]() { count2 += 2;};
	auto del_3 = [&]() { count3 += 3;};
	auto del_4 = [&]() { count4 += 4;};
	auto del_un = []() { 1 + 2; };


	SECTION("Initialized event must have zero(0) subscriber") 
	{
		// Set and Execute and Eval
		REQUIRE(e_Event.GetSubscriberCount() == 0);
	}

	SECTION("Adding a subscriber must increase sub count by one") 
	{
		// Set and Execute
		e_Event.Subscribe(del_1);

		// Eval
		REQUIRE(e_Event.GetSubscriberCount() == 1);
	}

	SECTION("Adding multiple subscriber must increase sub count by that number")
	{
		// Set and Execute
		e_Event.Subscribe(del_1);
		e_Event.Subscribe(del_3).Subscribe(del_2);

		// Eval
		REQUIRE(e_Event.GetSubscriberCount() == 3);
	}

	SECTION("Trying to remove an unsubscribed subscriber must not change count")
	{
		// Set and Execute
		e_Event.Subscribe(del_1).Subscribe(del_2);
		e_Event.Unsubscribe(del_un);

		// Eval
		REQUIRE(e_Event.GetSubscriberCount() == 2);
	}

	SECTION("Unsubscription should remove the exact subscribers only")
	{
		// Set
		e_Event.Subscribe(del_1).Subscribe(del_2).Subscribe(del_3).Subscribe(del_4);
		e_Event.Unsubscribe(del_3).Unsubscribe(del_4);

		// Execute
		e_Event.Fire();

		// Test
		REQUIRE(count1 == 1);
		REQUIRE(count2 == 2);
		REQUIRE(count3 == 0);
		REQUIRE(count4 == 0);
	}

	SECTION("Firing event over multiple subscriber must update all their values")
	{
		e_Event.Subscribe(del_1).Subscribe(del_2).Subscribe(del_3).Subscribe(del_4);

		// Execute
		e_Event.Fire();

		// Test
		REQUIRE(count1 == 1);
		REQUIRE(count2 == 2);
		REQUIRE(count3 == 3);
		REQUIRE(count4 == 4);
	}

	SECTION("Unsubscribing all should reset the subscriber count to zero(0)")
	{
		// Set and Execute
		e_Event.Subscribe(del_1).Subscribe(del_2).Subscribe(del_3).Subscribe(del_4);
		e_Event.UnsubscribeAll();

		// Test
		REQUIRE(e_Event.GetSubscriberCount() == 0);
	}

	SECTION("Multiple subscription should callback the subscriber multiple time")
	{
		// Set
		e_Event.Subscribe(del_1).Subscribe(del_1).Subscribe(del_2);

		// Execute
		e_Event.Fire();

		// Test
		REQUIRE(count1 == 2);
		REQUIRE(count2 == 2);
	}

	SECTION("For multiple subscription, unsubscribing should remove one subscription instance only")
	{
		// Set
		e_Event.Subscribe(del_1).Subscribe(del_1).Subscribe(del_2);
		e_Event.Unsubscribe(del_1);

		// Execute
		e_Event.Fire();

		// Test
		REQUIRE(count1 == 1);
		REQUIRE(count2 == 2);
	}

	SECTION("+= Operator should behave same as Subscribe()")
	{
		// Set
		e_Event += del_1;
		((e_Event += del_2) += del_3) += del_4;

		// Execute
		e_Event.Fire();

		// Evaluate
		REQUIRE(count1 == 1);
		REQUIRE(count2 == 2);
		REQUIRE(count3 == 3);
		REQUIRE(count4 == 4);
	}

	SECTION("-= Operator should behave same as Unsubscribe()")
	{
		// Set
		e_Event += del_1;
		((e_Event += del_2) += del_3) += del_4;

		e_Event -= del_3;
		((e_Event -= del_4) -= del_2);

		// Execute
		e_Event.Fire();

		// Evaluate
		REQUIRE(count1 == 1);
		REQUIRE(count2 == 0);
		REQUIRE(count3 == 0);
		REQUIRE(count4 == 0);
	}

	SECTION("<< Operator should behave same as Subscribe()")
	{
		// Set
		e_Event << del_1;
		e_Event << del_2 << del_3 << del_4;

		// Execute
		e_Event.Fire();

		// Evaluate
		REQUIRE(count1 == 1);
		REQUIRE(count2 == 2);
		REQUIRE(count3 == 3);
		REQUIRE(count4 == 4);
	}

	SECTION(">> Operator should behave same as Unsubscribe()")
	{
		// Set
		e_Event << del_1;
		e_Event << del_2 << del_3 << del_4;

		e_Event >> del_3;
		e_Event >> del_4 >> del_2;

		// Execute
		e_Event.Fire();

		// Evaluate
		REQUIRE(count1 == 1);
		REQUIRE(count2 == 0);
		REQUIRE(count3 == 0);
		REQUIRE(count4 == 0);
	}
}


TEST_CASE("Template| Event system for POD: int, int", "[Template]")
{
	// Set
	ish::Event<int, int> e_EventIntInt;
	e_EventIntInt << AddTwoInts << SubTwoInts << MulTwoInts;
	int arg1 = 10;
	int arg2 = 20;

	// Execute
	e_EventIntInt.Fire(arg1, arg2);

	// Test
	REQUIRE(sint_AccAdd == (arg1 + arg2));
	REQUIRE(sint_AccSub == (arg1 - arg2));
	REQUIRE(sint_AccMul == (arg1 * arg2));
}

TEST_CASE("Template| Event system for POD: void", "[Template]")
{
	// Set
	ish::Event<> e_EventVoid;
	e_EventVoid << TurnOnTrigger;

	// Execute
	e_EventVoid.Fire();

	// Test
	REQUIRE(sbool_Triggered == true);
}

TEST_CASE("Callable| Event system for Multiple Lambda", "[Lambda][Callable]")
{
	// Set
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;

	auto inc_a = [&]() { a += 1; };
	auto inc_b = [&]() { b += 2; };
	auto inc_c = [&]() { c += 3; };
	auto inc_d = [&]() { d += 4; };

	ish::Event<> e_EventLambda;
	e_EventLambda << inc_a << inc_b << inc_c << inc_d;
	//e_EventLambda.Subscribe(inc_a).Subscribe(inc_b).Subscribe(inc_c).Subscribe(inc_d);
	
	// Execute
	e_EventLambda.Fire();
	
	// Test
	REQUIRE(a == 1);
	REQUIRE(b == 2);
	REQUIRE(c == 3);
	REQUIRE(d == 4);
}