#ifndef GRAPHCONFIGPARSER_H
#define GRAPHCONFIGPARSER_H

#include "graphnode.h"
#include "graphedge.h"
#include "loggermanager.h"

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QMap>
#include <QList>

namespace Graph {

struct GraphConfig
{
    bool isUndirected;
    int startNodeId;
    int deviceSize;      // 设备控件尺寸，默认120
    int deviceSpacing;   // 设备控件间距，默认96
    QMap<int, QSharedPointer<GraphNode>> nodes;
    QList<int> nodeIdOrder;  // 节点ID的出现顺序
    
    struct EdgeInfo {
        int sourceId;
        int targetId;
        EdgeType type;
        double size;
        double foupLevelOffset;  // FoupLevel 视图下边长度的偏移值（像素），默认为0
        double setLevelOffset;   // SetLevel 视图下边长度的偏移值（像素），默认为0
    };
    QList<EdgeInfo> edges;
};

class GraphConfigParser
{
public:
    enum class ParseStage {
        NotStarted = 0,
        ParsedGraphConfigTag = 1,
        ParsedEdgesTag = 2,
        NodesCollected = 3,
        EdgeEndpointsCollected = 4,
        Completed = 5
    };

    explicit GraphConfigParser(const QString& filePath);
    ~GraphConfigParser();
    
    bool parse();
    void setFixSpacingValue(double value);
    
    const GraphConfig& getConfig() const { return m_config; }
    ParseStage getStage() const { return m_stage; }
    static const char* stageToString(ParseStage stage);
    
private:
    bool parseGraphSettings(const QDomElement& root);
    bool parseEdges(const QDomElement& root);
    void processNode(const QDomElement& nodeEle);
    bool parseSizeValue(const QString& sizeStr, double& size) const;
    
    QString getElementText(const QDomElement& parent, const QString& tagName);
    QString getAttributeText(const QDomElement& element, const QString& attrName);
    int getAttributeInt(const QDomElement& element, const QString& attrName, int defaultValue = 0);
    
private:
    QString m_filePath;
    GraphConfig m_config;
    bool m_startNodeFound;
    double m_fixSpacingValue;
    
    LoggerManager& m_logger;
    std::string m_loggerFileName;
    ParseStage m_stage;
};

}

#endif // GRAPHCONFIGPARSER_H
