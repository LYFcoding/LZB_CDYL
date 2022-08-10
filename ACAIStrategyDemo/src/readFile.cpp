#include "readFile.h"

char * conStr(int ctrl, int status) {
	char * ret = (char *) malloc(3);
	ret[0] = ctrl + '0';
	ret[1] = status + '0';
	ret[2] = 0;
	return ret;
}

// \param[filename] �ļ���
// \param[wr]		��д��־ 0:д, 1:��
// \param[data]		��ȡ��������
// \return 0:�ļ�������, 1:�򿪳ɹ�
int tryOpen(const char * filename, int wr, char * data, int size) {
	long file;
	struct _finddata_t find;
	//_chdir(".\\"); // ת��Ŀ¼
	if ((file = _findfirst(filename, &find)) == -1L) {	// δ׼���ý����ļ�
		return 0;
	}
	while (_findfirst( conStr(FILE_PY_CTRL, FILE_BUSY | (wr ^ 1)) , &find) != -1);	// ���ļ���python������ʱ, �ȴ�
	while (_findfirst( conStr(FILE_PY_CTRL, FILE_READY | (wr ^ 1)) , &find) == -1);	// �ļ�δ׼���ã��ȴ�
	_findclose(file);
	FILE *tmp = fopen( conStr(FILE_C_CTRL, FILE_BUSY | wr) , "w"); // �½�һ���ļ�����ʾ��æ
	fclose(tmp);
	remove( conStr(FILE_PY_CTRL, FILE_READY ^ wr) ); // ɾ���ļ���׼���õı�־
	// ��ʼ��д�ļ�
	if (wr == 0) { // д�ļ�
		FILE *fp = fopen(filename, "w");
		if (fp != NULL) fwrite(data, sizeof(char), size, fp);
		fclose(fp);
	} else {
		FILE *fp = fopen(filename, "rb");
		fseek(fp, 0, SEEK_END);
		long fsize = ftell(fp); // ��ȡ�ļ���С
		rewind(fp);
		size = size <= fsize ? size : fsize;
		if (fp != NULL) fread(data, sizeof(char), size, fp); // -1 �������
		fclose(fp);
	}
	tmp = fopen( conStr(FILE_C_CTRL, FILE_READY | wr), "w"); // �½��ļ���ʾ�ļ���������
	fclose(tmp);
	remove( conStr(FILE_C_CTRL, FILE_BUSY | wr) ); // �Ƴ�æ��־
	return 1;
}