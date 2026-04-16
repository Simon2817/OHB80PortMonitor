#include "shareddata.h"
#include <QDebug>

QVector<SetOfOHBInfo> SharedData::setOfOHBInfoList;

SharedData::SharedData() {
    if (setOfOHBInfoList.isEmpty()) {
        setOfOHBInfoList.reserve(20);
        int qrCodeStart = 12001;
        QVector<int> uiIds = {2,3,4,5,6,7,8,9,10,11,12,13,36,37,38,39,40,41,42,43};
        
        for (int i = 0; i < 20; ++i) {
            SetOfOHBInfo setInfo;
            setInfo.setUiId(uiIds[i]);
            
            // 创建 Foup 队列
            QVector<FoupOfOHBInfo> foups;
            foups.reserve(4);
            
            for (int j = 0; j < 4; ++j) {
                FoupOfOHBInfo foup;
                foup.qrCode = QString::number(qrCodeStart + i * 4 + j);
                foup.inletPressure = 0.12 + (i * 0.01) + (j * 0.002); // 示例压力值
                foup.inletFlow = 10.0 + (i * 0.5) + (j * 0.1); // 示例流量值
                foup.RH = 45.0 + (i * 2.0) + (j * 0.5); // 示例湿度值
                foups.append(foup);
            }
            
            // 一次性设置整个 Foup 队列
            setInfo.setFoups(foups);
            setOfOHBInfoList.append(setInfo);
        }
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
