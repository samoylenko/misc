// Logos.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

void ErrMsg(LPSTR lpstrMsg)
{
    MessageBox(NULL, 
               lpstrMsg,
               "Подменялка Logo.sys (c) PowerMike 4 Вован-107", 
               MB_OK | MB_ICONWARNING);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    WIN32_FIND_DATA fdFindData;
    HANDLE          hFindFile;
    UINT            uiNum = 0, uiCmdLineLen;
    char            cFileNames[1024][MAX_PATH + 1];
    char            cFullFileName[MAX_PATH + 1];

    if((uiCmdLineLen = strlen(lpCmdLine)) > 0)
    {
        if(lpCmdLine[uiCmdLineLen - 1] == '\\')
        {
            lpCmdLine[uiCmdLineLen - 1] = '\0';
        }
        SetCurrentDirectory(lpCmdLine);
    
        hFindFile = FindFirstFile("*.*", &fdFindData);
        if(hFindFile != INVALID_HANDLE_VALUE)
        {
            while(FindNextFile(hFindFile, &fdFindData)&&(uiNum < 1024))
            {
                if(!(fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    strcpy(&cFileNames[uiNum++][0], fdFindData.cFileName);
                }
            }
            FindClose(hFindFile);
    
            strcpy(&cFullFileName[0], lpCmdLine);
            srand((unsigned)time(NULL));
            sprintf(&cFullFileName[strlen(cFullFileName)], 
                    "\\%s",
                &cFileNames[rand() % --uiNum][0]);
            if(!CopyFile(&cFullFileName[0], "c:\\logo.sys", FALSE))
            {
                ErrMsg("Ошибка при копировании");
            }
        }
        else
        {
            ErrMsg("Неизвестная ошибка. Проверьте правильность ввода пути");
        }
    }
    else
    {
        ErrMsg("Путь указан неправильно");
    }
	return 0;
}



