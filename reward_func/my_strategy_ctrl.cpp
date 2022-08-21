#include "my_strategy.h"
#include "string.h"

int g_flight_state = 0;
int g_cnt_state = 0;
int g_enmy_state = 0;
int g_launch_state = 0;
int g_guide_state = 0;

void MyStrategy::onPKOut(unsigned int flightID)
{
	SetFlightState(flightID);
}

void MyStrategy::SetFlightState(unsigned int flightID)
{
	// �ж��ѷ������������ɻ�����������������״ֵ̬2
	if (flightID == 3 && mACFlightStatus.flightID == 4)
		g_flight_state = 2;
	if(flightID == 4 && mACFlightStatus.flightID == 3)
		g_flight_state = 2;
	if(flightID == 1 && mACFlightStatus.flightID == 2)
		g_flight_state = 2;
	if(flightID == 2 && mACFlightStatus.flightID == 1)
		g_flight_state = 2;
	if(flightID == mACFlightStatus.flightID)
		g_flight_state = 2;

	// �жϵз��ɻ�����������������״ֵ̬1
	if (flightID == 3 && mACFlightStatus.flightID == 1)
		g_flight_state = 1;
	if (flightID == 3 && mACFlightStatus.flightID == 2)
		g_flight_state = 1;
	if (flightID == 4 && mACFlightStatus.flightID == 1)
		g_flight_state = 1;
	if (flightID == 4 && mACFlightStatus.flightID == 2)
		g_flight_state = 1;
	if (flightID == 1 && mACFlightStatus.flightID == 3)
		g_flight_state = 1;
	if (flightID == 1 && mACFlightStatus.flightID == 4)
		g_flight_state = 1;
	if (flightID == 2 && mACFlightStatus.flightID == 3)
		g_flight_state = 1;
	if	(flightID == 2 && mACFlightStatus.flightID == 4)
		g_flight_state = 1;
}

int MyStrategy::OutputReward()
{
	int reward = 0;

	// �з��ɻ�����������������һ�ܵл���
	if(g_flight_state == 1)
	{
		// ����״̬����
		g_flight_state = 0;
		reward += 250;
	}
	// �ѷ��ɻ������������������ѷ��ɻ������䣩
	if(g_flight_state == 2)
	{
		// ����״̬����
		g_flight_state = 0;
		reward -= 250;
	}
	// ��������в��������û�зɻ�������ʱ��reward���ҷ����з�����������
	if (g_cnt_state < mACMslWarning.mslCnt && g_flight_state == 0 && mACFlightStatus.flightID == mACMslWarning.flightID)
	{
		g_cnt_state++;
		reward -= 5;
	}
	// ��������в�����ٲ���û�зɻ�׹��ʱ��reward���ҷ����ѵз�������
	if (g_cnt_state > mACMslWarning.mslCnt && g_flight_state == 0 && mACFlightStatus.flightID == mACMslWarning.flightID)
	{
		g_cnt_state--;
		reward += 10;
	}
	// ����鵽�л���������ҷ���õз���Ұ��
	if (g_enmy_state < mACRdrTarget.tgtCnt && g_flight_state == 0)
	{
		g_enmy_state++;
		reward += 2;
	}
	// ��쵽�ĵл��������٣��ҷ���ʧ�з���Ұ��
	if (g_enmy_state > mACRdrTarget.tgtCnt && g_flight_state == 0)
	{
		g_enmy_state--;
		reward -= 2;
	}
	// �ɻ����䵼��
	if (g_launch_state < mTeamMSLLaunched.mslCnt && g_flight_state == 0 && mTeamMSLLaunched.trajectoryInfos[0].launchFlightID == mACFlightStatus.flightID)
	{
		g_launch_state++;
		reward -= 10;
	}
	// �ҷ����������з�һ�ܷɻ�
	if (g_guide_state < mACMSLInGuide.mslCnt && g_flight_state == 0 && mACMSLInGuide.guideFlightID == mACFlightStatus.flightID)
	{
		g_guide_state++;
		reward += 5;
	}
	return reward;
}