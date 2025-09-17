# ESP32-S3 Serial Monitor for COM5
$port = New-Object System.IO.Ports.SerialPort COM5,115200,None,8,one
$port.ReadTimeout = 1000

try {
    $port.Open()
    Write-Host "Connected to ESP32-S3 on COM5 at 115200 baud" -ForegroundColor Green
    Write-Host "Reading output for 10 seconds..." -ForegroundColor Yellow
    Write-Host ""

    $startTime = Get-Date
    while ((Get-Date) - $startTime -lt [TimeSpan]::FromSeconds(10)) {
        if ($port.BytesToRead -gt 0) {
            $line = $port.ReadLine()
            # Color code output based on content
            if ($line -match "curiosity|Curiosity") {
                Write-Host "🔍 $line" -ForegroundColor Cyan
            }
            elseif ($line -match "learning|Learning") {
                Write-Host "🧠 $line" -ForegroundColor Magenta
            }
            elseif ($line -match "sensor|Sensor") {
                Write-Host "📡 $line" -ForegroundColor Yellow
            }
            elseif ($line -match "RGB|rgb|LED") {
                Write-Host "🌈 $line" -ForegroundColor Blue
            }
            elseif ($line -match "emotion|Emotion") {
                Write-Host "❤️ $line" -ForegroundColor Red
            }
            else {
                Write-Host "   $line"
            }
        }
        Start-Sleep -Milliseconds 10
    }
}
catch {
    Write-Host "Error: $_" -ForegroundColor Red
}
finally {
    if ($port.IsOpen) {
        $port.Close()
        Write-Host "`nPort closed" -ForegroundColor Yellow
    }
}