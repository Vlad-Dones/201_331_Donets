#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

DWORD GetProcessIdByName(const TCHAR* processName) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Получаем снимок всех процессов
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    std::cout << "First " << std::endl;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Получаем информацию о первом процессе
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return 0;
    }

    std::cout << "Second " << std::endl;

    // Перебираем все процессы, пока не найдем нужный
    do {
        if (_tcscmp(pe32.szExeFile, processName) == 0) {
            CloseHandle(hProcessSnap);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    std::cout << "Last" << std::endl;
    CloseHandle(hProcessSnap);
    return 0;
}

int main() {
    const TCHAR* processName = _T("201_331_Donets.exe"); // Имя нашего процесса
    DWORD processId = GetProcessIdByName(processName);

    if (processId != 0) {
        std::cout << "PID of process " << " : " << processId << std::endl;
    } else {
        std::cout << "Process " << processName << "is not founded" << std::endl;
    }

    //Подключение к приложению как отладчик

    bool isAttached = DebugActiveProcess(processId);
    if(!isAttached) {
        DWORD lastError = GetLastError();
        std::cout << std::hex << "DebugActiveProcess is FAILED, LastError is " << lastError;
    }
    else {
        std::cout << "DebugActiveProcess is success" << std::endl;
    }

    // Пропускать поступающие сигналы отладки
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, processId);
    DEBUG_EVENT debugEvent;
    while(true) {
        DWORD result = WaitForSingleObject(hProcess, 1); //Статус процесса

        // Проверяем результат
        if (result == WAIT_OBJECT_0) {
            // Процесс завершился, выходим из цикла
            break;
        }
        else {

            bool result1 = WaitForDebugEvent(&debugEvent, INFINITE);
            bool result2 = ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE);
        }
    }
    std::cout << "Process is stoped!"<< std::endl;


    return 0;
}

