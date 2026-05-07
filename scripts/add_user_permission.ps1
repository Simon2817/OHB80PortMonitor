# 给所有日志数据库表添加 user_permission 字段
# 默认值 0 = UserPermission::Guest
# 同时支持新建库（CREATE TABLE 已含字段）和老库迁移（ALTER TABLE ADD COLUMN）

$ErrorActionPreference = 'Stop'
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)

$dbDirs = @(
    "OHB80PortMonitor_V_1_0_0\bin\x32\databases",
    "OHB80PortMonitor_V_1_0_0\bin\x64\databases"
)

function Update-File($path, [scriptblock]$transform) {
    if (-not (Test-Path $path)) { Write-Host "skip (missing): $path"; return }
    $content = [System.IO.File]::ReadAllText($path, [System.Text.Encoding]::UTF8)
    $newContent = & $transform $content
    if ($newContent -ne $content) {
        [System.IO.File]::WriteAllText($path, $newContent, $utf8NoBom)
        Write-Host "updated: $path"
    } else {
        Write-Host "unchanged: $path"
    }
}

foreach ($dir in $dbDirs) {
    Write-Host "=== Processing $dir ==="

    # ---- create_operation_log.sql ----
    $createSql = Join-Path $dir "create_operation_log.sql"
    Update-File $createSql {
        param($c)
        # operation_log: 在 description TEXT NOT NULL 后增加列
        $c = $c -replace '(?ms)(CREATE TABLE IF NOT EXISTS operation_log\s*\([^;]*?description TEXT NOT NULL)(\s*\)\s*;)',
            '$1,
    user_permission INTEGER NOT NULL DEFAULT 0$2'

        # communicate_log
        $c = $c -replace '(?ms)(CREATE TABLE IF NOT EXISTS communicate_log\s*\([^;]*?description TEXT NOT NULL)(\s*\)\s*;)',
            '$1,
    user_permission INTEGER NOT NULL DEFAULT 0$2'

        # alarm_log
        $c = $c -replace '(?ms)(CREATE TABLE IF NOT EXISTS alarm_log\s*\([^;]*?description TEXT NOT NULL)(\s*\)\s*;)',
            '$1,
    user_permission INTEGER NOT NULL DEFAULT 0$2'

        # device_param_log（结尾是 foup_status 行）
        $c = $c -replace '(?ms)(CREATE TABLE IF NOT EXISTS device_param_log\s*\([^;]*?foup_status INTEGER NOT NULL CHECK\(foup_status IN \(0, 1\)\))(\s*\)\s*;)',
            '$1,
    user_permission INTEGER NOT NULL DEFAULT 0$2'

        # 在文件末尾追加 ALTER TABLE 迁移语句（幂等：老库新增列，新库会失败但被忽略）
        $marker = "-- 用户权限字段迁移（老库升级用，新库会因列已存在而失败）"
        if ($c -notmatch [regex]::Escape($marker)) {
            $migration = @"


-- ============================================
$marker
-- 默认值 0 对应 UserPermission::Guest
-- ============================================
ALTER TABLE operation_log    ADD COLUMN user_permission INTEGER NOT NULL DEFAULT 0;
ALTER TABLE communicate_log  ADD COLUMN user_permission INTEGER NOT NULL DEFAULT 0;
ALTER TABLE alarm_log        ADD COLUMN user_permission INTEGER NOT NULL DEFAULT 0;
ALTER TABLE device_param_log ADD COLUMN user_permission INTEGER NOT NULL DEFAULT 0;
"@
            $c = $c.TrimEnd() + $migration + "`n"
        }
        return $c
    }

    # ---- operation_log_queries.sql: insert_record ----
    Update-File (Join-Path $dir "operation_log_queries.sql") {
        param($c)
        $c = $c -replace 'INSERT INTO operation_log \(occur_time, log_type, description\)\s*\r?\nVALUES \(\?, \?, \?\);',
            "INSERT INTO operation_log (occur_time, log_type, description, user_permission)`r`nVALUES (?, ?, ?, ?);"
        return $c
    }

    # ---- alarm_log_queries.sql: insert_record ----
    Update-File (Join-Path $dir "alarm_log_queries.sql") {
        param($c)
        $c = $c -replace 'INSERT INTO alarm_log \(alarm_level, occur_time, qr_code, alarm_type, is_resolved, resolve_time, customer_visible, description\)\s*\r?\nVALUES \(\?, \?, \?, \?, \?, \?, \?, \?\);',
            "INSERT INTO alarm_log (alarm_level, occur_time, qr_code, alarm_type, is_resolved, resolve_time, customer_visible, description, user_permission)`r`nVALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"
        return $c
    }

    # ---- communicate_log_queries.sql: insert_record ----
    Update-File (Join-Path $dir "communicate_log_queries.sql") {
        param($c)
        $c = $c -replace 'INSERT INTO communicate_log \(send_time, response_time, command_id, qr_code, exec_status, retry_count, send_frame, response_frame, description\)\s*\r?\nVALUES \(\?, \?, \?, \?, \?, \?, \?, \?, \?\);',
            "INSERT INTO communicate_log (send_time, response_time, command_id, qr_code, exec_status, retry_count, send_frame, response_frame, description, user_permission)`r`nVALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
        return $c
    }

    # ---- device_param_log_queries.sql: insert_record ----
    Update-File (Join-Path $dir "device_param_log_queries.sql") {
        param($c)
        $c = $c -replace 'INSERT INTO device_param_log \(qr_code, record_time, inlet_pressure, outlet_pressure, inlet_flow, humidity, temperature, foup_status\)\s*\r?\nVALUES \(\?, \?, \?, \?, \?, \?, \?, \?\);',
            "INSERT INTO device_param_log (qr_code, record_time, inlet_pressure, outlet_pressure, inlet_flow, humidity, temperature, foup_status, user_permission)`r`nVALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"
        return $c
    }
}

Write-Host "`nDone."
