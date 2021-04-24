#pragma once

static int sint_AccAdd = 0;
static int sint_AccSub = 0;
static int sint_AccMul = 0;

static void AddTwoInts(int num1, int num2) { sint_AccAdd = num1 + num2; }
static void SubTwoInts(int num1, int num2) { sint_AccSub = num1 - num2; }
static void MulTwoInts(int num1, int num2) { sint_AccMul = num1 * num2; }

static bool sbool_Triggered = false;
static void TurnOnTrigger() { sbool_Triggered = true; }