#pragma once

class MouseCtrler
{
public:
	MouseCtrler(void);
	virtual ~MouseCtrler(void);

	virtual void OnMouseMove(int x,int y);
	virtual void OnLeftButtonClick(int x,int y);

	virtual void OnLeftButtonDown(int x,int y);
	virtual void OnLeftButtonUp(int x,int y);
	virtual void OnRightButtonDown(int x,int y);
	virtual void OnRightButtonUp(int x,int y);
};
