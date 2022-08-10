#ifndef __FILE_COMMU_H
#define __FILE_COMMU_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#define C_IN_PY_OUT			"cpy"	// C �����ļ���python ����ļ�
#define PY_IN_C_OUT			"pyc"	// python �����ļ���C ����ļ�
#define FILE_BUSY			0		// 0000
#define FILE_READY			2		// 0010
#define FILE_WRITE			0		// 0000
#define FILE_READ			1		// 0001
#define FILE_WRITE_BUSY		FILE_WRITE | FILE_BUSY		// 0000
#define FILE_READ_BUSY		FILE_READ | FILE_BUSY		// 0001
#define FILE_WRITE_READY	FILE_WRITE | FILE_READY		// 0010
#define FILE_READ_READY		FILE_READ | FILE_READY		// 0011
#define FILE_C_CTRL			0
#define FILE_PY_CTRL		1
// ��ϵ���˼:
// 00 ��ʾ "cpy" ���ڱ�Cд��
// 01 ��ʾ "pyc" ���ڱ�C��ȡ
// 02 ��ʾ "cpy" �ѱ�Cд�����
// 03 ��ʾ "pyc" �ѱ�C��ȡ���

int tryOpen(const char *filename, int wr, char *data, int size);

#endif
