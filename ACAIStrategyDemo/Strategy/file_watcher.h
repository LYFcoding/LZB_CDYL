#ifndef _FILE_WATCHER_H
#define _FILE_WATCHER_H

#include <windows.h>

typedef void (LPFileDeal*) (LPVOID); // �ļ��ı�ص�����

bool watch(LPCTSTR dir, LPFileDeal lpFileDeal, LPVOID lParam); // �����ļ�

#endif