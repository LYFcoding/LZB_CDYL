#include "file_watcher.h"

bool watch(LPCTSTR dir, const char* filename, LPFileDeal lpFileDeal, LPVOID lParam) {
	HANDLE hDir = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL); // ���ļ���
	if (hDir == INVALID_HANDLE_VALUE) return false;
	BYTE buf[2 * sizeof(FILE_NOTIFY_INFORMATION) + 2 * MAX_PATH + 2];
	FILE_NOTIFY_INFORMATION *pNotify = (FILE_NOTIFY_INFORMATION*) buf;
	char multiBytesFilename[MAX_PATH * 2 + 2]; // խ�ַ��ļ���
	DWORD ret;
	int tryTimes = 1000;
	while (tryTimes--) {
		if (ReadDirectoryChangesW(hDir, pNotify, sizeof(buf) - 2,
			false, FILE_NOTIFY_CHANGE_SIZE, // �������Ŀ¼������ļ��Ƿ��޸�
			&ret, NULL, NULL)) {
				for (PFILE_NOTIFY_INFORMATION p = pNotify; p != NULL;) {
					WCHAR c = p->FileName[p->FileNameLength / 2];
					p->FileName[p->FileNameLength / 2] = L'\0';

					int size = WideCharToMultiByte(CP_ACP, 0, p->FileName, -1, NULL, 0, NULL, NULL); // ���ַ�תխ�ַ�
					WideCharToMultiByte(CP_ACP, 0, p->FileName, -1, multiBytesFilename, size, NULL, NULL);
					if (strcmp(multiBytesFilename, filename) == 0) { // Ŀ���ļ����޸�
						lpFileDeal(filename, lParam);
						CloseHandle(hDir);
						return true;
					}

					p->FileName[p->FileNameLength / 2] = c;
					if (p->NextEntryOffset) {
						p = (PFILE_NOTIFY_INFORMATION) ((BYTE*) p + p->NextEntryOffset); // ȷ����ַ��������
					} else {
						p = NULL;
					}
				}
		}
	}
	CloseHandle(hDir);
	return false;
}
