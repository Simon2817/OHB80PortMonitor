#pragma once
#include <QStringList>

// ====================================================================
// AlarmHeaderConfig - CSV/Widget unified header configuration
//
// This structure contains all header-related configuration that
// needs to be passed down through the component hierarchy.
// ====================================================================
struct AlarmHeaderConfig
{
    // Header strings in display order
    QStringList headers;

    // Constructor with default headers
    AlarmHeaderConfig()
        : headers({
            QStringLiteral("Level"),
            QStringLiteral("SendTime"),
            QStringLiteral("QRCode"),
            QStringLiteral("AlarmId"),
            QStringLiteral("Resolved"),
            QStringLiteral("ResolveTime"),
            QStringLiteral("Message")
        })
    {}

    // Constructor with custom headers
    explicit AlarmHeaderConfig(const QStringList &customHeaders)
        : headers(customHeaders)
    {}

    // Helper to get column index by header name
    int headerToColumn(const QString &header) const
    {
        return headers.indexOf(header);
    }
};
