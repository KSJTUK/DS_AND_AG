#include "pch.h"
#include "ConsoleHandling.h"

Timer::Timer() {
	QueryPerformanceFrequency(reinterpret_cast<PLARGE_INTEGER>(&m_nTick));
	QueryPerformanceCounter(reinterpret_cast<PLARGE_INTEGER>(&m_nPrevTime));
}

Timer::~Timer() {

}

void Timer::Update() {
	++m_nFPSCounter;

	__int64 nCurTime{ };
	QueryPerformanceCounter(reinterpret_cast<PLARGE_INTEGER>(&nCurTime));

	m_fTimeDelta = (nCurTime - m_nPrevTime) / static_cast<float>(m_nTick);
	
	m_fElapsedTime += m_fTimeDelta;
	m_fFPSTimeCounter += m_fTimeDelta;

	m_nPrevTime = nCurTime;
	
	if (m_fFPSTimeCounter > 1.0f) {
		m_fFPS = m_nFPSCounter / m_fFPSTimeCounter;
		m_fFPSTimeCounter = 0.f;
		m_nFPSCounter = 0;
	}
}

Console::Console() {
	m_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	m_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}

Console::~Console() { }

void Console::SwapScreen() {
	SetConsoleActiveScreenBuffer(m_hScreen[static_cast<int>(m_bScreenIndex)]);
	m_bScreenIndex = !m_bScreenIndex;
}

void Console::ScreenClear() {
	COORD pos{ };
	DWORD dw{ };
	FillConsoleOutputCharacter(m_hScreen[m_bScreenIndex], ' ', 80 * 40, pos, &dw);
}

void Console::Write(const std::string& text) {
	m_szStringBuffer += text + "\n";
}

void Console::ClearText() {
	m_szStringBuffer.clear();
}

void Console::Render() {
	ScreenClear();

	DWORD dw{ };
	SetConsoleCursorPosition(m_hScreen[m_bScreenIndex], COORD{ });
	WriteFile(m_hScreen[m_bScreenIndex], m_szStringBuffer.c_str(), static_cast<DWORD>(m_szStringBuffer.size()), &dw, NULL);

	SwapScreen();
}
