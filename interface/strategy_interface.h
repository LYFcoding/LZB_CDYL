/*******************************************************************************
 * \file        strategy_interface.h
 * \authors     weiqm<4146>, linwx<4204>
 * \copyright   Copyright (c) 2021, 611.CADI All rights reserved.
 *******************************************************************************/

#ifndef STRATEGY_INTERFACE_H
#define STRATEGY_INTERFACE_H

#include "strategy_global.h"
#include "strategy_io.h"
#include <stdio.h>

// STRATEGT_INTF_VERSION is (major << 16) + (minor << 8) + patch.
#define STRATEGT_INTF_VERSION 0x020000
#define STRATEGT_INTF_VERSION_STR "2.01"

#ifndef UNUSED
#define UNUSED(para) (void)para;
#endif

/// \brief AI��ս����ģ��ӿ�
class STRATEGY_EXPORT CStrategy
{
public:
    /// \brief Ĭ�Ϲ��캯��
    CStrategy()
        : m_pFlyCtrl(0), m_pSnsCtrl(0), m_pWpnCtrl(0)
        , m_pSendITDB(0), m_pEventLog(0)
    {}

    /// \brief Ĭ������������
    virtual ~CStrategy() {}

    /// \brief ��ȡ���԰汾��
    /// \return ���԰汾��
    virtual unsigned int version() const { return 0xFFFFFF; }

    /// \brief ��ȡ��������
    /// \return ��������
    virtual const char* name() const { return "<AI_NAME>"; }

    /// \brief ��ȡ���Ա���
    /// \return ���Ա���
    virtual const char* slogan() const { return "<AI_SLOGAN>"; }

    /// \brief ��ȡ���Լ���
    /// \return ���Լ���
    virtual const char* info() const { return "<AI_INFO>"; }

    /// \brief ��ȡ������֯/����
    /// \return ������֯/����
    virtual const char* organization() const { return "<AI_ORGANIZATION>"; }

    /// \brief ��ȡ��������/�Ŷ����ߴ���
    /// \return �������߻򿪷��Ŷ����ߴ���
    virtual const char* author() const { return "<AI_AUTHOR>"; }

    /// \brief ��ȡ���������Ŷӳ�Ա����
    /// \return ���������Ŷӳ�Ա����
    virtual const char* teamMembers() const { return "<AI_TEAM_MEMBERS>"; }

    /// \brief ������ʼ������
    /// \details ÿ�ֱ�����ʼʱ����
    /// \param[in] pkConfig ����������Ϣ \sa ACAI::PKConfig
    virtual void onPKStart(const ACAI::PKConfig &pkConfig) { UNUSED(pkConfig); }

    /// \brief ��������������
    /// \details ÿ�ֱ�������ʱ����
    virtual void onPKEnd() { }

	/// \brief �����ɻ������¼�����
    /// \details �ѷ���з��ɻ������л�����˳�����ʱ����
    /// \param[in] flightID �ɻ���� 
    virtual void onPKOut(unsigned int flightID) { UNUSED(flightID); }

    ///---------------------------------------

    /// \brief �����ɻ�״̬�������ݸ���
    /// \param[in] inputData �ɻ�״̬���� \sa ACAI::ACFlightStatus
    virtual void onACFlightStatusUpdate(const ACAI::ACFlightStatus &inputData) { UNUSED(inputData); }

    /// \brief �����״�̽��Ŀ�����ݸ���
    /// \param[in] inputData �״�̽��Ŀ�� \sa ACAI::ACRdrTarget
    virtual void onACRdrTargetUpdate(const ACAI::ACRdrTarget &inputData) { UNUSED(inputData); }

    /// \brief ��������ս̽��Ŀ�����ݸ���
    /// \param[in] inputData ����ս̽��Ŀ�� \sa ACAI::ACEwsTarget
    virtual void onACEwsTargetUpdate(const ACAI::ACEwsTarget &inputData) { UNUSED(inputData); }

    /// \brief ����������в�澯���ݸ���
    /// \param[in] inputData ������в�澯 \sa ACAI::ACMslWarning
    virtual void onACMslWarningUpdate(const ACAI::ACMslWarning &inputData) { UNUSED(inputData); }

    /// \brief ���������ذ������ݸ���
    /// \param[in] inputData �����ذ��� \sa ACAI::ACFCCStatus
    virtual void onACFCCStatusUpdate(const ACAI::ACFCCStatus &inputData) { UNUSED(inputData); }

    /// \brief �����Ƶ�������Ϣ���ݸ���
    /// \param[in] inputData �Ƶ�������Ϣ \sa ACAI::ACMSLInGuide
    virtual void onACMSLInGuideUpdate(const ACAI::ACMSLInGuide &inputData) { UNUSED(inputData); }

    /// \brief ��ӷ��䵼��ʵʱ��Ϣ���ݸ���
    /// \param[in] inputData ��ӷ��䵼��ʵʱ��Ϣ \sa ACAI::TeamMSLLaunched
    virtual void onTeamMSLLaunchedUpdate(const ACAI::TeamMSLLaunched &inputData) { UNUSED(inputData); }

    /// \brief ��ӳ�Ա�ɻ�״̬�������ݸ���
    /// \param[in] inputData ��ӳ�Ա�ɻ�״̬���� \sa ACAI::COFlightStatus
    virtual void onCOFlightStatusUpdate(const ACAI::COFlightStatus &inputData) { UNUSED(inputData); }

    /// \brief ��ӳ�Ա�״�̽��Ŀ�����ݸ���
    /// \param[in] inputData ��ӳ�Ա�״�̽��Ŀ�� \sa ACAI::CORdrTarget
    virtual void onCORdrTargetUpdate(const ACAI::CORdrTarget &inputData) { UNUSED(inputData); }

    /// \brief ��ӳ�Ա����ս̽��Ŀ�����ݸ���
    /// \param[in] inputData ��ӳ�Ա����ս̽��Ŀ�� \sa ACAI::COEwsTarget
    virtual void onCOEwsTargetUpdate(const ACAI::COEwsTarget &inputData) { UNUSED(inputData); }

    /// \brief ��ӳ�Ա����ս�����澯���ݸ���
    /// \param[in] inputData ��ӳ�Ա����ս�����澯 \sa ACAI::COMslWarning
    virtual void onCOMslWarningUpdate(const ACAI::COMslWarning &inputData) { UNUSED(inputData); }

    /// \brief ��ӳ�Ա�����ذ������ݸ���
    /// \param[in] inputData ��ӳ�Ա�����ذ��� \sa ACAI::COFCCStatus
    virtual void onCOFCCStatusUpdate(const ACAI::COFCCStatus &inputData) { UNUSED(inputData); }

    /// \brief ��ӳ�Ա�Ƶ�������Ϣ���ݸ���
    /// \param[in] inputData ��ӳ�Ա�Ƶ�������Ϣ \sa ACAI::COMSLInGuide
    virtual void onCOMSLInGuideUpdate(const ACAI::COMSLInGuide &inputData) { UNUSED(inputData); }

	/// \brief ��ӳ�Ա����ڲ����ݰ�����
    /// \param[in] inputData ��ӳ�Ա����ڲ����ݰ� \sa ACAI::InTeamDataBag
    virtual void onCOTeamDataBagUpdate(const ACAI::InTeamDataBag &inputData) { UNUSED(inputData); }

    ///---------------------------------------

    /// \brief ��̨����
    /// \details ��̨�����߳�
    virtual void backGround() {}

    /// \brief 20ms��������
    /// \details 20ms�����߳�
    virtual void timeSlice20() {}

    /// \brief 40ms��������
    /// \details 40ms�����߳�
    virtual void timeSlice40() {}

public:
    /// \brief ע����п��ƻص�����
    /// \param[in] pCallBack ���п��ƻص����� \sa ACAI::P_CALL_BACK_FLY_CTRL
    void registerFlyCtrlCallback(ACAI::P_CALL_BACK_FLY_CTRL pCallBack);

    /// \brief ע�ᴫ�������ƻص�����
    /// \param[in] pCallBack ���������ƻص����� \sa ACAI::P_CALL_BACK_SNS_CTRL
    void registerSnsCtrlCallback(ACAI::P_CALL_BACK_SNS_CTRL pCallBack);

    /// \brief ע���������ƻص�����
    /// \param[in] pCallBack �������ƻص����� \sa ACAI::P_CALL_BACK_WPN_CTRL
    void registerWpnCtrlCallback(ACAI::P_CALL_BACK_WPN_CTRL pCallBack);

    /// \brief ע���Ӽ���Ϣ���ͻص�����
    /// \param[in] pCallBack ��Ӽ���Ϣ���ͻص����� \sa ACAI::P_CALL_BACK_SEND_ITDB
    void registerSendITDBCallback(ACAI::P_CALL_BACK_SEND_ITDB pCallBack);

    /// \brief ע���¼���־��¼�ص�����
    /// \param[in] pCallBack �¼���־��¼�ص����� \sa ACAI::P_CALL_BACK_EVENT_LOG
    void registereventLogCallback(ACAI::P_CALL_BACK_EVENT_LOG pCallBack);

protected:
    /// \brief ���ͷ��п���ָ��
    /// \param[in] outputData ���п���ָ�� \sa ACAI::FlyControlCmd
    void sendFlyControlCmd(const ACAI::FlyControlCmd &outputData) const;

    /// \brief ���ʹ���������ָ��
    /// \param[in] outputData ����������ָ�� \sa ACAI::SnsControlCmd
    void sendSnsControlCmd(const ACAI::SnsControlCmd &outputData) const;

    /// \brief ������������ָ��
    /// \param[in] outputData ��������ָ�� \sa ACAI::WpnControlCmd
    void sendWpnControlCmd(const ACAI::WpnControlCmd &outputData) const;

    /// \brief ���ͱ���ڲ����ݰ�
    /// \param[in] outputData ����ڲ����ݰ� \sa ACAI::InTeamDataBag
    void sendInTeamDataBag(const ACAI::InTeamDataBag &outputData) const;

    /// \brief ��¼�¼���־
    /// \param[in] outputData �¼���־ \sa ACAI::EventLog
    void logEvent(const ACAI::EventLog &outputData) const;

private:
    ACAI::P_CALL_BACK_FLY_CTRL m_pFlyCtrl;   ///< ָ����ƻص����� \sa ACAI::P_CALL_BACK_FLY_CTRL
    ACAI::P_CALL_BACK_SNS_CTRL m_pSnsCtrl;   ///< ��ʾ��Ϣ����ص����� \sa ACAI::P_CALL_BACK_SNS_CTRL
    ACAI::P_CALL_BACK_WPN_CTRL m_pWpnCtrl;   ///< ָ����ƻص����� \sa ACAI::P_CALL_BACK_WPN_CTRL
    ACAI::P_CALL_BACK_SEND_ITDB m_pSendITDB; ///< ��ʾ��Ϣ����ص����� \sa ACAI::P_CALL_BACK_SEND_ITDB
    ACAI::P_CALL_BACK_EVENT_LOG m_pEventLog; ///< ָ����ƻص����� \sa ACAI::P_CALL_BACK_EVENT_LOG
};

extern "C" {
    /// \brief ��������ģ�鶯̬���ӿ�ʵ��
    /// \return ����ģ�鶯̬���Ӷ���ʾ��������ʧ�ܷ���0 \sa CStrategy
    STRATEGY_EXPORT CStrategy* CreateStrategyInstance();
}

#endif // STRATEGY_INTERFACE_H
