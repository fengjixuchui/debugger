#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <thread>

#include <Windows.h>

#include "DebugEventHandler.h"
#include "DebugExceptionHandler.h"
#include "Breakpoint.h"
#include "SafeHandle.h"

namespace CodeReversing
{

class Debugger final
{
public:
    friend class DebugEventHandler;
    friend class DebugExceptionHandler;

    Debugger() = delete;
    Debugger(const DWORD dwProcessId, const bool bKillOnExit = false);

    Debugger(const Debugger &copy) = delete;
    Debugger &operator=(const Debugger &copy) = delete;

    ~Debugger();

    const bool Start();
    const bool Stop();
    const bool StepInto();
    const bool Continue();

    void PrintCallStack();

    void PrintContext();
    const bool SetExecutingContext(const CONTEXT &ctx);
    const CONTEXT GetExecutingContext();

    const volatile bool IsActive() const;

    const bool AddBreakpoint(const DWORD_PTR dwAddress);
    const bool RemoveBreakpoint(const DWORD_PTR dwAddress);
    Breakpoint * FindBreakpoint(const DWORD_PTR dwAddress);

    const HANDLE Handle() const;

private:
    volatile bool m_bIsActive;
    bool m_bKillOnExit;
    DWORD m_dwProcessId;
    SafeHandle m_hProcess;
    SafeHandle m_hContinueEvent;

    Breakpoint *m_pLastBreakpoint;
    DWORD m_dwExecutingThreadId;
    CONTEXT m_lastContext;
    volatile bool m_bIsStepping;

    const bool DebuggerLoop();

    const bool Continue(const bool bIsStepping);
    const bool WaitForContinue() const;

    const DWORD ChangeMemoryPermissions(const DWORD_PTR dwAddress, const size_t ulSize, DWORD dwNewPermissions);
    SafeHandle OpenCurrentThread();

    std::unique_ptr<DebugEventHandler> m_pEventHandler;
    std::unique_ptr<DebugExceptionHandler> m_pExceptionHandler;

    std::list<std::unique_ptr<Breakpoint>> m_lstBreakpoints;

};

}