#pragma once

class OptionBase
{
protected:
	bool m_isClosed;

public:
	OptionBase();
	virtual ~OptionBase() = default;

	virtual bool SystemInit();
	virtual void GameInit();
	virtual void Update();
	virtual void Draw() const;
	virtual bool Release();

	// 共通の関数
	bool IsClosed() const { return m_isClosed; }

};
