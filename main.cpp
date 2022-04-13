#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpCmd, int nShow)
{
    wchar_t msg[1024];
    HANDLE file;
    DWORD fileSize;
    DWORD readSize;
    DWORD err;
    TCHAR errStr[32];
    void* buf;

    file = CreateFile(L"\\NAND\\version.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        err = GetLastError();
        switch (err)
        {
            case 2:
            case 3:
                swprintf(errStr, L"File not found");
                break;
            default:
                swprintf(errStr, L"Yabai error sorry");
                break;
        }

        swprintf(msg, L"Failed to open version.txt\n%#010x: %s", err, errStr);
        MessageBox(NULL, msg, L"(X_X) < OMG", MB_ICONWARNING);
        return 1;
    }

    fileSize = GetFileSize(file, NULL);
    buf = LocalAlloc(LPTR, fileSize+1);
    if (buf == NULL)
    {
        MessageBox(NULL, L"Failed to allocate memory", L"(X_X) < OMG", MB_ICONWARNING);
        goto disaster;
    }

    if (!ReadFile(file, buf, fileSize, &readSize, NULL))
    {
        MessageBox(NULL, L"Failed to read version.txt", L"(X_X) < OMG", MB_ICONWARNING);
        goto disaster;
    }

    CloseHandle(file);

    ((char *)buf)[fileSize] = '\0';
    mbstowcs(msg, (char *)buf, fileSize);
    MessageBox(NULL, msg, L"(>_O)b < My version.txt is...", MB_ICONINFORMATION);

    file = CreateFile(L"\\Storage Card\\version.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, L"Failed to create \\Storage Card\\version.txt", L"(X_X) < OMG", MB_ICONWARNING);
        return 1;
    }

    if (!WriteFile(file, buf, fileSize, NULL, NULL))
    {
        MessageBox(NULL, L"Failed to write \\Storage Card\\version.txt", L"(X_X) < OMG", MB_ICONWARNING);
        goto disaster;
    }

    CloseHandle(file);
    return 0;

disaster:
    CloseHandle(file);
    return 1;
}
