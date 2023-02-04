#include "ServiceHelper.h"
#include <iostream>
#include <Windows.h>
#include <string>

ServiceHelper::ServiceHelper(const std::string& serviceName)
    : serviceName(serviceName)
{
}

ServiceHelper::~ServiceHelper()
{
}

void ServiceHelper::init()
{
    if (createWindowsService())
        addServiceToAutoload();
}

bool ServiceHelper::createWindowsService()
{
    // Define the service table
    SERVICE_TABLE_ENTRY serviceTable[] = 
    {
        { (LPSTR)serviceName.c_str(), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };

    // Start the service control manager
    if (!StartServiceCtrlDispatcher(serviceTable))
    {
        std::cerr << "Failed to create Windows Service " << serviceName << std::endl;
        return false;
    }

    std::cout << "Successfully created Windows Service " << serviceName << std::endl;
    return true;
}

void ServiceHelper::addServiceToAutoload()
{
    HKEY hKey;
    DWORD dwDisp;
    LONG lRet;
    std::wstring subKey = L"SYSTEM\\CurrentControlSet\\Services\\" + std::wstring(serviceName.begin(), serviceName.end());
    std::wstring value = L"ImagePath";
    std::wstring data = L"\"" + std::wstring(serviceName.begin(), serviceName.end()) + L"\"";

    // Open the registry key
    lRet = RegCreateKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisp);
    if (lRet != ERROR_SUCCESS)
    {
        std::cerr << "Failed to open registry key: " << GetLastError() << std::endl;
        return;
    }

    // Set the registry value
    lRet = RegSetValueEx(hKey, value.c_str(), 0, REG_SZ, (BYTE*)data.c_str(), (DWORD)((data.length() + 1) * sizeof(wchar_t)));
    if (lRet != ERROR_SUCCESS)
    {
        std::cerr << "Failed to set registry value: " << GetLastError() << std::endl;
        RegCloseKey(hKey);
        return;
    }

    // Close the registry key
    RegCloseKey(hKey);

    std::cout << "Successfully added Windows Service " << serviceName << " to autoload." << std::endl;
}