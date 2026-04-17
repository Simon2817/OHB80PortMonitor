#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <QVector>
#include <QSharedPointer>
#include "classes/setofohbinfo.h"

class SharedData
{
public:
    SharedData();
    
    static QVector<SetOfOHBInfo> setOfOHBInfoList;
    
    // 根据 uiId 获取 SetOfOHBInfo 对象
    static QSharedPointer<SetOfOHBInfo> getSetOfOHBInfoByUiId(int uiId);
    

private:
    static bool s_modbusManagerInitialized;
};

#endif // SHAREDDATA_H
