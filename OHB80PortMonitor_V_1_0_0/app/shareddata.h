#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <QVector>
#include <QSharedPointer>
#include "classes/setofohbinfo.h"
#include "classes/foupofohbinfo.h"

class SharedData
{
public:
    SharedData();
    
    static QVector<SetOfOHBInfo> setOfOHBInfoList;
    
    // 根据 uiId 获取 SetOfOHBInfo 对象
    static QSharedPointer<SetOfOHBInfo> getSetOfOHBInfoByUiId(int uiId);
    
    // 根据 qrCode 获取 FoupOfOHBInfo 指针（用于直接修改）
    static FoupOfOHBInfo* getFoupByQRCode(const QString& qrCode);

private:
    static bool s_modbusManagerInitialized;
};

#endif // SHAREDDATA_H
