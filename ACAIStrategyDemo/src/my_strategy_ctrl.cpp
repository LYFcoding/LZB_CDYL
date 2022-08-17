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
	int check = Rule();
	if (check != MyStrategy::NoWarn) {
		ACAI::FlyControlCmd cmd;
		memset(&cmd, 0, sizeof(cmd));
		cmd.altCtrlCmd = true;
		if (check & MyStrategy::AltWarn) {
			if (mACFlightStatus.alt >= mPKConfig.MaxFlyHeight - 200) {
				cmd.desireAlt = mPKConfig.MaxFlyHeight - 1000;
			} else {
				cmd.desireAlt = mPKConfig.MinFlyHeight + 1000;
			}
		} 

		if (check & MyStrategy::LonWarn) {
			if (mACFlightStatus.lon >= mPKConfig.RightUpLon - 5) {
				cmd.desireNavLon = mPKConfig.RightUpLon - 10;
			} else {
				cmd.desireNavLon = mPKConfig.LeftDownLon + 10;
			}
		}

		if (check & MyStrategy::LatWarn) {
			if (mACFlightStatus.lat >= mPKConfig.RightUpLat - 5) {
				cmd.desireNavLat = mPKConfig.RightUpLat - 10;
			} else {
				cmd.desireNavLat = mPKConfig.LeftDownLat + 10;
			}
		}
		sendFlyControlCmd(cmd);
	}

	//����ս��
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
			DoTacAltClimb();
			strcpy(outputData.EventDes, "����ͻ��");
			logEvent(outputData);
		}
		if (mACRdrTarget.tgtCnt > 0 && mACMslWarning.mslCnt == 0 && mACMSLInGuide.mslCnt == 0) {
			// �ж�׷������ͻ��
			// ���ڵ���ʱ
			if ((mACFCCStatus.envInfos[0].FPoleValid || mACFCCStatus.envInfos[0].APoleValid) &&
				((int) mACFlightStatus.timeCounter - (int) m_lastWpnShootTimeCounter > 5000)) {
					m_lastWpnShootTimeCounter = mACFlightStatus.timeCounter;
					DoTacWpnShoot(0);
					strcpy(outputData.EventDes, "��������");
					logEvent(outputData);
			} else {
				DoTacToTar(0);
				strcpy(outputData.EventDes, "׷��");
				logEvent(outputData);
			}
			// ����������ʱ
			if (mACRdrTarget.tgtCnt == 2) {
				if (mACFCCStatus.envInfos[1].FPoleValid == true &&
					(int) mACFlightStatus.timeCounter - (int) m_lastWpnShootTimeCounter > 5000) {
					m_lastWpnShootTimeCounter = mACFlightStatus.timeCounter;
					DoTacWpnShoot(1);
					strcpy(outputData.EventDes, "��������");
					logEvent(outputData);
				}
			}
		}
		if (mACRdrTarget.tgtCnt > 0 && mACMslWarning.mslCnt == 0 && mACMSLInGuide.mslCnt > 0) {
			DoTacHeadGuide();
			strcpy(outputData.EventDes, "ƫ���Ƶ�");
			logEvent(outputData);
		}
//	} else {

//	}
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
					DoTacWpnShoot(0);
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
//����
int MyStrategy::Rule()
{
	int flag = MyStrategy::NoWarn;
	if(mACFlightStatus.alt < mPKConfig.MinFlyHeight + 200 ||
		mACFlightStatus.alt > mPKConfig.MaxFlyHeight - 200)
		flag |= MyStrategy::AltWarn;
	if(mACFlightStatus.lon > mPKConfig.RightUpLon - 5 ||
		mACFlightStatus.lon < mPKConfig.LeftDownLon + 5)
		flag |= MyStrategy::LonWarn;
	if(mACFlightStatus.lat > mPKConfig.RightUpLat - 5 ||
		mACFlightStatus.lat < mPKConfig.LeftDownLat + 5)
		flag |= MyStrategy::LatWarn;
	return flag;
}

//--------------------------------------
//����������

//ת��Ŀ��
void MyStrategy::DoTacToTar(int target)
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
	outputData.altCtrlCmd = true;        ///< �߶ȱ���ָ��
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireAlt = mACRdrTarget.tgtInfos[target].alt + 1500;       ///< �����߶�(m)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData.desireHead =mACRdrTarget.tgtInfos[target].azGeo;
	sendFlyControlCmd(outputData);
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
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireHead = mACFlightStatus.heading-PI/2.0;		///< ������·����(rad)
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
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireHead = mACFlightStatus.heading+PI/2.0;		///< ������·����(rad)
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
	outputData.headCtrlCmd = true;       ///< ���򱣳�ָ��
	outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.desireHead = mACRdrTarget.tgtInfos[0].azGeo;		///< ������·����(rad)
	outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
	outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}

//--------------------------------------
//ս��������
//�л��Ƶ���
void MyStrategy::SwitchGuideFlight() {
	// TODO:�л��Ƶ���
	return;
}

//׷��
void MyStrategy::DoTrack(int target) {
	DoTacToTar(target);
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

// ��������
void MyStrategy::DoTacWpnShoot(int target)
{
	ACAI::WpnControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.launchPlaneID = mACFlightStatus.flightID; ///< ��������
    outputData.guidePlaneID	 = mACFlightStatus.flightID;  ///< �Ƶ������
    outputData.mslLockTgtID = mACRdrTarget.tgtInfos[target].tgtID;  ///< ����Ŀ����
    outputData._cmdCnt	= mACFlightStatus.timeCounter;       ///< ָ�����
	sendWpnControlCmd(outputData);
}

// ����ͻ��
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
    outputData.desireNavAlt = 4000;		///< ������·�߶�(m)
    outputData.desireSpeed	= 1000;		///< ������·�ٶ�(m/s)
    outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}

// ��β����
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

// ��������
void MyStrategy::DoTacAltClimb()
{
	ACAI::FlyControlCmd outputData;
	memset(&outputData, 0, sizeof(outputData));
	outputData.executePlaneID = mACFlightStatus.flightID; ///< Ŀ�ķɻ����
    outputData.speedCtrlCmd = true;      ///< �ٶȱ���ָ��
	outputData.navCtrlCmd = true;
    outputData.desireSpeed = 1000;///< ������·�ٶ�(m/s)
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
	outputData.desireNavAlt = mACFlightStatus.alt + 100;
    outputData._cmdCnt = mACFlightStatus.timeCounter;   ///< ָ�����
	sendFlyControlCmd(outputData);
}
