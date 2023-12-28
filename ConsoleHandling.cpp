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

HandleConsole::Console::Console() {
	m_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	m_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	DisableQuickEditMode();

	m_szStringBuffers = new ConsoleChar*[HandleConsole::CONSOLE_MAX_ROW_SIZE]{ };
	for (int i = 0; i < HandleConsole::CONSOLE_MAX_ROW_SIZE; ++i) {
		m_szStringBuffers[i] = new ConsoleChar[HandleConsole::CONSOLE_BUF_SIZE];
	}
}

HandleConsole::Console::~Console() {
	for (int i = 0; i < HandleConsole::CONSOLE_MAX_ROW_SIZE; ++i) {
		delete[] m_szStringBuffers[i];
		m_szStringBuffers[i] = nullptr;
	}
	delete[] m_szStringBuffers;
	m_szStringBuffers = nullptr;
}

void HandleConsole::Console::DisableQuickEditMode() {
	DWORD consoleModePrev;
	HANDLE consoleHandle = GetStdHandle(STD_INPUT_HANDLE);    
	GetConsoleMode(consoleHandle, &consoleModePrev);    
	SetConsoleMode(consoleHandle, consoleModePrev & ~ENABLE_QUICK_EDIT_MODE);
}

void HandleConsole::Console::SwapScreen() {
	SetConsoleActiveScreenBuffer(m_hScreen[static_cast<int>(m_bScreenIndex)]);
	m_bScreenIndex = !m_bScreenIndex;
}

void HandleConsole::Console::ScreenClear() {
	COORD pos{ };
	DWORD dw{ };
	FillConsoleOutputCharacter(m_hScreen[m_bScreenIndex], ' ', 80 * 40, pos, &dw);
}

void HandleConsole::Console::HideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo{ };

	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(m_hScreen[0], &cursorInfo);
	SetConsoleCursorInfo(m_hScreen[1], &cursorInfo);
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void HandleConsole::Console::Write(const std::string& text) {
	unsigned int nLoopSize = text.size() > HandleConsole::CONSOLE_BUF_SIZE ? HandleConsole::CONSOLE_BUF_SIZE : text.size();
	for (unsigned int i = 0; i < nLoopSize; ++i) {
		m_szStringBuffers[m_nBufferRowSize][i].cCharacter = text[i];
		m_szStringBuffers[m_nBufferRowSize][i].eColor = ConsoleColor::WHITE;
	}
	++m_nBufferRowSize;
}

void HandleConsole::Console::Write(const std::string& text, ConsoleColor color) {
	int nStoreNewLineIdx{ };
	unsigned int nLoopSize = text.size() > HandleConsole::CONSOLE_BUF_SIZE ? HandleConsole::CONSOLE_BUF_SIZE : text.size();
	for (unsigned int i = 0; i < nLoopSize; ++i) {
		if (text[i] == '\n') {
			++m_nBufferRowSize;
			nStoreNewLineIdx = i + 1;
			continue;
		}
		m_szStringBuffers[m_nBufferRowSize][i - nStoreNewLineIdx].cCharacter = text[i];
		m_szStringBuffers[m_nBufferRowSize][i - nStoreNewLineIdx].eColor = color;
	}
	++m_nBufferRowSize;
}

void HandleConsole::Console::Write(unsigned short nRowIndex, const std::string& text, ConsoleColor color) {
	m_nBufferRowSize = nRowIndex;
	memset(m_szStringBuffers[m_nBufferRowSize], 0, HandleConsole::CONSOLE_BUF_SIZE);

	int nStoreNewLineIdx{ };
	unsigned int nLoopSize = text.size() > HandleConsole::CONSOLE_BUF_SIZE ? HandleConsole::CONSOLE_BUF_SIZE : text.size();
	for (unsigned int i = 0; i < nLoopSize; ++i) {
		if (text[i] == '\n') {
			++m_nBufferRowSize;
			nStoreNewLineIdx = i + 1;
			continue;
		}
		m_szStringBuffers[m_nBufferRowSize][i - nStoreNewLineIdx].cCharacter = text[i];
		m_szStringBuffers[m_nBufferRowSize][i - nStoreNewLineIdx].eColor = color;
	}
	++m_nBufferRowSize;
}

void HandleConsole::Console::WriteRandomColors(const std::string& text) {
	int nStoreNewLineIdx{ };
	unsigned int nLoopSize = text.size() > HandleConsole::CONSOLE_BUF_SIZE ? HandleConsole::CONSOLE_BUF_SIZE : text.size();
	for (unsigned int i = 0; i < nLoopSize; ++i) {
		if (text[i] == '\n') {
			++m_nBufferRowSize;
			nStoreNewLineIdx = i + 1;
			continue;
		}
		m_szStringBuffers[m_nBufferRowSize][i - nStoreNewLineIdx].cCharacter = text[i];
		m_szStringBuffers[m_nBufferRowSize][i - nStoreNewLineIdx].eColor = 
			static_cast<ConsoleColor>(Random::RandInt((int)ConsoleColor::BLACK, (int)ConsoleColor::WHITE));
	}
	++m_nBufferRowSize;
}

void HandleConsole::Console::Write(const ConsoleChar* text) {
	int nStoreNewLineIdx{ };
	unsigned int nLoopSize = HandleConsole::consoleStrLen(text);
	for (unsigned int i = 0; i < nLoopSize; ++i) {
		if (text[i].cCharacter == '\n') {
			++m_nBufferRowSize;
			nStoreNewLineIdx = i + 1;
			continue;
		}
		m_szStringBuffers[m_nBufferRowSize][i - nStoreNewLineIdx].cCharacter = text[i].cCharacter;
		m_szStringBuffers[m_nBufferRowSize][i - nStoreNewLineIdx].eColor = text[i].eColor;
	}
	++m_nBufferRowSize;
}

void HandleConsole::Console::RenderText(unsigned short nBufferRowIndex) {
	unsigned int nStrLen{ HandleConsole::consoleStrLen(m_szStringBuffers[nBufferRowIndex]) };

	DWORD dw{ };
	COORD pos{ 0, nBufferRowIndex };
	for (unsigned int i = 0; i < nStrLen; ++i) {
		HandleConsole::setConsoleTextColor(m_hScreen[m_bScreenIndex], m_szStringBuffers[nBufferRowIndex][i].eColor);
		SetConsoleCursorPosition(m_hScreen[m_bScreenIndex], pos);
		WriteFile(m_hScreen[m_bScreenIndex], &m_szStringBuffers[nBufferRowIndex][i].cCharacter, 1, &dw, NULL);

		++pos.X;
	}
}

void HandleConsole::Console::ClearText() {
	for (unsigned int i = 0; i < m_nBufferRowSize; ++i) {
		memset(m_szStringBuffers[i], 0, HandleConsole::CONSOLE_BUF_SIZE);
	}
	m_nBufferRowSize = 0;
}

void HandleConsole::Console::Render() {
	ScreenClear();

	for (unsigned int i = 0; i < m_nBufferRowSize; ++i) {
		RenderText(i);
	}

	SwapScreen();
}

ConsoleChar* HandleConsole::toConsoleStrRandomColor(const std::string& str)
{
	ConsoleChar* lpszRtStr = new ConsoleChar[HandleConsole::CONSOLE_BUF_SIZE]{ };
	unsigned int nLoopSize = str.size() > HandleConsole::CONSOLE_BUF_SIZE ? HandleConsole::CONSOLE_BUF_SIZE : str.size();
	for (unsigned int i = 0; i < nLoopSize; ++i) {
		lpszRtStr[i].cCharacter = str[i];
		lpszRtStr[i].eColor = static_cast<ConsoleColor>(Random::RandInt((int)ConsoleColor::BLACK, (int)ConsoleColor::WHITE));
	}
	return lpszRtStr;
}
