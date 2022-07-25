/*******************************************************************************
 * \file        strategy_interface.cpp
 * \authors     weiqm<4146>, linwx<4204>
 * \copyright   Copyright (c) 2021, 611.CADI All rights reserved.
 *******************************************************************************/

#include "strategy_interface.h"

void CStrategy::registerFlyCtrlCallback(ACAI::P_CALL_BACK_FLY_CTRL pCallBack)
{
    printf("[CStrategy] CALL_BACK_FLY_CTRL registered at %08x.\n", pCallBack);
    m_pFlyCtrl = pCallBack;
}

void CStrategy::registerSnsCtrlCallback(ACAI::P_CALL_BACK_SNS_CTRL pCallBack)
{
    printf("[CStrategy] CALL_BACK_SNS_CTRL registered at %08x.\n", pCallBack);
    m_pSnsCtrl = pCallBack;
}

void CStrategy::registerWpnCtrlCallback(ACAI::P_CALL_BACK_WPN_CTRL pCallBack)
{
    printf("[CStrategy] CALL_BACK_WPN_CTRL registered at %08x.\n", pCallBack);
    m_pWpnCtrl = pCallBack;
}

void CStrategy::registerSendITDBCallback(ACAI::P_CALL_BACK_SEND_ITDB pCallBack)
{
    printf("[CStrategy] CALL_BACK_SEND_ITDB registered at %08x.\n", pCallBack);
    m_pSendITDB = pCallBack;
}

void CStrategy::registereventLogCallback(ACAI::P_CALL_BACK_EVENT_LOG pCallBack)
{
    printf("[CStrategy] CALL_BACK_EVENT_LOG registered at %08x.\n", pCallBack);
    m_pEventLog = pCallBack;
}


void CStrategy::sendFlyControlCmd(const ACAI::FlyControlCmd &outputData) const
{
    if (!m_pFlyCtrl || !m_pFlyCtrl(outputData)) {
        if (!m_pFlyCtrl) {
            printf("[CStrategy] ���п���ָ��ͽӿ�δע�ᣡ\n");
        }
        else {
            printf("[CStrategy] ���п���ָ���ʧ�ܣ�\n");
        }
    }
}

void CStrategy::sendSnsControlCmd(const ACAI::SnsControlCmd &outputData) const
{
    if (!m_pSnsCtrl || !m_pSnsCtrl(outputData)) {
        if (!m_pSnsCtrl) {
            printf("[CStrategy] ����������ָ��ͽӿ�δע�ᣡ\n");
        }
        else {
            printf("[CStrategy] ����������ָ���ʧ�ܣ�\n");
        }
    }
}

void CStrategy::sendWpnControlCmd(const ACAI::WpnControlCmd &outputData) const
{
    if (!m_pWpnCtrl || !m_pWpnCtrl(outputData)) {
        if (!m_pWpnCtrl) {
            printf("[CStrategy] ��������ָ��ͽӿ�δע�ᣡ\n");
        }
        else {
            printf("[CStrategy] ��������ָ���ʧ�ܣ�\n");
        }
    }
}

void CStrategy::sendInTeamDataBag(const ACAI::InTeamDataBag &outputData) const
{
    if (!m_pSendITDB || !m_pSendITDB(outputData)) {
        if (!m_pSendITDB) {
            printf("[CStrategy] ����ڲ����ݰ����ͽӿ�δע�ᣡ\n");
        }
        else {
            printf("[CStrategy] ����ڲ����ݰ�����ʧ�ܣ�\n");
        }
    }
}

void CStrategy::logEvent(const ACAI::EventLog &outputData) const
{
    if (!m_pEventLog || !m_pEventLog(outputData)) {
        if (!m_pEventLog) {
            printf("[CStrategy] �¼���־��¼�ӿ�δע�ᣡ\n");
        }
        else {
            printf("[CStrategy] �¼���־��¼ʧ�ܣ�\n");
        }
    }
}
