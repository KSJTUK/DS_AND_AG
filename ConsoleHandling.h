#pragma once

namespace ConsoleColor {
    //»ö»ó
    enum {
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
}

namespace HandleConsole {
    inline void setConsoleTextColor(unsigned short textColor)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor);
    }

    inline void consoleClear()
    {
        system("cls");
    }
}

class Console {
public:
    Console();
    ~Console();

private:
    bool m_bScreenIndex{ false }; // 0 or 1
    HANDLE m_hScreen[2]{ };

    std::string m_szStringBuffer{ };

public:
    void SwapScreen();
    void ScreenClear();
    void Write(const std::string& text);
    void ClearText();
    void Render();
};

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