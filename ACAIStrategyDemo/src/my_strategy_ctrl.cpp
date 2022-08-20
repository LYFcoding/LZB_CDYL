#include "my_strategy.h"
#include "string.h"

void MyStrategy::onPKStart(const ACAI::PKConfig &pkConfig)
{
    memcpy(&mPKConfig, &pkConfig, sizeof(mPKConfig));
    // TODO : �˴�����û�������Ӧ������ʼ
}

void MyStrategy::onPKEnd()
{
    // TODO : �˴�����û�������Ӧ��������
	initData();
}

void MyStrategy::onPKOut(unsigned int flightID)
{
	// TODO : �˴�����û�������Ӧ�ѷ����ߵз��ɻ������л�����˳������¼�
}

void MyStrategy::backGround()
{
    // TODO : �˴�����û�����ִ�к�̨����
}

void MyStrategy::timeSlice20()
{
    // TODO : �˴�����û�����ִ��20ms��������
}

bool inRange(double angle, double range) {
	return (angle <= range && angle >= -range) ||
		(PI - angle <= range && PI - angle >= -range);
}

void MyStrategy::timeSlice40()
{
    // TODO : �˴�����û�����ִ��40ms��������
	ACAI::EventLog outputData;
	memset(&outputData, 0, sizeof(outputData));
	/*
	if( mACMslWarning.mslCnt > 0 )
	{// ��β����
		DoTacHeadEvade();
		// ��־��ʾ
		strcpy(outputData.EventDes, "��β����");
		logEvent(outputData);
	}
	if(    0 == mACRdrTarget.tgtCnt 
		&& 0 == mACMslWarning.mslCnt 
		&& 0 == mACMSLInGuide.mslCnt 
		&& false == mACFCCStatus.envInfos[0].FPoleValid )
	{// ����ͻ��
		DoTacPointAtk();
		// ��־��ʾ
		strcpy(outputData.EventDes, "����ͻ��");
		logEvent(outputData);
	}
	if(    mACRdrTarget.tgtCnt > 0 
		&& 0 == mACMslWarning.mslCnt 
		&& mACMSLInGuide.mslCnt > 0  )
	{// ƫ���Ƶ�
		// ��־��ʾ
		strcpy(outputData.EventDes, "ƫ���Ƶ�");
		DoTacHeadGuide();
		logEvent(outputData);
	}
	if(    0 < mACRdrTarget.tgtCnt 
		&& 0 == mACMslWarning.mslCnt 
		&& 0 == mACMSLInGuide.mslCnt 
		&& 0 == mTeamMSLLaunched.mslCnt )
	{// ��������
		if( mACFCCStatus.envInfos[0].FPoleValid && ((int)mACFlightStatus.timeCounter) - (int)m_lastWpnShootTimeCounter > 10000 )
		{// ��������
			m_lastWpnShootTimeCounter = mACFlightStatus.timeCounter;
			DoTacWpnShoot();
			// ��־��ʾ
			strcpy(outputData.EventDes, "��������");
			logEvent(outputData);
		}
		else
		{
			// ��־��ʾ
			strcpy(outputData.EventDes, "��������");
			logEvent(outputData);
		}
		DoTacAltClimb();
	}*/
	// ԭս��

	//--------------------------------------
	//����

	int HeightEdge = (mPKConfig.MaxFlyHeight - mPKConfig.MinFlyHeight) / 20;
	int LonEdge = (mPKConfig.RightUpLon - mPKConfig.LeftDownLon) / 20;
	int LatEdge = (mPKConfig.RightUpLat - mPKConfig.LeftDownLat) / 20;
	int HeightCorrect = (mPKConfig.MaxFlyHeight - mPKConfig.MinFlyHeight) / 10;
	int LonCorrect = (mPKConfig.RightUpLon - mPKConfig.LeftDownLon) / 10;
	int LatCorrect = (mPKConfig.RightUpLat - mPKConfig.LeftDownLat) / 10;
	ACAI::FlyControlCmd cmd;
	memset(&cmd, 0, sizeof(cmd));
	
	cmd.desireAlt = mACFlightStatus.alt;
	if (mACFlightStatus.alt < mPKConfig.MinFlyHeight + HeightEdge) {
		cmd.navCtrlCmd = true;
		cmd.desireNavAlt = mPKConfig.MaxFlyHeight - HeightCorrect;
	} else if (mACFlightStatus.alt > mPKConfig.MaxFlyHeight - HeightEdge) {
		cmd.desireNavAlt = mPKConfig.MinFlyHeight + HeightCorrect;
		cmd.navCtrlCmd = true;
	}

	cmd.desireNavLon = mACFlightStatus.lon;
	if (mACFlightStatus.lon > mPKConfig.RightUpLon - LonEdge) {
		cmd.desireNavLon = mPKConfig.RightUpLon - LonCorrect;
		cmd.navCtrlCmd = true;
	} else if (mACFlightStatus.lon < mPKConfig.LeftDownLon + LonEdge) {
		cmd.desireNavLat = mPKConfig.LeftDownLon + LonCorrect;
		cmd.navCtrlCmd = true;
	}

	cmd.desireNavLat = mACFlightStatus.lat;
	if (mACFlightStatus.lat > mPKConfig.RightUpLat - LatEdge) {
		cmd.desireNavLat = mPKConfig.RightUpLat - LatCorrect;
		cmd.navCtrlCmd = true;
	} else if (mACFlightStatus.lat < mPKConfig.LeftDownLat + LatEdge) {
		cmd.desireNavLat = mPKConfig.LeftDownLat + LatCorrect;
		cmd.navCtrlCmd = true;
	}
	sendFlyControlCmd(cmd);

	//����ս��
	int mslWarningStartTime;
//	if (mCOFlightStatus.flightMemCnt == 0) {
	if (mACMslWarning.mslCnt > 0) {
		if (mACFlightStatus.timeCounter - mslWarningStartTime > 3000) { // �������濪ʼ3�����
			DoTacHeadEvade();
			strcpy(outputData.EventDes, "��β����");
			logEvent(outputData);
		}
	} else { // ��û�е�������ʱˢ�µ��������ʱ��
		mslWarningStartTime = mACFlightStatus.timeCounter;
	}
	// �޷�������������ʱȫ��ͻ��
	if (mACRdrTarget.tgtCnt == 0 &&	mACMslWarning.mslCnt ==0) {
		//DoTacPointAtk();
		DoTacAltClimbP30();
		strcpy(outputData.EventDes, "��30������");
		logEvent(outputData);
	}
	if (mACRdrTarget.tgtCnt > 0 && mACMslWarning.mslCnt == 0 && mACMSLInGuide.mslCnt == 0) {
		// �ж�׷������ͻ��
		// ���ڵ���ʱ
		if ((mACFCCStatus.envInfos[0].FPoleValid || mACFCCStatus.envInfos[0].APoleValid) &&
			((int) mACFlightStatus.timeCounter - (int) m_lastWpnShootTimeCounter > 5000)) {
				m_lastWpnShootTimeCounter = mACFlightStatus.timeCounter;
				DoTacWpnShoot();
				strcpy(outputData.EventDes, "��������");
				logEvent(outputData);
		} else {
			DoTacToTar();
			strcpy(outputData.EventDes, "׷��");
			logEvent(outputData);
		}
		// ����������ʱ
		if (mACRdrTarget.tgtCnt == 2) {
			if (mACFCCStatus.envInfos[1].FPoleValid == true &&
				(int) mACFlightStatus.timeCounter - (int) m_lastWpnShootTimeCounter > 5000) {
					m_lastWpnShootTimeCounter = mACFlightStatus.timeCounter;
					DoTacWpnShoot();
					strcpy(outputData.EventDes, "��������");
					logEvent(outputData);
			}
		}
	}
	if (mACRdrTarget.tgtCnt > 0 && mACMslWarning.mslCnt == 0 && mACMSLInGuide.mslCnt > 0) {
		DoTurnRight30();
		strcpy(outputData.EventDes, "��תƫ��30��");
		logEvent(outputData);
		DoTacWpnShoot();
		strcpy(outputData.EventDes,"��������");

	}
}

//--------------------------------------
//��ȡ����
struct Action {
	int fin, sin; // һ��������0�����У�1���������������������嶯��
};

void deal(const char* filename, LPVOID lParam) { // ����ԭ�򲻷���ӵ�MyStrategy����
	FILE* fp = fopen(filename, "r");
	int fin, sin; // һ����������������
	fscanf(fp, "%d %d", &fin, &sin);
	fclose(fp);
	struct Action* act = (struct Action*) lParam;
	act->fin = fin;
	act->sin = sin;
	return;
}

void MyStrategy::readAction() {
	struct Action act = {-1, -1};
	if (mACFlightStatus.flightRole == ACAI::V_FLIGHT_ROLE_LEAD) {
		if (watch(L".", "action1.csv", deal, &act)) {
			if (act.fin == 0) { // TODO:���ݶ�������ִ�ж���
				switch (act.sin) {
				case 1:
				default:
					DoTacHeadEvade();
				}
			} else {
				if (act.sin == 1) {
					DoTacWpnShoot();
				} else {
					SwitchGuideFlight();
				}
			}
		} else { // �����ڼ��ļ�δ�����ı�
			DoTacHeadEvade();
			return;
		}
		// TODO:�յ�action��ķ���
		PrintStatus("state1.csv");
	} else {
		if (watch(L".", "action2.csv", deal, &act)) {

		}
		PrintStatus("state2.csv");
	}
	return;
	
}

#define POW2(x) (x) * (x)

double getTdAngle(const double vect1[3], const double vect2[3]) {
	double vectDot = vect1[0] * vect2[0] + vect1[1] * vect2[1] + vect1[2] * vect2[2];
	double vectMod1 = sqrt( POW2(vect1[0]) + POW2(vect1[1]) + POW2(vect1[2]) );
	double vectMod2 = sqrt( POW2(vect2[0]) + POW2(vect2[1]) + POW2(vect2[2]) );
	double cosValue = vectDot / (vectMod1 * vectMod2);
	double rad = acos(cosValue);
	return rad;
}

void MyStrategy::PrintStatus(const char * filename) {
	static const double dis2Lons = 160000 / (mPKConfig.RightUpLon - mPKConfig.LeftDownLon);
	static const double dis2Lats = 80000 / (mPKConfig.RightUpLat - mPKConfig.LeftDownLat);
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		fclose(fp);
		fp = fopen(filename, "w");
	} else {
		fclose(fp);
		fp = fopen(filename, "a");
	}
	fprintf(fp, "[%d] %f %f %f %f %f %f %f %f %f %f %d %d %d\n",
		mACFlightStatus.timeCounter,						// [ʱ��]
		mACRdrTarget.tgtInfos[0].slantRange,				// Ŀ�����
		mACFlightStatus.alt - mACRdrTarget.tgtInfos[0].alt, // Ŀ��߶Ȳ�
		mACRdrTarget.tgtInfos[0].azGeo,						// Ŀ�� ����ϵ ��λ��
		mACRdrTarget.tgtInfos[0].sbsSpeed,					// Ŀ���ٶ�(m/s)
		mCOFlightStatus.memFlightStatus[0].groundSpeed,		// �ѻ��ٶ�(m/s)
		sqrt( POW2(mACFlightStatus.alt - mCOFlightStatus.memFlightStatus[0].alt) +
		POW2((mACFlightStatus.lon - mCOFlightStatus.memFlightStatus[0].lon) * dis2Lons) +
		POW2((mACFlightStatus.lat - mCOFlightStatus.memFlightStatus[0].lat) * dis2Lats) ),
															// �ѻ�����
		mACFlightStatus.alt - mCOFlightStatus.memFlightStatus[0].alt,
															// �ѻ��߶Ȳ�
		getTdAngle(mACFlightStatus.velNWU, mCOFlightStatus.memFlightStatus[0].velNWU),
															// �ѻ��ٶ�ʸ���н�
		mACFlightStatus.groundSpeed,						// �����ٶ�
		getTdAngle(mACFlightStatus.velNWU, mACRdrTarget.tgtInfos[0].velNWU),
															// �л��ٶ�ʸ���н�
		mACMslWarning.mslCnt > 0,							// �Ƿ񱻵�������
		mACMSLInGuide.mslCnt > 0,							// �Ƿ����Ƶ�
		mACFlightStatus.remainWpnNum						// ʣ��������
		);
	fclose(fp);
}

//--------------------------------------
//����������
void MyStrategy::maneuver_i(int fin,int sin )//һ����������������
{
	if (fin == 0) {
		switch(sin) {
		case 1: DoTacPointAtk();break;
		case 2: DoTacToTar();break;
		case 3:	DoTacAltClimbP30();break;
		case 4: DoTacAltClimbP60();break;
		case 5: DoTacNoseDiveM30();break;
		case 6:	DoTacNoseDiveM60();break;
		case 7: DoTacStaHov();break;
		case 8: DoTurnLeft30();break;
		case 9: DoTurnLeft60();break;
		case 10: DoTurnRight30();break;
		case 11: DoTacHeadEvade();break;
		case 12: DoTurnEvad30();break;
		case 13: DoTurnEvad60();break;
		default: DoTacHeadEvade();break;
		}
	} else {
		switch(sin) {
		case 1:DoTacWpnShoot();break;
		case 2:SwitchGuideFlight();break;
		default:break;
		}
	}
	
}
//��з����߷���
void MyStrategy::DoTacPointAtk()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.navCtrlCmd	= true;        ///< ��·����ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	if( ACAI::V_FLIGHT_TEAM_RED == mACFlightStatus.flightTeam )
	{
		outputData.desireNavLon	= mPKConfig.RedMissionLon;    ///< ������·����(rad)
		outputData.desireNavLat	= mPKConfig.RedMissionLat;    ///< ������·γ��(rad)
	}
	else
	{
		outputData.desireNavLon	= mPKConfig.BlueMissionLon;    ///< ������·����(rad)
		outputData.desireNavLat	= mPKConfig.BlueMissionLat;    ///< ������·γ��(rad)
	}
	outputData.desireNavAlt = 2000;    ///< ������·�߶�(m)
	outputData.desireSpeed	= 1000;     ///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//��һ�ܵл�����
void MyStrategy::DoTacToTar()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 2000;       ///< �����߶�(m)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData.desireHead =mACRdrTarget.tgtInfos[0].azGeo;
	sendFlyControlCmd(outputData);

}
//+30�ȼ�������
void MyStrategy::DoTacAltClimbP30()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 4000;       ///< �����߶�(m)
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo+PI/6.0;///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//+60�ȼ�������
void MyStrategy::DoTacAltClimbP60()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 4000;       ///< �����߶�(m)
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo+PI/3.0;///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//-30����Ǳ
void MyStrategy::DoTacNoseDiveM30()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = mPKConfig.MinFlyHeight+100;       ///< �����߶�(m)
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo-PI/6.0;///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//-60����Ǳ
void MyStrategy::DoTacNoseDiveM60()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = mPKConfig.MinFlyHeight+100;       ///< �����߶�(m)
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo-PI/3.0;///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//���λ���
void MyStrategy::DoTacStaHov()
{
	int nowCnt = mACFlightStatus.timeCounter;
	int cnt = nowCnt;
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnLeft();			//�������
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnFor();			//��ǰ����
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnRight();			//���ҷ���
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnFor();			//��ǰ����
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnRight();			//���ҷ���
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnFor();			//��ǰ����
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnLeft();			//�������
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnFor();			//��ǰ����
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
}
//��תƫ��30
void MyStrategy::DoTurnLeft30()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 2000;			///< �����߶�(m)
	outputData.desireHead = mACFlightStatus.heading-PI/6.0;	///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//��תƫ��60
void MyStrategy::DoTurnLeft60()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 2000;			///< �����߶�(m)
	outputData.desireHead = mACFlightStatus.heading-PI/3.0;	///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//��תƫ��30
void MyStrategy::DoTurnRight30()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 2000;			///< �����߶�(m)
	outputData.desireHead = mACFlightStatus.heading+PI/6.0;	///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//��תƫ��60
void MyStrategy::DoTurnRight60()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 2000;			///< �����߶�(m)
	outputData.desireHead = mACFlightStatus.heading+PI/3.0;	///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//��ͷ
void MyStrategy::DoTacHeadEvade()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 2000;       ///< �����߶�(m)
	outputData.desireHead = mACMslWarning.threatInfos[0].azGeo + PI;///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//�ػ�
void MyStrategy::DoTacCir() {
	unsigned long nowCnt = mACFlightStatus.timeCounter;
	unsigned long cnt = nowCnt;
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnLeft();			//�������
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnFor();			//��ǰ����
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnLeft();			//�������
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnFor();			//��ǰ����
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnLeft();			//�������
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnFor();			//��ǰ����
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnLeft();			//�������
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
	while(Rule()&&((nowCnt-cnt)<=5)){
		MyStrategy::DoTurnFor();			//��ǰ����
		nowCnt=mACFlightStatus.timeCounter;	//����ʱ��	
	}
}
//��ͷ��30����Ǳ
void MyStrategy::DoTurnEvad30()
{
	MyStrategy::DoTacHeadEvade();
	MyStrategy::DoTacNoseDiveM30();
}
//��ͷ��60����Ǳ
void MyStrategy::DoTurnEvad60()
{
	MyStrategy::DoTacHeadEvade();
	MyStrategy::DoTacNoseDiveM60();
}

//���丩�����
void MyStrategy::DoTacNoseDive()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = mPKConfig.MinFlyHeight+100;       ///< �����߶�(m)
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo;///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//��ת��
void MyStrategy::DoTurnLeft()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = mACFlightStatus.heading-PI/2.0;			///< �����߶�(m)
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo;		///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//��ת��
void MyStrategy::DoTurnRight()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = mACFlightStatus.heading+PI/2.0;			///< �����߶�(m)
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo;		///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
//��ǰ����
void MyStrategy::DoTurnFor()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = mACFlightStatus.heading;			///< �����߶�(m)
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo;		///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}

//--------------------------------------
//ս��������
// ׷��
void MyStrategy::DoTrack() {
	MyStrategy::DoTacToTar();
}
// ��������
void MyStrategy::DoTacWpnShoot()
{
	ACAI::WpnControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.launchPlaneID = mACFlightStatus.flightID; ///< ��������
	outputData.guidePlaneID	 = mACFlightStatus.flightID;  ///< �Ƶ������
	outputData.mslLockTgtID = mACRdrTarget.tgtInfos[0].tgtID;  ///< ����Ŀ����
	outputData._cmdCnt	= mACFlightStatus.timeCounter;       ///< ָ�����
	sendWpnControlCmd(outputData);
}

//�л��Ƶ���
void MyStrategy::SwitchGuideFlight() {
	// TODO:�л��Ƶ���
	if (mCOMSLInGuide.memMSLInGuide[0].mslCnt > 0
		&& mACMSLInGuide.mslCnt == 0) { // ����ѻ��Ƶ����Լ�δ�Ƶ�
			ACAI::FlyControlCmd outputData;
			memset(&outputData, 0, sizeof(outputData));
			outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
			outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
			outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
			outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
			outputData.desireAlt = 2000;       ///< �����߶�(m)
			outputData.desireHead = mCOMSLInGuide.memMSLInGuide[0].guideInfos[0].mslGuideAz+35*PI/180;///< ������·����(rad)
			outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
			outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
			sendFlyControlCmd(outputData);
	}
}

// ƫ���Ƶ�
void MyStrategy::DoTacHeadGuide()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = 2000;       ///< �����߶�(m)
	outputData.desireHead = mACMSLInGuide.guideInfos[0].mslGuideAz+35*PI/180;///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}