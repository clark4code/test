class ChannelInfo {
	bool	m_bEnable;
	int		m_nCoupling;
	double	m_dbOffset;
	double	m_dbGain;
};
 
class TriggerInfo {
	bool	m_bEnable;
	int		m_nChannel;
	bool	m_bRisingEdge;
	double	m_dbThreshold;
	double	m_dbPosOffsetPersent;
};

struct OSCInfo {
	size_t	m_szSampleCount;
	double	m_dbFrequencyHz;
	TriggerInfo	m_Trigger;
	std::vector<ChannelInfo>	m_Channels;
};

class Data {
	OSCInfo	m_OSCInfo;
	COleSafeArray	m_saDataX;
	std::vector<COleSafeArray>	m_arrDataY;
};
t
class OSC {
public:
	OSCInfo const&	GetInfo() const {
		return m_Info;
	}
	virtual void SetTrigger(const Trigger& trigger) {
		std::lock_gard lck(&m_Mutex);
		memcpy(&m_Info.m_Trigger, &trigger, sizeof(m_Info.m_Trigger));
	}
	virtual void SetSampleCount(size_t	szValue) {
		std::lock_gard lck(&m_Mutex);
		m_Info.m_szSampleCount = szValue;
	}
	virtual void SetFrequencyHz(size_t	dbValue) {
		std::lock_gard lck(&m_Mutex);
		m_Info.m_dbFrequencyHz = dbValue;
	}
	virtual void SetChannel(size_t szIndex, const ChannelInfo& info) {
		std::lock_gard lck(&m_Mutex);
		if (szIndex < m_Info.m_Channels.size())
			memcpy(m_Info.m_Channels.data() + szIndex, &info, sizeof(info));
	}
	
	virtual CString GetFamilyName() const = 0;
	virtual void InitConnectPanel(CWnd* parent) = 0;
	virtual bool connect() = 0;
	virtual bool Run() = 0;
	virtual bool IsRuning() = 0;
	virtual void Stop() = 0;
	
private:
	std::mutex	m_Mutex;
	OSCInfo	m_Info;
	safe_queue<Data>	m_DataQueue;
};