#pragma once

 //»ö»ó
enum class ConsoleColor {
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DAKR_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    SKYBLUE,
    RED,
    VIOLET,
    YELLOW,
    WHITE,
};

struct ConsoleChar {
    ConsoleColor eColor{ ConsoleColor::WHITE };
    char cCharacter{ '\0' };
};

namespace HandleConsole {
    constexpr unsigned short CONSOLE_BUF_SIZE{ 512 };
    constexpr unsigned short CONSOLE_MAX_ROW_SIZE{ 100 };

    inline void setConsoleTextColor(ConsoleColor textColor)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<unsigned short>(textColor));
    }

    inline void setConsoleTextColor(HANDLE screenBuffer, ConsoleColor textColor)
    {
        SetConsoleTextAttribute(screenBuffer, static_cast<unsigned short>(textColor));
    }

    inline void consoleClear()
    {
        system("cls");
    }

    inline unsigned int consoleStrLen(const ConsoleChar* consoleStr)
    {
        unsigned int nStrLen{ };
        for (int i = 0; i < CONSOLE_BUF_SIZE; ++i) {
            if (consoleStr[i].cCharacter == '\0') {
                return nStrLen;
            }
            ++nStrLen;
        }
    }
    
    inline ConsoleChar* toConsoleStr(const std::string& str)
    {
        ConsoleChar* lpszRtStr = new ConsoleChar[HandleConsole::CONSOLE_BUF_SIZE]{ };
        unsigned int nLoopSize = str.size() > HandleConsole::CONSOLE_BUF_SIZE ? HandleConsole::CONSOLE_BUF_SIZE : str.size();
        for (unsigned int i = 0; i < nLoopSize; ++i) {
            lpszRtStr[i].cCharacter = str[i];
            lpszRtStr[i].eColor = ConsoleColor::WHITE;
        }
        return lpszRtStr;
    }

    ConsoleChar* toConsoleStrRandomColor(const std::string& str);

    class Console {
    public:
        Console();
        ~Console();

    private:
        bool m_bScreenIndex{ false }; // 0 or 1
        HANDLE m_hScreen[2]{ };

        ConsoleChar** m_szStringBuffers{ };
        unsigned short m_nBufferRowSize{ };
        
    public:
        unsigned short GetCurrentRowIndex() const { return m_nBufferRowSize; }

    public:
        void DisableQuickEditMode();
        void SwapScreen();
        void ScreenClear();
        void HideCursor();
        void Write(const std::string& text);
        void Write(const std::string& text, ConsoleColor color);
        void Write(unsigned short nRowIndex, const std::string& text, ConsoleColor color = ConsoleColor::WHITE);
        void WriteRandomColors(const std::string& text);
        void Write(const ConsoleChar* text);
        void RenderText(unsigned short nBufferRowIndex);
        void ClearText();
        void Render();
    };
}

class Timer {
public:
    Timer();
    ~Timer();

private:
    __int64 m_nTick{ };
    __int64 m_nPrevTime{ };

    float m_fTimeDelta{ };
    float m_fFPS{ };
    __int32 m_nFPSCounter{ };
    float m_fFPSTimeCounter{ };
    float m_fElapsedTime{ };

public:
    float GetFPS() const { return m_fFPS; }
    float GetTimeDelta() const { return m_fTimeDelta; }
    float GetElapsedTime() const { return m_fElapsedTime; }

public:
    void Update();
};