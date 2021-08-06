function getBytesReceived()
	return getCIMV2Value("Win32_PerfRawData_Tcpip_NetworkInterface", "BytesReceivedPersec")
end

function getBytesReceivedPerSec()
	return getCIMV2Value("Win32_PerfFormattedData_Tcpip_NetworkInterface", "BytesReceivedPersec")
end

function getBytesSent()
	return getCIMV2Value("Win32_PerfRawData_Tcpip_NetworkInterface", "BytesSentPersec")
end

function getBytesSentPerSec()
	return getCIMV2Value("Win32_PerfFormattedData_Tcpip_NetworkInterface", "BytesSentPersec")
end

function getCPUTemp()
	return getOHMValue("CPU Package", "Temperature")
end

function getCPULoad()
	return getOHMValue("CPU Total", "Load")
end

function getGPUTemp()
	return getOHMValue("GPU Core", "Temperature")
end

function getGPULoad()
	return getOHMValue("GPU Core", "Load")
end

function compactBytes(nBytes)
	if (nBytes <= 1024.0) then
		return string.format("%4.1fB", nBytes)
	elseif (nBytes <= 1048576) then
		local kBytes = nBytes / 1024.0
		return string.format("%4.1fK", kBytes)
	elseif (nBytes <= 1073741824) then
		local mBytes = nBytes / 1048576.0
		return string.format("%4.1fM", mBytes)
	else
		local gBytes = nBytes / 1073741824.0
		return string.format("%1.4fG", gBytes)
	end
end

function _update()
	local line0 = "line 0"
	local line1 = "line 1"

	local byteReceived, byteReceivedPerSec = getInTraffic()
	local byteSent, byteSentPerSec = getOutTraffic()

	line0 = string.format("D: %s U: %s TD: %s TU: %s",
		compactBytes(byteReceivedPerSec), compactBytes(byteSentPerSec),
		compactBytes(byteReceived), compactBytes(byteSent))
	local cpuTemp = math.floor(getCPUTemp())
	local cpuLoad = getCPULoad();
	local gpuTemp = math.floor(getGPUTemp())
	local gpuLoad = getGPULoad();
	line1 = string.format("CPU: %d(%3.1f%%) GPU: %d(%3.1f%%) Q: %4.2fms", cpuTemp, cpuLoad, gpuTemp, gpuLoad, getQueryTime())

	return line0, line1
end

_update()