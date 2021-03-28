#include "CRandom16.hpp"


CRandom16* CRandom16::g_randomNumber = nullptr;                // &DefaultRandom;
CGlobalRandom* CGlobalRandom::g_currentGlobalRandom = nullptr; //&DefaultGlobalRandom;
namespace {
u32 g_numNextCalls = 0;
};

void CRandom16::IncrementNumNextCalls() { ++g_numNextCalls; }
u32 CRandom16::GetNumNextCalls() { return g_numNextCalls; }
void CRandom16::ResetNumNextCalls() { g_numNextCalls = 0; }
