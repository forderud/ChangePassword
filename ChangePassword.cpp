#include <string>
#include <iostream>
#include <Windows.h>

#include <Lm.h>
#pragma comment(lib, "Netapi32.lib") // NetUserChangePassword


static void CheckResult(NET_API_STATUS res) {
    switch (res) {
    case NERR_Success:
        return;
    case ERROR_ACCESS_DENIED:
        throw std::logic_error("ERROR_ACCESS_DENIED");
    case ERROR_INVALID_PASSWORD:
        throw std::logic_error("ERROR_INVALID_PASSWORD");
    case NERR_InvalidComputer:
        throw std::logic_error("NERR_InvalidComputer");
    case NERR_NotPrimary:
        throw std::logic_error("NERR_NotPrimary");
    case NERR_UserNotFound:
        throw std::logic_error("NERR_UserNotFound");
    case NERR_PasswordTooShort:
        throw std::logic_error("NERR_PasswordTooShort");
    case ERROR_INCORRECT_ACCOUNT_TYPE:
        throw std::logic_error("incorrect account type"); // The system is not authoritative for the specified account and therefore cannot complete the operation. Please retry the operation using the provider associated with this account. If this is an online provider please use the provider's online site. 
    default:
        throw std::logic_error("unknown error");
    }
}

int wmain (int argc, wchar_t* argv[]) {
    if (argc < 3) {
        std::cout << "Utility for changing the password of the currently logged in user.\n";
        std::cout << "Assumes that account type is locally managed.\n";
        std::cout << "Usage: ChangePassword <old-pwd> <new-pwd>" << std::endl;
        return +1;
    }

    std::wstring old_pwd = argv[1];
    std::wstring new_pwd = argv[2];
    std::wcout << L"Changing password from " << old_pwd << L" to " << new_pwd << std::endl;

    try {
        NET_API_STATUS res = NetUserChangePassword(NULL, NULL, old_pwd.c_str(), new_pwd.c_str());
        CheckResult(res);
    } catch (const std::exception& err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return -1;
    }

    return 0;
}
