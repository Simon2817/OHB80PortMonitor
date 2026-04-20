#include "shareddata.h"
#include "appconfig.h"
#include "networkconfig.h"
#include "qrcodeconfig.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include <QDebug>
#include <QHash>

QVector<SetOfOHBInfo> SharedData::setOfOHBInfoList;
bool SharedData::s_modbusManagerInitialized = false;

SharedData::SharedData() {

    if (setOfOHBInfoList.isEmpty()) {
        setOfOHBInfoList.reserve(20);
        QVector<int> uiIds = {2,3,4,5,6,7,8,9,10,11,12,13,36,37,38,39,40,41,42,43};
        
        // 读取网络配置
        QVector<OHBNetworkInfo> networkInfos = AppConfig::getInstance().getNetworkConfig().readNetworkConfig();
        // 读取二维码配置
        QVector<QString> qrCodeInfos = AppConfig::getInstance().getQRCodeConfig().readQRCodeMapping();
        // 索引
        int index = 0;
        
        for (int i = 0; i < 20; ++i) {
            SetOfOHBInfo setInfo;
            setInfo.setUiId(uiIds[i]);
            
            QVector<FoupOfOHBInfo> foups;
            for (int j = 0; j < 4; ++j) {
                FoupOfOHBInfo foup;
                foup.qrCode = qrCodeInfos.at(index);
                foup.ip = networkInfos.at(index).ip;
                foup.port = networkInfos.at(index++).port;
                foup.inletPressure = 0;
                foup.inletFlow = 0;
                foup.RH = 0;
                foup.foupIn = false;
                foup.hasAlarm = true;
                foups.append(foup);

                // static int num = 0;
                if (!foup.ip.isEmpty() && foup.port > 0 ) {
                    ModbusTcpMasterManager::instance().addMaster(foup.ip, foup.port, foup.qrCode);
                    // num++;
                }
                else {
                    qDebug() << "Invalid IP or port for foup:" << foup.qrCode;
                }
            }
            
            // 一次性设置整个 Foup 队列
            setInfo.setFoups(foups);
            setOfOHBInfoList.append(setInfo);
        }
        
        qDebug() << "SharedData initialized" << setOfOHBInfoList.size() << "OHB items from config";

    }
}

QSharedPointer<SetOfOHBInfo> SharedData::getSetOfOHBInfoByUiId(int uiId)
{
    // 遍历 setOfOHBInfoList 查找匹配的 uiId
    for (int i = 0; i < setOfOHBInfoList.size(); ++i) {
        if (setOfOHBInfoList[i].getUiId() == uiId) {
            // 创建一个指向该对象的智能指针
            // 注意：这里返回的是指向静态列表中对象的指针，生命周期由SharedData管理
            return QSharedPointer<SetOfOHBInfo>(&setOfOHBInfoList[i], [](SetOfOHBInfo*) {
                // 空的删除器，因为对象由SharedData管理，不需要删除
            });
        }
    }
    
    // 未找到时返回空指针
    return QSharedPointer<SetOfOHBInfo>(nullptr);
}

FoupOfOHBInfo* SharedData::getFoupByQRCode(const QString& qrCode)
{
    // 遍历所有 SetOfOHBInfo，查找匹配的 qrCode
    for (int i = 0; i < setOfOHBInfoList.size(); ++i) {
        SetOfOHBInfo& setInfo = setOfOHBInfoList[i];
        QVector<FoupOfOHBInfo>& foups = setInfo.getFoups();
        for (int j = 0; j < foups.size(); ++j) {
            if (foups[j].qrCode == qrCode) {
                // 返回指向该 Foup 的指针
                return &foups[j];
            }
        }
    }
    
    // 未找到时返回空指针
    return nullptr;
}
